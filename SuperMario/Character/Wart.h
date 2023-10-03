#pragma once

enum class WartState
{
	Idle,
	Run,
	Attack,
	Hit,
	Death,
};

class Wart : public IEnemy
{
public:	
	Wart(const Vector2& position, float totailSize, float speed, float Gdrainage, int maxHp, Direction direction);

public:
	virtual void Init() override;
	virtual void Collision(shared_ptr<Player> player) override;
	virtual void Turn() override {}
	virtual void Destroy() override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUI(int ordinal) override;

	virtual shared_ptr<AnimationRect> GetAnimRect() override { return animRect; }
	virtual int GetHp()  override { return hp; }
	virtual bool GetDelete() override { return bDelete; }
	virtual EnemyState GetState() override { return state; }
	virtual bool GetIsThrow() override { return false; }

	virtual void SetHit(bool hit, Vector2 hitPos) override { hp -= hit; this->hitPos = hitPos; }
	virtual void SetGroundPos(Vector2 groundPos) override { this->groundPos = groundPos; }
	virtual void SetIsMod(bool bMod) override  { this->bMod = bMod; }

public:
	void Move();
	void Death();

private:
	shared_ptr<AnimationRect> animRect;
	shared_ptr<BubbleManager> bullets;

	EnemyState state;
	WartState animState = WartState::Run;

	// currentState
	int hp;
	Direction direction;
	mt19937 mt;

	// ground
	Vector2 groundPos = Vector2(-1000, -1000);
	bool bGround = false;

	Vector2 hitPos = Vector2();
	float speed = 0.0f;

	float vel = 0.0f;
	float jumpSpeed = 0.0f;

	float deltaTime = 0.0f;
	bool bCheckTime = false;
	bool bFront = true;

	int count = 0;
	float attackTime = 0.0f;

	float hitTime = 0.0f;
	bool bHit = false;

	bool bIdle = true;
	bool bRun = false;
	bool bAttack = false;

	bool bDeath = false;
	bool bCheck = true;

	bool bMod = true;
	bool bDelete = false;

	bool bClick = false;
};
