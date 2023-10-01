#pragma once

enum class ShyguyState
{
	Run,
	Hold,
	Death,
};

class Shyguy : public IEnemy
{
public:	
	Shyguy(const Vector2& position, float totailSize, float speed, int maxHp, Direction direction);

public:
	virtual void Init() override;
	virtual void Collision(shared_ptr<Player> player) override;
	virtual void Turn() override;
	virtual void Destroy() override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUI(int ordinal) override;

	virtual shared_ptr<AnimationRect> GetAnimRect() override { return animRect; }
	virtual int GetHp()  override { return hp; }
	virtual bool GetDelete() override { return bDelete; }
	virtual EnemyState GetState() override { return state; }
	virtual bool GetIsThrow() override { return bThrow; }

	virtual void SetState(EnemyState state)  override { this->state = state; }
	virtual void SetHit(bool hit, Vector2 hitPos) override { hp -= hit; this->hitPos = hitPos; }
	virtual void SetGroundPos(Vector2 groundPos) override { this->groundPos = groundPos; }
	virtual void SetIsMod(bool bMod) override  { this->bMod = bMod; }

public:
	void Move();
	void Death();

private:
	shared_ptr<AnimationRect> animRect;

	EnemyState state;
	ShyguyState animState = ShyguyState::Run;

	// currentState
	int hp;
	Direction direction;

	// ground
	Vector2 groundPos = Vector2(-1000, -1000);
	bool bGround = false;

	Vector2 hitPos = Vector2();
	float speed = 0.0f;

	float vel = 0.0f;
	float Gdrainage = 150.0f;
	float jumpSpeed = 0.0f;

	bool bHolding = false;
	bool bHold = false;
	bool bThrow = false;

	bool bTurn = false;
	float turnTime = 0.0f;
	float attackTime = 0.0f;

	bool bDeath = false;
	bool bCheck = true;

	bool bMod = true;
	bool bDelete = false;

	bool bClick = false;
};
