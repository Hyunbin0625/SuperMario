#include "stdafx.h"
#include "Grass_01.h"

Grass_01::Grass_01(const Vector2& position, const float& spacing, const Vector2& tileIndex, float regen)
	: spacing(spacing)
{
	state = { BlockType::Grass_01, tileIndex, regen };

	textureRect = make_shared<TextureRect>(position, Vector2(spacing, spacing), 0.0f, L"_Textures/Grass_01.png");

	textureRect->AddComponent(make_shared<ColliderComponent>(ColliderType::RECT));

	random_device random;
	mt = mt19937(random());
}

void Grass_01::Init()
{
	textureRect->SetScale(Vector2(spacing, spacing));
	bCheck = true;
}

void Grass_01::Collision(shared_ptr<Player> player)
{
	if (textureRect->GET_COMP(Collider)->Intersect(player->GetAnimRect()->GET_COMP(Collider)) && player->GetIsHolding() && !bHolding && bCheck)
	{
		bHolding = true;

		uniform_int_distribution<int> randomItemType(0, 1);
		itemType = (ItemType)randomItemType(mt);
	}
}

void Grass_01::Update()
{
	if (bMod)
	{
		if (!(ImGui::IsAnyItemActive()) && textureRect->GET_COMP(Collider)->Intersect(INPUT->GetMousePosition()) && INPUT->Press(VK_LBUTTON) || bClick)
		{
			if (INPUT->Press(VK_LBUTTON))
				bClick = true;
			else
				bClick = false;

			textureRect->SetPosition(INPUT->GetMousePosition());
		}
		Init();
	}
	if (state.regen < 0.0f)
		bRegen = false;
	else
		bRegen = true;

	if (!bCheck && bRegen)
	{
		deltaTime += DELTA;
		if (deltaTime >= state.regen)
		{
			Init();
			deltaTime = 0.0f;
			wstring path = L"_Textures/Grass_01.png";
			textureRect->SetSRV(path);
		}
		else
		{
			wstring path = L"_Textures/Grass_01_Regen.png";
			textureRect->SetSRV(path);
		}
	}

	textureRect->Update();
}

void Grass_01::Render()
{
	if (bRegen || (!bRegen && bCheck))
		textureRect->Render();
}

void Grass_01::GUI(int ordinal)
{
	string objName = to_string(ordinal) + ". Grass_01";

	if (ImGui::BeginMenu(objName.c_str()))
	{
		textureRect->SetOutline(true);

		ImGui::Text(objName.c_str());

		if (ImGui::Checkbox("Regen", &bRegen))
		{
			if (bRegen)
				state.regen = 0.0f;
			else
				state.regen = -1;
		}

		if (bRegen)
		{
			if (ImGui::InputFloat("RegenTime", &state.regen, 0.1f, 1.0f, "%.2f"))
			{
				if (state.regen < 0.0f)
					bRegen = false;
			}
		}

		if (ImGui::Button("Delete", ImVec2(50, 30)))
			bDelete = 1;

		ImGui::EndMenu();
	}
	else
	{
		textureRect->SetOutline(false);
	}
}
