#include "pch.h"
#include "Timer.h"

void Timer::Init()
{
	// 얼마나 흘렀는지
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	// 기준점
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

void Timer::Update()
{
	uint64 currentCount;
	// 현재 카운터값을 기록 (CPU 클럭 관련값)
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// 현재 카운터와 이전 카운터를 빼서 frequency를 나눔
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount; // prev 업데이트

	_frameCount++; // 프레임 횟수 증가
	_frameTime += _deltaTime; // 누적 시간을 계속 기록중 (0 ~ 1초까지 증가중)

	// 프레임 누적시간이 1초가 넘었다면
	if (_frameTime > 1.f)
	{
		// 대략 1인 값으로 나눔. 그걸 fps로 지정 (평균 1초당, 이라고 읽을수 있음)
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		// 초기화
		_frameTime = 0.f;
		_frameCount = 0;
	}
}