#include "raylib.h"

const int windowWidth{640};
const int windowHeight{480};
const int fps{60};
const int meterInPixels{40};

class Sprite
{
    unsigned int height;
    unsigned int width;
    unsigned int nRows;
    unsigned int nColumns;

    unsigned int row{0};
    unsigned int column{0};

    unsigned int perImage;
    unsigned int frame{0};

    Texture2D texture;

    void nextFrame()
    {
        frame += 1;
        if (frame >= perImage)
        {
            frame = 0;
            nextImage();
        }
    }

    void nextImage()
    {
        column += 1;
        if (column >= nColumns)
        {
            column = 0;
            row += 1;
            if (row >= nRows)
            {
                row = 0;
            }
        }
    }

public:
    Sprite(const char *fileName, unsigned int rows, unsigned int columns, unsigned int framesPerImage)
    {
        perImage = framesPerImage;
        nRows = rows;
        nColumns = columns;
        texture = LoadTexture(fileName);
        height = texture.height / rows;
        width = texture.width / columns;
    }

    unsigned int getWidth()
    {
        return width;
    }

    unsigned int getHeight()
    {
        return height;
    }

    void draw(Vector2 pos)
    {
        nextFrame();
        Rectangle source;
        source.width = width;
        source.height = height;
        source.x = width * column;
        source.y = height * row;
        DrawTextureRec(texture, source, pos, WHITE);
    }
};

class Character
{
    float posX{0};
    float posY{0};

    float velocityUp = 0;
    bool onGround = false;


    const float gravity = (9.8 * 2) / fps;
    const float jumpForce = 13.0f;
    const float maxJumps = 2;
    int numJumps{maxJumps};

    const Color color = BLUE;
    Sprite sprite = Sprite("textures/scarfy.png", 1, 6, 10);
    int width;
    int height;

public:
    Character()
    {
        width = sprite.getWidth();
        height = sprite.getHeight();
    }

    void jump()
    {
        if (numJumps > 0) {
            numJumps--;
            velocityUp = jumpForce;
            onGround = false;
        }
    }

    void update()
    {
        velocityUp -= gravity;
        posY += velocityUp;
        if (posY <= 0)
        {
            posY = 0;
            velocityUp = 0;
            onGround = true;
            numJumps = maxJumps;
        }
    }

    void draw()
    {
        Vector2 pos = {
            posX, 
            (windowHeight - posY) - height
         };
        sprite.draw(pos);
    }
};

class Controller
{
    Character *character;

public:
    Controller(Character *character)
    {
        this->character = character;
    }

    void checkInput()
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            character->jump();
        }
    }
};

int main()
{
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");
    SetTargetFPS(fps);
    Character character = Character();
    Controller controller = Controller(&character);

    while (!WindowShouldClose())
    {
        controller.checkInput();
        character.update();

        BeginDrawing();
        ClearBackground(WHITE);

        character.draw();

        EndDrawing();
    }
    CloseWindow();
}