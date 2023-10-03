#pragma once

class Grass_01 : public IBlock
{
public:
	Grass_01(const Vector2& position, const float& spacing, const Vector2& tileIndex = Vector2(-1, -1), float regen = -1);

public:
	virtual void Init() override;
	virtual void Collision(shared_ptr<Player> player) override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUI(int ordinal) override;

	virtual shared_ptr<TextureRect> GetTextureRect() override { return textureRect; }
	virtual BlockState GetState() override { return state; }
	virtual ItemType GetItemType() override { return itemType; }
	virtual bool GetIsHolding() override { return bHolding; }
	virtual bool GetIsClick() override { return bClick; }
	virtual bool GetDelete() override { return bDelete; }

	virtual void SetTileIndex(Vector2 tileIndex) override { state.tlieIndex = tileIndex; }
	virtual void SetSpacing(UINT spacing) override { this->spacing = spacing; }
	virtual void SetIsHolding(bool bHolding) override { this->bHolding = bHolding; }
	virtual void SetIsCheck(bool bCheck) override { this->bCheck = bCheck; }
	virtual void SetIsMod(bool bMod) override { this->bMod = bMod; }

private:
	shared_ptr<TextureRect> textureRect;
	BlockState state;

	ItemType itemType = ItemType::None;
	mt19937 mt;

	UINT spacing = 0;

	float deltaTime = 0.0f;

	bool bHolding = false;
	bool bClick = false;

	bool bRegen = false;

	bool bCheck = true; 
	bool bDelete = false;
	bool bMod = true;
};
