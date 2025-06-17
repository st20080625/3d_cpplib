// C:\SDL3-3.2.12\include\SDL3
// C:\SDL3-3.2.12\lib\x64
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "3d.hpp"
#include "draw.hpp"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <functional>
#include <condition_variable>

using namespace std;
using namespace cpp3d;
using namespace draw;

vector<vector<vec3d>> STL2vec3d(const string &file_path){
    ifstream file(file_path, ios::binary);
    char header[80];
    file.read(header, 80);

    uint32_t triangle_count;
    file.read(reinterpret_cast<char*>(&triangle_count), 4);

    vector<vec3d> normals;
    vector<vec3d> vectors;
    for(uint32_t i = 0; i < triangle_count; ++i){
        float nx;
        float ny;
        float nz;
        file.read(reinterpret_cast<char*>(&nx), 4);
        file.read(reinterpret_cast<char*>(&ny), 4);
        file.read(reinterpret_cast<char*>(&nz), 4);
        vec3d normal_vector(nx, ny, nz);
        normals.push_back(normal_vector);
        float x;
        float y;
        float z;
        for(int j = 0; j < 3; ++j){
            file.read(reinterpret_cast<char*>(&x), 4);
            file.read(reinterpret_cast<char*>(&y), 4);
            file.read(reinterpret_cast<char*>(&z), 4);
            vec3d vector(x, y, z);
            vectors.push_back(vector);
        }
        uint16_t attribute_byte_count;
        file.read(reinterpret_cast<char*>(&attribute_byte_count), 2);
    }
    return {normals, vectors};
}

// 1. ピクセルバッファを使用した描画（ダブルバッファリング）
struct FrameBuffer {
    vector<uint8_t> color_data;
    vector<float> z_buffer;
    int width, height;
    
    FrameBuffer(int w, int h) : width(w), height(h) {
        color_data.resize(w * h * 4, 0);  // RGBA
        z_buffer.resize(w * h, std::numeric_limits<float>::max());
    }
    
    void clear() {
        std::fill(color_data.begin(), color_data.end(), 0);
        std::fill(z_buffer.begin(), z_buffer.end(), std::numeric_limits<float>::max());
    }
    
    void setPixel(int x, int y, const vec3d& color, float depth) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        
        int index = (y * width + x) * 4;
        float current_depth = z_buffer[y * width + x];
        
        // Zバッファ: 手前の物体だけ描画
        if (depth < current_depth) {
            z_buffer[y * width + x] = depth;
            color_data[index] = static_cast<uint8_t>(color.x);
            color_data[index + 1] = static_cast<uint8_t>(color.y);
            color_data[index + 2] = static_cast<uint8_t>(color.z);
            color_data[index + 3] = 255;  // アルファ値
        }
    }
};

// 2. バックフェースカリング（裏面の三角形を描画しない）
bool isFrontFacing(const vec3d& normal, const vec3d& view_dir) {
    return normal.dot(view_dir) < 0;
}

// 3. 事前に回転行列を計算（毎頂点で再計算しない）
matrix createRotationMatrix(float anglex, float angley, float anglez) {
    // 既存のmove関数から回転行列部分だけを抽出
    float sx = sin(anglex);
    float cx = cos(anglex);
    float sy = sin(angley);
    float cy = cos(angley);
    float sz = sin(anglez);
    float cz = cos(anglez);
    
    return matrix(vector<vector<float>>{
        {cy * cz, cy * sz, -sy, 0},
        {sx * sy * cz - cx * sz, sx * sy * sz + cx * cz, sx * cy, 0},
        {cx * sy * cz + sx * sz, cx * sy * sz - sx * cz, cx * cy, 0},
        {0, 0, 0, 1}
    });
}

// 回転行列を適用する関数（追加）
vec3d apply_transformation(const vec3d& point, const matrix& m) {
    // 同次座標に変換
    matrix p = matrix(vector<vector<float>>{
        {point.x},
        {point.y},
        {point.z},
        {1.0f}
    });
    
    // 行列乗算
    matrix result = m.mul(p);
    
    // 3D座標に戻す
    return vec3d(result.data[0][0], result.data[1][0], result.data[2][0]);
}

