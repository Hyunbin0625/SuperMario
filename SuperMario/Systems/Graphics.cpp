#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics()
{
    EnumerateAdapters();
    CreateSwapChain();
}

Graphics::~Graphics() {}

/*
    GPU에서 사용할 자원(핵심)
    크게 두종류
    Buffer : CPU에서 GPU로 값들을 전달하고 싶을 때 임시로 담는 역할
    Texture : 보통 어떤 그림 같은 걸 전달하고 싶을때, 하지만 반드시 그림만 보내지 않는다.

    Buffer 인덱스, 정점, 상수
    texture 1d, 2d, 3d

    GPU에게 명령을 내리기 위해
    shader HLSL(High Level Shader Language)
    인간이 활용하기 편리한 형태로 shader을 작성할 수 있다.
    컴파일(Compile) 하는 과정이 필요하다.

    자원뷰
    정점 버퍼, 인덱스 버퍼, 상수 버퍼
    자원뷰 없이 파이프라인에 꽂아 줄 수 있다.(분명한 목적성이 존재하기 때문에)

    Texture : 그림, 애니메이션 등 여러가지의 정보를 담을 수 있기 때문에 용도가 명확하지 않기 때문에
    자원뷰를 만들어서 연결한 후, 자원뷰를 렌더링 파이프라인에 꽂아준다. 따라서 연결된 자원이 넘어가며 어떤 용도인지 명확하게 할 수 있다.

    리소스 뷰
    바인딩해줄 리소스를 어떻게 사용할지 정의하는 것

    RTV(RenderTargetView)
    백버퍼를 만들 때 사용
    도화지의 개념
    ShaderResourceView
    자원을 shader에서 읽을 때 사용
    자원을 ShaderResourceView 형태로 만들어서 shader에 넘겨 줄 수 있다.
    DepthStencilView
    깊이 표현을 하는 뷰
    UnorderedAccessView
    */

void Graphics::Begin()
{
    SetViewport();
    SetBackBufferToRTV();
}

void Graphics::End()
{
    // 다 그린후 swapchiain을 통해 frontBuffer로 swap
    HRESULT hr = swapChain->Present(1, 0);   // 수직 동기화
    CHECK(hr);
}

void Graphics::Resize(float width, float height)
{
    DeleteSurface();    // 초기화
    {
        HRESULT hr = swapChain->ResizeBuffers   // 크기 재지정, 0 = 이전에 설정된 값, 이외의 값 : 해당 값으로 정보 수정
        (
            0,
            (UINT)gWinWidth,
            (UINT)gWinHeight,
            DXGI_FORMAT_UNKNOWN,    // 0, 가독성을 위해서 사용
            0
        );
        CHECK(hr);
    }
    CreateRenderTargetView();
    SetViewport(width, height);

    cout << width << " X " << height << '\n';
}

void Graphics::SetViewport(float width, float height)
{
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;  // 2D 공간이라도, 0~1로 기본 공간이다.
    viewport.MaxDepth = 1.0f;
}

void Graphics::SetViewport()
{
    // RS 단계에서 뷰표트 변환 세팅
    deviceContext->RSSetViewports(1, &viewport);
}

void Graphics::SetBackBufferToRTV()
{
    // OM 단계에서 도화지 세팅, 그릴 대상 세팅
    deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
    // 매 프레임에 업데이트 되기 때문에 기본색으로 초기화
    deviceContext->ClearRenderTargetView(rtv.Get(), clearColor);
}

bool Graphics::GetFullScreenState()
{
    BOOL bFullScreen = false;
    swapChain->GetFullscreenState(&bFullScreen, nullptr);

    return bFullScreen;
}

void Graphics::SetFullScreenState(bool bFullScreen)
{
    swapChain->SetFullscreenState(bFullScreen, nullptr);
    Resize(gWinWidth, gWinHeight);
}

void Graphics::EnumerateAdapters()
{
    ComPtr<IDXGIFactory1> factory;  // 뒤에 붙는 숫자는 버전을 의미

    HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), &factory);
    CHECK(hr);

    UINT index = 0;
    while (true)
    {
        ComPtr<IDXGIAdapter1> adapter;

        HRESULT hr = factory->EnumAdapters1(index, &adapter);   // 생성한 factory를 통해서 index를 넣어 adapter를 받아온다.

        if (hr == DXGI_ERROR_NOT_FOUND)  // 어뎁터를 못찾은 경우의 에러
            break;  // index에 맞는 어뎁터가 없는 경우 탈출

        CHECK(hr);

        auto adapterInfo = make_shared<D3DEnumAdapterInfo>();
        adapterInfo->adapterOrdinal = index;

        hr = adapter->GetDesc1(&adapterInfo->adapterDesc);
        CHECK(hr);
        adapterInfo->adapter = adapter;

        EnumerateAdapterOutput(adapterInfo);
        adapterInfos.push_back(adapterInfo);

        ++index;
    }
}

