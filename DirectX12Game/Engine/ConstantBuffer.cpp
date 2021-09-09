#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	// ������� ���۰� �ִٸ�
	if (_cbvBuffer)
	{
		// ����Ű�� ���� �����Ѵٸ�
		if (_cbvBuffer != nullptr)
			// �Ѳ��� �ݴ´�.
			_cbvBuffer->Unmap(0, nullptr);
		// �׸��� nullptr �ٲ���. dangling pointer ����
		_cbvBuffer = nullptr;
	}
}


void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// ��� ���۴� 256 ����Ʈ(BYTE) ����� ������ �Ѵ�.
	// 0 256 512 768 (2^8, 2^16, 2^24...)
	
	// ~255�� 0000 0000�� �ǹ���.
	// �װ� & �ϴ°� ���� 8��Ʈ�� �� ���������ڴٴ� �ǹ�.
	_elementSize = (size + 255) & ~255; 
	_elementCount = count;

	CreateBuffer();
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	
	// UPLOAD ����� ����߸� �Ѵٰ� ��.
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// ���ҽ� ����
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	// �Ѱ��� ����. �ٵ� Unmap�� �������� �ʴ´� => ��� �� �����Ӹ��� ���ۿ� �����͸� ������ �����̱� ����.
	// �� ������ ���� ���۰� �� ������ �ð� �ӿ��� ���������� ���̰� �� ���̴�. Unmap�� �ϴ� ��������.
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

void ConstantBuffer::Clear()
{
	// �� �����Ӹ��� ���Ӱ� �ʱ�ȭ ���ش�.
	// �׷� �ٽ� 0������ ���۸� �ϳ��� �׾ƿø��°���.
	_currentIndex = 0;
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	// ������ ������ ���. �ƴϸ� ũ����.
	assert(_currentIndex < _elementSize);
	// ���� GPU ���ۿ� �����͸� �о�ִ� �Լ�. _mappedBuffer�� �̿���.
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	// GPU ���� �޸𸮸� �޾ƿ��� ���� ����. ���� �ε����� �ش��ϴ� GPU Buffer �ε����� ������.
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	
	// ���������� GPU �������Ϳ� ����� �����͸� �о�־��� GPU ���۸� ���� ������ ������ ��
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	_currentIndex++;
}

// GPU ������ ���� �ּҸ� �������� getter
D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	// gpu ���� ���� �ּ� ���� ����. ù ������ ���� �ּҸ� ������. (���۴� �׻� 256����Ʈ ���)
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize; // �� ������ �ε��� ��ŭ �ּҰ��� �ű�
	return objCBAddress;
}