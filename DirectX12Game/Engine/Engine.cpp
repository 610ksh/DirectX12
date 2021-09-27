#include "pch.h"
#include "Engine.h"
#include "Material.h" // for MaterialParams
#include "Transform.h" // for TransformMatrix

// ��ġ ���� ���
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

/// ���� �ʱ�ȭ
void Engine::Init(const WindowInfo& info)
{
	_window = info;

	/*
		1. ��ü ���� ����� ���� ��� Ŭ���� ���� ���� �� �ʱ�ȭ
		2. 2���� Constant Buffer ����
		- Transform�� ���� b0 �������� ���. ����
		- Material�� ���� b1 �������� ���. ����
	*/

	// �׷��� ȭ�� ũ�⸦ ����
	_viewport = { 0, 0, static_cast<FLOAT>(info.width), static_cast<FLOAT>(info.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height); // �Լ��� C�� ������ d3dx12.h �Լ� �̿����� �ǹ���.

	// �ش��ϴ� ������ �ʱ�ȭ. ���빰�� ����
	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_tableDescHeap->Init(256);
	// ����� ���� �ڵ�� ResizeWindow���� �ѹ��� ȣ���ϰ� �ִ�.
	// �н��� �������� ��ü������ � �������� �����ϱ� ���� ���ε��� ����.
	_depthStencilBuffer->Init(_window);

	/// Constant Buffer ����. Transform �뵵�� Material �뵵.
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(TransformParams), 256);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(MaterialParams), 256);

	ResizeWindow(info.width, info.height); // ȭ�� ũ�⸦ ������.

	/// ��ġ �ʱ�ȭ
	GET_SINGLE(Input)->Init(info.hwnd);
	GET_SINGLE(Timer)->Init();
}

void Engine::Update()
{
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	
	// �� ������Ʈ
	GET_SINGLE(SceneManager)->Update();

	// ������
	Render();

	// ���� FPS ����ϴ� �Լ�
	ShowFps();
}

void Engine::Render()
{
	RenderBegin();

	/// TODO : ������ ��ü�� �׸���.
	// �ش� ���� �ִ� ��� ��ü�� �׸���.
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
	uint32 fps = GET_SINGLE(Timer)->GetFps();

	WCHAR text[100] = L"";
	// wsprintf�� ���ڿ��� ������ִ� �Լ��ε�,
	// �߰��� ���ڿ� ���� ���¸� ����ٴ� �ǹ�.
	// 3��°�� C����� printf �� ���� ����.
	// ���������� text�� �� ���ڿ��� ���±���
	::wsprintf(text, L"FPS : %d", fps);

	// WinAPI�� ȭ�鿡 ����ϰ� �ִ�. ���� â
	::SetWindowText(_window.hwnd, text);
}

// ����� �������� ��ȣ, ���ۻ������ ������ ����
void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	// �������� ��ȣ�� �޾ƿ�
	uint8 typeInt = static_cast<uint8>(reg);
	// ������� �ް� �ִ��� üũ�ϴ� �Լ�.
	// 0������ n������ �������͸� �ݵ�� ������� ���. 
	assert(_constantBuffers.size() == typeInt);

	// ���۸� ���ο��� ����.
	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	// ���� �ʱ�ȭ �Լ� ȣ��.
	buffer->Init(reg, bufferSize, count);
	// ������ ���۸� ���� �迭�� �߰�
	_constantBuffers.push_back(buffer);
}