#pragma once

enum class BlockType
{
	None = -1,
	RandomBox,
	Grass_01,
};

#pragma pack(push, 1)
struct BlockState
{
	BlockType type = BlockType::None;
	Vector2 tlieIndex = Vector2();

	bool bCheck = true;
};
#pragma pack(pop)

class IBlock
{
public:
	IBlock() = default;
	virtual ~IBlock() = default;

public:
	virtual bool Collision(shared_ptr<Player> player) = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void GUI(int ordinal) = 0;

	virtual shared_ptr<AnimationRect> GetAnimRect() = 0;
	virtual shared_ptr<TextureRect> GetTextureRect() = 0;
	virtual bool GetDelete() = 0;
	virtual BlockState GetState() = 0;
	virtual bool GetCollision() = 0;

	virtual void SetState(BlockState state) = 0;
	virtual void SetMod(bool mod) = 0;
};
