#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent); // 이벤트를 모두 제거
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain, shared_ptr<DescriptorHeap> descHeap)
{
	// 받아온 변수들을 초기화 해준다.
	_swapChain = swapChain;
	_descHeap = descHeap;

	// CreateCommandQueue를 하기 위한 도구 생성. 일종의 설명서
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// _cmdQueue 변수 생성. device를 통해서 한다. 위에서 만든 도구를 넣어줌 (설명서)
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	// - D3D12_COMMAND_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록.
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));


	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// Allocator (위에서 먼저 Allocator를 생성해야한다. 선행필수)
	// 초기 상태 (그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));


	// CommandList는 Close / Open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념.
	_cmdList->Close();


	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다.
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

// CPU와 GPU의 동기화를 위한 코드. 책 내용 그대로 옮김.
void CommandQueue::WaitSync()
{
	// Advance the fence value to mark commands up to this fence point
	_fenceValue++;
	
	// Add an instruction to the command queue to set a new fence point.
	// Because we are on the GPU timeline.
	// the new fence point won't be set until the GPU finishes.
	// processing all the commands prior to this Signal()
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}

}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT * vp, const D3D12_RECT * rect)
{
	// 한번 초기화를 싹다해줌. 실제로 줄어드는건 아님 (vector의 capacity 개념)
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	// 배리어 생성
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET); // 외주 결과물

	// 위에서 만든 배리어를 List에 넣음
	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// 어떤 버퍼에 그림을 그려야할지 설정.
	// Specify the buffers we are going to render to.
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _descHeap->GetBackBufferView();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
	// 기본적으로 Begin의 반대개념임.

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력

	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close(); // 여기까지 작업한것을 넘겨주기 위해 리스트를 닫음. 일감 중단.

	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr); // 실행을 요청함

	_swapChain->Present(); // 버퍼를 교체함.

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync(); // 될때가지 기다림.

	_swapChain->SwapIndex(); // 인덱스 교체
}
