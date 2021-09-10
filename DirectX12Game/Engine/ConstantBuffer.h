#pragma once

/*
	ConstantBuffer�� �뵵
	1) Buffer ����
	2) Buffer�� �츮�� ���ϴ� ������ �о� ����
	3) Buffer�� �ּҸ� register���ٰ� �����ؼ� �˷���
*/

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	D3D12_CPU_DESCRIPTOR_HANDLE PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	// Handle���� ������ִ� �Լ�
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	// ComPtr�� gpu�� ������
	// �������� gpu�� ���۸� �ǹ�
	ComPtr<ID3D12Resource>	_cbvBuffer; // �� �ſ� �߿���
	// CPU�ʿ��� �����͸� �о������ ����ϴ� ����. �Ѳ� ���� ���.
	BYTE*					_mappedBuffer = nullptr;
	// �� ������ ũ��� ����
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	/// for DescriptorHeap(= ConstantBufferViewHeap)
	ComPtr<ID3D12DescriptorHeap>		_cbvHeap; // ��ü cbvHeap
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {}; // ���� �ڵ��ּ�(v0�� �����ּ���)
	uint32								_handleIncrementSize = 0; // �ڵ��� �ε���

	// ���� ������ ���۸� �����ϰ� �ִ����� ���� �ε���.
	// �������� �� �������� �׸����� �ٽ� 0���� �ʱ�ȭ��.
	uint32					_currentIndex = 0;
};

