#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h" // 클라이언트쪽에서 필요하기 떄문에 넣어줌. (Game.cpp)
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"
#include "DepthStencilBuffer.h"

/*
	웬만하면 엔진쪽은 렌더링 관련된 부분만 놔두자.
*/

class Engine
{
public:
	// Client 프로젝트의 Game 클래스에서 호출할 예정인 함수들
	// public으로 선언해줘야한다.
	void Init(const WindowInfo& info);
	void Update();

public:
	// 다른곳에서 각 클래스들이 필요할때마다 가져다 쓸 예정. Engine을 접근하면 다 있음.
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; };
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; };
	shared_ptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return _depthStencilBuffer; }

	// 벡터를 꺼내 쓰도록 getter 추가
	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }

public:
	void Render();
	// 요청사항을 CommandQueue에 넣어주는것
	void RenderBegin();
	// 쌓아놓은 일감을 최종적으로 GPU에게 외주로 넘겨줌
	void RenderEnd();
	// 화면 조정
	void ResizeWindow(int32 width, int32 height);

private:
	void ShowFps();
	// CB 만듦.
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);

private:
	// 그려질 화면 크기 관련 3총사
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	// 장치 초기화에 필요한 클래스 변수 4총사, shared_ptr로 선언.
	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<CommandQueue> _cmdQueue = make_shared<CommandQueue>();
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();
	shared_ptr<TableDescriptorHeap> _tableDescHeap = make_shared<TableDescriptorHeap>();
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer = make_shared<DepthStencilBuffer>();

	// 다양한 Constant Buffer를 저장해둘 배열 선언
	vector<shared_ptr<ConstantBuffer>> _constantBuffers;
};
