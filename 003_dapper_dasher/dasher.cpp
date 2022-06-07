#include "raylib.h"
#include <cmath>
#include <vector>

const int windowWidth{640};
const int windowHeight{480};

class GameObject
{
public:
    virtual void update(float deltaTime) = 0;
    virtual void draw() = 0;
};

class Sprite
{
    unsigned int height;
    unsigned int width;
    unsigned int nRows;
    unsigned int nColumns;
    unsigned int nFrames;

    unsigned int frame;

    float seconds{0};
    float secondsPerFrame;

    Texture2D texture;

public:
    Sprite(const char *fileName, unsigned int rows, unsigned int columns, unsigned int frames, unsigned int fps)
    {
        secondsPerFrame = 1.0f/fps;
        nRows = rows;
        nColumns = columns;
        nFrames = frames;
        texture = LoadTexture(fileName);
        height = texture.height / rows;
        width = texture.width / columns;
    }

    ~Sprite()
    {
        UnloadTexture(texture);
    }

    unsigned int getWidth()
    {
        return width;
    }

    unsigned int getHeight()
    {
        return height;
    }

    void update(float deltaTime)
    {
        seconds += deltaTime;
        if (seconds > secondsPerFrame)
        {
            seconds = fmod(seconds, secondsPerFrame);
            frame += 1;
            frame = frame % nFrames;
        }
    }

    Rectangle framePos()
    {
        unsigned int column = frame % nColumns;
        unsigned int row = frame / nRows;
        return {
            (float)(width * column),
            (float)(height * row),
            (float)width,
            (float)height,
        };
    }

    void draw(Vector2 pos)
    {
        DrawTextureRec(texture, framePos(), pos, WHITE);
    }
};

class Nebula : GameObject
{
    Vector2 pos {100.0, 100.0};
    Vector2 velocity {0.0, 0.0};

    Sprite sprite = Sprite("textures/12_nebula_spritesheet.png", 8, 8, 61, 61);
    unsigned int width = sprite.getWidth();
    unsigned int height = sprite.getWidth();

public:
    Nebula(float posX, float posY, float velocityX, float velocityY)
    {
        pos = {posX, posY};
        velocity = {velocityX, velocityY};
    }

    void update(float deltaTime)
    {
        pos.x += velocity.x * deltaTime;
        pos.y += velocity.y * deltaTime;
        sprite.update(deltaTime);
    }

    void draw()
    {
        Vector2 alteredPos = {
            pos.x,
            (windowHeight - pos.y) - height
        };
        sprite.draw(alteredPos);
    }
};

class Character : GameObject
{
    float posX{0};
    float posY{0};

    float velocityUp = 0;
    bool onGround = false;

    const float forceScale = 50.0f;
    const float gravity = 9.8f * forceScale;
    const float jumpForce = 8.0f * forceScale;
    const unsigned int maxJumps = 2; // 0 for no jumping, 1 for normal jumping, 2 or more for air jumping
    unsigned int numJumps{maxJumps};
    const float jumpGracePeriod{0.3};
    float jumpGraceTimer{0.0};

    Sprite sprite = Sprite("textures/scarfy.png", 1, 6, 6, 12);
    unsigned int width = sprite.getWidth();
    unsigned int height = sprite.getWidth();

public:
    void jump()
    {
        if (numJumps > 0) {
            numJumps--;
            velocityUp = jumpForce;
            onGround = false;
        } else {
            jumpGraceTimer = jumpGracePeriod; // Allow jumping if the player pressed the jump button _just_ before the ground
        }
    }

    void update(float deltaTime)
    {
        velocityUp -= gravity * deltaTime;
        posY += velocityUp * deltaTime;

        if (posY <= 0)
        {
            sprite.update(deltaTime);
            posY = 0;
            velocityUp = 0;
            onGround = true;
            numJumps = maxJumps;
        }

        // Jump delayed while being in the air
        if (jumpGraceTimer > 0)
        {
            jumpGraceTimer -= deltaTime; // doesn't matter if it goes negative
            if (onGround)
            {
                jump();
            }
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

class Game
{
    Character character = Character();
    Controller controller = Controller(&character);
    std::vector<Nebula> nebulas{};
    unsigned int score{0};
    const float gameSpeed = 300;

public:
    Game(unsigned int targetFps)
    {
        SetTargetFPS(targetFps);
    }

    ~Game()
    {
        CloseWindow();
    }

    void tick()
    {
        controller.checkInput();
        float deltaTime = GetFrameTime();
        
        BeginDrawing();
        ClearBackground(WHITE);

        character.update(deltaTime);
        character.draw();
        for (Nebula nebula : nebulas)
        {
            nebula.update(deltaTime);
            nebula.update(deltaTime);
        }

        EndDrawing();
    }

    bool shouldContinue()
    {
        return !WindowShouldClose();
    }
};

int main()
{
    const unsigned int fps{60};
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");
    Game game = Game(fps);
    while (game.shouldContinue())
    {
        game.tick();
    }
}