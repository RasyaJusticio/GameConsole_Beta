#pragma once

#include <Arduino.h>
#include "../Game.h"
#include "../Vector2.h"
#include "../../Engine.h"

class SnakeGame : public Game {
private:
	float GAME_INTERVAL = 0.1;
	static const int WIDTH = 16;
	static const int HEIGHT = 8;

	float mAdditionalInterval = 0;
	float mIntervalMultiplier = 0.02;

	float mTimeSinceLastMove;

	bool mIsGameOver;
	bool mIsRestarting;

	bool mIsFoodExists;

	unsigned char mBodyX[WIDTH * HEIGHT];
	unsigned char mBodyY[WIDTH * HEIGHT];

	int mSnakeLength;
	
	Vector2 mDirection;
	Vector2 mNextDirection;

	Vector2 mFoodPosition;

	int Sign(float value);

	void GetUserInput(Engine& engine);

	void HandleCollision(Vector2 headPosition, int bodyIndex);

	void MoveSnake(Vector2& newHeadPosition);

	void GrowSnake();

	void SpawnFood();

	void DrawSnake(Engine& engine);

	void DoGameOverSequence(Engine& engine);

public:
	SnakeGame();

	virtual bool IsGameOver();
	virtual void Update(Engine& engine);
};