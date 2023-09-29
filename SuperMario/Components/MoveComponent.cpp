#include "stdafx.h"
#include "MoveComponent.h"

MoveComponent::MoveComponent(float maxSpeed, const string& name)
	:Component(name), maxSpeed(maxSpeed)
{
}

void MoveComponent::Update()
{
	if (INPUT->Press(VK_LEFT) || INPUT->Press(VK_RIGHT))
		speed = maxSpeed;
	else
		speed = 0.0f;

	if (INPUT->Press(VK_LEFT))
		owner->Move(Vector2(-speed, 0));
	if (INPUT->Press(VK_RIGHT))
		owner->Move(Vector2(speed, 0));
}
