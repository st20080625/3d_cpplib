// C:\SDL3-3.2.12\include\SDL3
// C:\SDL3-3.2.12\lib\x86
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "3d.hpp"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

using namespace std;
using namespace cpp3d;

void DrawCircle(SDL_Renderer *renderer, float centerX, float centerY, int radius)
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

vec2d calc_coords(const vec3d &a, int center_x, int center_y)
{
    float scale = 500 / (500 + a.z);
    float x = (a.x / scale);
    float y = (-a.y / scale);
    x += center_x;
    y += center_y;
    return vec2d(x, y);
}

int main()
{
    const int window_w = 1280;
    const int window_h = 720;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return 1;
    }

    //  SDLのWindowの作成
    SDL_Window *window = SDL_CreateWindow("Hello SDL", window_w, window_h, 0);
    if (!window)
    {
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
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

    float angle1 = 0.0f;
    float angle2 = 0.0f;
    float angle3 = 0.0f;

    float x = 0.0f;
    float y = 0.0f;
    vec3d a = vec3d(0.0, 0.0, 0.0);
    vec3d b = vec3d(100.0, 0.0, 0.0);
    vec3d c = vec3d(0.0, 100.0, 0.0);
    vec3d d = vec3d(0.0, 0.0, 100.0);
    vec3d e = vec3d(100.0, 0.0, 100.0);
    vec3d f = vec3d(100.0, 100.0, 0.0);
    vec3d g = vec3d(0.0, 100.0, 100.0);
    vec3d h = vec3d(100.0, 100.0, 100.0);
    vector<vec3d> coords = {a, b, c, d, e, f, g, h};
    vector<vec2d> coords_2d(8, vec2d(0, 0));
    while (true)
    {
        //  システムイベントの処理
        while (SDL_PollEvent(&ev))
        {
            //  ウィンドウを閉じる場合
            if (ev.type == SDL_EVENT_QUIT)
                return 0;
        }
        // 画面の描画
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // DrawCircle(renderer, x, y, 100);
        // SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        // SDL_RenderPoint(renderer, centerX, centerY);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        vector<vec3d> rotated_coords(8, vec3d(0, 0, 0));
        for(size_t i = 0; i < coords.size(); i++){
           rotated_coords[i] = rotate_with_quaternion_with_three_angles(coords[i], angle1, angle2, angle3);
           coords_2d[i] = calc_coords(rotated_coords[i], centerX, centerY);
        }  
            /*for(size_t i = 0; i < coords.size(); i++){
            coords[i].reset();
            rotated_coords[i] = move(coords[i],vec3d(100.0f, 100.0f, 100.0f), 0.0, 1.0f/2.0f * M_PI, 0.0);
            coords_2d[i] = calc_coords(rotated_coords[i], centerX, centerY);
        }*/
        SDL_RenderLine(renderer, coords_2d[0].x, coords_2d[0].y, coords_2d[1].x, coords_2d[1].y);
        SDL_RenderLine(renderer, coords_2d[0].x, coords_2d[0].y, coords_2d[2].x, coords_2d[2].y);
        SDL_RenderLine(renderer, coords_2d[0].x, coords_2d[0].y, coords_2d[3].x, coords_2d[3].y);
        SDL_RenderLine(renderer, coords_2d[1].x, coords_2d[1].y, coords_2d[4].x, coords_2d[4].y);
        SDL_RenderLine(renderer, coords_2d[1].x, coords_2d[1].y, coords_2d[5].x, coords_2d[5].y);
        //SDL_RenderLine(renderer, coords_2d[2].x, coords_2d[2].y, coords_2d[4].x, coords_2d[4].y);
        SDL_RenderLine(renderer, coords_2d[2].x, coords_2d[2].y, coords_2d[6].x, coords_2d[6].y);
        SDL_RenderLine(renderer, coords_2d[2].x, coords_2d[2].y, coords_2d[5].x, coords_2d[5].y);
        SDL_RenderLine(renderer, coords_2d[3].x, coords_2d[3].y, coords_2d[4].x, coords_2d[4].y);
        SDL_RenderLine(renderer, coords_2d[3].x, coords_2d[3].y, coords_2d[6].x, coords_2d[6].y);
        SDL_RenderLine(renderer, coords_2d[4].x, coords_2d[4].y, coords_2d[7].x, coords_2d[7].y);
        SDL_RenderLine(renderer, coords_2d[5].x, coords_2d[5].y, coords_2d[7].x, coords_2d[7].y);
        SDL_RenderLine(renderer, coords_2d[6].x, coords_2d[6].y, coords_2d[7].x, coords_2d[7].y);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 約60FPS
        angle1 += 0.01;
        angle2 += 0.01;
        angle3 += 0.01;
    }

    // ループ終了時:作成したSDL関連のものを破棄する
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}