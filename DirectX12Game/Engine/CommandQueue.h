#pragma once

class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	// �ʿ�����ѵ�, �̺�Ʈ �Ҹ� ������ �߰���.
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain, shared_ptr<DescriptorHeap> descHeap);
	void WaitSync(); // CPU�� GPU�� ����ȭ�� ���� �Լ�. å ���� �״�� �ű�.
	
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();
	
	// getter, �ܺο��� CommandQueue�� �ʿ��Ҷ� ����ϱ� ����. _cmdQueue�� ������ �ȴ�.
	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }

private:
	// ũ�� ���� CommandQueue�� Fence�� ����°Ͱ� ����.

	// CommandQueue : DX12�� ����
	// ���ָ� ��û�� ��, �ϳ��� ��û�ϸ� ��ȿ����
	// [���� ���]�� �ϰ��� �������� ����ߴٰ� �� �濡 ��û�ϴ� ��.
	// CommandQueue�� �����ϴ� ���� 3�ѻ�.
	ComPtr<ID3D12CommandQueue>			_cmdQueue; // �ϰ��� �ִ°�
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc; // �޸� �Ҵ��� ������
	ComPtr<ID3D12GraphicsCommandList>	_cmdList; // �ϰ� ����Ʈ. gpu���� �ѹ��� �Ѱ���

	// Fence : ��Ÿ��
	// CPU / GPU ����ȭ�� ���� ������ ����
	// Fence�� �����ϴ� ���� 3�ѻ�.
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	// SwapChain, DescriptorHeap
	shared_ptr<SwapChain>				_swapChain;
	shared_ptr<DescriptorHeap>			_descHeap;

};