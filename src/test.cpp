#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "3d.hpp"
#include <iostream>
#include <cmath>
#include <vector>

void DrawCircle(SDL_Renderer* renderer, float centerX, float centerY, int radius)
{
    int offsetX = radius - 1;
    int offsetY = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);
    centerX -= radius;
    while (offsetX >= offsetY)
    {
        // 円の8方向に対称な点を描画
        SDL_RenderPoint(renderer, centerX + offsetX, centerY + offsetY);
        SDL_RenderPoint(renderer, centerX + offsetY, centerY + offsetX);
        SDL_RenderPoint(renderer, centerX - offsetY, centerY + offsetX);
        SDL_RenderPoint(renderer, centerX - offsetX, centerY + offsetY);
        SDL_RenderPoint(renderer, centerX - offsetX, centerY - offsetY);
        SDL_RenderPoint(renderer, centerX - offsetY, centerY - offsetX);
        SDL_RenderPoint(renderer, centerX + offsetY, centerY - offsetX);
        SDL_RenderPoint(renderer, centerX + offsetX, centerY - offsetY);

        if (err <= 0)
        {
            offsetY++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0)
        {
            offsetX--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

vec2d calc_coords(const vec3d &a, window_w, window_h) const{
    float scale = 500/(500 + a.z);
    float x = a.x / scale + window_w / 2;
    float y = a.y / scale + window_w / 2;
    x *= 100;
    y *= 100;
    return vec2d(x, y);
}

int main()
{
    const int window_w = 1280;
    const int window_h = 720;

    if(!SDL_Init(SDL_INIT_VIDEO)){
        return 1;
    }

    //  SDLのWindowの作成
    SDL_Window *window = SDL_CreateWindow("Hello SDL", window_w, window_h,0);
    if(!window){
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if(!renderer){
        return 1;
    }

    // ループ開始前に一度画面を描画する
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderPresent(renderer);

    SDL_Event ev;
    //  メインループ
    const float centerX = window_w / 2.0f;
    const float centerY = window_h / 2.0f;
    const float radius = 100.0f;
    double angle = 0.0;
    float x = 0.0f;
    float y = 0.0f;
    vec3d a = vec3d(1.0, 0.0, 0.0);
    vec3d b = vec3d(0.0, 1.0, 0.0);
    vec3d c = vec3d(0.0, 0.0, 1.0);
    vec3d d = vec3d(1.0, 0.0, 1.0);
    vec3d e = vec3d(1.0, 1.0, 0.0);
    vec3d f = vec3d(0.0, 1.0, 0.0);
    vec3d g = vec3d(0.0, 1.0, 1.0);
    vec3d h = vec3d(1.0, 1.0, 1.0);
    vector<vector<float>> coords = {
        {a},
        {b},
        {c},
        {d},
        {e},
        {f},
        {g},
        {h}
    }
    while (true)
    {
        //  システムイベントの処理
        while (SDL_PollEvent(&ev))
        {
            //  ウィンドウを閉じる場合
            if (ev.type == SDL_EVENT_QUIT)
                return 0;
        }
        x = centerX + radius * cos(angle) + radius;
        y = centerY + radius * sin(angle);
        // 画面の描画
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        DrawCircle(renderer, x, y, 100);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderPoint(renderer, centerX, centerY);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        for(int i = 0; i < coords.size(); i++){
            vec2d coord = calc_coords(coords[i], window_w, window_h);
            SDL_RenderPoint(renderer, coord.x, coord.y);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 約60FPS
        angle += 0.01;
    }

    // ループ終了時:作成したSDL関連のものを破棄する
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}

