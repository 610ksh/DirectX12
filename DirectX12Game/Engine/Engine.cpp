#include "pch.h"
#include "Engine.h"


void Engine::Init(const WindowInfo& info)
{
	_window = info;

	// �׷��� ȭ�� ũ�⸦ ����
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height); // �Լ��� C�� ������ d3dx12.h �Լ� �̿����� �ǹ���.

	// �ش��ϴ� ������ �ʱ�ȭ. ���빰�� ����
	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_cb->Init(sizeof(Transform), 256);
	_tableDescHeap->Init(256);
	// ����� ���� �ڵ�� ResizeWindow���� �ѹ��� ȣ���ϰ� �ִ�.
	// �н��� �������� ��ü������ � �������� �����ϱ� ���� ���ε��� ����.
	_depthStencilBuffer->Init(_window);

	/// ��ġ �ʱ�ȭ
	_input->Init(info.hwnd);
	_timer->Init();

	ResizeWindow(info.width, info.height); // ȭ�� ũ�⸦ ������.
}

void Engine::Render()
{
	RenderBegin();

	// TODO : ������ ��ü�� �׸���.

	RenderEnd();
}

void Engine::Update()
{
	_input->Update();
	_timer->Update();

	// ���� FPS ����ϴ� �Լ�
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

	// Rect : WinAPI (������(����), ������ �Ʒ�), 2������ ����� rectangle
	RECT rect = { 0, 0, width, height };
	// ������ ũ�� ����, :: �ǹ̴� WinAPI�� �Լ�(�۷ι�)
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	// ������ ��ġ ����, ������ �ڵ� ���� �ʿ�
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);

	// DSB �ʱ�ȭ (DepthStencilBuffer)
	_depthStencilBuffer->Init(_window);
}


void Engine::ShowFps()
{
	// timer���� Fps�� ������. ���ʸ��� ������������
	uint32 fps = _timer->GetFps();

	WCHAR text[100] = L"";
	// wsprintf�� ���ڿ��� ������ִ� �Լ��ε�,
	// �߰��� ���ڿ� ���� ���¸� ����ٴ� �ǹ�.
	// 3��°�� C����� printf �� ���� ����.
	// ���������� text�� �� ���ڿ��� ���±���
	::wsprintf(text, L"FPS : %d", fps);

	// WinAPI�� ȭ�鿡 ����ϰ� �ִ�. ���� â
	::SetWindowText(_window.hwnd, text);
}