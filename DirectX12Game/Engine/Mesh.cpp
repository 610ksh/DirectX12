#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Material.h"

Mesh::Mesh() : Object(OBJECT_TYPE::MESH)
{

}

Mesh::~Mesh()
{

}

//////////////////////////////////////////

// discarded version, only study (�ʱ� ���)
void Mesh::Init(vector<Vertex>& vec)
{
	// ���� �� 
	_vertexCount = static_cast<uint32>(vec.size());
	// ��ü ���� ������ = ������ * �� Vertex ũ��
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	// �䱸���� ����
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// CommittedResource ����. GPU ������ �Ҵ�޴� ��ɾ�
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));
	// vertexBuffer�� gpu���� �Ҵ���� ������ ����Ű�Ե�.
	
	// Copy the triangle data to the vertex buffer.
	// gpu ������ �����͸� �����ϴ� ����(Map, Unmap)
	void* vertexDataBuffer = nullptr; // �ӽ� ������ ����
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer); // gpu ������ _vertexBuffer�� vertexDataBuffer�� ����

	// Game���� �Ѿ�� �������� ��� vec�� �����ּҿ� �����Ͽ�
	// bufferSize��ŭ�� ��� vertexDataBuffer(GPU)�� ����Ű�� �ִ� ������ ����. 3���� ����
	::memcpy(vertexDataBuffer, &vec[0], bufferSize); // gpu������ ���� ����Ǿ� �Ѿ�� �Լ�
	_vertexBuffer->Unmap(0, nullptr); // gpu ������ ��������

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress(); // GPU�� �ּҵ� ������
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // ���� 1�� ũ��
	_vertexBufferView.SizeInBytes = bufferSize; // ������ ũ��	
}

//////////////////////////////////////////

/// ���� VertexBuffer�� IndexBuffer�� �Ѱ������.
void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

/// Game�� Update �Լ����� �� ������Ʈ���� ������
void Mesh::Render()
{
	/// 1) IA �ø���, Input Assembler stage
	// �ﰢ������Ʈ ���·� ����Ÿ� ������. (Input Assembler �ܰ�)
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// � ���۸� ������� ����. vertexBufferView���� 3���� ������ ����. Slot: (0~15)
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // VertexBuffer
	CMD_LIST->IASetIndexBuffer(&_indexBufferView); // IndexBuffer
	
	/// 2) ���������� DescriptorHeap�� �������͸� ��������
	GEngine->GetTableDescHeap()->CommitTable();
	
	/// 3) ������ ȭ�鿡 �׸�
	// VertexBuffer ������� �׸��� �Լ�
	// CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
	// IndexBuffer�� �̿��ؼ� �׸��� �Լ�
	CMD_LIST->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}


void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	_vertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // ���� 1�� ũ��
	_vertexBufferView.SizeInBytes = bufferSize; // ������ ũ��	
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	_indexBuffer->Unmap(0, nullptr);

	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT; // uint16���� �ҰŸ� DXGI_FORMAT_R16_UINT
	_indexBufferView.SizeInBytes = bufferSize;
}