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

	/// 리소스 전용 커맨드 리스트 초기화. 위와 인자는 같다.
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));

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
	_cmdList->Reset(_cmdAlloc.Get(), nullptr); // close였던 cmdlist를 open함

	int8 backIndex = _swapChain->GetBackBufferIndex();

	// 배리어 생성
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET); // 외주 결과물


	/// 루트 시그니처 추가.
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	/// 시작전 ConstantBuffer 먼저 밀어주자. 깔끔하게 새거로 초기화.
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();
	
	/// DescriptorHeap 관련 추가
	GEngine->GetTableDescHeap()->Clear(); // Table도 초기화
	// 기존에 만들어둔 DescriptorHeap을 통째로 가져옴. _descHeap을 받아오는거임.
	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	// ★ SetDescriptorHeaps 은 매우 느린 함수라서 렌더할때 딱 1번만 실행되도록 한다.
	// (전체) DescriptorHeaps 개수와 다음 인자로 만들어둔 descHeap을 지정함. 즉, 어떤 힙을 사용할지 지정하는 역할을 담당.
	// 참고로 cmdList로 descHeap을 먼저 선행으로 지정해줘야한다. (순서 주의)
	// 그 다음에 비로소 레지스터 쪽으로 위로 올려보내는 SetGraphicsRootDescriptorTable 함수가 실행되어야 한다.
	_cmdList->SetDescriptorHeaps(1, &descHeap); // 1개의 큰 DescriptorHeaps를 지정함.

	// 위에서 만든 배리어를 List에 넣음
	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

}

// 백버퍼에 그려진 내용을 화면에 출력하도록 교체해줌.
void CommandQueue::RenderEnd()
{
	int8 backIndex = _swapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력


	// 위에서 만든 배리어를 리스트에 넣음.
	_cmdList->ResourceBarrier(1, &barrier);
	// 여기까지 작업한 것을 넘겨주기 위해 리스트를 닫음. 일감 중단.
	_cmdList->Close(); // 커맨드 리스트를 닫음 ★

	// 해당하는 리스트를 ID3D12CommandList로 다시 받아옴.
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };

	// CommandList를 수행함. Execute. 현재 화면에 실행을 요청함.
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr); 

	// 버퍼를 교체함. swapChain에게 알려줘야함. 실제 버퍼는 거기에 있으니까.
	_swapChain->Present(); 
	WaitSync(); // 한 프레임에 모두 출력 될때까지 기다림.

	_swapChain->SwapIndex(); // 인덱스도 마저 교체해주자.
}

// 일감을 밀어넣은 뒤, 리소스를 로드하는 명령어를 실행해주는 함수
void CommandQueue::FlushResourceCommandQueue()
{
	// 리소스 CMD_LIST를 닫음
	_resCmdList->Close();

	// Execute (실행함)
	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	// Wait
	WaitSync();

	// Reset해서 다시 재사용하도록 함.
	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}