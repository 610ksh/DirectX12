#include "pch.h"
#include "DescriptorHeap.h"
#include "SwapChain.h" // 전방선언 했던것 여기서 추가

void DescriptorHeap::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;

	// Descriptor (DX12) = View (~DX11)
	// [서술자 힙]으로 RTV 생성
	// DX11의 RTV(RenderTargetView), DSV(DepthStencilView), 
	// CBV(ConstantBufferView), SRV(ShaderResourceView), UAV(UnorderedAccessView)를 전부 하나로 합침!

	_rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// 이번에도 DescriptorHeap을 생성하기 위한 설명서 도구 제작. rtv 설명서.
	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // RTV
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT; // 버퍼 숫자만큼 DescriptorHeap도 생성한다.
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // NONE
	rtvDesc.NodeMask = 0;

	// 같은 종류의 데이터끼리 배열로 관리
	// RTV 목록 : [ ] [ ]    (2개)
	// DescriptorHeap 객체 생성 -> rtvHeap
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap)); // rtvHeap은 배열 형태임.

	// RTV의 시작 위치 주소를 들고 있음. 정수 형태란 게 특징.
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// d3dx12.h 헬퍼 클래스 기능을 이용함.
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * _rtvHeapSize);
		device->CreateRenderTargetView(swapChain->GetRenderTarget(i).Get(), nullptr, _rtvHandle[i]);
	}

	/// 여기까지 진행하면 GPU에게 view라는것을 넘겨줄 수 있는 상태가 된것. 포장 끝!
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetBackBufferView()
{
	// 현재 SwapChain에서 backBufferIndex를 이용해서 rtv를 반환
	return GetRTV(_swapChain->GetCurrentBackBufferIndex());
}
