#include "stdafx.h"
#include "Scene_Test.h"

void Test::Init()
{
	tileMap = make_unique<TileMap>(Vector2(), Vector2(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 0.0f, L"_Textures/World1_Block.png", 16, 16);

	player = make_shared<Player>(CENTER, 1.0f);

	itemSet = make_unique<ItemSet>();
}

void Test::Destroy()
{
	tileMap.reset();
}

void Test::Update()
{
	Collision();
	tileMap->PlayerCollisions(player);
	for (const auto& item : itemList)
		item->Collision(player);

	// Delete Item
	for (int i = 0; i < itemList.size(); ++i)
	{
		if (itemList[i]->GetDelete())
			itemList.erase(itemList.begin() + i);
	}
	// Add Item
	if (itemSet->GetSelectedIndex() == 0)
		itemList.push_back(make_shared<S_Radish>(CAMERA->GetPosition() + CENTER, 1.0f));

	// ItemList Update
	for (const auto& item : itemList)
		item->Update();

	tileMap->Update();
	player->Update();
}

void Test::PreRender()
{
}

void Test::Render()
{
	tileMap->Render();
	player->Render();

	// ItemList Render
	for (const auto& item : itemList)
		item->Render();
}

void Test::PostRender()
{
	IMGUI->ItemGUIS(itemList, "ItemList");

	if (ImGui::BeginTabBar("Tool"))
	{
		tileMap->GUI();
		itemSet->GUI();
		player->GUI();

		if (ImGui::BeginTabItem("MapDetails"))
		{
			if (ImGui::Checkbox("CreateMod", &bMod))
			{
				CAMERA->SetPosition(player->GetPosition() - CENTER);
			}
			player->SetIsMod(bMod);
			for (const auto& item : itemList)
				item->SetIsMod(bMod);

	
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

}

void Test::Collision()
{
	for (auto& item : itemList)
	{
		bool bCheck = false;
		for (auto& tileCollision : tileMap->GetTileCollisions())
		{
			if (item->GetTextureRect()->GET_COMP(Collider)->Intersect(tileCollision->GetLineRect()->GET_COMP(Collider)))
			{
				Collision::RECT itemRect(item->GetTextureRect()->GetPosition(), item->GetTextureRect()->GetScale());
				Collision::RECT tileRect(tileCollision->GetLineRect()->GetPosition(), tileCollision->GetLineRect()->GetScale());

				// À§
				if ((itemRect.max.x >= tileRect.min.x && itemRect.min.x <= tileRect.max.x) && itemRect.min.y > tileCollision->GetLineRect()->GetPosition().y)
				{
					bCheck = true;
				} // ¿·
				else if (itemRect.max.x < tileCollision->GetLineRect()->GetPosition().x
					|| itemRect.min.x > tileCollision->GetLineRect()->GetPosition().y)
					item->Reverse();

			//	Vector2 playerPos = player->GetPosition() - tileCollision->GetLineRect()->GetPosition();
			//	playerPos = playerPos / (tileMap->GetSpacing() / 2);
			//
			//	cout << playerPos.x << '\n';
			//	cout << playerPos.y << '\n';
			}
		}

		if (bCheck)
			item->SetIsGround(true);
		else
			item->SetIsGround(false);

	}
}
