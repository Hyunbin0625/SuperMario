#include "stdafx.h"
#include "Wart.h"

Wart::Wart(const Vector2& position, float totailSize, float speed, float Gdrainage, int maxHp, Direction direction)
	: state({ EnemyType::Wart, position, totailSize, speed, Gdrainage, maxHp, direction })
{
	// 초기화
	this->hp = state.maxHp;
	direction = state.direction;
	
	animRect = make_shared<AnimationRect>(position, Vector2(42, 49) * state.totalSize, 0.0f, L"_Textures/Wart_Idle_L.png");

	// Move
	// Idle
	animRect->AddAnimClip(make_shared<AnimationClip>(L"IdleL", L"_Textures/Wart_Idle_L.png", 1, false, false, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"IdleR", L"_Textures/Wart_Idle_R.png", 1, true, false, 0.1));

	// Run
	animRect->AddAnimClip(make_shared<AnimationClip>(L"RunL", L"_Textures/Wart_Run_Front_L.png", 2, false, true, 0.4));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"RunR", L"_Textures/Wart_Run_Front_R.png", 2, true, true, 0.4));
	
	// Attack
	animRect->AddAnimClip(make_shared<AnimationClip>(L"AttackL", L"_Textures/Wart_Attack_L.png", 2, false, true, 0.4));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"AttackR", L"_Textures/Wart_Attack_R.png", 2, true, true, 0.4));
	
	// Hit
	animRect->AddAnimClip(make_shared<AnimationClip>(L"HitL", L"_Textures/Wart_Hit_L.png", 4, false, true, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"HitR", L"_Textures/Wart_Hit_R.png", 4, true, true, 0.1));
	
	// Death
	animRect->AddAnimClip(make_shared<AnimationClip>(L"DeathL", L"_Textures/Wart_Death_L.png", 1, false, false, 0.1));
	animRect->AddAnimClip(make_shared<AnimationClip>(L"DeathR", L"_Textures/Wart_Death_R.png", 1, true, false, 0.1));
	
	// AddAnimator
	animRect->AddComponent(make_shared<AnimatorComponent>(animRect->GetAnimClips()));
	// animRect SetAnimator
	animRect->SetAnimator(animRect->GET_COMP(Animator));

	// Components
	animRect->AddComponent(make_shared<ColliderComponent>(ColliderType::RECT));

	bullets = make_shared<BubbleManager>(5);

	random_device random;
	mt = mt19937(random());
}

void Wart::Init()
{
	animRect->SetPosition(state.position);
	animRect->SetScale(Vector2(42, 49) * state.totalSize);
	animRect->SetRotation(0.0f);
	hp = state.maxHp;
	direction = state.direction;
	groundPos = Vector2(-1000, -1000);

	vel = 0.0f;
	jumpSpeed = 0.0f;

	deltaTime = 0.0f;
	bCheckTime = false;
	bFront = true;
	hitTime = 0.0f;
	bHit = false;

	bDeath = false;
	bCheck = true;
}

void Wart::Collision(shared_ptr<Player> player)
{
	if (animRect->GET_COMP(Collider)->Intersect(player->GetAnimRect()->GET_COMP(Collider)) && animState != WartState::Death)
		player->SetHit(true);

	// Bullet
	for (auto& bullet : bullets->GetBullets())
	{
		if (bullet->GetAnimRect()->GET_COMP(Collider)->Intersect(player->GetAnimRect()->GET_COMP(Collider)) && animState != WartState::Death)
		{
			player->SetHit(true);
			bullet->SetActivation(false);
		}
	}
}

void Wart::Destroy()
{
}

