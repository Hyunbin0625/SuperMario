#include "stdafx.h"
#include "TileSet.h"
#include "TileMap.h"

TileMap::TileMap(const Vector2& position, const Vector2& scale, float rotation, Color backGroundColor, const wstring& tileSetPath, UINT tileXCount, UINT tileYCount, UINT spacing)
	: Drawable("TileMap", position, scale, rotation, L"_Shaders/VertexTile.hlsl"), color(backGroundColor), spacing(spacing)
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

	// CreateBlendState
	{
		D3D11_BLEND_DESC desc;
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;

		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = DEVICE->CreateBlendState(&desc, &blendState);
		CHECK(hr);
	}

	tileCollisions.reserve(10);

	backGround = make_unique<ColorRect>(scale * 0.5f, scale, 0.0f, color);
}

void TileMap::Update()
{
	SUPER::Update();

	if (ImGui::IsAnyItemActive())
		return;

	if (INPUT->Press(VK_LBUTTON) && INPUT->Press(VK_SPACE))
	{
		Tile* tile = GetTile(INPUT->GetMousePosition());

		if (tile)
		{
			tile->uvStart = tileSet->selectedStartUV;
			tile->uvEnd = tileSet->selectedStartUV + tileSet->texelTileSize;

			if (bCollision)
				tileCollisions.back()->Resize(*tile, spacing);
		}
	}

	backGround->Update();
	for (auto& tileCollision : tileCollisions)
		tileCollision->Update();
}

