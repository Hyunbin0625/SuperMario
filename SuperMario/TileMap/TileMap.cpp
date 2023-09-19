#include "stdafx.h"
#include "TileSet.h"
#include "TileMap.h"

TileMap::TileMap(const Vector2& position, const Vector2& scale, float rotation, const wstring& tileSetPath, UINT tileXCount, UINT tileYCount, UINT spacing)
	: Drawable("TileMap", position, scale, rotation, L"_Shaders/VertexTile.hlsl"), spacing(spacing)
{
	tileSet = make_unique<TileSet>(tileSetPath, tileXCount, tileYCount);

	width = (UINT)scale.x / spacing;
	height = (UINT)scale.y / spacing;

	GenerateTileMap(width, height, spacing);

	vertices.assign(4, VertexTexture());

	vertices[0].position = Vector2(0, 0);
	vertices[1].position = Vector2(0, (float)spacing);
	vertices[2].position = Vector2((float)spacing, 0);
	vertices[3].position = Vector2((float)spacing, (float)spacing);

	vertices[0].uv = Vector2(0, 1);
	vertices[1].uv = Vector2(0, 0);
	vertices[2].uv = Vector2(1, 1);
	vertices[3].uv = Vector2(1, 0);

	VB->Create(vertices, D3D11_USAGE_DYNAMIC);

	indices = { 0, 1, 2, 2, 1, 3 };
	IB = make_unique<IndexBuffer>();
	IB->Create(indices, D3D11_USAGE_IMMUTABLE);

	IL->Create(VertexTexture::descs, VS->GetBlob());

	srv = tileSet->srv;
}

void TileMap::Update()
{
	SUPER::Update();

	if (ImGui::IsAnyItemActive())
		return;

	if (INPUT->Press(VK_LBUTTON))
	{
		Tile* tile = GetTile(INPUT->GetMousePosition());

		if (tile)
		{
			tile->uvStart = tileSet->selectedStartUV;
			tile->uvEnd = tileSet->selectedStartUV + tileSet->texelTileSize;
		}
	}
}

void TileMap::Render()
{
	SUPER::Render();

	IB->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DC->PSSetShaderResources(0, 1, srv.GetAddressOf());

	for (UINT y = 0; y < height; ++y)
	{
		for (UINT x = 0; x < width; ++x)
		{
			const Tile& tile = tiles[y][x];

			D3D11_MAPPED_SUBRESOURCE subResource;

			HRESULT hr = DC->Map(VB->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
			CHECK(hr);
			{
				vertices[0].uv = Vector2(tile.uvStart.x, tile.uvEnd.y);
				vertices[1].uv = tile.uvStart;
				vertices[2].uv = tile.uvEnd;
				vertices[3].uv = Vector2(tile.uvEnd.x, tile.uvStart.y);

				memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());

			}
			DC->Unmap(VB->GetResource(), 0);

			SetWorld(XMMatrixTranslationFromVector(position + tile.pos));
			WB->SetVSBuffer(0);

			DC->DrawIndexed(IB->GetCount(), 0, 0);
		}
	}
}

void TileMap::GUI()
{
	tileSet->GUI();

	static bool bOpen = true;
	if (ImGui::Begin("TileMap", &bOpen))
	{
		if (ImGui::Button("Save", ImVec2(50, 30)))
			SaveTileMap();

		if (ImGui::Button("Load", ImVec2(50, 30)))
			LoadTileMap();
	}
	ImGui::End();
}

void TileMap::GenerateTileMap(UINT width, UINT height, UINT spacing)
{
	if (width == 0 || height == 0 || spacing == 0)
	{
		assert(false);
		return;
	}

	tiles.clear();
	tiles.assign(height, vector<Tile>());
	for (UINT y = 0; y < height; ++y)
	{
		tiles[y].assign(width, Tile());

		for (UINT x = 0; x < width; ++x)
			tiles[y][x].pos = Vector2((float)x, (float)y) * (float)spacing;
	}
}

Tile* TileMap::GetTile(const Vector2& unprojectedMousePos)
{
	UINT x = (UINT)unprojectedMousePos.x / spacing;
	UINT y = (UINT)unprojectedMousePos.y / spacing;

	if (x >= width || y >= height || x < 0 || y < 0)
		return nullptr;

	return &tiles[y][x];
}

void TileMap::SaveTileMap(const wstring& path)
{
	if (path.empty())
	{
		function<void(wstring)> func = bind(&TileMap::SaveTileMap, this, placeholders::_1);
		Path::SaveFileDialog(L"", Path::TileMapFilter, L"_Maps/", func, gHandle);
	}
	else
	{
		if (tiles.empty()) return;

		ofstream out(path.c_str());

		if (out.is_open())
		{
			out.write((char*)&width, sizeof(width));
			out.write((char*)&height, sizeof(height));
			out.write((char*)&spacing, sizeof(spacing));

			for (UINT i = 0; i < height; ++i)
				out.write((char*)tiles[i].data(), sizeof(tiles[0][0]) * tiles[i].size());
		}

		out.close();
	}
}

void TileMap::LoadTileMap(const wstring& path)
{
	if (path.empty())
	{
		function<void(wstring)> func = bind(&TileMap::LoadTileMap, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::TileMapFilter, L"_Maps/", func, gHandle);
	}
	else
	{
		ifstream in(path.c_str());

		if (in.is_open())
		{
			in.read((char*)&width, sizeof(width));
			in.read((char*)&height, sizeof(height));
			in.read((char*)&spacing, sizeof(spacing));
			GenerateTileMap(width, height, spacing);

			for (UINT i = 0; i < height; ++i)
				in.read((char*)tiles[i].data(), sizeof(tiles[0][0]) * tiles[i].size());
		}

		in.close();
	}
}
