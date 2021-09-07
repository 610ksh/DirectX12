#include "pch.h"
#include "SwapChain.h"

void SwapChain::Init(const WindowInfo & info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// ������ ���� ���� ������
	_swapChain.Reset();

	// ����(����) ��ü�� ȭ�� �ػ󵵿� ���ƾ� �ϱ� ������ �����ϰ� ����������.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(info.width); // ������ �ػ� �ʺ�
	sd.BufferDesc.Height = static_cast<uint32>(info.height); // ������ �ػ� ����
	sd.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� ���� ���� // ���� (���ӷ��̷�)
	sd.BufferDesc.RefreshRate.Denominator = 1; // ȭ�� ���� ���� // �и� (��ʹ̳��̷�)
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ ���÷��� ���� (RGBA, 8��Ʈ�� = 32bit)
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1; // ��Ƽ ���ø� OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // �ĸ� ���ۿ� �������� �� 
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT; // ����+�ĸ� ������ ���� (������۸�, 2��)
	sd.OutputWindow = info.hwnd; // ȭ�� �ڵ� ����
	sd.Windowed = info.windowed; // ������ ����(��üȭ�� or â���)
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����(discard)
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // flag ����

	// ���� SwapChain ���� �Լ�. ������ ���� sd�������� ���� _swapChain�� ����
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);

	// ���� ���� ��ŭ ���鼭 swapChain ������ ���� ���� Ÿ���� �������´�. ���� �ε����� renderTarget����
	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));
}

// ���� �������� ����
void SwapChain::Present()
{
	// Present the frame.
	_swapChain->Present(0, 0);
}

// �� ������ �ε����� ��ü��
void SwapChain::SwapIndex()
{
	// �츮�� 2���ϱ� 0�� 1�� �ݺ��ǵ�����. 0 -> 1, 1 -> 0 ���� �ٲ�
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}
