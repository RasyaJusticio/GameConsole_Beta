#include "Engine.h"
#include "src/Game.h"
#include "src/games/TestGame.h"
#include "src/games/SnakeGame.h"

const bool showStartupSequence = false;

const unsigned long TARGET_DELAY = 16;
unsigned long oldTime;

int activeGameIndex = 0; // 0: SnakeGame, 1: SpaceGame

Engine engine = Engine();
Game* game = new SnakeGame();
    
void setup() 
{
	Serial.begin(9600);

	engine.SetDisplayBrightness(1);

    if (showStartupSequence) 
    {
        StartupSequence();
    }

	oldTime = millis();

    Serial.print("\nSetup is over\n");
}

void loop() 
{
    unsigned long frameStartTime = millis();
    unsigned long deltaTimeMs = frameStartTime - oldTime;
    float deltaTime = deltaTimeMs / 1000.0;

    oldTime = frameStartTime;

    // Update
    engine.ClearDisplay();
    engine.Update(deltaTime);
    HandleGameRestarting();
    HandleGames(deltaTime);
    engine.DrawToDisplay();

    unsigned long endTime = millis();
    unsigned long frameTime = endTime - frameStartTime;

    if (frameTime < TARGET_DELAY) 
    {
        unsigned long waitFPSTime = TARGET_DELAY - frameTime;
        delay(waitFPSTime);
    }
}

void HandleGames(float deltaTime) 
{
    game->Update(engine);
}

void HandleGameRestarting()
{
    if (game->IsGameOver())
    {
        delete game;

        delay(1000);

        switch (activeGameIndex)
        {
            case -1:
                game = new TestGame;
            case 0:
                game = new SnakeGame;
            default:
                break;
        }
    }
}

void StartupSequence() 
{
    // Frame 1
    for (int y = 0; y < 8; y++) 
    {
        for (int x = 0; x < 16; x++) 
        {
            if (!(x <= y || x - 8 >= y)) 
            {
                engine.SetPixel(x, y);
            }
        }
    }
    engine.DrawToDisplay();
    engine.PlaySound(330, 240);
    delay(320);

    // Frame 2
    for (int y = 0; y < 8; y++) 
    {
        for (int x = 0; x < 16; x++) 
        {
            if (x <= y) {
                engine.SetPixel(x, y);
            }
        }
    }
    engine.DrawToDisplay();
    engine.PlaySound(392, 240);
    delay(320);

    // Frame 3
    for (int y = 0; y < 8; y++) 
    {
        for (int x = 0; x < 16; x++) 
        {
            engine.SetPixel(x, y);
        }
    }
    engine.DrawToDisplay();
    engine.PlaySound(494, 640);
    delay(700);

    // Transition out
    for (int i = 0; i < 16; i++) 
    {
        for (int x = 0; x < 16; x++) 
        {
            for (int y = 0; y < 8; y++) 
            {
                if ((x + (8 - y) <= i || 16 - x + y <= i)) 
                {
                    engine.SetPixel(x, y, false);
                }
            }
        }
        engine.DrawToDisplay();
        delay(35);
    }
    delay(30);
}
