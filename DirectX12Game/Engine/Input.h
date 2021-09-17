#pragma once

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1), //256개 활용
	KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::END),
};


/*
	Unity 스타일로 만듦.
*/
class Input
{
public:
	void Init(HWND hwnd);
	void Update();

	// 누르고 있을 때 (이전에 누른 상태(state)와 비교)
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// 맨 처음 눌렀을 때 (이전에 누른 상태(state)와 비교)
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// 맨 처음 눌렀다 뗐을 때 (이전에 누른 상태(state)와 비교)
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8>(key)]; }

private:
	HWND _hwnd;

	// 각 키에 해당하는 벡터를 만들어서 상태를 지정할거임.
	// W = PRESS or DOWN or UP or NONE 등등. 디폴트는 NONE
	vector<KEY_STATE> _states;
};

