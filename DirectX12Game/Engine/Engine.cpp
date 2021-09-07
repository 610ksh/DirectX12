#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"

void Engine::Init(const WindowInfo& info)
{
	_window = info;
	ResizeWindow(info.width, info.height);

	// 그려질 화면 크기를 설정
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height); // 함수명 C가 붙은건 d3dx12.h 함수 이용함을 의미함.

	// 각 클래스에 해당하는 변수들 동적할당으로 생성. (클래스를 인스턴스화함)
	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_descHeap = make_shared<DescriptorHeap>();

	// 해당하는 변수들 초기화. 내용물이 생김
	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain, _descHeap);
	_swapChain->Init(info, _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_descHeap->Init(_device->GetDevice(), _swapChain);
}

void Engine::Render()
{
	RenderBegin();

	// TODO : 나머지 물체를 그린다.

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

