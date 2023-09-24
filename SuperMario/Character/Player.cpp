#include "stdafx.h"
#include "Player.h"

Player::Player(const Vector2& position, const float& scale)
	: position(position), totalSize(scale)
{
	animRect = make_shared<AnimationRect>(position, Vector2(17, 22) * totalSize, 0.0f, L"_Textures/S_Mario_Idle_R.png");

	// Move
	// Idle
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SIdleR", L"_Textures/S_Mario_Idle_R.png", 1, false, false, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SIdleL", L"_Textures/S_Mario_Idle_L.png", 1, false, false, 0.1));

	animRect->AddAnimClip(make_shared<AnimationClip>(L"BIdleR", L"_Textures/B_Mario_Idle_R.png", 1, false, false, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"BIdleL", L"_Textures/B_Mario_Idle_L.png", 1, false, false, 0.1));

	// Run
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SRunR", L"_Textures/S_Mario_Run_R.png", 2, false, true, 0.15));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SRunL", L"_Textures/S_Mario_Run_L.png", 2, true, true, 0.15));

	animRect->AddAnimClip(make_shared<AnimationClip>(L"BRunR", L"_Textures/B_Mario_Run_R.png", 2, false, true, 0.15));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"BRunL", L"_Textures/B_Mario_Run_L.png", 2, true, true, 0.15));

	// Jump
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SJumpR", L"_Textures/S_Mario_Jump_R.png", 1, false, false, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SJumpL", L"_Textures/S_Mario_Jump_L.png", 1, true, false, 0.1));

	animRect->AddAnimClip(make_shared<AnimationClip>(L"BJumpR", L"_Textures/B_Mario_Jump_R.png", 1, false, false, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"BJumpL", L"_Textures/B_Mario_Jump_L.png", 1, true, false, 0.1));
	
	// Up
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SUp", L"_Textures/S_Mario_Up.png", 2, false, true, 0.1));

	animRect->AddAnimClip(make_shared<AnimationClip>(L"BUp", L"_Textures/B_Mario_Up.png", 2, false, true, 0.1));

	// Hold
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SHold", L"_Textures/S_Mario_Hold.png", 2, false, false, 0.1));

	animRect->AddAnimClip(make_shared<AnimationClip>(L"BHold", L"_Textures/B_Mario_Hold.png", 2, false, false, 0.1));

	// Hold_Run
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SHoldRunR", L"_Textures/S_Mario_Hold_Run_R.png", 2, false, true, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SHoldRunL", L"_Textures/S_Mario_Hold_Run_L.png", 2, false, true, 0.1));

	animRect->AddAnimClip(make_shared<AnimationClip>(L"BHoldRunR", L"_Textures/B_Mario_Hold_Run_R.png", 2, false, true, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"BHoldRunL", L"_Textures/B_Mario_Hold_Run_L.png", 2, false, true, 0.1));

	// Throw
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SIdleR", L"_Textures/S_Mario_Throw_R.png", 1, false, true, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"SIdleL", L"_Textures/S_Mario_Throw_L.png", 1, false, true, 0.1));

	animRect->AddAnimClip(make_shared<AnimationClip>(L"BIdleR", L"_Textures/B_Mario_Throw_R.png", 1, false, true, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"BIdleL", L"_Textures/B_Mario_Throw_L.png", 1, false, true, 0.1));

	// AddAnimator
	animRect->AddComponent(make_shared<AnimatorComponent>(animRect->GetAnimClips()));
	// animRect SetAnimator
	animRect->SetAnimator(animRect->GET_COMP(Animator));

	// Components
	animRect->AddComponent(make_shared<ColliderComponent>(ColliderType::RECT));
	animRect->AddComponent(make_shared<MoveComponent>(speed));
}

