#pragma once

class B_Radish : public IItem
{
public:
	B_Radish(const Vector2& position, const float& totalSize, bool bCreated = false);

public:
	virtual void Init() override;
	virtual void Collision(shared_ptr<Player> player) override;
	virtual void Reverse() override;
	virtual void Death() override;

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUI(int ordinal) override;

	virtual shared_ptr<TextureRect> GetTextureRect() override { return textureRect; }
	virtual bool GetDelete() override { return bDelete; }
	virtual ItemState GetState() override { return state; }
	virtual bool GetIsThrow() override { return bThrow; }
	virtual bool GetIsCreated() override { return bCreated; }

	virtual void SetIsGround(bool bGround) override { this->bGround = bGround; }
	virtual void SetIsMod(bool bMod) override { this->bMod = bMod; }

private:
	shared_ptr<TextureRect> textureRect;
	ItemState state;

	float vel = 0.0f;
	float Gdrainage = 150.0f;
	float jumpSpeed = 0.0f;
	float speed = 0.0f;

	bool bHolding = false;
	bool bHold = false;
	bool bThrow = false;

	bool bDeath = false;
	bool bCheck = true;
	bool bGround = false;

	bool bCreated = false;
	bool bMod = true;
	bool bDelete = false;

	bool bClick = false;
};
