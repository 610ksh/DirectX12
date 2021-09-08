#pragma once

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"

class Engine
{
public:
	// Client ������Ʈ�� Game Ŭ�������� ȣ���� ������ �Լ���
	// public���� ����������Ѵ�.
	void Init(const WindowInfo& info);
	void Render();

public:
	// �ٸ������� �� Ŭ�������� �ʿ��Ҷ����� ������ �� ����. Engine�� �����ϸ� �� ����.
	shared_ptr<Device> GetDevice() { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; };
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature; };

public:
	// ��û������ CommandQueue�� �־��ִ°�
	void RenderBegin();
	// �׾Ƴ��� �ϰ��� ���������� GPU���� ���ַ� �Ѱ���
	void RenderEnd();
	// ȭ�� ����
	void ResizeWindow(int32 width, int32 height);

private:
	// �׷��� ȭ�� ũ�� ���� 3�ѻ�
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	// ��ġ �ʱ�ȭ�� �ʿ��� Ŭ���� ���� 4�ѻ�, shared_ptr�� ����, ���漱��
	shared_ptr<Device> _device;
	shared_ptr<CommandQueue> _cmdQueue;
	shared_ptr<SwapChain> _swapChain;
	shared_ptr<RootSignature> _rootSignature;

};
