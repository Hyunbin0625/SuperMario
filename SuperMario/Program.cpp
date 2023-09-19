#include "stdafx.h"
#include "Scenes/SceneList.h"
#include "Program.h"

Program::Program()
{
}

void Program::Init()
{
	sceneList.push_back(make_shared<Scene10>());

	currentScene = sceneList[0];
	currentScene->Init();
}

void Program::Update()
{
	CAMERA->Update();
	currentScene->Update();
}

void Program::PreRender()
{
	currentScene->PreRender();
}

void Program::Render()
{
	CAMERA->Render();
	currentScene->Render();
}

void Program::PostRender()
{
	currentScene->PostRender();
	IMGUI->PrintFrame();
}
