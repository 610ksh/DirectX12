#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	// �󸶳� �귶����
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	// ������
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU Ŭ��
}

void Timer::Update()
{
	uint64 currentCount;
	// ���� ī���Ͱ��� ��� (CPU Ŭ�� ���ð�)
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// ���� ī���Ϳ� ���� ī���͸� ���� frequency�� ����
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount; // prev ������Ʈ

	_frameCount++; // ������ Ƚ�� ����
	_frameTime += _deltaTime; // ���� �ð��� ��� ����� (0 ~ 1�ʱ��� ������)

	// ������ �����ð��� 1�ʰ� �Ѿ��ٸ�
	if (_frameTime > 1.f)
	{
		// �뷫 1�� ������ ����. �װ� fps�� ���� (��� 1�ʴ�, �̶�� ������ ����)
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		// �ʱ�ȭ
		_frameTime = 0.f;
		_frameCount = 0;
	}
}