#pragma once

enum class EnemyType
{
	None = -1,
	Shyguy,
	Turtle,
	Wart
};

#pragma pack(push, 1)
struct EnemyState
{
	EnemyType type = EnemyType::None;

	Vector2 position = Vector2();
	float totalSize = 1;
	float speed = 0.0f;

	int maxHp = 0;
	Direction direction = Direction::R;
};
#pragma pack(pop)

class IEnemy
{
public:
	IEnemy() = default;
	virtual ~IEnemy() = default;

public:
	virtual void Init() = 0;
	virtual void Collision(shared_ptr<Player> player) = 0;
	virtual void Turn() = 0;
	virtual void Destroy() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void GUI(int ordinal) = 0;

	virtual shared_ptr<AnimationRect> GetAnimRect() = 0;
	virtual int GetHp() = 0;
	virtual bool GetDelete() = 0;
	virtual EnemyState GetState() = 0;
	virtual bool GetIsThrow() = 0;


	virtual void SetState(EnemyState state) = 0;
	virtual void SetHit(bool hit, Vector2 hitPos) = 0;
	virtual void SetGroundPos(Vector2 groundPos) = 0;
	virtual void SetIsMod(bool bMod) = 0;
};
