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

private:
	unique_ptr<TileMap> tileMap;

	unique_ptr<Player> player;

	bool bMod = false;
};
