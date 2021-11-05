#include "pch.h"
#include "Engine.h"
#include "Material.h" // for MaterialParams
#include "Transform.h" // for TransformMatrix
#include "Light.h"

// ��ġ ���� ���
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Resources.h"

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

	/// b0�� LightParams �뵵�� ����Ұ���. �� 1���� �����ϸ� �Ǵϱ� CBV size�� 1�� ����.
	CreateConstantBuffer(CBV_REGISTER::b0, sizeof(LightParams), 1);
	/// Constant Buffer ����. Transform �뵵�� Material �뵵. b0�� ���� �뵵
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformParams), 256); // b1�� �о����
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(MaterialParams), 256); // b2�� �о����

	CreateRenderTargetGroups();

	ResizeWindow(info.width, info.height); // ȭ�� ũ�⸦ ������.

	/// ��ġ �ʱ�ȭ
	GET_SINGLE(Input)->Init(info.hwnd);
	GET_SINGLE(Timer)->Init();

	GET_SINGLE(Resources)->Init();
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