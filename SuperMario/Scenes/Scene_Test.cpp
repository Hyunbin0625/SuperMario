#include "stdafx.h"
#include "Scene_Test.h"

void Test::Init()
{
	world_current = 0;

	tileMap = make_unique<TileMap>(Vector2(), Vector2(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 0.0f, Color(0.35f, 0.7f, 1.0f, 1.0f), L"_Textures/World1_Block.png", 16, 16);

	player = make_shared<Player>(CENTER, 2.0f);

	itemSet = make_unique<ItemSet>();
	enemySet = make_unique<EnemySet>();
	blockSet = make_unique<BlockSet>();

	wstring path = L"_Maps/World_0" + to_wstring(world_current + 1) + L".tile";
	LoadTileMap(path);
}

void Test::Destroy()
{
	tileMap.reset();
}

void Test::Update()
{
	Collision();
	if (!player->GetIsDeath())
		tileMap->PlayerCollisions(player);

	for (const auto& item : itemList)
		item->Collision(player);
	for (const auto& enemy : enemyList)
		enemy->Collision(player);
	for (const auto& block : blockList)
		block->Collision(player);

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
	// Delete Block
	for (int i = 0; i < blockList.size(); ++i)
	{
		if (blockList[i]->GetDelete())
			blockList.erase(blockList.begin() + i);
	}

	// Add Item
	if (itemSet->GetSelectedIndex() == 0)
		itemList.push_back(make_shared<S_Radish>(CAMERA->GetPosition() + CENTER, 2.0f));
	else if (itemSet->GetSelectedIndex() == 1)
		itemList.push_back(make_shared<B_Radish>(CAMERA->GetPosition() + CENTER, 2.0f));
	// Add Enemy
	if (enemySet->GetSelectedIndex() == 0)
		enemyList.push_back(make_shared<Shyguy>(CAMERA->GetPosition() + CENTER, 2.0f, 60.0f, 150.0f, 1, Direction::L));
	else if (enemySet->GetSelectedIndex() == 1)
		enemyList.push_back(make_shared<Turtle>(CAMERA->GetPosition() + CENTER, 2.0f, 50.0f, 150.0f, 1, Direction::L));
	else if (enemySet->GetSelectedIndex() == 2)
		enemyList.push_back(make_shared<Wart>(CAMERA->GetPosition() + CENTER, 2.0f, 40.0f, 50.0f, 4, Direction::L));
	// Add Block
	if (blockSet->GetSelectedIndex() == 0)
		blockList.push_back(make_shared<Grass_01>(CAMERA->GetPosition() + CENTER, tileMap->GetSpacing()));

	// TileMap이 변형되었을 때
	if (tileMap->GetIsRegenerate())
	{
		for (auto& block : blockList)
		{
			block->GetTextureRect()->SetPosition(tileMap->GetTilePos(block->GetState().tlieIndex));
			block->SetSpacing(tileMap->GetSpacing());
		}
	}

	if (bMod)
	{
		for (auto& block : blockList)
		{
			if (!block->GetIsClick())
			{
				Vector2 tileIndex = tileMap->GetTileIndex(block->GetTextureRect()->GetPosition());
				block->SetTileIndex(tileIndex);
				block->GetTextureRect()->SetPosition(tileMap->GetTilePos(tileIndex) + Vector2(tileMap->GetSpacing(), tileMap->GetSpacing()) * 0.5f);
			}
		}

		// Delete Item
		for (int i = 0; i < itemList.size(); ++i)
		{
			if (itemList[i]->GetIsCreated())
				itemList.erase(itemList.begin() + i);
		}
	}
	else
	{
		for (auto& block : blockList)
		{
			if (block->GetIsHolding())
			{
				if (block->GetItemType() == ItemType::S_Radish)
					itemList.push_back(make_shared<S_Radish>(block->GetTextureRect()->GetPosition(), 2.0f, true));
				else if (block->GetItemType() == ItemType::B_Radish)
					itemList.push_back(make_shared<B_Radish>(block->GetTextureRect()->GetPosition(), 2.0f, true));
				itemList.back()->SetIsMod(bMod);
				block->SetIsHolding(false);
				block->SetIsCheck(false);
			}
		}
	}

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
	// BlockList Update
	for (const auto& block : blockList)
		block->Update();

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
	// BlockList Render
	for (const auto& block : blockList)
		block->Render();
	UI->Render();
}

void Test::PostRender()
{
	if (bMod)
	{
		IMGUI->ItemGUIS(itemList, "ItemList");
		IMGUI->EnemyGUIS(enemyList, "EnemyList");
		IMGUI->BlockGUIS(blockList, "BlockList");

		if (ImGui::BeginTabBar("Tool"))
		{
			tileMap->GUI();
			itemSet->GUI();
			ImGui::NewLine();
			enemySet->GUI();
			ImGui::NewLine();
			blockSet->GUI();
			player->GUI();

			if (ImGui::BeginTabItem("MapDetails"))
			{
				const char* worlds[] = { "World_01", "World_02", "World_03", "World_04", "World_05", "World_06", "World_07" };
				if (ImGui::Combo("World", &world_current, worlds, IM_ARRAYSIZE(worlds)))
				{
					wstring path = L"_Maps/World_0" + to_wstring(world_current + 1) + L".tile";
					cout << String::ToString(path) << '\n';
					LoadTileMap(path);
				}

				if (ImGui::Button("Play", ImVec2(50, 30)))
				{
					bMod = false;
					CAMERA->SetPosition(player->GetPosition() - CENTER);
				}
				player->SetIsMod(bMod);
				for (const auto& item : itemList)
					item->SetIsMod(bMod);
				for (const auto& enemy : enemyList)
					enemy->SetIsMod(bMod);
				for (const auto& block : blockList)
					block->SetIsMod(bMod);

				// File Save&Load
				string name = "File Save&Load";
				ImGui::Text(name.c_str());
				if (ImGui::Button("Save", ImVec2(50, 30)))
					SaveTileMap();
				ImGui::SameLine();
				if (ImGui::Button("Load", ImVec2(50, 30)))
					LoadTileMap();

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	else
	{
		if (INPUT->Press(VK_ESCAPE))
		{
			bMod = true;
			CAMERA->SetPosition(player->GetPosition() - CENTER);
		}
	}
	
}

void Test::SaveTileMap(const wstring& path)
{
	if (path.empty())
	{
		function<void(wstring)> func = bind(&Test::SaveTileMap, this, placeholders::_1);
		Path::SaveFileDialog(L"", Path::TileMapFilter, L"_Maps/", func, gHandle);
	}
	else
	{
		ofstream out(path.c_str());

		if (out.is_open())
		{
			// Player
			player->SaveFile(out);

			// TileMap
			tileMap->SaveFile(out);

			// ItemList
			int listSize = (int)itemList.size();
			out.write((char*)&listSize, sizeof(listSize));

			ItemState itemState;
			for (UINT i = 0; i < itemList.size(); ++i)
			{
				itemState = itemList[i]->GetState();
				out.write((char*)&itemState, sizeof(itemState));
			}

			// EnemyList
			listSize = (int)enemyList.size();
			out.write((char*)&listSize, sizeof(listSize));

			EnemyState enemyState;
			for (UINT i = 0; i < enemyList.size(); ++i)
			{
				enemyState = enemyList[i]->GetState();
				out.write((char*)&enemyState, sizeof(enemyState));
			}

			// BlockList
			listSize = (int)blockList.size();
			out.write((char*)&listSize, sizeof(listSize));

			BlockState blockState;
			for (UINT i = 0; i < blockList.size(); ++i)
			{
				blockState = blockList[i]->GetState();
				out.write((char*)&blockState, sizeof(blockState));
			}
		}
		out.close();
	}
}

void Test::LoadTileMap(const wstring& path)
{
	if (path.empty())
	{
		function<void(wstring)> func = bind(&Test::LoadTileMap, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::TileMapFilter, L"_Maps/", func, gHandle);
	}
	else
	{
		ifstream in(path.c_str());

		if (in.is_open())
		{
			// Player
			player->LoadFile(in);

			// TileMap
			tileMap->LoadFile(in);

			// ItemList
			int listSize;
			in.read((char*)&listSize, sizeof(listSize));

			itemList.clear();
			itemList.resize(listSize);

			ItemState itemState;
			for (UINT i = 0; i < itemList.size(); ++i)
			{
				in.read((char*)&itemState, sizeof(itemState));

				if (itemState.type == ItemType::S_Radish)
					itemList[i] = make_shared<S_Radish>(itemState.position, itemState.totalSize);
				else if (itemState.type == ItemType::B_Radish)
					itemList[i] = make_shared<B_Radish>(itemState.position, itemState.totalSize);
			}

			// EnemyList
			in.read((char*)&listSize, sizeof(listSize));

			enemyList.clear();
			enemyList.resize(listSize);

			EnemyState enemyState;
			for (UINT i = 0; i < enemyList.size(); ++i)
			{
				in.read((char*)&enemyState, sizeof(enemyState));

				if (enemyState.type == EnemyType::Shyguy)
					enemyList[i] = make_shared<Shyguy>(enemyState.position, enemyState.totalSize, enemyState.speed, enemyState.Gdrainage, enemyState.maxHp, enemyState.direction);
				else if (enemyState.type == EnemyType::Turtle)
					enemyList[i] = make_shared<Turtle>(enemyState.position, enemyState.totalSize, enemyState.speed, enemyState.Gdrainage, enemyState.maxHp, enemyState.direction);
				else if (enemyState.type == EnemyType::Wart)
					enemyList[i] = make_shared<Wart>(enemyState.position, enemyState.totalSize, enemyState.speed, enemyState.Gdrainage, enemyState.maxHp, enemyState.direction);
			}

			// BlockList
			in.read((char*)&listSize, sizeof(listSize));

			blockList.clear();
			blockList.resize(listSize);

			BlockState blockState;
			for (UINT i = 0; i < blockList.size(); ++i)
			{
				in.read((char*)&blockState, sizeof(blockState));

				Vector2 position = tileMap->GetTilePos(blockState.tlieIndex);
				float spacing = tileMap->GetSpacing();
				if (blockState.type == BlockType::Grass_01)
					blockList[i] = make_shared<Grass_01>(position, spacing, blockState.tlieIndex, blockState.regen);
			}
		}
		in.close();
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

				// 위
				if ((itemRect.max.x >= tileRect.min.x && itemRect.min.x <= tileRect.max.x) && itemRect.min.y > tileCollision->GetLineRect()->GetPosition().y)
				{
					bCheck = true;
				} // 옆
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

				// 위
				if ((enemyRect.max.x >= tileRect.min.x && enemyRect.min.x <= tileRect.max.x) && enemyRect.min.y > tileCollision->GetLineRect()->GetPosition().y)
				{
					bCheck = true;
					enemy->SetGroundPos(tileRect.max);
				} // 옆
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
