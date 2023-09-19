#include "stdafx.h"
#include "TileSet.h"

TileSet::TileSet(const wstring& texturePath, UINT tileXCount, UINT tileYCount)
	: texturePath(texturePath), tileXCount(tileXCount), tileYCount(tileYCount)
{
	HRESULT hr = LoadFromWICFile(texturePath.c_str(), WIC_FLAGS_NONE, nullptr, tileSprite);
	CHECK(hr);

	hr = CreateShaderResourceView(DEVICE.Get(), tileSprite.GetImages(), tileSprite.GetImageCount(), tileSprite.GetMetadata(), &srv);
	CHECK(hr);

	textID = srv.Get();
	imageSize = Vector2((float)tileSprite.GetMetadata().width, (float)tileSprite.GetMetadata().height);

	texelTileSize = Vector2(1 / (float)tileXCount, 1 / (float)tileYCount);
}

void TileSet::GUI()
{
	static bool bOpen = true;
	if (ImGui::Begin("TileSprite", &bOpen))
	{
		int index = 0;

		for (UINT y = 0; y < tileYCount; ++y)
		{
			for (UINT x = 0; x < tileXCount; ++x)
			{
				ImGui::PushID(index++);	// ﻿ImGui가 버튼들에 대해서 자체적으로 구별하기 위한 ID 생성

				if (ImGui::ImageButton(textID, ImVec2(40, 40), ImVec2(x * texelTileSize.x, y * texelTileSize.y), ImVec2(texelTileSize.x + x * texelTileSize.x, texelTileSize.y + y * texelTileSize.y)))
				{
					selectedStartUV = Vector2(x * texelTileSize.x, y * texelTileSize.y);
				}

				ImGui::PopID();

				if (index % 4 != 0)
					ImGui::SameLine();
			}
		}
	}
	ImGui::End();
}
