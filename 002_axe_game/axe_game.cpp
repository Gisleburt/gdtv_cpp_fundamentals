#include "raylib.h"

int windowWidth = 640;
int windowHeight = 480;

class Circle {

protected:
    int x;
    int y;
    float radius;
    Color color;

public:
    Circle(int x, int y, float radius, Color color) {
        this->x = x;
        this->y = y;
        this->radius = radius;
        this->color = color;
    }

    void draw()
    {
        DrawCircle(x, y, radius, color);
    }

    void moveUp()
    {
        if (y - radius - 1 >= 0)
        {
            y -= 1;
        }
    }

    void moveDown()
    {
        if (y + radius + 1 <= windowHeight)
        {
            y += 1;
        }
    }

    void moveLeft()
    {
        if (x - radius - 1 >= 0)
        {
            x -= 1;
        }
    }

    void moveRight()
    {
        if (x + radius + 1 <= windowWidth)
        {
            x += 1;
        }
    }

    void handleInput()
    {
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
        {
            this->moveUp();
        }
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
            this->moveDown();
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            this->moveLeft();
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            this->moveRight();
        }
    }
};

int main()
{

    InitWindow(windowWidth, windowHeight, "Axe Game");
    Circle circle = Circle(20, 20, 20.0f, RED);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);
        circle.handleInput();
        circle.draw();
        EndDrawing();
    }
    CloseWindow();
}