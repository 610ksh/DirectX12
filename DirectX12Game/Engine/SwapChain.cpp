#include "pch.h"
#include "SwapChain.h"

void SwapChain::Init(const WindowInfo & info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// 이전에 만든 정보 날린다
	_swapChain.Reset();

	// 종이(버퍼) 자체도 화면 해상도와 같아야 하기 때문에 동일하게 셋팅해주자.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(info.width); // 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<uint32>(info.height); // 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60; // 화면 갱신 비율 // 분자 (뉴머레이럴)
	sd.BufferDesc.RefreshRate.Denominator = 1; // 화면 갱신 비율 // 분모 (디너미네이럴)
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 버퍼의 디스플레이 형식 (RGBA, 8비트씩 = 32bit)
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 후면 버퍼에 렌더링할 것 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // 전면+후면 버퍼의 개수 (더블버퍼링, 2개)
	sd.OutputWindow = info.hwnd; // 화면 핸들 정보
	sd.Windowed = info.windowed; // 윈도우 정보(전체화면 or 창모드)
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 전면 후면 버퍼 교체 시 이전 프레임 정보 버림(discard)
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // flag 설정

	// 실제 SwapChain 생성 함수. 위에서 만든 sd정보르르 토대로 _swapChain에 저장
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);

	// 버퍼 개수 만큼 돌면서 swapChain 변수와 실제 렌더 타깃을 연결짓는다. 버퍼 인덱스와 renderTarget연결
	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));
}

// 현재 프레임을 보냄
void SwapChain::Present()
{
	// Present the frame.
	_swapChain->Present(0, 0);
}

// 백 버퍼의 인덱스를 교체함
void SwapChain::SwapIndex()
{
	// 우리는 2개니까 0과 1이 반복되도록함. 0 -> 1, 1 -> 0 으로 바뀜
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}
