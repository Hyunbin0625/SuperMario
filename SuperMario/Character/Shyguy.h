#pragma once

enum class ShyGuyState
{
	Run,
	Hold,
	Death,
};

class ShyGuy : public IEnemy
{
public:	
	ShyGuy(const Vector2& position, float totailSize, float speed, int maxHp, bool bRegen, float regenTime, Direction direction);

public:
	virtual void Collision(shared_ptr<Player> player) override;
	virtual void Init() override;
	virtual void Destroy() override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUI(int ordinal) override;

	virtual shared_ptr<AnimationRect> GetAnimRect() override { return animRect; }
	virtual shared_ptr<class LSeedManager> GetBullet() override { return nullptr; }

	virtual int GetHp()  override { return hp; }
	virtual bool GetDelete() override { return bDelete; }
	virtual ForestEnemyState GetState() override { return state; }

	virtual void SetState(ForestEnemyState state)  override { this->state = state; }
	virtual void SetHit(bool hit) override  { hp -= hit; }
	virtual void SetMod(bool mod) override  { this->bMod = mod; }
	virtual void SetWall(bool bWall) override { this->bWall = bWall; }
	virtual void SetGroundPos(Vector2 groundPos) override { this->groundPos = groundPos; }

public:
	void Move();

private:
	shared_ptr<AnimationRect> animRect;

	ForestEnemyState state;

	FwGruntState animState = FwGruntState::Run;

	// currentState
	int hp;
	Direction direction;

	// ground
	Vector2 groundPos = Vector2(-1000, -1000);
	bool bGround = false;

	bool bWall = false;

	float time = 0;

	bool bTurn = false;
	bool bMod = false;
	bool bDelete = false;

	// Sounds
	bool bFloatS = false;
	bool bLandS = false;
	bool bDeathS = false;
};
