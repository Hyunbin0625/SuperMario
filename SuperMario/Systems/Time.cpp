#include "stdafx.h"
#include "Time.h"

Time::Time()
{
	// 초기화
	lastTime = chrono::steady_clock::now();
}

Time::~Time() {}

void Time::Update()
{
	currentTime = chrono::steady_clock::now();	// 현재 시간을 나노초 단위로 반환하는 함수
	chrono::duration<double> delta = currentTime - lastTime;
	deltaTime = delta.count();

	lastTime = currentTime;
	worldTime += deltaTime;	// 흐른시간 누적

	fpsTimeElpased += deltaTime;	// deltaTime 누적
	++frameCount;

	if (fpsTimeElpased >= 1.0)
	{
		fps = frameCount;
		frameCount = 0;
		fpsTimeElpased = 0.0;

//		CheckTime();
	}
}