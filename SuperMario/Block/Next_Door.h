#pragma once

class Next_Door : public IBlock
{
public:
	Next_Door(const Vector2& position, const float& spacing, const Vector2& tileIndex = Vector2(-1, -1), float regen = -1);

public:
	virtual void Init() override;
	virtual void Collision(shared_ptr<Player> player) override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUI(int ordinal) override;

	virtual shared_ptr<TextureRect> GetTextureRect() override { return textureRect; }
	virtual BlockState GetState() override { return state; }
	virtual ItemType GetItemType() override { return ItemType::None; }
	virtual bool GetIsHolding() override { return false; }
	virtual bool GetIsClick() override { return bClick; }
	virtual bool GetDelete() override { return bDelete; }

	virtual void SetTileIndex(Vector2 tileIndex) override { state.tlieIndex = tileIndex; }
	virtual void SetSpacing(UINT spacing) override { this->spacing = spacing; }
	virtual void SetIsHolding(bool bHolding) override {}
	virtual void SetIsCheck(bool bCheck) override { this->bCheck = bCheck; }
	virtual void SetIsMod(bool bMod) override { this->bMod = bMod; }

private:
	shared_ptr<TextureRect> textureRect;
	BlockState state;

	UINT spacing = 0;

	bool bClick = false;

	bool bCheck = true; 
	bool bDelete = false;
	bool bMod = true;
};
