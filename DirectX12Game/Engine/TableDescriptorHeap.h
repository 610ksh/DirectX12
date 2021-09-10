#pragma once

/*
	Descriptor Table�� �����ϰԵ� ���� Descriptor Heap�� �ǹ���
*/

class TableDescriptorHeap
{
public:
	// DescriptorHeap�� ������.
	void Init(uint32 count);

	void Clear();
	// Init���� ���� DescriptorHeap�� ������ ���� DescriptorHeap�� Copy�ؿ�
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	
	// GPU �������Ϳ� ������ DescriptorHeap�� �ּҷ� ����. ���� �÷������� ����.
	void CommitTable(); 

	// getter
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }

	// CPUHandle �� ���. �ڵ� �ּ� ����ϴ� �Լ�
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
private:
	// ���� GetCPUHandle �Լ� �����ε�
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32 reg);

private:
	/// ������ DescriptorHeap ������ ���� �뵵
	ComPtr<ID3D12DescriptorHeap> _descHeap; // ��ü DescriptorHeap
	uint64					_handleSize = 0; // �ڵ� ������
	
	/// group ����
	// �� �׷��� ��ü ũ��
	uint64					_groupSize = 0;
	// �׷���� ��ü ����
	uint64					_groupCount = 0;
	// ���� �׷��� �ε���
	uint32					_currentGroupIndex = 0;
};