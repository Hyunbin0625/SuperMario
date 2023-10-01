#include "stdafx.h"
#include "Scene_Test.h"

void Test::Init()
{
	tileMap = make_unique<TileMap>(Vector2(), Vector2(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 0.0f, L"_Textures/World1_Block.png", 16, 16);

	player = make_shared<Player>(CENTER, 2.0f);

	itemSet = make_unique<ItemSet>();
	enemySet = make_unique<EnemySet>();
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
	for (const auto& enemy : enemyList)
		enemy->Collision(player);

	// Delete Item
	for (int i = 0; i < itemList.size(); ++i)
	{
		if (itemList[i]->GetDelete())
			itemList.erase(itemList.begin() + i);
	}
	// Delete Enemy
	for (int i = 0; i < enemyList.size(); ++i)
	{
		if (enemyList[i]->GetDelete())
			enemyList.erase(enemyList.begin() + i);
	}
	// Add Item
	if (itemSet->GetSelectedIndex() == 0)
		itemList.push_back(make_shared<S_Radish>(CAMERA->GetPosition() + CENTER, 2.0f));
	// Add Enemy
	if (enemySet->GetSelectedIndex() == 0)
		enemyList.push_back(make_shared<Shyguy>(CAMERA->GetPosition() + CENTER, 2.0f, 60.0f, 1, Direction::L));
	else if (enemySet->GetSelectedIndex() == 1)
		enemyList.push_back(make_shared<Turtle>(CAMERA->GetPosition() + CENTER, 2.0f, 50.0f, 1, Direction::L));

	if (!bMod)
	{
		CAMERA->ZoomSet(Vector2(183.559f, 103.252f), Vector2(1096.44f, 616.748f));
		CAMERA->SetPosition(player->GetAnimRect()->GetPosition() - CENTER);

		CAMERA->SetEdges(Vector2(-183, -103), tileMap->GetTileMapScale() + Vector2(183, 103));
	}
	else
		CAMERA->SetEdges(false);

	// ItemList Update
	for (const auto& item : itemList)
		item->Update();
	// EnemyList Update
	for (const auto& enemy : enemyList)
		enemy->Update();

	tileMap->Update();
	player->Update();

	UI->Init(player->GetHp());
	UI->Update();
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
	// EnemyList Render
	for (const auto& enemy : enemyList)
		enemy->Render();
	UI->Render();
}

void Test::PostRender()
{
	IMGUI->ItemGUIS(itemList, "ItemList");
	IMGUI->EnemyGUIS(enemyList, "EnemyList");

	if (ImGui::BeginTabBar("Tool"))
	{
		tileMap->GUI();
		itemSet->GUI();
		ImGui::NewLine();
		enemySet->GUI();
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
			for (const auto& enemy : enemyList)
				enemy->SetIsMod(bMod);
	
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

}

void Test::Collision()
{
	bool bCheck = false;
	// Item
	for (auto& item : itemList)
	{
		bCheck = false;
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
			}
		}

		if (bCheck)
			item->SetIsGround(true);
		else
			item->SetIsGround(false);
	}

	// Enemy
	for (auto& enemy : enemyList)
	{
		bCheck = false;
		for (auto& tileCollision : tileMap->GetTileCollisions())
		{
			if (enemy->GetAnimRect()->GET_COMP(Collider)->Intersect(tileCollision->GetLineRect()->GET_COMP(Collider)))
			{
				Collision::RECT enemyRect(enemy->GetAnimRect()->GetPosition(), enemy->GetAnimRect()->GetScale());
				Collision::RECT tileRect(tileCollision->GetLineRect()->GetPosition(), tileCollision->GetLineRect()->GetScale());

				// À§
				if ((enemyRect.max.x >= tileRect.min.x && enemyRect.min.x <= tileRect.max.x) && enemyRect.min.y > tileCollision->GetLineRect()->GetPosition().y)
				{
					bCheck = true;
					enemy->SetGroundPos(tileRect.max);
				} // ¿·
				else if (enemyRect.max.x < tileCollision->GetLineRect()->GetPosition().x
					|| enemyRect.min.x > tileCollision->GetLineRect()->GetPosition().y)
					enemy->Turn();
			}
		}
		if (!bCheck)
			enemy->SetGroundPos(Vector2(-1000, -1000));
	}

	// Item&Enemy
	for (auto& item : itemList)
	{
		if (item->GetIsThrow())
		{
			for (auto& enemy : enemyList)
			{
				if (enemy->GetAnimRect()->GET_COMP(Collider)->Intersect(item->GetTextureRect()->GET_COMP(Collider)))
				{
					item->Death();
					enemy->SetHit(true, item->GetTextureRect()->GetPosition());
				}
			}
		}
	}

	// Enemy&Enemy
	shared_ptr<IEnemy> Tenemy = nullptr;
	for (auto& enemy : enemyList)
	{
		if (enemy->GetIsThrow())
		{
			Tenemy = enemy;
			break;
		}
	}

	if (Tenemy != nullptr)
	{
		for (auto& enemy : enemyList)
		{
			if (Tenemy == enemy)
				continue;

			if (enemy->GetAnimRect()->GET_COMP(Collider)->Intersect(Tenemy->GetAnimRect()->GET_COMP(Collider)))
			{
				Tenemy->SetHit(true, enemy->GetAnimRect()->GetPosition());
				enemy->SetHit(true, Tenemy->GetAnimRect()->GetPosition());
			}
		}
	}

}
