#pragma once

extern class Tile;
class TileCollision
{
public:
	TileCollision(Vector2 tileIndex);

	void Resize(Tile tile, float spacing);

	void Update();
	void Render();

public:
	shared_ptr<LineRect> GetLineRect() { return lineRect; }
	Vector2 GetTileIndex() { return tileIndex; }

private:
	Vector2 tileIndex = Vector2();

	shared_ptr<LineRect> lineRect;
};

