#pragma once
#include "Object.h"

class Texture : public Object
{
public:
	Texture();
	virtual ~Texture();

	void Init(const wstring& path);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }

public:
	void CreateTexture(const wstring& path);
	void CreateView();

private:
	ScratchImage			 		_image;  // �ؽ�ó�� �ε��ϴ� ����
	ComPtr<ID3D12Resource>			_tex2D;

	ComPtr<ID3D12DescriptorHeap>	_srvHeap; // view�� �޴� ����
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle = {}; // �ʱ�ȭ ����.
};

