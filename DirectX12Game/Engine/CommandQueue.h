#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	// 필요없긴한데, 이벤트 소멸 때문에 추가함.
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain, shared_ptr<DescriptorHeap> descHeap);
	void WaitSync(); // CPU와 GPU의 동기화를 위한 함수. 책 내용 그대로 옮김.
	
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();
	
	// getter, 외부에서 CommandQueue를 필요할때 사용하기 위함. _cmdQueue만 있으면 된다.
	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }

private:
	// 크게 보면 CommandQueue와 Fence를 만드는것과 같다.

	// CommandQueue : DX12에 등장
	// 외주를 요청할 때, 하나씩 요청하면 비효율적
	// [외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것.
	// CommandQueue를 구성하는 변수 3총사.
	ComPtr<ID3D12CommandQueue>			_cmdQueue; // 일감을 넣는곳
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc; // 메모리 할당을 도와줌
	ComPtr<ID3D12GraphicsCommandList>	_cmdList; // 일감 리스트. gpu에게 한번에 넘겨줌

	// Fence : 울타리
	// CPU / GPU 동기화를 위한 간단한 도구
	// Fence를 구성하는 변수 3총사.
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	// SwapChain, DescriptorHeap
	shared_ptr<SwapChain>				_swapChain;
	shared_ptr<DescriptorHeap>			_descHeap;

};