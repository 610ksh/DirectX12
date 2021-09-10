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
	
	// Copy the triangle data to the vertex buffer.
	// gpu 공간에 데이터를 복사하는 과정(Map, Unmap)
	void* vertexDataBuffer = nullptr; // 임시 데이터 버퍼
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer); // gpu 공간과 연결.
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

	/// 기존의 방식 ConstantBuffer 방식
	// 1) GPU 램에 있는 Buffer에 데이터를 밀어 넣어줌 (세팅)
	// 2) 그 Buffer의 주소를 GPU에 있는 register(b0,b1등등)에다가 전송시켜서 연결지음.
	// ex) CMD_LIST->SetGraphicsRootConstantBufferView(0, ??) 형태임.
	// => ConstantBuffer 클래스에서 위의 내용을 처리했음.

	/// TODO, 우리가 여기서 처리해야하는 내용
	// 1) Buffer에다가 데이터 세팅 (PushData로 함)
	// 2) TableDescriptorHeap에다가 CBV(Heap) 내용을 그대로 copy함 (CopyDescriptors)
	// 3) 모든 세팅이 끝났으면 TableDescriptorHeap을 Commit한다. (레지스터로 올려보냄)

	// (연결지을 레지스터 번호, 복사할 데이터값, 그 크기)
	// 이 부분에서 쉐이더 코드의 0과 1이 결정됨. 첫번째 4바이트, 2번째 4바이트
	// ※ 넘기는 transform이 같은 변수이기 때문에 결국 1개의 transform을 가지고,
	// 2개의 변수, 위치값과 색상값을 변경하고 있음. 그래서 독립적으로 보이지 않은거임.
	{
		// 위치값 조절
		// 1)
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
		// 2)
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0);
	}
	{
		//GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform)); // 색상값 조절
		// 색상값 조절
		// 1)
		D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform));
		// 2)
		GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b1);
	}

	// 3) 최종적으로 DescriptorHeap과 레지스터를 연결지음
	GEngine->GetTableDescHeap()->CommitTable();
	
	// 실제로 화면에 그림
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}
