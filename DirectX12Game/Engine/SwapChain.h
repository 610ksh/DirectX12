#pragma once


// 교환 사슬
// ※ 외주 : GPU에게 일감을 넘기는 행위
//
// [외주 전체 대략적인 과정]
// - 현재 게임 세상에 있는 상황을 묘사
// - 어떤 공식으로 어떻게 계산할지 던져줌
// - GPU가 열심히 계산 (외주)
// - 결과물 받아서 화면에 그려준다

// 이때, [외주 결과물]을 어디에 받지?
// Idea !
// - 어떤 종이(Buffer)에 그려서 건네달라고 부탁해보자
// - 특수 종이(Buffer)를 만들어서 -> 처음에 건네주고 -> 결과물을 해당 종이에 받는다 OK
// - 우리 화면에 특수 종이(외주 결과물) 출력해준다

// [?] 아이디어의 문제점!
// - 그런데 화면에 현재 결과물 출력하는 와중에, 다음 화면도 외주를 맡겨야 함
// - 현재 화면 결과물은 이미 화면 출력에 사용중. 종이를 이미 사용하고 있음.
// - 특수 종이를 2개 만들어서, 하나는 현재 화면을 그려주고, 하나는 외주 맡기고...
// - Double Buffering!

// - [0] [1] (=Double Buffering)
// 현재 화면 [1]  <-> GPU 작업중 [1] BackBuffer

class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present();
	void SwapIndex();

	// getter
	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }
	uint8 GetBackBufferIndex() { return _backBufferIndex; }
	
private:
	void CreateSwapChain(const WindowInfo & info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	// DescriptorHeap 부분의 Init 함수를 가져옴

private:
	// SwapChain 변수 3총사
	ComPtr<IDXGISwapChain>	_swapChain; // swapChain
	uint32					_backBufferIndex = 0; // 백버퍼에 해당하는 배열의 인덱스를 지정.
};