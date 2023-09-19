#include "stdafx.h"
#include "MoveComponent.h"

MoveComponent::MoveComponent(float maxSpeed, const string& name)
	:Component(name), maxSpeed(maxSpeed)
{
}

void MoveComponent::Update()
{
	ResetKeyCount();

	if (INPUT->Press('W'))
		AddKeyCount();
	if (INPUT->Press('S'))
		AddKeyCount();
	if (INPUT->Press(VK_LEFT))
		AddKeyCount();
	if (INPUT->Press(VK_RIGHT))
		AddKeyCount();

	if (keyCount == 2)
		speed = maxSpeed / sqrtf(2.0f);
	else
		speed = maxSpeed;

	if (INPUT->Press('W'))
		owner->Move(Vector2(0, speed));
	if (INPUT->Press('S'))
		owner->Move(Vector2(0, -speed));
	if (INPUT->Press(VK_LEFT))
		owner->Move(Vector2(-speed, 0));
	if (INPUT->Press(VK_RIGHT))
		owner->Move(Vector2(speed, 0));

	if (INPUT->Press(VK_DOWN))
		owner->Scale(Vector2(-speed / 2.0f));
	if (INPUT->Press(VK_UP))
		owner->Scale(Vector2(speed / 2.0f));

	if (INPUT->Press(VK_LEFT))
		owner->Rotate(-speed);
	if (INPUT->Press(VK_RIGHT))
		owner->Rotate(speed);
}
