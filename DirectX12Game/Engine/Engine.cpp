#include "pch.h"
#include "Engine.h"


void Engine::Init(const WindowInfo& info)
{
	_window = info;

	// 그려질 화면 크기를 설정
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height); // 함수명 C가 붙은건 d3dx12.h 함수 이용함을 의미함.

	// 해당하는 변수들 초기화. 내용물이 생김
	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_cb->Init(sizeof(Transform), 256);
	_tableDescHeap->Init(256);
	// 참고로 밑의 코드는 ResizeWindow에서 한번더 호출하고 있다.
	// 학습을 목적으로 전체적으로 어떤 구조인지 이해하기 위해 나두도록 하자.
	_depthStencilBuffer->Init(_window);

	/// 장치 초기화
	_input->Init(info.hwnd);
	_timer->Init();

	ResizeWindow(info.width, info.height); // 화면 크기를 재조정.
}

void Engine::Render()
{
	RenderBegin();

	// TODO : 나머지 물체를 그린다.

	RenderEnd();
}

void Engine::Update()
{
	_input->Update();
	_timer->Update();

	// 현재 FPS 출력하는 함수
	ShowFps();
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

	// DSB 초기화 (DepthStencilBuffer)
	_depthStencilBuffer->Init(_window);
}


void Engine::ShowFps()
{
	// timer에서 Fps를 가져옴. 매초마다 몇프레임인지
	uint32 fps = _timer->GetFps();

	WCHAR text[100] = L"";
	// wsprintf는 문자열을 만들어주는 함수인데,
	// 중간의 문자열 포맷 형태를 만든다는 의미.
	// 3번째는 C언어의 printf 와 같은 문법.
	// 최종적으로 text에 그 문자열이 들어가는구조
	::wsprintf(text, L"FPS : %d", fps);

	// WinAPI로 화면에 출력하고 있다. 현재 창
	::SetWindowText(_window.hwnd, text);
}