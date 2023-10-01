#pragma once

class TextureRect;
class Ui
{
public:
	DECLARE_SINGLETON(Ui);

public:
	void Init(UINT currentHp);

public:
	void Update();
	void Render();

private:
	vector<unique_ptr<TextureRect>> hpList;

	UINT currentHp = 0;
};
