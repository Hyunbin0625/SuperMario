#pragma once

enum PlayerState
{
	None = -1,
	Idle,
	Run,
	Jump,
	Up,
	Hold,
	Throw,
	Death
};

enum Direction
{
	L = 0,
	R,
};

class Player
{
public:
	Player(const Vector2& position, const float& scale);

public:
	void Update();
	void Render();

	void GUI();

public:
	shared_ptr<AnimationRect> GetAnimRect() { return animRect; }
	Vector2 GetPosition() { return position; }
	float GetSpeed() { return speed; }

	void SetVel(float vel) { this->vel = vel; }
	void SetJumpSpeed(float jumpSpeed) { this->jumpSpeed = jumpSpeed; }

	void SetIsGround(bool bGround) { this->bGround = bGround; }
	void SetMod(bool bMod) { this->bMod = bMod; }

private:
	shared_ptr<AnimationRect> animRect;

	PlayerState state = PlayerState::None;
	Direction direction = Direction::R;

	int hp = 1;
	Vector2 position = Vector2();
	float totalSize = 1.0f;
	float speed = 100.0f;

	float vel = 0;
	float Gdrainage = 150.0f;
	float jumpSpeed = 0;
	float jumpMaxSpeed = 400;

	bool bGround = false;
	bool bClick = false;
	bool bMod = true;
};
