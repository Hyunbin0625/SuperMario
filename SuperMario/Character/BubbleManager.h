#pragma once

class Bubble
{
public:
	Bubble();

public:
	void Init(Vector2 position, float rotation, float bulletSpeed);

public:
	void Update();
	void Render();

public:
	shared_ptr<AnimationRect> GetAnimRect() { return animRect; }
	bool GetActivation() { return bActivation; }

	void SetActivation(bool bActivation) { this->bActivation = bActivation; }
	void SetSpeed(float speed) { this->speed = speed; }
	void SetTotalSize(float totalSize) { this->totalSize = totalSize; }

private:
	shared_ptr<AnimationRect> animRect;
	bool bActivation = false;

	float vel = 0.0f;
	float jumpSpeed = 0.0f;
	float speed = 0.0f;

	float totalSize = 1.0f;
};

class BubbleManager
{
public:
	BubbleManager(UINT totalBullet);

public:
	void CreateBullet();
	void Init(Vector2 position, float rotation, float bulletSpeed);
	void IndexManagement();

	void Update();
	void Render();

public:
	shared_ptr<Bubble> GetBullet(int index) { return bullets[index]; }
	vector<shared_ptr<Bubble>> GetBullets() { return bullets; }

	int GetCurrentIndex() { return currentIndex; }

	void SetCurrentIndex(int currentIndex) { this->currentIndex = currentIndex; }
	void SetPosition(Vector2 position) { this->position = position; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetTotalSize(float totalSize) { this->totalSize = totalSize; }
	void SetBulletSpeed(float bulletSpeed) { this->bulletSpeed = bulletSpeed; }

private:
	vector<shared_ptr<Bubble>> bullets;

	UINT totalBullet = 1;
	float bulletSpeed = 0.0f;

	// Index
	int currentIndex = 0;

	Vector2 position = Vector2();
	float rotation = 0.0f;

	float totalSize = 1.0f;
};