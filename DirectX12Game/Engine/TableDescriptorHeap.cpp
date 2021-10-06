#include "pch.h"
#include "TableDescriptorHeap.h"
#include "Engine.h"

void TableDescriptorHeap::Init(uint32 count)
{
	// 인자(count)로 그룹을 몇개로 만들지 받고 있다.
	_groupCount = count; // 

	// DESCRIPTOR_HEAP 설명서 제작
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * (REGISTER_COUNT - 1); // b0는 전역이라서 -1해줌 // 그룹 개수 * 한 그룹에 존재하는 레지스터 개수
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // FLAG_SHADER_VISIBLE로 해줘야한다. NONE하면 안됨.
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // TYPE_CBV로 이용한다고 선언

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap)); // DescriptorHeap 생성.

	// 핸들 사이즈와 그룹 사이즈 값을 미리 계산해두자.
	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * (REGISTER_COUNT - 1); // b0는 전역 // 한 그룹의 전체 사이즈
}

// 한 프레임 이후 초기화 용도
void TableDescriptorHeap::Clear()
{
	_currentGroupIndex = 0;
}

// table 전용 DescriptorHeap에 이전에 만든 DescriptorHeap의 내용을 복사. CopyDescriptors
void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	// CPU HANDLE을 계산. 최종 목적지
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCPUHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	
	/// CopyDescriptors ★
	// src(source)에 있던것을 dest(destination, 목적지)에 복사하겠다는 의미. Range값을 넣고 TYPE도 설정해줌.
	// ex) v0 -> view0 로 복사, v1 -> view1 로 복사, v2 -> view2 로 복사 등등.
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

// GPU 레지스터와 만들어둔 Heap과 주소로 연결짓는것. SetCBV을 모두 다 했으면 위로 올려줌. 예약함.
void TableDescriptorHeap::CommitTable()
{
	// 레지스터에 해당하는 핸들(주소)의 값을 가져옴.
	// 이때 개별적인 view의 주소를 올리는게 아니라 전체 DescriptorHeap 하나를 넘김
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;

	// 0번 Param[0]의 내용과 핸들값으로 넘김.
	CMD_LIST->SetGraphicsRootDescriptorTable(1, handle); // 연결짓도록 예약.

	_currentGroupIndex++; // 그룹 인덱스 증가시킴. 일종의 커서용
}

// enum class를 이용해서 넣는 경우
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(CBV_REGISTER reg)
{
	// CBV_REGISTER는 b0,b1,b2,b3 이런식으로 사용됨.
	return GetCPUHandle(static_cast<uint32>(reg));
}

// for SRV
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(SRV_REGISTER reg)
{
	return GetCPUHandle(static_cast<uint8>(reg));
}

// 숫자로 들어오는 경우
D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCPUHandle(uint8 reg)
{
	assert(reg > 0); // 0보다 크면 통과. 아니면 에러

	// 시작 핸들 주소값을 가져오고 있음.
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize; // 원하는 그룹 인덱스로 이동함. (크게 점프, 몇동)
	handle.ptr += (reg - 1) * _handleSize; // 마지막으로 내부에서 원하는 레지스터 만큼 번호를 이동함. (내부에서 이동, 몇호)
	return handle; // 최종 주소값 결과를 넘김
}