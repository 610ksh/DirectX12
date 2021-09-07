#pragma once


// [기안서]
// 외주(일감)를 GPU에게 맡길 때 이런 저런 정보들을 같이 넘겨줘야 하는데,
// 아무 형태로나 요청하면 GPU가 못 알아먹는다.
// - 각종 리소스를 어떤 용도로 사용하는지 꼼꼼하게 적어서 넘겨주는 용도
// 그냥 넘겨줄 수도 있는데, 한번 더 포장해서 넘겨준다고 이해하면 됨.
// DX12에 새롭게 추가된 개념임. DX11 이하에는 없음.

class DescriptorHeap // = View
{
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int32 idx) { return _rtvHandle[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE		GetBackBufferView();

private:
	// rtv : Render Target View의 줄임말.
	// DescriptorHeap의 변수 3총사
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap; // rtvHeap이라고 하자.
	uint32							_rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	// swapChain 전방선언
	shared_ptr<class SwapChain>		_swapChain;
};

