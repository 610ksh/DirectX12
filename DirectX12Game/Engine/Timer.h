#pragma once


class Timer
{
	/// �̱��� �߰�
	DECLARE_SINGLE(Timer);
	
public:
	void Init();
	void Update();

	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	uint64	_frequency = 0;
	uint64	_prevCount = 0;
	/// ������ �����ӿ��� ���� �����ӱ��� ����� �ð�
	float	_deltaTime = 0.f;

private:
	/// frame�� ����ϱ� ���� ����
	uint32	_frameCount = 0;
	float	_frameTime = 0.f;
	// fps : frame per seconds
	// ���ʸ��� ������ ������Ʈ Ƚ��
	// Update���� ���ʸ��� ���ο� ������ �ڵ� �ʱ�ȭ��.
	uint32	_fps = 0;
};

