#include "HardwareSerial.h"
#include "Engine.h"

float Engine::Remap(float currentValue, float oldMin, float oldMax, float newMin, float newMax) 
{
	return newMin + (currentValue - oldMin) / (oldMax - oldMin) * (newMax - newMin);
}

Engine::Engine() 
{
	mLedController.shutdown(0, false);
	mLedController.shutdown(1, false);

	pinMode(PIN_BUTTON, INPUT);
	pinMode(PIN_BUZZER, OUTPUT);

	ClearDisplay();
	time = 0;
}

void Engine::Update(float deltaTime) 
{
	this->deltaTime = deltaTime;
	time += deltaTime;

	// Getting button input:
	int oldButtonState = mButtonState;
	mButtonState = digitalRead(PIN_BUTTON);
	isButtonDown = mButtonState == 1;
	isButtonDownThisFrame = isButtonDown && mButtonState != oldButtonState;
	isButtonUpThisFrame = mButtonState == 0 && oldButtonState == 1;

	if (isButtonDownThisFrame) 
	{
		buttonDownDuration = 0;
	}
	if (isButtonDown) 
	{
		buttonDownDuration += deltaTime;
	}

	// Getting joystick input:
	jyInputX = Remap(analogRead(PIN_X_AXIS), 0, 1023, -1, 1);
	jyInputY = Remap(analogRead(PIN_Y_AXIS), 0, 1023, -1, 1);

	if (abs(jyInputX) < JOYSTICK_TRESHOLD) 
	{
		jyInputX = 0;
	}
	if (abs(jyInputY) < JOYSTICK_TRESHOLD) 
	{
		jyInputY = 0;
	}
}

void Engine::DrawToDisplay() 
{
	for (int row = 0; row < 8; row++) 
	{
		mLedController.setRow(0, row, mDisplay0[row]);
		mLedController.setRow(1, row, mDisplay1[row]);
	}
}

void Engine::ClearDisplay() 
{
	for (int row = 0; row < 8; row++) 
	{
		mDisplay0[row] = 0;
		mDisplay1[row] = 0;
	}
}

void Engine::SetPixel(int x, int y, boolean value)
{
	if ((x < 0 || x >= DISPLAY_WIDTH) || (y < 0 || y >= DISPLAY_HEIGHT)) 
	{
		Serial.print("ERROR : `.SetPixel()` parameters is outside of the boundaries!\n");
		return;
	}
	
	if (x < 8) // mDisplay0[]
	{ 
		x = 7 - x;
		y = 7 - y;

		if (value) 
		{
			mDisplay0[x] |= 1 << y;
		} else 
		{
			mDisplay0[x] &= ~(1 << y);
		}
	} else // mDisplay1[]
	{ 
		if (value) 
		{
			mDisplay1[x - 8] |= 1 << y;
		} else 
		{
			mDisplay1[x - 8] &= ~(1 << y);
		}
	}
}

void Engine::SetPixel(Vector2 position, boolean value) 
{
	this->SetPixel(position.x, position.y, value);
}

void Engine::PlaySound(int frequency, int duration) 
{
	tone(PIN_BUZZER, frequency, duration);
}

void Engine::SetDisplayBrightness(int brightness) 
{
	mLedController.setIntensity(0, brightness);
	mLedController.setIntensity(1, brightness);
}
