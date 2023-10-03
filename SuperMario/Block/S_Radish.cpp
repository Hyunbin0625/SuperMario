#include "stdafx.h"
#include "S_Radish.h"

S_Radish::S_Radish(const Vector2& position, const float& totalSize, bool bCreated)
	: bCreated(bCreated)
{
	state = { ItemType::S_Radish, position, totalSize };

	textureRect = make_shared<TextureRect>(position, Vector2(10, 16) * totalSize, 0.0f, L"_Textures/S_Radish.png");

	textureRect->AddComponent(make_shared<ColliderComponent>(ColliderType::RECT));
}

void S_Radish::Init()
{
	// State
	textureRect->SetPosition(state.position);
	textureRect->SetScale(Vector2(10, 16) * state.totalSize);
	textureRect->SetRotation(0.0f);

	vel = 0.0f;
	jumpSpeed = 0.0f;
	speed = 0.0f;

	bHolding = false;
	bHold = false;
	bThrow = false;

	bDeath = false;
	bCheck = true;
}

void S_Radish::Collision(shared_ptr<Player> player)
{
	if (player->GetIsThrow() && bHold)
	{
		bHold = false;
		bThrow = true;
		speed = player->GetAnimRect()->GET_COMP(Move)->GetSpeed() + 200.0f;
		if (!(bool)player->GetDirection())
			speed *= -1;
		vel = 0.0f;
		jumpSpeed = 50.0f;
	}
	else if (player->GetIsHold() && bHold)
	{
		textureRect->SetPosition(player->GetAnimRect()->GetPosition() + Vector2(0, player->GetAnimRect()->GetScale().y / 2));
	}
	else if (textureRect->GET_COMP(Collider)->Intersect(player->GetAnimRect()->GET_COMP(Collider)) && player->GetIsHolding())
	{
		bHolding = true;
		textureRect->SetPosition(Vector2(player->GetAnimRect()->GetPosition().x, textureRect->GetPosition().y));

		textureRect->Move(Vector2(0, 80));
		if (textureRect->GetPosition().y >= player->GetAnimRect()->GetPosition().y + player->GetAnimRect()->GetScale().y / 2)
		{
			bHolding = false;
			bHold = true;
			player->SetIsHolding(false);
			player->SetIsHold(true);
		}
	}
}

void S_Radish::Reverse()
{
	speed *= -1 * 0.3f;
	bThrow = false;
}

void S_Radish::Death()
{
	vel = 0.0f;
	jumpSpeed = 20.0f;
	if (speed > 0)
	{
		speed = -50.0f;
		textureRect->SetRotation(textureRect->GetRotation() - 45.0f);
	}
	else
	{
		speed = 50.0f;
		textureRect->SetRotation(textureRect->GetRotation() + 45.0f);
	}
	bThrow = false;
	bDeath = true;
}

void S_Radish::Update()
{
	if (bMod)
	{
		if (!(ImGui::IsAnyItemActive()) && textureRect->GET_COMP(Collider)->Intersect(INPUT->GetMousePosition()) && INPUT->Press(VK_LBUTTON) || bClick)
		{
			if (INPUT->Press(VK_LBUTTON))
				bClick = true;
			else
				bClick = false;

			state.position = INPUT->GetMousePosition();
		}
		Init();
	}

	if (bGround)
		bThrow = false;

	if ((!bGround && !bHolding && !bHold && !bMod) || bDeath)
	{
		vel += (float)(G * Gdrainage * DELTA);
		jumpSpeed -= vel * DELTA;
		textureRect->Move(Vector2(speed, jumpSpeed) * state.totalSize);

		if (bDeath && textureRect->GetPosition().y <= CAMERA->GetPosition().y - textureRect->GetScale().y)
			bCheck = false;
	}
	textureRect->SetScale(Vector2(10, 16) * state.totalSize);

	textureRect->Update();
}

void S_Radish::Render()
{
	if (bCheck)
		textureRect->Render();
}

void S_Radish::GUI(int ordinal)
{
	string objName = to_string(ordinal) + ". S_Radish";

	if (ImGui::BeginMenu(objName.c_str()))
	{
		textureRect->SetOutline(true);

		ImGui::Text(objName.c_str());

		ImGui::SliderFloat2("Position", (float*)&state.position, CAMERA->GetPosition().x, CAMERA->GetPosition().x + WIN_DEFAULT_WIDTH);
		ImGui::SliderFloat("Size", &state.totalSize, 0.1f, 5.0f, "%.2f");
		ImGui::InputFloat("G_Drainage", &Gdrainage, 1.0f, 100.0f);

		if (ImGui::Button("Delete", ImVec2(50, 30)))
			bDelete = 1;

		ImGui::EndMenu();
	}
	else
	{
		textureRect->SetOutline(false);
	}
}
