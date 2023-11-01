#include "stdafx.h"
#include "Turtle.h"

Turtle::Turtle(const Vector2& position, float totailSize, float speed, float Gdrainage, int maxHp, Direction direction)
	: state({ EnemyType::Turtle, position, totailSize, speed, Gdrainage, maxHp, direction })
{
	// ÃÊ±âÈ­
	this->hp = state.maxHp;
	direction = state.direction;
	
	animRect = make_shared<AnimationRect>(position, Vector2(19, 26) * state.totalSize, 0.0f, L"_Textures/Turtle_Run_R.png");

	// Move
	// Run
	animRect->AddAnimClip(make_shared<AnimationClip>(L"RunR", L"_Textures/Turtle_Run_R.png", 2, false, true, 0.25));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"RunL", L"_Textures/Turtle_Run_L.png", 2, true, true, 0.25));

	// Into
	animRect->AddAnimClip(make_shared<AnimationClip>(L"Into", L"_Textures/Turtle_Into.png", 2, false, false, 0.1));
	// Outto
	animRect->AddAnimClip(make_shared<AnimationClip>(L"Outto", L"_Textures/Turtle_Into.png", 2, true, false, 0.1));
	// In
	animRect->AddAnimClip(make_shared<AnimationClip>(L"In", L"_Textures/Turtle_In.png", 1, false, false, 0.1));

	// AddAnimator
	animRect->AddComponent(make_shared<AnimatorComponent>(animRect->GetAnimClips()));
	// animRect SetAnimator
	animRect->SetAnimator(animRect->GET_COMP(Animator));

	// Components
	animRect->AddComponent(make_shared<ColliderComponent>(ColliderType::RECT));
}

void Turtle::Init()
{
	animRect->SetPosition(state.position);
	animRect->SetScale(Vector2(19, 26) * state.totalSize);
	animRect->SetRotation(0.0f);
	hp = state.maxHp;
	direction = state.direction;
	groundPos = Vector2(-1000, -1000);

	vel = 0.0f;
	jumpSpeed = 0.0f;

	bHolding = false;
	bHold = false;
	bThrow = false;

	bDeath = false;
	bCheck = true;
}

void Turtle::Collision(shared_ptr<Player> player)
{
	if (animRect->GET_COMP(Collider)->Intersect(player->GetAnimRect()->GET_COMP(Collider)) && animState != TurtleState::Death)
	{
		attackTime += DELTA;
		// À§
		if (player->GetIsHolding() || bHolding)
		{
			bHolding = true;
			animRect->SetPosition(Vector2(player->GetAnimRect()->GetPosition().x, animRect->GetPosition().y));

			animRect->Move(Vector2(0, 70));
			if (animRect->GetPosition().y >= player->GetAnimRect()->GetPosition().y + player->GetAnimRect()->GetScale().y / 2)
			{
				bHolding = false;
				bHold = true;
				player->SetIsHolding(false);
				player->SetIsHold(true);
			}
		} // ¿·
		else if ((!bHolding && !bHold && !player->GetIsThrow()) && attackTime >= 0.15f)
		{
			player->SetHit(true);
			attackTime = 0.0f;
		}
	}
	else
		attackTime = 0.0f;

	if (player->GetIsThrow() && bHold)
	{
		bHold = false;
		bThrow = true;
		speed = player->GetAnimRect()->GET_COMP(Move)->GetSpeed() + 200.0f;
		direction = player->GetDirection();
		vel = 0.0f;
		jumpSpeed = 50.0f;
	}
	else if (player->GetIsHold() && bHold)
		animRect->SetPosition(player->GetAnimRect()->GetPosition() + Vector2(0, player->GetAnimRect()->GetScale().y / 2));

}

void Turtle::Turn()
{
	if (direction && bTurn)
	{
		bTurn = false;
		direction = (Direction)false;
	}
	else if (bTurn)
	{
		bTurn = false;
		direction = (Direction)true;
	}
}

void Turtle::Destroy()
{
}

