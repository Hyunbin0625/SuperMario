#include "stdafx.h"
#include "UI.h"

Ui::Ui()
{
	// Hp
	hpList.resize(3);
	for (auto& hp : hpList)
		hp = make_unique<TextureRect>(Vector2(), Vector2(18, 17), 0.0f, L"_Textures/player_UI_Hp.png");
}

Ui::~Ui()
{
}

void Ui::Init(UINT currentHp)
{
	for (int i = 0; i < 3; ++i)
	{
		hpList[i]->SetPosition(CAMERA->GetPosition() + CENTER + Vector2(-383, 134) + (Vector2(0, 18) * 2 * i));
		hpList[i]->SetScale(Vector2(18, 17) * 2.0f);
	}

	this->currentHp = currentHp;
}

void Ui::Update()
{
	for (auto& hp : hpList)
		hp->Update();
}

void Ui::Render()
{
	if (currentHp == 0)
		return;

	for (int i = 0; i < currentHp; ++i)
	{
		if (i >= 3)
			break;
		hpList[i]->Render();
	}
}
