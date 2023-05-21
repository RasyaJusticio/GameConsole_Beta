#pragma once

#include <Arduino.h>
#include <LedControl.h>
#include "src/Vector2.h"

class Engine 
{
private:
    const float JOYSTICK_TRESHOLD = 0.8;

    const int PIN_BUTTON = 12;
    const int PIN_X_AXIS = A5;
    const int PIN_Y_AXIS = A4;

    const int PIN_BUZZER = 7;

    const int PIN_CS = 8;
    const int PIN_DIN = 9;
    const int PIN_CLK = 10;
     
    LedControl mLedController = LedControl(PIN_DIN, PIN_CLK, PIN_CS, 2);

    unsigned char mDisplay0[8];
    unsigned char mDisplay1[8];

    int mButtonState;

    float Remap(float currentValue, float oldMin, float oldMax, float newMin, float newMax);
public:
    static const int DISPLAY_WIDTH = 16;
    static const int DISPLAY_HEIGHT = 8;

    float deltaTime;
    unsigned long time;

    float jyInputX;
    float jyInputY;
    bool isButtonDown;
    bool isButtonDownThisFrame;
    bool isButtonUpThisFrame;
    float buttonDownDuration;

    Engine();

    void Update(float deltaTime);

    void DrawToDisplay();

    void ClearDisplay();

    void SetPixel(int x, int y, boolean value = true);

    void SetPixel(Vector2 position, boolean value = true);

    void PlaySound(int frequency, int duration);

    // [0 - 15]
    void SetDisplayBrightness(int brightness);
};