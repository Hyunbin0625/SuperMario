#pragma once

class Program
{
public:
	Program();
	~Program() = default;

public:
	void Init();

	void Update();
	void PreRender();
	void Render();
	void PostRender();

private:
	vector<shared_ptr<IScene>> sceneList;
	shared_ptr<IScene> currentScene;

	int currentIndex = 0;
};