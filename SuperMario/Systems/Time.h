#pragma once

class Time
{
	DECLARE_SINGLETON(Time);

public:
	void Update();

	float GetDeltaTime() const { return (float)deltaTime; }
	UINT GetFPS() const { return fps; }
	double GetWorldTime() const { return worldTime; }

	void CheckTime() const { cout << "FPS : " << fps << ", DeltaTime : " << deltaTime << ", WorldTime : " << worldTime << '\n'; }

private:
	chrono::steady_clock::time_point currentTime;
	chrono::steady_clock::time_point lastTime;
	double deltaTime = 0.0;

	UINT fps = 0;
	double worldTime = 0.0f;	// 총 실행시간

	// fps를 측정하기 위한 변수
	UINT frameCount = 0;
	double fpsTimeElpased = 0.0f;	// 경과한 시간
};