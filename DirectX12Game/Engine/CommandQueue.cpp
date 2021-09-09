#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent); // 이벤트를 모두 제거
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	// 받아온 변수들을 초기화 해준다.
	_swapChain = swapChain;

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

// 어디에 그림을 그릴지 결정해줌(백버퍼 지정)
void CommandQueue::RenderBegin(const D3D12_VIEWPORT * vp, const D3D12_RECT * rect)
{
	// 한번 초기화를 싹다해줌. 실제로 줄어드는건 아님 (vector의 capacity 개념)
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	// 배리어 생성
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET); // 외주 결과물

	/// 루트 시그니처 추가.
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	GEngine->GetCB()->Clear(); // 시작전 ConstantBuffer 초기화 먼저 해주자.

	// 위에서 만든 배리어를 List에 넣음
	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// 어떤 버퍼에 그림을 그려야할지 설정.
	// Specify the buffers we are going to render to.  // 실제 RTV가 필요함
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	// 현재 backbuffer의 rtv를 초기화해준다. 종이를 리셋시켜줌.
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

// 백버퍼에 그려진 내용을 화면에 출력하도록 교체해줌.
void CommandQueue::RenderEnd()
{
	// 기본적으로 Begin의 반대개념임.
	// 백버퍼에서 그렸던것을 이제 화면으로 가져오고,
	// 화면에 출력하고 있던 것을 백버퍼로 위치를 바꿔줘야함.

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력

	// 위에서 만든 배리어를 리스트에 넣음.
	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close(); // 여기까지 작업한 것을 넘겨주기 위해 리스트를 닫음. 일감 중단.

	// 해당하는 리스트를 ID3D12CommandList로 다시 받아옴.
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	// CommandList를 수행함. Execute. 현재 화면에 실행을 요청함.
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr); 

	// 버퍼를 교체함. swapChain에게 알려줘야함. 실제 버퍼는 거기에 있으니까.
	_swapChain->Present(); 

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	WaitSync(); // 한 프레임에 모두 출력 될때까지 기다림.

	_swapChain->SwapIndex(); // 인덱스도 마저 교체해주자.
}
