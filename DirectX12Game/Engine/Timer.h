#pragma once


class Timer
{
	/// 싱글톤 추가
	DECLARE_SINGLE(Timer);
	
public:
	void Init();
	void Update();

	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	uint64	_frequency = 0;
	uint64	_prevCount = 0;
	/// ☆이전 프레임에서 현재 프레임까지 경과된 시간
	float	_deltaTime = 0.f;

private:
	/// frame을 계산하기 위한 변수
	uint32	_frameCount = 0;
	float	_frameTime = 0.f;
	// fps : frame per seconds
	// 매초마다 프레임 업데이트 횟수
	// Update에서 매초마다 새로운 값으로 자동 초기화됨.
	uint32	_fps = 0;
};

