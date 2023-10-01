#pragma once

enum class ItemType
{
	None = -1,
	Star,
	Coin,
	S_Radish,
	B_Radish,
};

#pragma pack(push, 1)
struct ItemState
{
	ItemType type = ItemType::None;

	Vector2 position = Vector2();;
	float totalSize = 1.0f;
};
#pragma pack(pop)

class IItem
{
public:
	IItem() = default;
	virtual ~IItem() = default;

public:
	virtual void Init() = 0;
	virtual void Collision(shared_ptr<Player> player) = 0;
	virtual void Reverse() = 0;
	virtual void Death() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void GUI(int ordinal) = 0;

	virtual shared_ptr<TextureRect> GetTextureRect() = 0;
	virtual bool GetDelete() = 0;
	virtual ItemState GetState() = 0;
	virtual bool GetIsThrow() = 0;

	virtual void SetState(ItemState state) = 0;

	virtual void SetIsGround(bool bGround) = 0;
	virtual void SetIsMod(bool mod) = 0;
};