bool Graphics::EnumerateAdapterOutput(const shared_ptr<D3DEnumAdapterInfo>& adapterInfo)
{
    ComPtr<IDXGIOutput> outputBase;
    
    HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &outputBase);

    if (hr == DXGI_ERROR_NOT_FOUND)
        return false;

    ComPtr<IDXGIOutput1> output;

    outputBase->QueryInterface<IDXGIOutput1>(&output);

    auto outputInfo = make_shared<D3DEnumOutputInfo>();

    hr = output->GetDesc(&outputInfo->outputDesc);
    CHECK(hr);

    outputInfo->output = output;

    UINT numModes = 0;
    vector<DXGI_MODE_DESC1> displayModes;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

    hr = output->GetDisplayModeList1(format, 0, &numModes, nullptr);
    CHECK(hr);

    displayModes.assign(numModes, DXGI_MODE_DESC1());
    hr = output->GetDisplayModeList1(format, 0, &numModes, displayModes.data());
    CHECK(hr);

    for (const auto& mode : displayModes)
    {
        resolutionList.push_back(Vector3((float)mode.Width, (float)mode.Height, (float)mode.RefreshRate.Numerator /mode.RefreshRate.Denominator));

        if (mode.Width == WIN_DEFAULT_WIDTH && mode.Height == WIN_DEFAULT_HEIGHT)
        {
            outputInfo->numerator = mode.RefreshRate.Numerator;
            outputInfo->denominator = mode.RefreshRate.Denominator;
        }
    }

    for (const auto& res : resolutionList)
        cout << res.x << " X " << res.y << " (" << res.z << ")" << '\n';

    adapterInfo->outputInfo = outputInfo;

    return true;
}

void Graphics::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC desc = { 0 };
    //	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));	// 초기화

    desc.BufferDesc.Width = 0;
    desc.BufferDesc.Height = 0;

    UINT maxVideoMemory = 0;
    for (UINT i = 0; i < adapterInfos.size(); ++i)
    {
        if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVideoMemory)  // 비디오 메모리가 큰 경우, 메인 그래픽 카드
        {
            selectedAdapterIndex = i;
            maxVideoMemory = (UINT)adapterInfos[i]->adapterDesc.DedicatedVideoMemory;
        }
    }

    const auto& adapterInfo = adapterInfos[0];

    numerator = adapterInfo->outputInfo->numerator;
    denominator = adapterInfo->outputInfo->denominator;

    if (bVsync)
    {
        desc.BufferDesc.RefreshRate.Numerator = numerator;
        desc.BufferDesc.RefreshRate.Denominator = denominator;
    }
    else
    {
        desc.BufferDesc.RefreshRate.Numerator = 0;  // 분자 / 문모가 0일때 프레임 제한 X
        desc.BufferDesc.RefreshRate.Denominator = 1;
    }
    
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // 나타낼 화면 각 채널당 색상을 얼마나 할당할 것인지 , R8G8B8A8_UNORM
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // buffer가 어떤 용도로 사용될 것인지

    desc.BufferCount = 1;   // 버퍼의 개수, backBuffer가 하나임으로 1개

    desc.OutputWindow = gHandle;    // 띄울 윈도우창의 핸들을 받는다.

    desc.Windowed = true;   // 창 모드 사용할 것인지

    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // backBuffer과 frontBuffer가 swap을 할때 어떠한 효과를 추가할 수 있다, 기본값으로 아무 효과가 없다.

    vector<D3D_FEATURE_LEVEL> featureLevel
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1, // 해당 컴퓨터에서 11버전이 안될 경우 버전을 내리며 사용가능한 버전을 사용
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL currentFeature;

    // 객체 생성 요청
    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,   // 게임 개발에 흔히 쓰이는 옵션
        nullptr,
        0,
        featureLevel.data(),
        (UINT)featureLevel.size(),
        D3D11_SDK_VERSION,
        &desc,
        &swapChain, // & : 이미 할당된 자원이 해제가 되며, 해당 스마트 포인터 객체를 가지고 온다.
        &device,
        &currentFeature,    // 현재 시스템이 사용하고 있는 Level을 담아준다.
        &deviceContext
    );

    CHECK(hr);  // hr가 SUCCEEDED를 통과 했을때만 assert 통과

    gpuMemorySize = maxVideoMemory / 1024 / 1024;
    gpuDescription = adapterInfos[selectedAdapterIndex]->adapterDesc.Description;

    cout << '\n' << "DedicatedVideoMemory : " << gpuMemorySize << '\n';
    wcout << "GPU Description : " << gpuDescription << '\n';

    cout << "Numerator : " << numerator << '\n';
    cout << "Denominator : " << denominator << '\n';
    cout << "RefreshRate : " << numerator / denominator << "\n\n";

    Resize(WIN_DEFAULT_WIDTH, WIN_DEFAULT_HEIGHT);
}

void Graphics::CreateRenderTargetView()
{
    // 백버퍼는 Texture2D의 형태가 더 적합하다.
    ComPtr<ID3D11Texture2D> backBuffer;
    // 스왑체인으로부터 백버퍼를 받아오는 함수
    HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);  // 몇번째 버퍼, __uuidof : 인터페이스의 아이디 값을 가져온다., 표면
    CHECK(hr);

    // 자원뷰의 생성
    hr = device->CreateRenderTargetView
    (
        backBuffer.Get(),
        nullptr,    // swapChain의 GetBuffer 함수를 통해 필요한 정보가 들어있는 자원이 있기 때문에
        &rtv
    );
    CHECK(hr);
}

void Graphics::DeleteSurface()  // RTV를 날리는 과정
{
    rtv.Reset();
}
