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

	void SaveTileMap(const wstring& path = L"");
	void LoadTileMap(const wstring& path = L"");

public:
	void Collision();

private:
	unique_ptr<TileMap> tileMap;

	shared_ptr<Player> player;

	vector<shared_ptr<IItem>> itemList;
	unique_ptr<ItemSet> itemSet;

	vector<shared_ptr<IEnemy>> enemyList;
	unique_ptr<EnemySet> enemySet;

	vector<shared_ptr<IBlock>> blockList;
	unique_ptr<BlockSet> blockSet;

	int world_current = 0;
	bool bMod = true;
};
