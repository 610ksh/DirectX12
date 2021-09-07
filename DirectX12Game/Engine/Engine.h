#pragma once



class Engine
{
public:
	// Client 프로젝트의 Game 클래스에서 호출할 예정인 함수들
	// public으로 선언해줘야한다.
	void Init(const WindowInfo& info);
	void Render();

public:
	// 요청사항을 CommandQueue에 넣어주는것
	void RenderBegin();
	// 쌓아놓은 일감을 최종적으로 GPU에게 외주로 넘겨줌
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	// 그려질 화면 크기 관련 3총사
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	// 장치 초기화에 필요한 클래스 변수 4총사, shared_ptr로 선언, 전방선언
	shared_ptr<class Device> _device;
	shared_ptr<class CommandQueue> _cmdQueue;
	shared_ptr<class SwapChain> _swapChain;
	shared_ptr<class DescriptorHeap> _descHeap;
	
};