// 4. 三角形を高速描画する関数
void fillTriangleFast(FrameBuffer& buffer, 
                      const vec2d& p1, const vec2d& p2, const vec2d& p3,
                      float z1, float z2, float z3,
                      const vec3d& color) {
    // 境界ボックスを計算
    int minX = max(0, static_cast<int>(min({p1.x, p2.x, p3.x})));
    int maxX = min(buffer.width - 1, static_cast<int>(max({p1.x, p2.x, p3.x})));
    int minY = max(0, static_cast<int>(min({p1.y, p2.y, p3.y})));
    int maxY = min(buffer.height - 1, static_cast<int>(max({p1.y, p2.y, p3.y})));
    
    // エッジ関数を使った効率的な三角形塗りつぶし
    auto edge = [](const vec2d& a, const vec2d& b, const vec2d& p) {
        return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
    };
    
    // 境界ボックス内のピクセルを処理
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            vec2d p(x + 0.5f, y + 0.5f);
            
            // 三角形内部判定
            float e1 = edge(p1, p2, p);
            float e2 = edge(p2, p3, p);
            float e3 = edge(p3, p1, p);
            
            if ((e1 >= 0 && e2 >= 0 && e3 >= 0) || (e1 <= 0 && e2 <= 0 && e3 <= 0)) {
                // 重心座標を計算
                float area = abs(edge(p1, p2, p3));
                float w1 = abs(edge(p2, p3, p)) / area;
                float w2 = abs(edge(p3, p1, p)) / area;
                float w3 = abs(edge(p1, p2, p)) / area;
                
                // 深度を補間
                float depth = w1 * z1 + w2 * z2 + w3 * z3;
                
                // ピクセルをセット
                buffer.setPixel(x, y, color, depth);
            }
        }
    }
}

