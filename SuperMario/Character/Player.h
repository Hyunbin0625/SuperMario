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
	Vector2 GetPosition() { return position; }

	void SetMod(bool bMod) { this->bMod = bMod; }

private:
	shared_ptr<AnimationRect> animRect;

	PlayerState state = PlayerState::None;
	Direction direction = Direction::R;

	int hp = 1;
	Vector2 position = Vector2();
	float totalSize = 1.0f;

	bool bMod = false;
};
