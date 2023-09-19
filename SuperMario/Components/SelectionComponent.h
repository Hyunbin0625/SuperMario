#pragma once

class SelectionComponent : public Component
{
public:
	SelectionComponent(UINT selection = 1, UINT slot = 0);

public:
	void Update() override;
	void Render() override;

public:
	void SetSelection(UINT selection) { SB->SetSelection(selection); }

private:
	unique_ptr<SelectionBuffer> SB;
	UINT slot = 0;
};