// メイン関数内のマルチスレッド処理を最適化
int main() {
    // 既存のコード（初期化部分）
    const int window_w = 1280;
    const int window_h = 720;

    vec2d window_center(window_w / 2, window_h / 2);
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Tinko", window_w, window_h, 0);
    if (!window)
    {
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        return 1;
    }

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderPresent(renderer);

    SDL_Event ev;

    string cover_path = "hand_dummy.stl";
    
    vector<vector<vec3d>> covers = STL2vec3d(cover_path);
    vector<vec3d> cover_normals = covers[0];
    vector<vec3d> cover_vectors = covers[1];
    float anglex = 0;
    float angley = 0;
    float anglez = 0;

    // フレームバッファを作成
    FrameBuffer frame_buffer(window_w, window_h);
    
    // テクスチャを作成（バッファからレンダラーへデータ転送用）
    SDL_Texture* screen_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        window_w,
        window_h
    );
    
    // スレッドプールの作成（起動時に一度だけ）
    std::vector<std::thread> thread_pool;
    std::vector<bool> thread_busy;
    std::vector<std::function<void()>> thread_tasks;
    std::mutex tasks_mutex;
    std::condition_variable task_cv;
    bool should_terminate = false;
    
    // スレッド数を決定
    unsigned int num_threads = std::thread::hardware_concurrency();
    num_threads = std::max(2u, std::min(16u, num_threads));
    
    thread_busy.resize(num_threads, false);
    thread_tasks.resize(num_threads);
    
    // スレッドワーカー関数
    auto thread_worker = [&](int thread_id) {
        while (!should_terminate) {
            std::function<void()> task;
            
            {
                std::unique_lock<std::mutex> lock(tasks_mutex);
                task_cv.wait(lock, [&]{ return should_terminate || thread_busy[thread_id]; });
                
                if (should_terminate) return;
                
                task = thread_tasks[thread_id];
            }
            
            // タスク実行
            task();
            
            {
                std::lock_guard<std::mutex> lock(tasks_mutex);
                thread_busy[thread_id] = false;
            }
            task_cv.notify_one();
        }
    };
    
    // スレッドプールを起動
    for (unsigned int i = 0; i < num_threads; i++) {
        thread_pool.emplace_back(thread_worker, i);
    }
    
    // メインループ
    while (true) {
        // イベント処理
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT) {
                // スレッドプールの終了処理
                should_terminate = true;
                task_cv.notify_all();
                for (auto& thread : thread_pool) {
                    thread.join();
                }
                
                // リソース解放
                SDL_DestroyTexture(screen_texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                return 0;
            }
        }
        
        // フレームバッファをクリア
        frame_buffer.clear();
        
        // 回転行列を一度だけ計算
        matrix rotation_matrix = createRotationMatrix(anglex, angley, anglez);
        
        // 視点方向ベクトル（カメラからモデルへの方向）
        vec3d view_dir(0, 0, 1);
        
        // タスクの分割
        size_t triangles_count = cover_vectors.size() / 3;
        size_t triangles_per_thread = (triangles_count + num_threads - 1) / num_threads;
        
        // タスクを各スレッドに割り当て
        for (unsigned int t = 0; t < num_threads; t++) {
            size_t start_triangle = t * triangles_per_thread;
            size_t end_triangle = std::min((t + 1) * triangles_per_thread, triangles_count);
            
            if (start_triangle >= triangles_count) break;
            
            // スレッドタスクを設定
            thread_tasks[t] = [&, start_triangle, end_triangle]() {
                // スレッド専用のバッファを作成
                FrameBuffer local_buffer(window_w, window_h);
                
                for (size_t tri = start_triangle; tri < end_triangle; tri++) {
                    size_t idx = tri * 3;
                    
                    // 頂点を取得
                    vec3d p1 = cover_vectors[idx];
                    vec3d p2 = cover_vectors[idx + 1];
                    vec3d p3 = cover_vectors[idx + 2];
                    
                    // 法線ベクトルを取得
                    vec3d normal = cover_normals[tri];
                    
                    // バックフェースカリング
                    if (isFrontFacing(normal, view_dir)) {
                        // 回転行列を適用
                        p1 = apply_transformation(p1, rotation_matrix);
                        p2 = apply_transformation(p2, rotation_matrix);
                        p3 = apply_transformation(p3, rotation_matrix);
                        
                        // ビューポートに変換
                        vec2d p1_screen = Calc_Coords(p1, window_center);
                        vec2d p2_screen = Calc_Coords(p2, window_center);
                        vec2d p3_screen = Calc_Coords(p3, window_center);
                        
                        // フラスタムカリング（簡易版）- 全ての頂点が画面外なら描画しない
                        if ((p1_screen.x < 0 || p1_screen.x >= window_w || p1_screen.y < 0 || p1_screen.y >= window_h) &&
                            (p2_screen.x < 0 || p2_screen.x >= window_w || p2_screen.y < 0 || p2_screen.y >= window_h) &&
                            (p3_screen.x < 0 || p3_screen.x >= window_w || p3_screen.y < 0 || p3_screen.y >= window_h)) {
                            continue;
                        }
                        
                        // シェーディング
                        vec3d light_dir(0, 0, -1);
                        float brightness = max(0.2f, -normal.dot(light_dir));
                        vec3d color(0, 255 * brightness, 0);
                        
                        // 三角形を描画
                        fillTriangleFast(local_buffer, p1_screen, p2_screen, p3_screen, 
                                         p1.z, p2.z, p3.z, color);
                    }
                }
                
                // メインバッファにマージ
                std::lock_guard<std::mutex> lock(tasks_mutex);
                for (int y = 0; y < window_h; y++) {
                    for (int x = 0; x < window_w; x++) {
                        int idx = (y * window_w + x);
                        float depth = local_buffer.z_buffer[idx];
                        
                        if (depth < frame_buffer.z_buffer[idx]) {
                            frame_buffer.z_buffer[idx] = depth;
                            int color_idx = idx * 4;
                            for (int c = 0; c < 4; c++) {
                                frame_buffer.color_data[color_idx + c] = local_buffer.color_data[color_idx + c];
                            }
                        }
                    }
                }
            };
            
            // スレッドをビジー状態に
            {
                std::lock_guard<std::mutex> lock(tasks_mutex);
                thread_busy[t] = true;
            }
            task_cv.notify_one();
        }
        
        // すべてのスレッドの完了を待つ
        {
            std::unique_lock<std::mutex> lock(tasks_mutex);
            task_cv.wait(lock, [&]() {
                for (unsigned int t = 0; t < num_threads; t++) {
                    if (thread_busy[t]) return false;
                }
                return true;
            });
        }
        
        // フレームバッファをテクスチャに転送
        SDL_UpdateTexture(screen_texture, NULL, frame_buffer.color_data.data(), window_w * 4);
        
        // テクスチャを表示
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, screen_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        // 角度を更新
        anglex += 0.01;
        angley += 0.01;
        anglez += 0.01;
        
        SDL_Delay(16); // 約60FPS
    }
    
    return 0;
}