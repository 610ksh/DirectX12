#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	// 사용중인 버퍼가 있다면
	if (_cbvBuffer)
	{
		// 가리키는 값도 존재한다면
		if (_cbvBuffer != nullptr)
			// 뚜껑을 닫는다.
			_cbvBuffer->Unmap(0, nullptr);
		// 그리고 nullptr 바꿔줌. dangling pointer 방지
		_cbvBuffer = nullptr;
	}
}


void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// 상수 버퍼는 256 바이트(BYTE) 배수로 만들어야 한다.
	// 0 256 512 768 (2^8, 2^16, 2^24...)
	// 버퍼는 실제로 배열이 아닌데, 마치 배열같은 형태를 가지게 된다.
	
	// ~255는 0000 0000을 의미함.
	// 그걸 & 하는건 하위 8비트를 다 날려버리겠다는 의미.
	_elementSize = (size + 255) & ~255; // 256 배수임
	_elementCount = count;

	CreateBuffer();
}

// 버퍼를 만듦.
void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	
	// UPLOAD 방식을 따라야만 한다고 함.
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// 리소스 생성
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	// 뚜겅을 연다. 근데 Unmap이 존재하지 않는다 => 계속 매 프레임마다 버퍼에 데이터를 복사할 예정이기 때문.
	// 이 과정을 통해 버퍼가 한 프레임 시간 속에서 지속적으로 쌓이게 될 것이다. Unmap을 하는 순간까지.
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));

	/// 사실 위의 부분에서 동기화 문제가 발생할 수 있는데, 우리는 이미 CommandQueue에서 WaitSync로
	/// 무식하게 GPU에서 일감 처리가 모두 끝날때까지 기다리는 기능을 이용하고 있어서, 위에서 동기화 문제가 발생하지 않음.
	
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

// 매 프레임마다 새롭게 초기화 해준다.
void ConstantBuffer::Clear()
{
	// 그럼 다시 0번부터 버퍼를 하나씩 쌓아올리는거임.
	_currentIndex = 0;
}

// 실질적으로 버퍼에다가 데이터를 넣는 부분. (rootParamIndex : 루트 테이블의 슬롯 번호, buffer : 버퍼 주소값)
void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	// 조건이 맞으면 통과. 아니면 크래쉬. 혹시라도 처음 만들어준 버퍼의 크기를 넘어서는 순간 체크
	assert(_currentIndex < _elementSize);

	// 실제 GPU 버퍼에 데이터를 밀어넣는 함수. _mappedBuffer를 이용함.
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	// GPU 가상 메모리를 받아오는 변수 선언. 현재 인덱스에 해당하는 GPU Buffer 인덱스를 가져옴.
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex); // 버퍼 주소 계산
	
	/// 주의! 아래 CMD_LIST는 위의 내용과 다르게 예약하는 시스템이라, 바로 실행되지 않는다.
	// 최종적으로 GPU 레지스터와 방금전 데이터를 밀어넣었던 GPU 버퍼를 연결 짓도록 예약함 ★
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address); // 레지스터와 버퍼를 연결
	_currentIndex++;
}

// GPU 버퍼의 가상 주소를 가져오는 getter
D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	// gpu 버퍼 가상 주소 변수 선언. 첫 버퍼의 시작 주소를 가져옴. (버퍼는 항상 256바이트 배수)
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize; // 그 다음에 인덱스 만큼 주소값을 옮김
	return objCBAddress;
}