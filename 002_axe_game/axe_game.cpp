#include "raylib.h"

int windowWidth = 640;
int windowHeight = 480;

class Circle {

protected:
    int posX;
    int posY;
    float radius;
    Color color;
    int speed;

public:
    Circle(int posX, int posY, float radius, Color color, int speed) {
        this->posX = posX;
        this->posY = posY;
        this->radius = radius;
        this->color = color;
        this->speed = speed;
    }

    void draw()
    {
        DrawCircle(posX, posY, radius, color);
    }

    void moveUp()
    {
        if (posY - radius - speed >= 0)
        {
            posY -= speed;
        }
    }

    void moveDown()
    {
        if (posY + radius + speed <= windowHeight)
        {
            posY += speed;
        }
    }

    void moveLeft()
    {
        if (posX - radius - speed >= 0)
        {
            posX -= speed;
        }
    }

    void moveRight()
    {
        if (posX + radius + speed <= windowWidth)
        {
            posX += speed;
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

    bool pointIsInside(int x, int y)
    {
        float distX = posX - x;
        float distY = posY - y;
        return (distX * distX) + (distY * distY) - (radius * radius) < 0;
    }
};

class Axe
{

protected:
    int posX;
    int posY;
    int width;
    int height;
    Color color;
    int speed;

public:
    Axe(int posX, int posY, int width, int height, Color color, int speed)
    {
        this->posX = posX;
        this->posY = posY;
        this->width = width;
        this->height = height;
        this->color = color;
        this->speed = speed;
    }

    void draw()
    {
        DrawRectangle(posX, posY, width, height, color);
    }

    void updatePositon()
    {
        if (posY + speed < 0 || posY + height + speed > windowHeight)
        {
            speed *= -1;
        }
        posY += speed;
    }

    bool hasCollided(Circle circle) {
        return circle.pointIsInside(posX,         posY)
            || circle.pointIsInside(posX,         posY + height)
            || circle.pointIsInside(posX + width, posY)
            || circle.pointIsInside(posX + width, posY + height);
    }
};

int main()
{
    InitWindow(windowWidth, windowHeight, "Axe Game");
    Circle circle = Circle(windowWidth / 2, windowHeight / 2, 20.0f, GREEN, 10);
    Axe axe = Axe(20, 20, 50, 50, RED, 10);

    SetTargetFPS(30);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);
        //axe.updatePositon();
        axe.draw();
        if (axe.hasCollided(circle)) {
            break;
        }
        circle.handleInput();
        circle.draw();

        EndDrawing();
    }
    CloseWindow();
}