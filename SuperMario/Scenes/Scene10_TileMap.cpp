#include "stdafx.h"
#include "Scene10_TileMap.h"

void Scene10::Init()
{
	tileMap = make_unique<TileMap>(Vector2(), Vector2(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 0.0f, L"_Textures/tiles.jpg", 10, 18);
}

void Scene10::Destroy()
{
	tileMap.reset();
}

void Scene10::Update()
{
	tileMap->Update();
}

void Scene10::PreRender()
{
}

void Scene10::Render()
{
	tileMap->Render();
}

void Scene10::PostRender()
{
	tileMap->GUI();
}
