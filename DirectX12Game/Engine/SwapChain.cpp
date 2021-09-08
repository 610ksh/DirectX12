#include "pch.h"
#include "SwapChain.h"
#include "Engine.h"
#include "Device.h"

void SwapChain::Init(const WindowInfo & info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// CreateSwapChain
	CreateSwapChain(info, dxgi, cmdQueue);
	// CrateRTV
	CreateRTV(device);
}

void SwapChain::CreateSwapChain(const WindowInfo & info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// ������ ���� ���� ������
	_swapChain.Reset();

	// ����(����) ��ü�� ȭ�� �ػ󵵿� ���ƾ� �ϱ� ������ �����ϰ� ����������.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(info.width); // ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<uint32>(info.height); // ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� ���� ���� // ���� (���ӷ��̷�)
	sd.BufferDesc.RefreshRate.Denominator = 1; // ȭ�� ���� ���� // �и� (��ʹ̳��̷�)
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ ���÷��� ���� (RGBA, 8��Ʈ�� = 32bit)
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // ����+�ĸ� ������ ���� (������۸�, 2��)
	sd.OutputWindow = info.hwnd; // ȭ�� �ڵ� ����
	sd.Windowed = info.windowed; // ������ ����(��üȭ�� or â���)
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����(discard)
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // flag ����

	// ���� SwapChain ���� �Լ�. ������ ���� sd�������� ���� _swapChain�� ����
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);

	// ���� ���� ��ŭ ���鼭 swapChain ������ ���� ���� Ÿ���� �������´�. ���� �ε����� renderTarget����
	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
}

// ������ DescriptorHeap�� Init �κ�. RTV�� ����� �뵵
void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// Descriptor (DX12) = View (~DX11)
	// [������ ��]���� RTV ����
	// DX11�� RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)�� ���� �ϳ��� ��ħ!

	int rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// �̹����� DescriptorHeap�� �����ϱ� ���� ���� ���� ����. rtv ����.
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // RTV
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // ���� ���ڸ�ŭ DescriptorHeap�� �����Ѵ�.
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // NONE
	rtvDesc.NodeMask = 0;

	// ���� ������ �����ͳ��� �迭�� ����
	// RTV ��� : [ ] [ ]    (2��)
	// DescriptorHeap ��ü ���� -> rtvHeap
	DEVICE->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap)); // rtvHeap�� �迭 ������.

	// RTV�� ���� ��ġ �ּҸ� ��� ����. ���� ���¶� �� Ư¡.
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// d3dx12.h ���� Ŭ���� ����� �̿���.
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);
	}

	/// ������� �����ϸ� GPU���� view��°��� �Ѱ��� �� �ִ� ���°� �Ȱ�. ���� ��!
}


// ���� �������� ����. CommandQueue�� RenderEnd���� ȣ��
void SwapChain::Present()
{
	// Present the frame.
	_swapChain->Present(0, 0);
}

// �� ������ �ε����� ��ü��. CommandQueue�� RenderEnd���� ȣ��
void SwapChain::SwapIndex()
{
	// �츮�� 2���ϱ� 0�� 1�� �ݺ��ǵ�����. 0 -> 1, 1 -> 0 ���� �ٲ�
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}
