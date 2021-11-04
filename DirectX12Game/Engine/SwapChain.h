#pragma once


// ��ȯ �罽
// �� ���� : GPU���� �ϰ��� �ѱ�� ����
//
// [���� ��ü �뷫���� ����]
// - ���� ���� ���� �ִ� ��Ȳ�� ����
// - � �������� ��� ������� ������
// - GPU�� ������ ��� (����)
// - ����� �޾Ƽ� ȭ�鿡 �׷��ش�

// �̶�, [���� �����]�� ��� ����?
// Idea !
// - � ����(Buffer)�� �׷��� �ǳ״޶�� ��Ź�غ���
// - Ư�� ����(Buffer)�� ���� -> ó���� �ǳ��ְ� -> ������� �ش� ���̿� �޴´� OK
// - �츮 ȭ�鿡 Ư�� ����(���� �����) ������ش�

// [?] ���̵���� ������!
// - �׷��� ȭ�鿡 ���� ����� ����ϴ� ���߿�, ���� ȭ�鵵 ���ָ� �ðܾ� ��
// - ���� ȭ�� ������� �̹� ȭ�� ��¿� �����. ���̸� �̹� ����ϰ� ����.
// - Ư�� ���̸� 2�� ����, �ϳ��� ���� ȭ���� �׷��ְ�, �ϳ��� ���� �ñ��...
// - Double Buffering!

// - [0] [1] (=Double Buffering)
// ���� ȭ�� [1]  <-> GPU �۾��� [1] BackBuffer

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
	// DescriptorHeap �κ��� Init �Լ��� ������

private:
	// SwapChain ���� 3�ѻ�
	ComPtr<IDXGISwapChain>	_swapChain; // swapChain
	uint32					_backBufferIndex = 0; // ����ۿ� �ش��ϴ� �迭�� �ε����� ����.
};