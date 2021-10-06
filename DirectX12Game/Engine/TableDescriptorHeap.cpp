#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	// ����(count)�� �׷��� ��� ������ �ް� �ִ�.
	_groupCount = count; // 

	// DESCRIPTOR_HEAP ���� ����
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * (REGISTER_COUNT - 1); // b0�� �����̶� -1���� // �׷� ���� * �� �׷쿡 �����ϴ� �������� ����
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // FLAG_SHADER_VISIBLE�� ������Ѵ�. NONE�ϸ� �ȵ�.
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // TYPE_CBV�� �̿��Ѵٰ� ����

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap)); // DescriptorHeap ����.

	// �ڵ� ������� �׷� ������ ���� �̸� ����ص���.
	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * (REGISTER_COUNT - 1); // b0�� ���� // �� �׷��� ��ü ������
}

// �� ������ ���� �ʱ�ȭ �뵵
void TableDescriptorHeap::Clear()
{
	_currentGroupIndex = 0;
}

// table ���� DescriptorHeap�� ������ ���� DescriptorHeap�� ������ ����. CopyDescriptors
void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	// CPU HANDLE�� ���. ���� ������
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	
	/// CopyDescriptors ��
	// src(source)�� �ִ����� dest(destination, ������)�� �����ϰڴٴ� �ǹ�. Range���� �ְ� TYPE�� ��������.
	// ex) v0 -> view0 �� ����, v1 -> view1 �� ����, v2 -> view2 �� ���� ���.
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

// for texture
void TableDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

// GPU �������Ϳ� ������ Heap�� �ּҷ� �������°�. SetCBV�� ��� �� ������ ���� �÷���. ������.
void TableDescriptorHeap::CommitTable()
{
	// �������Ϳ� �ش��ϴ� �ڵ�(�ּ�)�� ���� ������.
	// �̶� �������� view�� �ּҸ� �ø��°� �ƴ϶� ��ü DescriptorHeap �ϳ��� �ѱ�
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;

	// 0�� Param[0]�� ����� �ڵ鰪���� �ѱ�.
	CMD_LIST->SetGraphicsRootDescriptorTable(1, handle); // ���������� ����.

	_currentGroupIndex++; // �׷� �ε��� ������Ŵ. ������ Ŀ����
}

// enum class�� �̿��ؼ� �ִ� ���
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	// CBV_REGISTER�� b0,b1,b2,b3 �̷������� ����.
	return GetCPUHandle(static_cast<uint32>(reg));
}

// for SRV
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint8>(reg));
}

// ���ڷ� ������ ���
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint8 reg)
{
	assert(reg > 0); // 0���� ũ�� ���. �ƴϸ� ����

	// ���� �ڵ� �ּҰ��� �������� ����.
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize; // ���ϴ� �׷� �ε����� �̵���. (ũ�� ����, �)
	handle.ptr += (reg - 1) * _handleSize; // ���������� ���ο��� ���ϴ� �������� ��ŭ ��ȣ�� �̵���. (���ο��� �̵�, ��ȣ)
	return handle; // ���� �ּҰ� ����� �ѱ�
}