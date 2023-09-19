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
	TileMap(const Vector2& position, const Vector2& scale, float rotation, const wstring& tileSetPath = L"", UINT tileXCount = 0, UINT tileYCount = 0, UINT spacing = 40);

	void Update() override;
	void Render() override;

	void GUI();

private:
	void GenerateTileMap(UINT width, UINT height, UINT spacing);
	Tile* GetTile(const Vector2& unprojectedMousePos);

	void SaveTileMap(const wstring& path = L"");
	void LoadTileMap(const wstring& path = L"");

private:
	UINT width = 0, height = 0;
	UINT spacing = 0;	// 한개의 타일이 차지하는 공간

	vector<VertexTexture> vertices;
	vector<UINT>indices;
	unique_ptr<IndexBuffer> IB;

	vector<vector<Tile>> tiles;

	ComPtr<ID3D11ShaderResourceView> srv;

	unique_ptr<class TileSet> tileSet;
};
