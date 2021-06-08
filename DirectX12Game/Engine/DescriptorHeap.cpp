#include "pch.h"
#include "DescriptorHeap.h"
#include "SwapChain.h"

void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	// Descriptor (DX12) = View (~DX11)
	// [������ ��]���� RTV ����
	// DX11�� RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)�� ���� �ϳ��� ��ħ!

	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // RTV
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // ���� ���� : 2
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // NONE
	rtvDesc.NodeMask = 0;

	// ���� ������ �����ͳ��� �迭�� ����
	// RTV ��� : [ ] [ ]    (2��)
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap)); // �迭 ����

	// RTV�� ���� ��ġ�� ��� ����. �������¶��� Ư¡.
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// d3dx12.h ���� Ŭ���� ����� �̿���.
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
	}

	/// ������� �����ϸ� GPU���� view��°��� �Ѱ��� �� �ִ� ���°� �Ȱ�. ���� ��!
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView()
{
	// ���� SwapChain���� backBufferIndex�� �̿��ؼ� rtv�� ��ȯ
	return GetRTV(_swapChain->GetCurrentBackBufferIndex());
}
