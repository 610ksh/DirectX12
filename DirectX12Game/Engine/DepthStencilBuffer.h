#pragma once

/*
	DepthStencilBuffer
	�̸��� Stencil�� ������, �ϴ��� �ش� ������ ���ԵǾ����� �ʴ�.
	Depth�� ������ �ΰ� ����.
	���۴� 1���� ����
	Format�� ���� �뵵�θ� �ҰŶ�� 4����Ʈ float���� �����Ѱ���.
	���� depth�뵵�θ� ����ϰڴٴ°� = DXGI_FORMAT_D32_FLOAT
*/

class DepthStencilBuffer
{
public:
	void Init(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

	// getter
	D3D12_CPU_DESCRIPTOR_HANDLE	GetDSVCpuHandle() { return _dsvHandle; }
	DXGI_FORMAT GetDSVFormat() { return _dsvFormat; }

private:
	// Depth Stencil View
	ComPtr<ID3D12Resource>				_dsvBuffer; // ����. 1���� �־ ��(�迭x)
	ComPtr<ID3D12DescriptorHeap>		_dsvHeap; // �� ���۸� �����ϴ� DescriptorHeap
	D3D12_CPU_DESCRIPTOR_HANDLE			_dsvHandle = {}; // �ڵ�(�ּ�)
	DXGI_FORMAT							_dsvFormat = {};
};

