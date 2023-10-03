#pragma once

enum class BlockType
{
	None = -1,
//	Next_Door,
	Grass_01,
};

#pragma pack(push, 1)
struct BlockState
{
	BlockType type = BlockType::None;
	Vector2 tlieIndex = Vector2();

	float regen = -1;	// -1 : false, 0~ : regenTime
};
#pragma pack(pop)

class IBlock
{
public:
	IBlock() = default;
	virtual ~IBlock() = default;

public:
	virtual void Init() = 0;
	virtual void Collision(shared_ptr<Player> player) = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void GUI(int ordinal) = 0;

	virtual shared_ptr<TextureRect> GetTextureRect() = 0;
	virtual BlockState GetState() = 0;
	virtual ItemType GetItemType() = 0;
	virtual bool GetIsHolding() = 0;
	virtual bool GetIsClick() = 0;
	virtual bool GetDelete() = 0;

	virtual void SetTileIndex(Vector2 tileIndex) = 0;
	virtual void SetSpacing(UINT spacing) = 0;
	virtual void SetIsHolding(bool bHolding) = 0;
	virtual void SetIsCheck(bool bCheck) = 0;
	virtual void SetIsMod(bool mod) = 0;
};
