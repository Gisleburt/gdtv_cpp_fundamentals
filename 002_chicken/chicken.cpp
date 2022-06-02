#include "raylib.h"
#include <math.h>

int windowWidth = 640;
int windowHeight = 480;

struct Point
{
    int x;
    int y;
};

class Chicken
{

protected:
    int posX;
    int posY;
    float radius;
    Color color;
    int speed;

public:
    Chicken(int posX, int posY, float radius, Color color, int speed)
    {
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
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
        {
            this->moveDown();
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            this->moveLeft();
        }
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            this->moveRight();
        }
    }

    bool pointIsInside(int x, int y)
    {
        float distX = posX - x;
        float distY = posY - y;
        return (distX * distX) + (distY * distY) - (radius * radius) < 0;
    }

    Point getClosestPoint(Point to)
    {
        float distX = posX - to.x;
        float distY = posY - to.y;
        float distance = sqrt((distX * distX) + (distY * distY));

        Point point;
        point.x = posX - (distX / distance * radius);
        point.y = posY - (distY / distance * radius);
        return point;
    }

    Point getCentre()
    {
        Point centre;
        centre.x = posX;
        centre.y = posY;
        return centre;
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

    bool pointIsInside(int x, int y)
    {
        return x > posX
            && x < posX + width
            && y > posY
            && y < posY + height;
    }

    bool hasCollided(Chicken chicken)
    {
        Point centre;
        centre.x = posX + (width / 2);
        centre.y = posY + (height / 2);

        Point closestPointToChicken = chicken.getClosestPoint(centre);
        DrawCircle(closestPointToChicken.x, closestPointToChicken.y, 2.0f, YELLOW);

        return pointIsInside(closestPointToChicken.x, closestPointToChicken.y)
            || chicken.pointIsInside(posX, posY) 
            || chicken.pointIsInside(posX, posY + height) 
            || chicken.pointIsInside(posX + width, posY) 
            || chicken.pointIsInside(posX + width, posY + height);
    }

    int getLeftSide()
    {
        return posX;
    }

    int getRightSide()
    {
        return posX + width;
    }

    void speedUp()
    {
        if (speed > 0) {
            speed += 1;
        } else {
            speed -= 1;
        }
    }
};

class Game {

protected:
    Chicken chicken = Chicken(50, windowHeight / 2, 20.0f, GREEN, 10);
    Axe axe = Axe(windowWidth / 2 - 25, windowHeight / 2, 50, 50, RED, 10);
    int score = 0;
    bool gameIsOver = false;

    bool isOnRightSide = false;

    void init()
    {
        chicken = Chicken(50, windowHeight / 2, 20.0f, GREEN, 10);
        axe = Axe(windowWidth / 2 - 25, windowHeight / 2, 50, 50, RED, 10);
        score = 0;
        gameIsOver = false;
        isOnRightSide = false;
    }

public:
    Game()
    {
        init();
    }

    void update()
    {
        if (gameIsOver)
        {
            updateGameOver();
        }
        else
        {
            updateGame();
        }
    }

    void updateGame()
    {
        axe.updatePositon();

        chicken.handleInput();

        if (axe.hasCollided(chicken))
        {
            gameIsOver = true;
        }

        Point centre = chicken.getCentre();
        bool wasOnRightNowOnLeft = isOnRightSide && centre.x < axe.getLeftSide();
        bool wasOnLeftNowOnRight = !isOnRightSide && centre.x > axe.getRightSide();
        bool changedSides = wasOnRightNowOnLeft || wasOnLeftNowOnRight;
        if (changedSides)
        {
            isOnRightSide = !isOnRightSide;
            score++;
            axe.speedUp();
        }
    }

    void updateGameOver()
    {
        if (IsKeyDown(KEY_R)) {
            init();
        }
    }

    void draw()
    {
        if (gameIsOver)
        {
            drawGameOver();
        }
        else
        {
            drawGame();
        }
    }

    void drawGame()
    {
        ClearBackground(BLUE);
        axe.draw();
        chicken.draw();
        DrawText(TextFormat("Score: %03i", score), 10, 10, 20, BLACK);
    }

    void drawGameOver()
    {
        ClearBackground(WHITE);
        DrawText("Game Over", 50, 50, 58, BLACK);
        DrawText(TextFormat("Score: %03i", score), 50, 100, 48 , BLACK);
        DrawText("Press R to restart", 50, 150, 48, BLACK);
    }
};

int main()
{
    InitWindow(windowWidth, windowHeight, "Chicken");
    Game game = Game();

    SetTargetFPS(30);
    while (!WindowShouldClose())
    {
        BeginDrawing();

        game.update();
        game.draw();

        EndDrawing();
    }
    CloseWindow();
}