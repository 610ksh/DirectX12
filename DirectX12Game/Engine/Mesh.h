#pragma once

// [����Ƽ¯]�� ���� �������� �̷���� ��ü
class Mesh
{
public:
	/// discarded version, only study
	void Init(vector<Vertex>& vec); 

	// ������ ���� ���ۿ� �ε��� ���۸� ������ �ȴ�.
	void Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexbuffer);
	void Render();

	// setter
	void SetTransform(const Transform& t) { _transform = t; }

private:
	void CreateVertexBuffer(const vector<Vertex>& buffer);
	void CreateIndexBuffer(const vector<uint32>& buffer); // tip : �޽ð� ũ�� ������ uint16���� �ص� ����ϴ�!

private:
	/// Vertex Buffer
	ComPtr<ID3D12Resource>		_vertexBuffer; // GPU�� ����
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	/// Index Buffer
	ComPtr<ID3D12Resource>		_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW		_indexBufferView;
	uint32 _indexCount = 0;

	Transform _transform = {};
};

