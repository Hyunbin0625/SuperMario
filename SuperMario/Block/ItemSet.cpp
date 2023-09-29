#include "stdafx.h"
#include "ItemSet.h"

ItemSet::ItemSet()
{
	SetTextID(L"_Textures/S_Radish.png");
}

void ItemSet::SetTextID(const wstring& texturePath)
{
	HRESULT hr = LoadFromWICFile(texturePath.c_str(), WIC_FLAGS_NONE, nullptr, tileSprite);
	CHECK(hr);

	srvs.push_back(nullptr);
	hr = CreateShaderResourceView(DEVICE.Get(), tileSprite.GetImages(), tileSprite.GetImageCount(), tileSprite.GetMetadata(), &srvs[srvs.size() - 1]);
	CHECK(hr);
}

void ItemSet::GUI()
{
	if (ImGui::BeginTabItem("Object"))
	{
		string title = "Item";
		ImGui::Text(title.c_str());
		int index = 0;
		selectedIndex = -1;

		for (UINT i = 0; i < srvs.size(); ++i)
		{
			ImGui::PushID(index++);	// ﻿ImGui가 버튼들에 대해서 자체적으로 구별하기 위한 ID 생성

			textID = srvs[i].Get();
			if (ImGui::ImageButton(textID, ImVec2(40, 50)))
			{
				selectedIndex = i;
			}

			ImGui::PopID();

			if (index % 4 != 0)
				ImGui::SameLine();
		}

	ImGui::EndTabItem();
	}
}