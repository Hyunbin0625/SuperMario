#pragma once

enum PlayerState
{
	None = -1,
	Idle,
	Run,
	Jump,
	Hold,
	Hold_Idle,
	Hold_Run,
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
	void Init();

	void Update();
	void Render();

	void GUI();

public:
	shared_ptr<AnimationRect> GetAnimRect() { return animRect; }
	Direction GetDirection() { return direction; }
	Vector2 GetPosition() { return position; }
	float GetSpeed() { return speed; }

	bool GetIsHolding() { return bHolding; }
	bool GetIsHold() { return bHold; }
	bool GetIsThrow() { return bThrow; }

	void SetVel(float vel) { this->vel = vel; }
	void SetJumpSpeed(float jumpSpeed) { this->jumpSpeed = jumpSpeed; }

	void SetIsHolding(bool bHolding) { this->bHolding = bHolding; }
	void SetIsHold(bool bHold) { this->bHold = bHold; }

	void SetIsGround(bool bGround) { this->bGround = bGround; }
	void SetIsMod(bool bMod) { this->bMod = bMod; }

private:
	shared_ptr<AnimationRect> animRect;

	PlayerState state = PlayerState::None;
	Direction direction = Direction::R;

	int hp = 1;
	Vector2 position = Vector2();
	float totalSize = 1.0f;
	float speed = 200.0f;

	float vel = 0;
	float Gdrainage = 150.0f;
	float jumpSpeed = 0;
	float jumpMaxSpeed = 200;

	float dletaTime = 0.0f;

	bool bHolding = false;
	bool bHold = false;
	bool bThrow = false;

	bool bGround = false;
	bool bClick = false;
	bool bMod = true;
};
