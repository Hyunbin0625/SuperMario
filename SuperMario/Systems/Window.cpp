#include "stdafx.h"
#include "Window.h"
#include "Program.h"

Window::Window(const WinDesc& initDesc)
    : desc(initDesc)
{
    WORD wHr = MyRegisterClass(desc);   // MyRegisterClass 함수를 호출해서 desc를 넘겨준다.
    assert(wHr != 0);   // 재대로 생성되었다면 통과, 안됐다면 0으로 통과할 수 없다.

    desc.handle = CreateWindowExW     // 윈도우 생성 함수, 만들 윈도우창들을 공통적인 특징이 될 수 있는 class만 등록, 반환형 HWND(Handle Window) 윈도우창 주소값 담고 있다.
    (
        WS_EX_APPWINDOW,        // 확장 창 스타일 공식문서 찾기 과제
        desc.appName.c_str(),   // class 이름
        desc.appName.c_str(),   // 윈도우 창 이름
        WS_OVERLAPPEDWINDOW,    // 윈도우 스타일, 흔하게 볼수 있는 윈도우창 스타일, 6개의 스타일을 가지고 있다.
        /*
        WS_OVERLAPPED     창이 겹치는 창, 제목표시줄의 틀만 있다.
        WS_CAPTION        창에 제목표시줄이 있다. 선행조건이 없다.
        WS_SYSMENU        제목표시줄에 창 메뉴가 있다. 선행조건 WS_CAPTION
        WS_THICKFRAME     창에 크기조절 테두리가 있다.
        WS_MINIMIZEBOX    최소화 버튼, 선행조건 WS_SYSMENU
        WS_MAXIMIZEBOX)   최대화 버튼, 선행조건 WS_SYSMENU
        */
        // X, Y : 생성될 창에 위치
        CW_USEDEFAULT,   // X, defualt 값을 적용
        CW_USEDEFAULT,   // Y
        CW_USEDEFAULT,   // Width
        CW_USEDEFAULT,   // Height
        nullptr,         // Parent의 window handle, 부모창이 따로 있는가, 있다면 handle을 달라는 뜻
        (HMENU)nullptr,  // Menu, 안만들었기때문에 nullptr
        desc.instance,   // 운영체제가 정한 프로그램의 주소값
        nullptr          // ??
    );

    // 사각형 영역에 대해서 정의
    RECT rect = { 0, 0, (LONG)desc.width, (LONG)desc.height };

    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW); // 창크기를 rect에 맞쳐서 바꿔주는 함수

    // 위치 지정
    UINT centerX = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;  // GetSystemMetrics 함수로 X길이
    UINT centerY = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2; // GetSystemMetrics 함수로 Y길이

    MoveWindow(desc.handle, centerX, centerY, rect.right - rect.left, rect.bottom - rect.top, true); // repaint 다시 그릴지

    ShowWindow(desc.handle, SW_SHOWNORMAL); // SW_SHOWNORMAL : 창을 기본적으로 활성 상태로 만든다. 윈도우창이 최소화, 최대화된 경우 원래 상태로 복원한다. 윈도우창 활성화. 기본적인 옵션이 여러게 있다. 
    UpdateWindow(desc.handle);  // 대기중이던 WM_PAINT를 우선순위로 처리한다.

    ShowCursor(true);   // 마우스 커서가 윈도창에서 보일것인가 안보일것인가
}

Window::~Window()
{
    DestroyWindow(desc.handle);
    UnregisterClassW(desc.appName.c_str(), desc.instance);
}

ATOM Window::MyRegisterClass(const WinDesc& Desc)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);   // 버전 확인을 위한 size check

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;   // cs : class style, H(horizon) : 수평선, V(vertical) : 수직의, REDRAW : 다시 그린다. DC(Device Context) : 무언가 화면에 그릴때 기본정보를 담고 있다. 항상 DC 통해서 그려야한다. CS_DBLCLKS : 더블 클릭에 대해서 처리유무
    wcex.lpfnWndProc = WndProc; // 메시지 처리함수
    wcex.cbClsExtra = 0;    // 쓸일이 없고 0으로 둔다.
    wcex.cbWndExtra = 0;    // 특수한 경우 제외
    wcex.hInstance = desc.instance;
    wcex.hIcon = LoadIcon(desc.instance, MAKEINTRESOURCE(IDI_CH));  // 작업표시줄 icon, IDI_WINLOGO : 기본적인 형태의 로고
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);  // 마우스 커서 모양
    wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);    // 윈도우창의 기본배경색, HBRUSH타입으로 형변환, GetStockObject : 함수를 사용해 색이름 색 지정
    wcex.lpszMenuName = nullptr;           // 윈도우창 메뉴, 게임에서 메뉴를 사용하지 않기 때문에 nullptr
    wcex.lpszClassName = desc.appName.c_str();     // 등록하고자 하는 윈도우 클래스 이름, lpszClassName는 LPCWSTR으로 WCHAR의 포인터로 받기 때문에 wstring인 appName을 c_str를 사용해 wchar 포인터로 바꿔 대입
    wcex.hIconSm = wcex.hIcon;    // 윈도우창 왼쪽 상단 아이콘

    return RegisterClassExW(&wcex); // 재대로 등록이 안된다면 0반환, unsigned short 2byte
}

WPARAM Window::Run()
{
    MSG msg = { 0 };

    program = make_unique<Program>();
    program->Init();

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            INPUT->Update();
            TIME->Update();
            
            IMGUI->Update();

            program->Update();

            program->PreRender();
            GRAPHICS->Begin();
            {
                program->Render();
                program->PostRender();
                IMGUI->Render();
            }
            GRAPHICS->End();
        }
    }
    return msg.wParam;
}

LRESULT Window::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (gHandle != nullptr)
    {
        INPUT->InputProc(message, lParam);
        if (IMGUI->MsgProc(handle, message, wParam, lParam))
            return true;
    }

    switch (message)
    {
    case WM_CREATE:
    {
        gHandle = handle;
    }
    break;
    case WM_SIZE:   // ﻿어떤 방식으로 윈도우 창의 크기가 변경될때 발생하는 메시지
    {
        gWinWidth = (float)GET_X_LPARAM(lParam);
        gWinHeight = (float)GET_Y_LPARAM(lParam);
        GRAPHICS->Resize(gWinWidth, gWinHeight);    // 사이즈 변경될때 호출해 재생성
    //  cout << gWinWidth << ", " << gWinHeight << '\n';
    }
    break;
    case WM_CLOSE:
    {
        PostQuitMessage(0);

        return 0;
    }
    }
    return DefWindowProc(handle, message, wParam, lParam);
}
