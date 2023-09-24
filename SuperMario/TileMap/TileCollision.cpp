#include "stdafx.h"
#include "TileCollision.h"

TileCollision::TileCollision(Vector2 tileIndex)
	: tileIndex(tileIndex)
{
	lineRect = make_shared<LineRect>(Vector2(), Vector2(), 0.0f, GREEN);

	// Component
	lineRect->AddComponent(make_shared<ColliderComponent>(ColliderType::RECT));
}

void TileCollision::Resize(Tile tile, float spacing)
{
	lineRect->SetPosition(tile.pos + Vector2(spacing) / 2);
	lineRect->SetScale(Vector2(spacing, spacing));
}

void TileCollision::Update()
{
	lineRect->Update();
}

void TileCollision::Render()
{
	lineRect->Render();
}