void TileMap::Render()
{
	backGround->Render();

	SUPER::Render();

	IB->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DC->PSSetShaderResources(0, 1, srv.GetAddressOf());
	DC->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

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

	if (bView)
	{
		for (auto& tileCollision : tileCollisions)
			tileCollision->Render();
	}

	DC->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void TileMap::GUI()
{
	tileSet->GUI();

	if (ImGui::BeginTabItem("MapDetails"))
	{
		if (ImGui::InputInt("Width", (int*)&width))
		{
			RegenerateTileMap();
			RegenerateTileCollision();
			RegenerateBackGround();
			bRegenerate = true;
		}
		if (ImGui::InputInt("Height", (int*)&height))
		{
			RegenerateTileMap();
			RegenerateTileCollision();
			RegenerateBackGround();
			bRegenerate = true;
		}
		if (ImGui::InputInt("Spacing", (int*)&spacing))
		{
			RegenerateTileMap();
			RegenerateTileCollision();
			RegenerateBackGround();
			bRegenerate = true;
		}

		string name = "BackGroundColor(0~1)";
		ImGui::Text(name.c_str());
		if (ImGui::InputFloat("R", &color.x, 0.1f, 1.0f))
			backGround->GET_COMP(Color)->SetColor(color);
		if (ImGui::InputFloat("G", &color.y, 0.1f, 1.0f))
			backGround->GET_COMP(Color)->SetColor(color);
		if(ImGui::InputFloat("B", &color.z, 0.1f, 1.0f))
			backGround->GET_COMP(Color)->SetColor(color);

		ImGui::Checkbox("bCollision", &bCollision);
		ImGui::Checkbox("bView", &bView);

		ImGui::EndTabItem();
	}
	
	// 나중에 이동
//	if (ImGui::Begin("TileMap", &bOpen))
//	{
//		if (ImGui::Button("Save", ImVec2(50, 30)))
//			SaveTileMap();
//
//		if (ImGui::Button("Load", ImVec2(50, 30)))
//			LoadTileMap();
//	}
//	ImGui::End();
}

void TileMap::PlayerCollisions(shared_ptr<Player> player)
{
	bool bGround = false;
	for (auto& tileCollision : tileCollisions)
	{
		if (player->GetAnimRect()->GET_COMP(Collider)->Intersect(tileCollision->GetLineRect()->GET_COMP(Collider)))
		{
			Collision::RECT playerRect(player->GetAnimRect()->GetPosition(), player->GetAnimRect()->GetScale());
			Collision::RECT tileRect(tileCollision->GetLineRect()->GetPosition(), tileCollision->GetLineRect()->GetScale());

			// 위
			if ((playerRect.max.x >= tileRect.min.x && playerRect.min.x <= tileRect.max.x) && playerRect.min.y > tileCollision->GetLineRect()->GetPosition().y)
			{
				bGround = true;
				player->SetVel(0.0f);
				player->SetJumpSpeed(0.0f);
		
				player->GetAnimRect()->SetPosition(Vector2(player->GetAnimRect()->GetPosition().x, (tileCollision->GetLineRect()->GetPosition().y + tileCollision->GetLineRect()->GetScale().y / 2) + player->GetAnimRect()->GetScale().y / 2));
			} // 아래
			else if ((playerRect.max.x >= tileRect.min.x && playerRect.min.x <= tileRect.max.x) && playerRect.max.y < tileCollision->GetLineRect()->GetPosition().y)
			{
				player->GetAnimRect()->SetPosition(Vector2(player->GetAnimRect()->GetPosition().x, (tileCollision->GetLineRect()->GetPosition().y - tileCollision->GetLineRect()->GetScale().y / 2) - player->GetAnimRect()->GetScale().y / 2));

				player->SetVel(0.0f);
				player->SetJumpSpeed(0.0f);
			} // 옆
			else if (playerRect.max.x < tileCollision->GetLineRect()->GetPosition().x)
			{
				player->GetAnimRect()->Move(Vector2(-player->GetSpeed() * 2, 0));
			}
			else if (playerRect.min.x > tileCollision->GetLineRect()->GetPosition().y)
			{
				player->GetAnimRect()->Move(Vector2(player->GetSpeed() * 2, 0));
			}
		}
	}

	if (bGround)
		player->SetIsGround(true);
	else
		player->SetIsGround(false);
}

void TileMap::SetTileCollisions(Vector2 Index)
{
	bool check = false;
	for (int i = 0; i < tileCollisions.size(); ++i)
	{
		if (tileCollisions[i]->GetTileIndex() == Index)
		{
			check = true;
			if (!bCollision)
				tileCollisions.erase(tileCollisions.begin() + i);
		}
	}
	
	if (!check && bCollision)
		tileCollisions.push_back(make_shared<TileCollision>(Vector2(Index.x, Index.y)));
}

void TileMap::RegenerateTileCollision()
{
	for (int i = 0; i < tileCollisions.size(); ++i)
	{
		if (tileCollisions[i]->GetTileIndex().y >= width
			|| tileCollisions[i]->GetTileIndex().x >= height)
		{
			tileCollisions.erase(tileCollisions.begin() + i);
			i = -1;
		}
	}

	// TileCollision
	for (auto& tileCollision : tileCollisions)
	{
		Tile tile = tiles[tileCollision->GetTileIndex().x][tileCollision->GetTileIndex().y];

		tileCollision->Resize(tile, spacing);
	}
}

void TileMap::RegenerateBackGround()
{
	backGround->SetPosition((Vector2(width, height) * spacing) * 0.5f);
	backGround->SetScale(Vector2(width, height) * spacing);
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

void TileMap::RegenerateTileMap()
{
	if (width == 0 || height == 0 || spacing == 0)
	{
		assert(false);
		return;
	}

	tiles.resize(height, vector<Tile>());
	for (UINT y = 0; y < height; ++y)
	{
		tiles[y].resize(width, Tile());

		for (UINT x = 0; x < width; ++x)
			tiles[y][x].pos = Vector2((float)x, (float)y) * (float)spacing;
	}

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
}

Tile* TileMap::GetTile(const Vector2& unprojectedMousePos)
{
	UINT x = (UINT)unprojectedMousePos.x / spacing;
	UINT y = (UINT)unprojectedMousePos.y / spacing;

	if (x >= width || y >= height || x < 0 || y < 0)
		return nullptr;

	SetTileCollisions(Vector2(y, x));
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

Vector2 TileMap::GetTileIndex(const Vector2& position)
{
	UINT x = (UINT)position.x / spacing;
	UINT y = (UINT)position.y / spacing;

	if (x >= width || y >= height || x < 0 || y < 0)
		return Vector2(-1, -1);

	return Vector2(y, x);
}
