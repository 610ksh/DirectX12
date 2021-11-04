#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"

CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent); // �̺�Ʈ�� ��� ����
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	// �޾ƿ� �������� �ʱ�ȭ ���ش�.
	_swapChain = swapChain;

	// CreateCommandQueue�� �ϱ� ���� ���� ����. ������ ����
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	// _cmdQueue ���� ����. device�� ���ؼ� �Ѵ�. ������ ���� ������ �־��� (����)
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));

	// - D3D12_COMMAND_TYPE_DIRECT : GPU�� ���� �����ϴ� ��� ���.
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU�� �ϳ��� �ý��ۿ����� 0����
	// DIRECT or BUNDLE
	// Allocator (������ ���� Allocator�� �����ؾ��Ѵ�. �����ʼ�)
	// �ʱ� ���� (�׸��� ����� nullptr ����)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));

	// CommandList�� Close / Open ���°� �ִµ�
	// Open ���¿��� Command�� �ִٰ� Close�� ���� �����ϴ� ����.
	_cmdList->Close();

	/// ���ҽ� ���� Ŀ�ǵ� ����Ʈ �ʱ�ȭ. ���� ���ڴ� ����.
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));

	// CreateFence
	// - CPU�� GPU�� ����ȭ �������� ���δ�.
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

// CPU�� GPU�� ����ȭ�� ���� �ڵ�. å ���� �״�� �ű�.
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

// ��� �׸��� �׸��� ��������(����� ����)
void CommandQueue::RenderBegin(const D3D12_VIEWPORT * vp, const D3D12_RECT * rect)
{
	// �ѹ� �ʱ�ȭ�� �ϴ�����. ������ �پ��°� �ƴ� (vector�� capacity ����)
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr); // close���� cmdlist�� open��

	int8 backIndex = _swapChain->GetBackBufferIndex();

	// �踮�� ����
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_PRESENT, // ȭ�� ���
		D3D12_RESOURCE_STATE_RENDER_TARGET); // ���� �����


	/// ��Ʈ �ñ״�ó �߰�.
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	/// ������ ConstantBuffer ���� �о�����. ����ϰ� ���ŷ� �ʱ�ȭ.
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();
	
	/// DescriptorHeap ���� �߰�
	GEngine->GetTableDescHeap()->Clear(); // Table�� �ʱ�ȭ
	// ������ ������ DescriptorHeap�� ��°�� ������. _descHeap�� �޾ƿ��°���.
	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	// �� SetDescriptorHeaps �� �ſ� ���� �Լ��� �����Ҷ� �� 1���� ����ǵ��� �Ѵ�.
	// (��ü) DescriptorHeaps ������ ���� ���ڷ� ������ descHeap�� ������. ��, � ���� ������� �����ϴ� ������ ���.
	// ����� cmdList�� descHeap�� ���� �������� ����������Ѵ�. (���� ����)
	// �� ������ ��μ� �������� ������ ���� �÷������� SetGraphicsRootDescriptorTable �Լ��� ����Ǿ�� �Ѵ�.
	_cmdList->SetDescriptorHeaps(1, &descHeap); // 1���� ū DescriptorHeaps�� ������.

	// ������ ���� �踮� List�� ����
	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

}

// ����ۿ� �׷��� ������ ȭ�鿡 ����ϵ��� ��ü����.
void CommandQueue::RenderEnd()
{
	int8 backIndex = _swapChain->GetBackBufferIndex();

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(backIndex)->GetTex2D().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // ���� �����
		D3D12_RESOURCE_STATE_PRESENT); // ȭ�� ���


	// ������ ���� �踮� ����Ʈ�� ����.
	_cmdList->ResourceBarrier(1, &barrier);
	// ������� �۾��� ���� �Ѱ��ֱ� ���� ����Ʈ�� ����. �ϰ� �ߴ�.
	_cmdList->Close(); // Ŀ�ǵ� ����Ʈ�� ���� ��

	// �ش��ϴ� ����Ʈ�� ID3D12CommandList�� �ٽ� �޾ƿ�.
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };

	// CommandList�� ������. Execute. ���� ȭ�鿡 ������ ��û��.
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr); 

	// ���۸� ��ü��. swapChain���� �˷������. ���� ���۴� �ű⿡ �����ϱ�.
	_swapChain->Present(); 
	WaitSync(); // �� �����ӿ� ��� ��� �ɶ����� ��ٸ�.

	_swapChain->SwapIndex(); // �ε����� ���� ��ü������.
}

// �ϰ��� �о���� ��, ���ҽ��� �ε��ϴ� ��ɾ �������ִ� �Լ�
void CommandQueue::FlushResourceCommandQueue()
{
	// ���ҽ� CMD_LIST�� ����
	_resCmdList->Close();

	// Execute (������)
	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	// Wait
	WaitSync();

	// Reset�ؼ� �ٽ� �����ϵ��� ��.
	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}