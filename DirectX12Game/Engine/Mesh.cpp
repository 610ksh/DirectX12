#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

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
	
	// Copy the triangle data to the vertex buffer. gpu 공간에 데이터를 복사하는 과정(Map, Unmap)
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer); // gpu 공간과 연결
	::memcpy(vertexDataBuffer, &vec[0], bufferSize); // gpu쪽으로 실제 복사되어 넘어가는 함수
	_vertexBuffer->Unmap(0, nullptr); // gpu 공간과 연결해제

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

void Mesh::Render()
{
	// 삼각형리스트 형태로 만들거
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 어떤 버퍼를 사용할지 설정
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	// 실제로 화면에 그림
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}