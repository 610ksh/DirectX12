#pragma once


// [��ȼ�]
// ����(�ϰ�)�� GPU���� �ñ� �� �̷� ���� �������� ���� �Ѱ���� �ϴµ�,
// �ƹ� ���·γ� ��û�ϸ� GPU�� �� �˾ƸԴ´�.
// - ���� ���ҽ��� � �뵵�� ����ϴ��� �Ĳ��ϰ� ��� �Ѱ��ִ� �뵵
// �׳� �Ѱ��� ���� �ִµ�, �ѹ� �� �����ؼ� �Ѱ��شٰ� �����ϸ� ��.
// DX12�� ���Ӱ� �߰��� ������. DX11 ���Ͽ��� ����.

class DescriptorHeap // = View
{
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<class SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE		GetRTV(int32 idx) { return _rtvHandle[idx]; }

	D3D12_CPU_DESCRIPTOR_HANDLE		GetBackBufferView();

private:
	// rtv : Render Target View�� ���Ӹ�.
	// DescriptorHeap�� ���� 3�ѻ�
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap; // rtvHeap�̶�� ����.
	uint32							_rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	// swapChain ���漱��
	shared_ptr<class SwapChain>		_swapChain;
};

