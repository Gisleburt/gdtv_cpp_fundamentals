#include "raylib.h"


int main()
{
    int width = 640;
    int height = 480;
    InitWindow(width, height, "Axe Game");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);
        EndDrawing();
    }
    CloseWindow();
}