void Wart::Update()
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
		if (!bCheckTime)
		{
			bCheckTime = true;
			hitTime = 0.0f;
		}
		if (hitTime <= 3.5f)
		{
			hitTime += DELTA;
			animState = WartState::Hit;
		}
		else
		{
			if (!bDeath)
				Death();
			bDeath = true;

			vel += (float)(G * state.Gdrainage * DELTA);
			jumpSpeed -= vel * DELTA;
			animRect->Move(Vector2(speed, jumpSpeed) * state.totalSize);

			if (bDeath && animRect->GetPosition().y <= CAMERA->GetPosition().y - animRect->GetScale().y)
				bCheck = false;

			animState = WartState::Death;
		}
	}
	else if (hp <= state.maxHp * 0.5f && !bHit)
	{
		if (!bCheckTime)
		{
			bCheckTime = true;
			hitTime = 0.0f;
		}

		hitTime += DELTA;
		animState = WartState::Hit;

		if (hitTime >= 2.0f)
		{
			bHit = true;
			bCheckTime = false;
		}
	}
	else
	{
		deltaTime += DELTA;
		if (bIdle && deltaTime >= 1.0f)
		{
			bIdle = false;
			bRun = true;
			deltaTime = 0.0f;

			uniform_int_distribution<int> randomAttack(0, 1);
			bAttack = (bool)randomAttack(mt);
		}
		else if (bIdle)
			animState = WartState::Idle;
		else if (bRun && deltaTime >= 1.5f)
		{
			bRun = false;
			bIdle = true;
			count = 0;
			attackTime = 0.0f;
			deltaTime = 0.0f;

			if (bFront)
				bFront = false;
			else
				bFront = true;
		}
		else if (bRun)
		{
			Move();
			if (bAttack)
				animState = WartState::Attack;
			else
				animState = WartState::Run;
		}

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

	switch (animState)
	{
	case WartState::Idle:
		if (direction == Direction::R)
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"IdleR");
		else
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"IdleL");
		break;
	case WartState::Run:
		if (direction == Direction::R)
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"RunR");
		else
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"RunL");
		break;
	case WartState::Attack:
		attackTime += DELTA;
		if (direction == Direction::R)
		{
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"AttackR");
			if (attackTime >= 0.2f && count < 5)
			{
				bullets->Init(animRect->GetPosition() + Vector2() * state.totalSize, 0.0f, (430.0f - count * 80.0f));	// speed 임시
				bullets->IndexManagement();
				++count;
				attackTime = 0.0f;
			}
		}
		else
		{
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"AttackL");
			if (attackTime >= 0.2f && count < 5)
			{
				bullets->Init(animRect->GetPosition() + Vector2() * state.totalSize, 0.0f, -(430.0f - count * 80.0f));	// speed 임시
				bullets->IndexManagement();
				++count;
				attackTime = 0.0f;
			}
		}
		break;
	case WartState::Hit:
		if (direction == Direction::R)
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"HitR");
		else
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"HitL");
		break;
	case WartState::Death:
		if (direction == Direction::R)
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"DeathR");
		else
			animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"DeathL");
		break;
	}
	animRect->SetScale(Vector2(42, 49)* state.totalSize);
	bullets->SetTotalSize(state.totalSize);

	animRect->Update();
	bullets->Update();
}

void Wart::Render()
{
	if (bCheck)
	{
		animRect->Render();
		bullets->Render();
	}
}

void Wart::GUI(int ordinal)
{
	string objName = to_string(ordinal) + ". Wart";

	if (ImGui::BeginMenu(objName.c_str()))
	{
		animRect->SetOutline(true);

		ImGui::Text(objName.c_str());

		ImGui::SliderFloat2("Position", (float*)&state.position, CAMERA->GetPosition().x, CAMERA->GetPosition().x + WIN_DEFAULT_WIDTH);
		ImGui::SliderFloat("Size", &state.totalSize, 0.1f, 5.0f, "%.2f");
		ImGui::SliderFloat("Speed", &state.speed, 10.0f, 100.0f, "%.2f");
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

void Wart::Move()
{
	if (direction == Direction::L)
	{
		if (bFront)
			animRect->Move(Vector2(-state.speed, 0) * state.totalSize);
		else
			animRect->Move(Vector2(state.speed, 0) * state.totalSize);
	}
	else
	{
		if (bFront)
			animRect->Move(Vector2(state.speed, 0) * state.totalSize);
		else
			animRect->Move(Vector2(-state.speed, 0) * state.totalSize);
	}
}

void Wart::Death()
{
	vel = 0.0f;
	jumpSpeed = 50.0f;
	if (direction == Direction::L)
		speed = 50.0f;
	else
		speed = -50.0f;
}