void Turtle::Update()
{
	if (bMod)
	{
		if (!(ImGui::IsAnyItemActive()) && animRect->GET_COMP(Collider)->Intersect(INPUT->GetMousePosition()) && INPUT->Press(VK_LBUTTON) || bClick)
		{
			if (INPUT->Press(VK_LBUTTON))
				bClick = true;
			else
				bClick = false;

			state.position = INPUT->GetMousePosition();
		}
		Init();
	}

	if (groundPos.x < -500)
		bGround = false;
	else
		bGround = true;

	if (hp <= 0)
	{
		if (!bDeath)
			Death();
		bDeath = true;
		bThrow = false;

		vel += (float)(G * state.Gdrainage * DELTA);
		jumpSpeed -= vel * DELTA;
		animRect->Move(Vector2(speed, jumpSpeed) * state.totalSize);

		if (bDeath && animRect->GetPosition().y <= CAMERA->GetPosition().y - animRect->GetScale().y)
			bCheck = false;

		animState = TurtleState::Death;
	}
	else if (bHolding || bHold)
	{
		animState = TurtleState::Hold;
	}
	else if (bThrow)
	{
		animState = TurtleState::Hold;

		if (!bGround)
		{
			vel += (float)(G * state.Gdrainage * DELTA);
			jumpSpeed -= vel * DELTA;
			if (direction)
				animRect->Move(Vector2(speed, jumpSpeed) * state.totalSize);
			else
				animRect->Move(Vector2(-speed, jumpSpeed) * state.totalSize);
		}
		else
		{
			speed -= 50 * DELTA;
			if (speed <= 0.0f)
			{
				speed = 0.0f;
				bThrow = false;
			}

			vel = 0.0f;
			jumpSpeed = 0.0f;
			animRect->SetPosition(Vector2(animRect->GetPosition().x, groundPos.y + animRect->GetScale().y / 2));
			if (direction)
				animRect->Move(Vector2(speed, 0) * state.totalSize);
			else
				animRect->Move(Vector2(-speed, 0) * state.totalSize);
		}
	}
	else
	{
		animState = TurtleState::Run;
		if (!bMod && hp > 0)
			Move();

		if (!bGround)
		{
			vel += (float)(G * state.Gdrainage * DELTA);
			jumpSpeed -= vel * DELTA;
			animRect->Move(Vector2(0, jumpSpeed) * state.totalSize);
		}
		else
		{
			vel = 0.0f;
			jumpSpeed = 0.0f;
			animRect->SetPosition(Vector2(animRect->GetPosition().x, groundPos.y + animRect->GetScale().y / 2));
		}
	}

	if (!bTurn && !bDeath)
		turnTime += DELTA;
	if (turnTime >= 0.3f)
	{
		turnTime = 0.0f;
		bTurn = true;
	}
	if (bDeath)
		bTurn = false;

	switch (animState)
	{
	case TurtleState::Run:
		animRect->SetRotation(0.0f);
		animRect->SetScale(Vector2(19, 26) * state.totalSize);
		if (direction == Direction::R)
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"RunR");
		else
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"RunL");
		break;
	case TurtleState::Hold:
		animRect->SetScale(Vector2(18, 15) * state.totalSize);
		animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"Into");
		break;
	case TurtleState::Death:
		animRect->SetScale(Vector2(18, 15) * state.totalSize);
		animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"In");
		break;
	}

	animRect->Update();
}

void Turtle::Render()
{
	if (bCheck)
		animRect->Render();
}

void Turtle::GUI(int ordinal)
{
	string objName = to_string(ordinal) + ". Turtle";

	if (ImGui::BeginMenu(objName.c_str()))
	{
		animRect->SetOutline(true);

		ImGui::Text(objName.c_str());

		ImGui::SliderFloat2("Position", (float*)&state.position, CAMERA->GetPosition().x, CAMERA->GetPosition().x + WIN_DEFAULT_WIDTH);
		ImGui::SliderFloat("Size", &state.totalSize, 0.1f, 5.0f, "%.2f");
		ImGui::SliderFloat("Speed", &state.speed, 10.0f, 500.0f, "%.2f");
		ImGui::InputFloat("G_Drainage", &state.Gdrainage, 1.0f, 100.0f);
		ImGui::InputInt("MaxHp", &state.maxHp);
		
		bool r = 0, l = 0;
		if (state.direction)
			l = 1;
		else
			r = 1;
		ImGui::Text("Default direction");
		if (ImGui::Checkbox("L", &l))
		{
			state.direction = Direction::L;
			r = 0;
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("R", &r))
		{
			state.direction = Direction::R;
			l = 0;
		}
	
		if (ImGui::Button("Delete", ImVec2(50, 30)))
			bDelete = 1;

		ImGui::EndMenu();
	}
	else
	{
		animRect->SetOutline(false);
	}
}

void Turtle::Move()
{
	if (direction == Direction::R)
	{
		animRect->Move(Vector2(state.speed, 0) * state.totalSize);
	}
	else
	{
		animRect->Move(Vector2(-state.speed, 0) * state.totalSize);
	}
}

void Turtle::Death()
{
	vel = 0.0f;
	jumpSpeed = 50.0f;
	if (hitPos.x < animRect->GetPosition().x)
	{
		speed = 50.0f;
		animRect->SetRotation(animRect->GetRotation() + 45.0f);
	}
	else
	{
		speed = -50.0f;
		animRect->SetRotation(animRect->GetRotation() - 45.0f);
	}
}
