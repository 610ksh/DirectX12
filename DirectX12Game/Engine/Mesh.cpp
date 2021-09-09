#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

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
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer); // gpu ������ ����.
	::memcpy(vertexDataBuffer, &vec[0], bufferSize); // gpu������ ���� ����Ǿ� �Ѿ�� �Լ�
	_vertexBuffer->Unmap(0, nullptr); // gpu ������ ��������

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // ���� 1�� ũ��
	_vertexBufferView.SizeInBytes = bufferSize; // ������ ũ��	
}

void Mesh::Render()
{
	// �ﰢ������Ʈ ���·� �����
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// � ���۸� ������� ����
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	/// TODO
	// 1) GPU ���� �ִ� Buffer�� �����͸� �о� �־��� (����)
	// 2) �� Buffer�� �ּҸ� GPU�� �ִ� register(b0,b1���)���ٰ� ���۽��Ѽ� ��������.
	// ex) CMD_LIST->SetGraphicsRootConstantBufferView(0, ??) ������.
	// => ConstantBuffer Ŭ�������� ���� ������ ó������.

	/// TODO ���� ���� ���
	// 1) Buffer���ٰ� ������ ����
	// 2) Buffer�� �ּҸ� register���ٰ� ����

	// (�������� �������� ��ȣ, ������ �����Ͱ�, �� ũ��)
	// �� �κп��� ���̴� �ڵ��� 0�� 1�� ������. ù��° 4����Ʈ, 2��° 4����Ʈ
	// �� �ѱ�� transform�� ���� �����̱� ������ �ᱹ 1���� transform�� ������,
	// 2���� ����, ��ġ���� ������ �����ϰ� ����. �׷��� ���������� ������ ��������.
	GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform)); // ��ġ�� ����
	GEngine->GetCB()->PushData(1, &_transform, sizeof(_transform)); // ���� ����
	
	// ������ ȭ�鿡 �׸�
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}
