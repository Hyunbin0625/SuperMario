#pragma once

struct Tile
{
	Vector2 pos;
	Vector2 uvStart;
	Vector2 uvEnd;
};

class TileMap : public Drawable
{
public:
	TileMap(const Vector2& position, const Vector2& scale, float rotation, Color backGroundColor, const wstring& tileSetPath = L"", UINT tileXCount = 0, UINT tileYCount = 0, UINT spacing = 40);

	void Update() override;
	void Render() override;

	void GUI();

public:
	void PlayerCollisions(shared_ptr<class Player> player);

private:
	void SetTileCollisions(Vector2 Index);
	void RegenerateTileCollision();
	void RegenerateBackGround();
	void GenerateTileMap(UINT width, UINT height, UINT spacing);
	void RegenerateTileMap();
	Tile* GetTile(const Vector2& unprojectedMousePos);

	void SaveTileMap(const wstring& path = L"");
	void LoadTileMap(const wstring& path = L"");

public:
	Vector2 GetTileIndex(const Vector2& position);
	Vector2 GetTilePos(const Vector2& tileIndex) { return tiles[tileIndex.x][tileIndex.y].pos; }
	Vector2 GetTileMapScale() { return (Vector2(width, height) * spacing); }
	int GetSpacing() { return (int)spacing; }

	bool GetIsRegenerate() { return bRegenerate; }

public:
	vector<shared_ptr<TileCollision>> GetTileCollisions() { return tileCollisions; }

private:
	UINT width = 0, height = 0;
	UINT spacing = 0;	// 한개의 타일이 차지하는 공간

	vector<VertexTexture> vertices;
	vector<UINT>indices;
	unique_ptr<IndexBuffer> IB;

	vector<vector<Tile>> tiles;

	ComPtr<ID3D11ShaderResourceView> srv;

	ComPtr<ID3D11BlendState> blendState;

	unique_ptr<class TileSet> tileSet;
	vector<shared_ptr<TileCollision>> tileCollisions;

	unique_ptr<ColorRect> backGround;
	Color color = Color();

	bool bCollision = false;
	bool bView = true;
	bool bRegenerate = false;
};
