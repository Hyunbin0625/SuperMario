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

	void SaveFile(ofstream& out);
	void LoadFile(ifstream& in);

public:
	void PlayerCollisions(shared_ptr<class Player> player);

private:
	void SetTileCollisions(Vector2 Index);
	void RegenerateTileCollision();
	void RegenerateBackGround();
	void GenerateTileMap(UINT width, UINT height, UINT spacing);
	void RegenerateTileMap();
	Tile* GetTile(const Vector2& unprojectedMousePos);

public:
	Vector2 GetTileIndex(const Vector2& position);
	Vector2 GetTilePos(const Vector2& tileIndex) { return tiles[tileIndex.x][tileIndex.y].pos; }
	Vector2 GetTileMapScale() { return (Vector2(width, height) * spacing); }
	int GetSpacing() { return (int)spacing; }
	bool GetIsRegenerate() { return bRegenerate; }

	void SetIsRegenerate(bool bRegenerate) { this->bRegenerate = bRegenerate; }

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

	bool bCollision = true;
	bool bView = true;
	bool bRegenerate = false;
};
