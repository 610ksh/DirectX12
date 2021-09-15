#pragma once

/*
	DepthStencilBuffer
	이름에 Stencil이 들어갔지만, 일단은 해당 내용은 포함되어있지 않다.
	Depth에 초점을 두고 설계.
	버퍼는 1개만 생성
	Format은 깊이 용도로만 할거라고 4바이트 float으로 지정한거임.
	전부 depth용도로만 사용하겠다는것 = DXGI_FORMAT_D32_FLOAT
*/

class DepthStencilBuffer
{
public:
	void Init(const WindowInfo& window, DXGI_FORMAT dsvFormat = DXGI_FORMAT_D32_FLOAT);

	// getter
	D3D12_CPU_DESCRIPTOR_HANDLE	GetDSVCpuHandle() { return _dsvHandle; }
	DXGI_FORMAT GetDSVFormat() { return _dsvFormat; }

private:
	// Depth Stencil View
	ComPtr<ID3D12Resource>				_dsvBuffer; // 버퍼. 1개만 있어도 됨(배열x)
	ComPtr<ID3D12DescriptorHeap>		_dsvHeap; // 그 버퍼를 묘사하는 DescriptorHeap
	D3D12_CPU_DESCRIPTOR_HANDLE			_dsvHandle = {}; // 핸들(주소)
	DXGI_FORMAT							_dsvFormat = {};
};

