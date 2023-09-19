#include "stdafx.h"
#include "Input.h"

Input::Input() {}

Input::~Input() {}

void Input::Update()
{
	keyOldState = move(keyState);

	if (GetKeyboardState(keyState.data()))
	{
		/*
		최상위 비트(1byte)
		현재 프레임에서 눌렸는지 안눌렸는지

		unsigned char -> 0 ~ 255
		0000 0000 ~ 1111 1111

		최하위 비트
		토글키(CAPS LOCK)가 켜져 있는지
		1 : On
		0 : Off

		128 = 1000 0000 = 0x80 = KEY_DOWN
		0000 0000(눌리지 않았다.)
		*/

		for (DWORD i = 0; i < MAX_INPUT_KEY; ++i)
		{
			keyState[i] = keyState[i] & 0x80 ? 1 : 0;	// 128 또는 0으로 삼항연산자를 사용해 1 또는 0으로 저장

			auto& oldState = keyOldState[i];
			auto& state = keyState[i];

			if (oldState == 0 && state == 1)
				keyMap[i] = KEY_INPUT_STATUS_DOWN;
			else if (oldState == 1 && state == 0)
				keyMap[i] = KEY_INPUT_STATUS_UP;
			else if (oldState == 1 && state == 1)
				keyMap[i] = KEY_INPUT_STATUS_PRESS;
			else
				keyMap[i] = KEY_INPUT_STATUS_NONE;
		}
	}
}

void Input::InputProc(UINT message, LPARAM lParam)
{
	// 메시지 함수에서 호출해서 처리
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)	// 마우스 관련 메시지만
	{
		mousePosition.x = (float)GET_X_LPARAM(lParam);
		mousePosition.y = WIN_DEFAULT_HEIGHT - (float)GET_Y_LPARAM(lParam);

		// 더블 클릭(운영체제에서 받아온다.)
		if (message == WM_LBUTTONDBLCLK)
			isDblClk = true;
		else
			isDblClk = false;

		mousePosition = CAMERA->UnProject(mousePosition);

	//	cout << mousePosition.x << ", " << mousePosition.y << '\n';
	}
}
