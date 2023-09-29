#pragma once

class MoveComponent : public Component
{
public:
	MoveComponent(float maxSpeed = 300.0f, const string& name = "Move");

public:
	void Update() override;

public:
	float GetSpeed() const { return speed; }
	float GetMaxSpeed() const { return maxSpeed; }

	void SetSpeed(float speed) { this->speed = speed; }
	void SetMaxSpeed(float maxSpeed) { this->maxSpeed = maxSpeed; }

private:
	float maxSpeed = 300.0f;
	float speed = maxSpeed;
};
