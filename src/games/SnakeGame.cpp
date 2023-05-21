#include "SnakeGame.h"

SnakeGame::SnakeGame() {
	mTimeSinceLastMove = 0;
	
	mIsGameOver = false;
	mIsRestarting = false;

	mIsFoodExists = false;

	mBodyX[0] = 0;
	mBodyY[0] = 0;

	mSnakeLength = 1;

	mDirection = { 0, 0 };
	mNextDirection = mDirection;
}

int SnakeGame::Sign(float value) {
	if (value == 0) { return; }

	return (value < 0) ? -1 : 1;
}

void SnakeGame::GetUserInput(Engine& engine)
{
	if (engine.jyInputX != 0 || engine.jyInputY != 0) {
		if (abs(engine.jyInputX) > abs(engine.jyInputY)) {
			int inputDirection = Sign(engine.jyInputX);

			if (inputDirection != -mDirection.x)
			{
				mNextDirection.x = inputDirection;
				mNextDirection.y = 0;
			}
		} else {
			int inputDirection = Sign(engine.jyInputY);

			if (inputDirection != -mDirection.y)
			{
				mNextDirection.y = inputDirection;
				mNextDirection.x = 0;
			}
		}
	}
}

void SnakeGame::HandleCollision(Vector2 newHeadPosition, int bodyIndex)
{
	if (newHeadPosition.x == mBodyX[bodyIndex] && newHeadPosition.y == mBodyY[bodyIndex])
	{
		mIsGameOver = true;
		mIsRestarting = true;
	}
}

void SnakeGame::MoveSnake(Vector2& newHeadPosition)
{
	for (int i = mSnakeLength - 1; i > 0; i--)
	{
		mBodyX[i] = mBodyX[i - 1];
		mBodyY[i] = mBodyY[i - 1];

		HandleCollision(newHeadPosition, i);
	}

	newHeadPosition.x = (newHeadPosition.x < 0) ? WIDTH - 1 : newHeadPosition.x;
	newHeadPosition.x = (newHeadPosition.x >= WIDTH) ? 0 : newHeadPosition.x;
	newHeadPosition.y = (newHeadPosition.y < 0) ? HEIGHT - 1 : newHeadPosition.y;
	newHeadPosition.y = (newHeadPosition.y >= HEIGHT) ? 0 : newHeadPosition.y;

	mBodyX[0] = newHeadPosition.x;
	mBodyY[0] = newHeadPosition.y;
}

void SnakeGame::GrowSnake()
{
	Vector2 nextPointDirection = { -mDirection.x, -mDirection.y };
	
	if (mSnakeLength > 1)
	{
		nextPointDirection.x = Sign((int)mBodyX[mSnakeLength - 1] - (int)mBodyX[mSnakeLength - 2]);
		nextPointDirection.y = Sign((int)mBodyY[mSnakeLength - 1] - (int)mBodyY[mSnakeLength - 2]);
	}

	Serial.println();
	Serial.println("Calculated next body point:");
	Serial.println((int)mBodyX[mSnakeLength - 1] + nextPointDirection.x);
	Serial.println((int)mBodyY[mSnakeLength - 1] + nextPointDirection.y);

	mBodyX[mSnakeLength] = mBodyX[mSnakeLength - 1] + nextPointDirection.x;
	mBodyY[mSnakeLength] = mBodyY[mSnakeLength - 1] + nextPointDirection.y;

	Serial.println();
	Serial.println("Actual next body point:");
	Serial.println((int)mBodyX[mSnakeLength]);
	Serial.println((int)mBodyY[mSnakeLength]);

	mSnakeLength++;
}

void SnakeGame::SpawnFood()
{
	const int numTiles = WIDTH * HEIGHT;
	int randomIndex = random(0, numTiles);

	bool occupancyMap[numTiles] = { false };
	for (int i = 0; i < mSnakeLength; i++)
	{
		int bodyIndex = mBodyY[i] * WIDTH + mBodyX[i];
		occupancyMap[bodyIndex] = true;
	}

	for (int i = 0; i < numTiles; i++)
	{
		if (occupancyMap[randomIndex])
		{
			randomIndex = (randomIndex + 1) % numTiles;
		} else
		{
			mFoodPosition = { randomIndex % WIDTH, randomIndex / WIDTH };
			mIsFoodExists = true;
			return;
		}
	}
}

void SnakeGame::DrawSnake(Engine& engine)
{
	for (int i = 0; i < mSnakeLength; i++)
	{
		engine.SetPixel((int)mBodyX[i], (int)mBodyY[i]);
	}
}

void SnakeGame::DoGameOverSequence(Engine& engine)
{
	delay(1000);

	for (int x = 0; x < 3; x++)
	{
		engine.ClearDisplay();
		engine.DrawToDisplay();

		delay(150);

		for (int i = 0; i < mSnakeLength; i++)
		{
			engine.SetPixel((int)mBodyX[i], (int)mBodyY[i], true);
		}
		engine.DrawToDisplay();
		engine.PlaySound(500, 150);

		delay(150);
	}

	mIsRestarting = false;

}

bool SnakeGame::IsGameOver()
{
	return mIsGameOver && !mIsRestarting;
}

void SnakeGame::Update(Engine& engine) {
	mTimeSinceLastMove += engine.deltaTime;

	if (mIsGameOver)
	{
		DoGameOverSequence(engine);

		return;
	}

	GetUserInput(engine);

	if (mTimeSinceLastMove > (GAME_INTERVAL - mAdditionalInterval)) {
		mTimeSinceLastMove -= (GAME_INTERVAL - mAdditionalInterval);

		mDirection = mNextDirection;

		Vector2 newHeadPosition = { mBodyX[0] + mDirection.x, mBodyY[0] + mDirection.y };

		MoveSnake(newHeadPosition);

		if (mIsFoodExists)
		{
			if (mBodyX[0] == mFoodPosition.x && mBodyY[0] == mFoodPosition.y)
			{
				mIsFoodExists = false;

				GrowSnake();

				if (mSnakeLength % 5 == 0)
				{
					mAdditionalInterval += mIntervalMultiplier;
					engine.PlaySound(2000, 1000);
				} else
				{
					engine.PlaySound(1000, 250);
				}
			}
		}

		DrawSnake(engine);

		if (mIsFoodExists)
		{
			engine.SetPixel(mFoodPosition);
		} else if (!mIsFoodExists && mDirection != 0)
		{
			SpawnFood();
		}
	}
}
