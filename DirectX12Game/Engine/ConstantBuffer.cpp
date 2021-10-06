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


void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{
	// �뵵�� ����ϴ� �������� ���
	_reg = reg;

	// ��� ���۴� 256 ����Ʈ(BYTE) ����� ������ �Ѵ�.
	// 0 256 512 768 (2^8, 2^16, 2^24...)
	// ���۴� ������ �迭�� �ƴѵ�, ��ġ �迭���� ���¸� ������ �ȴ�.
	
	// ~255�� 0000 0000�� �ǹ���.
	// �װ� & �ϴ°� ���� 8��Ʈ�� �� ���������ڴٴ� �ǹ�.
	_elementSize = (size + 255) & ~255; // 256 �����
	_elementCount = count;

	CreateBuffer();
	CreateView();
}

// ���۸� ����.
void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	
	// UPLOAD ����� ����߸� �Ѵٰ� ��.
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// ���ҽ� ���� = ������ GPU RAM�� ���۸� �����ϴ� �ڵ�.
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

	/// ��� ���� �κп��� ����ȭ ������ �߻��� �� �ִµ�, �츮�� �̹� CommandQueue���� WaitSync��
	/// �����ϰ� GPU���� �ϰ� ó���� ��� ���������� ��ٸ��� ����� �̿��ϰ� �־, ������ ����ȭ ������ �߻����� ����.
	
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
}

void ConstantBuffer::CreateView()
{
	// DESCRIPTOR_HEAP ����
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = _elementCount; // ������ ConstantBuffer�� �Ȱ��� _elementCount ������ŭ ���� ����.
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // NONE ���·� ����.
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // CBV�� Ȱ���ҰŶ� �̰� ������
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap)); // DescriptorHeap �� ������ ����.

	// ���߿� ����Ҷ� �ʿ��� �ڵ� ������ ó����. �̸� �����ͼ� ĳ���س�.
	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart(); // ���� �ڵ� �ּҰ�
	// ����� �Ʒ� size�� �Ʒ� ���ó�� ���� �����;� �Ѵٰ� ��. GPU���� ����� �޶� �ٸ� ���� ���� �� �ִٰ� ��.
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); // �� �ڵ��� ũ��

	/// CreateConstantBufferView. View�� ����°���. View�� ������
	for (uint32 i = 0; i < _elementCount; ++i)
	{
		// �� �ڵ鿡 ����
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);
		// CBV ���� ����
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		// CBV �ּ��� �ּ�
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;
		// CBV ������ ũ��
		cbvDesc.SizeInBytes = _elementSize;   // CB size is required to be 256-byte aligned.
		
		// CBV ����. v0~v4���� �̹� ������� ConstantBuffer�� ����Ű���� ��.
		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle); 
	}
}

// �� �����Ӹ��� ���Ӱ� �ʱ�ȭ ���ش�.
void ConstantBuffer::Clear()
{
	// �׷� �ٽ� 0������ ���۸� �ϳ��� �׾ƿø��°���.
	_currentIndex = 0;
}

// ���������� ���ۿ��ٰ� �����͸� �ִ� �κ�. (buffer : ���� �ּҰ�)
void ConstantBuffer::PushData(void* buffer, uint32 size)
{
	/// �Ʒ��� ���� ���
	// 1) Buffer���ٰ� ������ ����
	// 2) Buffer�� �ּҸ� register���ٰ� ����

	// assert : ������ ������ ���. �ƴϸ� ũ����.
	// Ȥ�ö� ó�� ������� ������ ũ�⸦ �Ѿ�� ���� üũ
	assert(_currentIndex < _elementCount);
	// Ȥ�ö� ������ �����͸� ���� �뵵�� �߰���.
	assert(_elementSize == ((size + 255) & ~255));

	// ���� GPU ���ۿ� �����͸� �о�ִ� �Լ�. _mappedBuffer�� �̿���.
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);
	
	// ���� �ش��ϴ� �ڵ鰪�� ����
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(_currentIndex);

	/// Set CBV
	GEngine->GetTableDescHeap()->SetCBV(cpuHandle, _reg);

	/*
	/// ������ ��� Constant Buffer ���� �ǽ� ���.
	// GPU ���� �޸𸮸� �޾ƿ��� ���� ����. ���� �ε����� �ش��ϴ� GPU Buffer �ε����� ������.
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex); // ���� �ּ� ���
	
	/// ����! �Ʒ� CMD_LIST�� ���� ����� �ٸ��� �����ϴ� �ý����̶�, �ٷ� ������� �ʴ´�.
	// ���������� GPU �������Ϳ� ����� �����͸� �о�־��� GPU ���۸� ���� ������ ������ ��
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address); // �������Ϳ� ���۸� ����
	*/
	_currentIndex++;
}

void ConstantBuffer::SetGlobalData(void* buffer, uint32 size)
{
	assert(_elementSize == ((size + 255) & ~255));
	::memcpy(&_mappedBuffer[0], buffer, size);
	CMD_LIST->SetGraphicsRootConstantBufferView(0, GetGpuVirtualAddress(0));
}

// GPU ������ ���� �ּҸ� �������� getter
D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	// gpu ���� ���� �ּ� ���� ����. ù ������ ���� �ּҸ� ������. (���۴� �׻� 256����Ʈ ���)
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize; // �� ������ �ε��� ��ŭ �ּҰ��� �ű�
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(uint32 index)
{
	/* 
	// �Ʒ� ����� ������ ���� �ڵ���
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _cpuHandleBegin;
	handle.ptr += index * _handleIncrementSize;
	return handle;
	*/

	// ���� �ڵ� �ּҿ� �� �ڵ� ũ���� �ε�����ŭ ���ؼ� ������.
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}
