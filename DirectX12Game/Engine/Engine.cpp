#include "pch.h"
#include "Engine.h"
#include "Material.h" // for MaterialParams
#include "Transform.h" // for TransformMatrix
#include "Light.h"

// 장치 관련 헤더
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Resources.h"

/// 엔진 초기화
void Engine::Init(const WindowInfo& info)
{
	_window = info;

	/*
		1. 전체 엔진 사용을 위한 모든 클래스 변수 생성 및 초기화
		2. 2개의 Constant Buffer 생성
		- Transform을 위한 b0 레지스터 사용. 연결
		- Material을 위한 b1 레지스터 사용. 연결
	*/

	// 그려질 화면 크기를 설정
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height); // 함수명 C가 붙은건 d3dx12.h 함수 이용함을 의미함.

	// 해당하는 변수들 초기화. 내용물이 생김
	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_tableDescHeap->Init(256);

	/// b0는 LightParams 용도로 사용할거임. 딱 1개만 셋팅하면 되니까 CBV size를 1로 해줌.
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	/// Constant Buffer 생성. Transform 용도와 Material 용도. b0는 전역 용도
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256); // b1로 밀어넣음
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParams), 256); // b2로 밀어넣음

	CreateRenderTargetGroups();

	ResizeWindow(info.width, info.height); // 화면 크기를 재조정.

	/// 장치 초기화
	GET_SINGLE(Input)->Init(info.hwnd);
	GET_SINGLE(Timer)->Init();

	GET_SINGLE(Resources)->Init();
}

void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	
	// 씬 업데이트
	GET_SINGLE(SceneManager)->Update();

	// 렌더링
	Render();

	// 현재 FPS 출력하는 함수
	ShowFps();
}

void Engine::Render()
{
	RenderBegin();

	// 해당 씬에 있는 모든 물체를 그린다.
	GET_SINGLE(SceneManager)->Render();

	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;

	// Rect : WinAPI (왼쪽위(원점), 오른쪽 아래), 2점으로 만드는 rectangle
	RECT rect = { 0, 0, width, height };
	// 윈도우 크기 조정, :: 의미는 WinAPI의 함수(글로벌)
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	// 윈도우 위치 설정, 윈도우 핸들 정보 필요
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}

void Engine::ShowFps()
{
	// timer에서 Fps를 가져옴. 매초마다 몇프레임인지
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	WCHAR text[100] = L"";
	// wsprintf는 문자열을 만들어주는 함수인데,
	// 중간의 문자열 포맷 형태를 만든다는 의미.
	// 3번째는 C언어의 printf 와 같은 문법.
	// 최종적으로 text에 그 문자열이 들어가는구조
	::wsprintf(text, L"FPS : %d", fps);

	// WinAPI로 화면에 출력하고 있다. 현재 창
	::SetWindowText(_window.hwnd, text);
}

// 사용할 레지스터 번호, 버퍼사이즈와 버퍼의 개수
void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	// 레지스터 번호를 받아옴
	uint8 typeInt = static_cast<uint8>(reg);
	// 순서대로 받고 있는지 체크하는 함수.
	// 0번부터 n번까지 레지스터를 반드시 순서대로 사용. 
	assert(_constantBuffers.size() == typeInt);

	// 버퍼를 내부에서 생성.
	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	// 버퍼 초기화 함수 호출.
	buffer->Init(reg, bufferSize, count);
	// 생성한 버퍼를 버퍼 배열에 추가
	_constantBuffers.push_back(buffer);
}


void Engine::CreateRenderTargetGroups()
{
	// DepthStencil
	shared_ptr<Texture> dsTexture = GET_SINGLE(Resources)->CreateTexture(L"DepthStencil",
		DXGI_FORMAT_D32_FLOAT, _window.width, _window.height,
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	// SwapChain Group
	{
		vector<RenderTarget> rtVec(SWAP_CHAIN_BUFFER_COUNT);

		for (uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
		{
			wstring name = L"SwapChainTarget_" + std::to_wstring(i);

			ComPtr<ID3D12Resource> resource;
			_swapChain->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&resource));
			rtVec[i].target = GET_SINGLE(Resources)->CreateTextureFromResource(name, resource);
		}

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)]->Create(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN, rtVec, dsTexture);
	}

	// Deferred Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"PositionTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"NormalTarget",
			DXGI_FORMAT_R32G32B32A32_FLOAT, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[2].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::G_BUFFER)]->Create(RENDER_TARGET_GROUP_TYPE::G_BUFFER, rtVec, dsTexture);
	}


	// Lighting Group
	{
		vector<RenderTarget> rtVec(RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT);

		rtVec[0].target = GET_SINGLE(Resources)->CreateTexture(L"DiffuseLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		rtVec[1].target = GET_SINGLE(Resources)->CreateTexture(L"SpecularLightTarget",
			DXGI_FORMAT_R8G8B8A8_UNORM, _window.width, _window.height,
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)] = make_shared<RenderTargetGroup>();
		_rtGroups[static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::LIGHTING)]->Create(RENDER_TARGET_GROUP_TYPE::LIGHTING, rtVec, dsTexture);
	}

}