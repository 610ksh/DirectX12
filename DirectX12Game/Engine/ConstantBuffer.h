#pragma once

/*
	ConstantBuffer의 용도
	1) Buffer 생성
	2) Buffer에 우리가 원하는 데이터 밀어 넣음
	3) Buffer의 주소를 register에다가 전송해서 알려줌
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
	// Handle값을 계산해주는 함수
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	// ComPtr은 gpu쪽 데이터
	// 실질적인 gpu쪽 버퍼를 의미
	ComPtr<ID3D12Resource>	_cbvBuffer; // ★ 매우 중요함
	// CPU쪽에서 데이터를 밀어넣을때 사용하는 버퍼. 뚜껑 열때 사용.
	BYTE*					_mappedBuffer = nullptr;
	// 각 버퍼의 크기와 개수
	uint32					_elementSize = 0;
	uint32					_elementCount = 0;

	/// for DescriptorHeap(= ConstantBufferViewHeap)
	ComPtr<ID3D12DescriptorHeap>		_cbvHeap; // 전체 cbvHeap
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {}; // 시작 핸들주소(v0의 시작주소임)
	uint32								_handleIncrementSize = 0; // 핸들의 인덱스

	// 지금 어디까지 버퍼를 생성하고 있는지에 대한 인덱스.
	// 렌더링이 한 프레임을 그리고나면 다시 0으로 초기화함.
	uint32					_currentIndex = 0;
};