void Player::Update()
{
	if (bMod)
	{
		if ((!(ImGui::IsAnyItemActive()) && animRect->GET_COMP(Collider)->Intersect(INPUT->GetMousePosition()) && INPUT->Press(VK_LBUTTON)) || bClick)
		{
			if (INPUT->Press(VK_LBUTTON))
				bClick = true;
			else
				bClick = false;
			position = INPUT->GetMousePosition();
		}

		animRect->SetPosition(position);
		vel = 0.0f;
		jumpSpeed = 0.0f;
	}
	
	if (INPUT->Press(VK_SPACE) && bGround)
	{
		jumpSpeed = jumpMaxSpeed;
		bGround = false;
	}

	if (!bGround && !bMod)
	{
		vel += (float)(G * Gdrainage * DELTA);
		jumpSpeed -= vel * DELTA;
		animRect->Move(Vector2(0, jumpSpeed) * totalSize);
	}
	else if(!bMod)
	{
		vel = 0.0f;
		jumpSpeed = 0.0f;
	}

	if (!bGround)
	{
		state = PlayerState::Jump;
		if (INPUT->Press(VK_LEFT))
			direction = Direction::L;
		else if (INPUT->Press(VK_RIGHT))
			direction = Direction::R;
	}
	else if (INPUT->Press(VK_LEFT))
	{
		state = PlayerState::Run;
		direction = Direction::L;
	}
	else if (INPUT->Press(VK_RIGHT))
	{
		state = PlayerState::Run;
		direction = Direction::R;
	}
	else
		state = PlayerState::Idle;

	switch (state)
	{
	case None:
		break;
	case Idle:
		if (direction)
		{
			if (hp == 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"SIdleR");
				animRect->SetScale(Vector2(17, 22) * totalSize);
			}
			else if (hp > 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"BIdleR");
				animRect->SetScale(Vector2(18, 30) * totalSize);
			}
		}
		else
		{
			if (hp == 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"SIdleL");
				animRect->SetScale(Vector2(17, 22) * totalSize);
			}
			else if (hp > 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"BIdleL");
				animRect->SetScale(Vector2(18, 30) * totalSize);
			}
		}
		break;
	case Run:
		if (direction)
		{
			if (hp == 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"SRunR");
				animRect->SetScale(Vector2(17, 22) * totalSize);
			}
			else if (hp > 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"BRunR");
				animRect->SetScale(Vector2(18, 30) * totalSize);
			}
		}
		else
		{
			if (hp == 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"SRunL");
				animRect->SetScale(Vector2(17, 22) * totalSize);
			}
			else if (hp > 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"BRunL");
				animRect->SetScale(Vector2(18, 30) * totalSize);
			}
		}
		break;
	case Jump:
		if (direction)
		{
			if (hp == 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"SJumpR");
				animRect->SetScale(Vector2(18, 22) * totalSize);
			}
			else if (hp > 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"BJumpR");
				animRect->SetScale(Vector2(18, 30) * totalSize);
			}
		}
		else
		{
			if (hp == 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"SJumpL");
				animRect->SetScale(Vector2(18, 22) * totalSize);
			}
			else if (hp > 1)
			{
				animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"BJumpL");
				animRect->SetScale(Vector2(18, 30) * totalSize);
			}
		}
		break;
	case Up:
		break;
	case Hold:
		break;
	case Throw:
		break;
	case Death:
		break;
	default:
		break;
	}

	animRect->Update();
}

void Player::Render()
{
	animRect->Render();
}

void Player::GUI()
{
	if (ImGui::BeginTabItem("Player"))
	{
		ImGui::SliderFloat2("Position", (float*)&position, CAMERA->GetPosition().x, CAMERA->GetPosition().x + WIN_DEFAULT_WIDTH);
		ImGui::SliderFloat("Scale", &totalSize, 0.0f, 10.0f);
		if (ImGui::InputFloat("Speed", &speed, 1.0f, 100.0f))
			animRect->GET_COMP(Move)->SetSpeed(speed);
		ImGui::InputFloat("Jump", &jumpMaxSpeed, 1.0f, 100.0f);
		ImGui::InputFloat("G_Drainage", &Gdrainage, 1.0f, 100.0f);

		string textHp = "Player Hp : " + to_string(hp);
		ImGui::Text(textHp.c_str());
		if (ImGui::Button("Down", ImVec2(50, 30)))
			--hp;
		ImGui::SameLine();
		if (ImGui::Button("Up", ImVec2(50, 30)))
			++hp;

		ImGui::EndTabItem();
	}
}
