#pragma once

struct WinDesc
{
	wstring appName = L"";
	HINSTANCE instance = nullptr;
	HWND handle = nullptr;
	float width = 0.0f;
	float height = 0.0f;
};

class Window
{
public:
	Window(const WinDesc& initDesc);
	~Window();

	ATOM MyRegisterClass(const WinDesc& Desc);

	WPARAM Run();


private:
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	// 객체와 관계없이 호출할 수 있도록 정적으로 선언

	WinDesc desc;

	unique_ptr<class Program> program;
};