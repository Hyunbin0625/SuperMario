#include "stdafx.h"
#include "Next_Door.h"

extern int world_current;
Next_Door::Next_Door(const Vector2& position, const float& spacing, const Vector2& tileIndex, float regen)
	: spacing(spacing)
{
	state = { BlockType::None, tileIndex, regen };

	textureRect = make_shared<TextureRect>(position, Vector2(spacing, spacing), 0.0f, L"_Textures/Next_Door.png");

	textureRect->AddComponent(make_shared<ColliderComponent>(ColliderType::RECT));
}

void Next_Door::Init()
{
	textureRect->SetScale(Vector2(spacing, spacing));
	bCheck = true;
}

void Next_Door::Collision(shared_ptr<Player> player)
{
	if (textureRect->GET_COMP(Collider)->Intersect(player->GetAnimRect()->GET_COMP(Collider)) && player->GetIsHolding() && bCheck)
	{
		bCheck = false;
		++world_current;
	}
}

void Next_Door::Update()
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

	textureRect->Update();
}

void Next_Door::Render()
{
	textureRect->Render();
}

void Next_Door::GUI(int ordinal)
{
	string objName = to_string(ordinal) + ". Next_Door";

	if (ImGui::BeginMenu(objName.c_str()))
	{
		textureRect->SetOutline(true);

		ImGui::Text(objName.c_str());

		if (ImGui::Button("Delete", ImVec2(50, 30)))
			bDelete = 1;

		ImGui::EndMenu();
	}
	else
	{
		textureRect->SetOutline(false);
	}
}
