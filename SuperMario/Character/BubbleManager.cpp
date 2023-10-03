#include "stdafx.h"
#include "BubbleManager.h"

Bubble::Bubble()
{
	animRect = make_shared<AnimationRect>(Vector2(), Vector2(15, 15), 0.0f, L"_Textures/Wart_Bubble.png");
	animRect->AddAnimClip(make_shared<AnimationClip>(L"Bubble", L"_Textures/Wart_Bubble.png", 2, false, true, 0.1));
	
	// AddAnimator
	animRect->AddComponent(make_shared<AnimatorComponent>(animRect->GetAnimClips()));
	// animRect SetAnimator
	animRect->SetAnimator(animRect->GET_COMP(Animator));

	// AddComponent
	animRect->AddComponent(make_shared<ColliderComponent>(ColliderType::CIRCLE));
}

void Bubble::Init(Vector2 position, float rotation, float bulletSpeed)
{
	animRect->SetPosition(position);
	animRect->SetRotation(rotation);
	vel = 0.0f;
	jumpSpeed = 200.0f;
	speed = bulletSpeed;
}

void Bubble::Update()
{
	if (bActivation)
	{
		animRect->SetScale(Vector2(15, 15) * totalSize);

		animRect->GET_COMP(Animator)->SetCurrentAnimClip(L"Bubble");
		vel += G * 1 * DELTA;
		jumpSpeed -= vel;
		animRect->Move(Vector2(speed, jumpSpeed));
		if (animRect->GetPosition().y < CAMERA->GetPosition().y)
			bActivation = false;
	}

	animRect->Update();
}

void Bubble::Render()
{
	animRect->Render();
}

// ManagerClass
BubbleManager::BubbleManager(UINT totalBullet)
	: totalBullet(totalBullet)
{
	// 초기화
	currentIndex = 0;
	totalSize = 1.0f;

	bullets.resize(totalBullet);

	for (auto& bullet : bullets)
		bullet = make_shared<Bubble>();
}

void BubbleManager::CreateBullet()
{
	totalBullet *= 2;
	bullets.resize(totalBullet);

	for (int i = totalBullet / 2; i < totalBullet; ++i)
		bullets[i] = make_shared<Bubble>();
}

void BubbleManager::Init(Vector2 position, float rotation, float bulletSpeed)
{
	this->position = position;
	this->rotation = rotation;
	this->bulletSpeed = bulletSpeed;
}

void BubbleManager::IndexManagement()
{
	bullets[currentIndex]->Init(position, rotation, bulletSpeed);
	bullets[currentIndex++]->SetActivation(true);

	// Loop시 
	if (currentIndex > totalBullet - 1)
	{
		if (bullets[0]->GetActivation() == true)	// 0이 활성화 되어 있을 때
			CreateBullet();
		else
			currentIndex = 0;
	}
}

void BubbleManager::Update()
{
	for (auto& bullet : bullets)
	{
		bullet->SetTotalSize(totalSize);
		if (!bullet->GetActivation())
			bullet->GetAnimRect()->SetPosition(Vector2(1000, -1000));

		bullet->Update();
	}
}

void BubbleManager::Render()
{
	for (auto& bullet : bullets)
	{
		if (bullet->GetActivation())
			bullet->Render();
	}
}
