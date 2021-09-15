#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Engine.h"

void DepthStencilBuffer::Init(const WindowInfo& window, DXGI_FORMAT dsvFormat)
{
	_dsvFormat = dsvFormat;

	/// 1. ���� �����ϱ� ���� ���� �۾�(RESOURCE_DESC)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// ���� ȭ�� �ȼ� ������ �̷����� ������ window �����Ͱ� �ʿ���. �ػ� ����. 800x600�̸� 800x600����.
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(_dsvFormat, window.width, window.height);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // Flag�� DEPTH_STENCIL�� �����Ͽ� ������.

	// ClearValue : �� �����Ӹ��� �ʱ�ȭ�� �ϰ� �Ǵµ�, 1.0f���� depth�� �ʱ�ȭ���ִ� ���� ������.
	// �̷��� ClearValue�� �̿��ϸ�, ���� ������ �� �����Ӹ��� �ʱ�ȭ�� �� �ִ�.
	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(_dsvFormat, 1.0f, 0);

	/// 2. ���� ����(CommittedResource)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&_dsvBuffer));

	/// 3. DescriptorHeap ����� ���� �����۾� (heapDesc)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1; // heap�� 1��¥���� ����.
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // type�� DSV

	/// 4. DescriptorHeap ����
	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_dsvHeap));

	/// 5. ���� DepthStencilView �ϼ�
	_dsvHandle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	DEVICE->CreateDepthStencilView(_dsvBuffer.Get(), nullptr, _dsvHandle);
}
