#pragma once

/*
	Descriptor Table이 참조하게될 전용 Descriptor Heap을 의미함
*/

class TableDescriptorHeap
{
public:
	// DescriptorHeap를 생성함.
	void Init(uint32 count);

	void Clear();
	// Init에서 만든 DescriptorHeap에 이전에 만든 DescriptorHeap을 Copy해옴
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	
	// GPU 레지스터와 만들어둔 DescriptorHeap의 주소로 연결. 위로 올려보내는 예약.
	void CommitTable(); 

	// getter
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return _descHeap; }

	// CPUHandle 값 계산. 핸들 주소 계산하는 함수
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(CBV_REGISTER reg);
private:
	// 위의 GetCPUHandle 함수 오버로딩
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32 reg);

private:
	/// 기존의 DescriptorHeap 변수와 같은 용도
	ComPtr<ID3D12DescriptorHeap> _descHeap; // 전체 DescriptorHeap
	uint64					_handleSize = 0; // 핸들 사이즈
	
	/// group 변수
	// 한 그룹의 전체 크기
	uint64					_groupSize = 0;
	// 그룹들의 전체 개수
	uint64					_groupCount = 0;
	// 현재 그룹의 인덱스
	uint32					_currentGroupIndex = 0;
};