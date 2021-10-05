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

// discarded version, only study (초기 방식)
void Mesh::Init(vector<Vertex>& vec)
{
	// 정점 수 
	_vertexCount = static_cast<uint32>(vec.size());
	// 전체 버퍼 사이즈 = 정점수 * 한 Vertex 크기
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	// 요구사항 제작
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// CommittedResource 생성. GPU 공간을 할당받는 명령어
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));
	// vertexBuffer는 gpu에서 할당받은 공간을 가리키게됨.
	
	// Copy the triangle data to the vertex buffer.
	// gpu 공간에 데이터를 복사하는 과정(Map, Unmap)
	void* vertexDataBuffer = nullptr; // 임시 데이터 버퍼
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer); // gpu 공간의 _vertexBuffer를 vertexDataBuffer로 연결

	// Game에서 넘어온 정점들이 담긴 vec의 시작주소에 접근하여
	// bufferSize만큼씩 끊어서 vertexDataBuffer(GPU)가 가리키고 있는 곳으로 복사. 3개의 정점
	::memcpy(vertexDataBuffer, &vec[0], bufferSize); // gpu쪽으로 실제 복사되어 넘어가는 함수
	_vertexBuffer->Unmap(0, nullptr); // gpu 공간과 연결해제

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress(); // GPU쪽 주소도 가져옴
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

//////////////////////////////////////////

/// 이제 VertexBuffer와 IndexBuffer를 넘겨줘야함.
void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

/// Game의 Update 함수에서 각 오브젝트마다 실행함
void Mesh::Render()
{
	/// 1) IA 시리즈, Input Assembler stage
	// 삼각형리스트 형태로 만들거를 선언함. (Input Assembler 단계)
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 어떤 버퍼를 사용할지 설정. vertexBufferView에는 3개의 정점이 있음. Slot: (0~15)
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // VertexBuffer
	CMD_LIST->IASetIndexBuffer(&_indexBufferView); // IndexBuffer
	
	/// 2) 최종적으로 DescriptorHeap과 레지스터를 연결지음
	GEngine->GetTableDescHeap()->CommitTable();
	
	/// 3) 실제로 화면에 그림
	// VertexBuffer 방식으로 그리는 함수
	// CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
	// IndexBuffer를 이용해서 그리는 함수
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
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
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
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT; // uint16으로 할거면 DXGI_FORMAT_R16_UINT
	_indexBufferView.SizeInBytes = bufferSize;
}