#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Engine.h"

void DepthStencilBuffer::Init(const WindowInfo& window, DXGI_FORMAT dsvFormat)
{
	_dsvFormat = dsvFormat;

	/// 1. 버퍼 생성하기 위한 사전 작업(RESOURCE_DESC)
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// 실제 화면 픽셸 단위로 이뤄지기 때문에 window 데이터가 필요함. 해상도 정보. 800x600이면 800x600으로.
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(_dsvFormat, window.width, window.height);
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // Flag만 DEPTH_STENCIL로 수정하여 지정함.

	// ClearValue : 매 프레임마다 초기화를 하게 되는데, 1.0f으로 depth를 초기화해주는 것을 지정함.
	// 이러한 ClearValue를 이용하면, 좀더 빠르게 매 프레임마다 초기화할 수 있다.
	D3D12_CLEAR_VALUE optimizedClearValue = CD3DX12_CLEAR_VALUE(_dsvFormat, 1.0f, 0);

	/// 2. 버퍼 생성(CommittedResource)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&optimizedClearValue,
		IID_PPV_ARGS(&_dsvBuffer));

	/// 3. DescriptorHeap 만들기 위한 사전작업 (heapDesc)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1; // heap을 1개짜리로 만듦.
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // type은 DSV

	/// 4. DescriptorHeap 생성
	DEVICE->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_dsvHeap));

	/// 5. 최종 DepthStencilView 완성
	_dsvHandle = _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	DEVICE->CreateDepthStencilView(_dsvBuffer.Get(), nullptr, _dsvHandle);
}
