#include "stdafx.h"
#include "Scene_Test.h"

void Test::Init()
{
	tileMap = make_unique<TileMap>(Vector2(), Vector2(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT), 0.0f, L"_Textures/World1_Block.png", 16, 16);

	player = make_shared<Player>(CENTER, 1.0f);
}

void Test::Destroy()
{
	tileMap.reset();
}

void Test::Update()
{
	tileMap->PlayerCollisions(player);

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
}

void Test::PostRender()
{
	if (ImGui::BeginTabBar("Tool"))
	{
		tileMap->GUI();
		player->GUI();

		if (ImGui::BeginTabItem("MapDetails"))
		{
			if (ImGui::Checkbox("CreateMod", &bMod))
			{
				CAMERA->SetPosition(player->GetPosition() - CENTER);
			}
			player->SetMod(bMod);
	
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

}
