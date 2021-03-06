#pragma once
#include "Object.h"

// 전방선언
class Material;

/*
	[유니티짱]과 같이 정점으로 이루어진 물체
	각 메시는 아래의 정보를 가지게 된다.
	1. Transform 정보
	2. Material 정보
*/
class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();

	/// discarded version, only study
	void Init(vector<Vertex>& vec); 

	// 이제는 정점 버퍼와 인덱스 버퍼를 받으면 된다.
	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexbuffer);
	void Render();

private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	// tip : 메시가 크지 않으면 uint16으로 해도 충분하다!
	void CreateIndexBuffer(const vector<uint32>& buffer); 

private:
	/// Vertex Buffer
	ComPtr<ID3D12Resource>		_vertexBuffer; // GPU쪽 변수
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	/// Index Buffer
	ComPtr<ID3D12Resource>		_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW		_indexBufferView;
	uint32 _indexCount = 0;

};

