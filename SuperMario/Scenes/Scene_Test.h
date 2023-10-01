#pragma once

class Test : public IScene
{
public:
	void Init() override;
	void Destroy() override;

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;

public:
	void Collision();

private:
	unique_ptr<TileMap> tileMap;

	shared_ptr<Player> player;

	vector<shared_ptr<IItem>> itemList;
	unique_ptr<ItemSet> itemSet;

	vector<shared_ptr<IEnemy>> enemyList;
	unique_ptr<EnemySet> enemySet;

	bool bMod = true;
};
