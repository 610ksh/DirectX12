#include "pch.h"
#include "Input.h"


void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	// 벡터를 처음에 KEY_TYPE_COUNT(255)개 만큼을 초기상태로 지정함
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

/// 매 프레임마다 Input의 Update를 호출한다.
// 호출하여 키값의 상황을 체크한다.
// 이 말은 즉, 동일한 프레임 내에서는 모든 키값은 일정하다는걸 가정해야한다.
// 기준은 항상 매 프레임으로 한다.
// 정상적인 게임이라면 한 프레임은 매우 빠른 찰나이기 떄문에,
// 우리가 걱정하는 아주 짧은 순간의 키값 조정(디테일한 컨트롤 영역)은 아마도 문제없을 것이다.
void Input::Update()
{
	// 현재 Active한 윈도우 받아옴.
	HWND hwnd = ::GetActiveWindow();
	// Active 윈도우가 다르다면, 모든 기존의 키값을 NONE으로 밀어버림.
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	// 전체 키값을 체크함
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 이전에 눌려 있으면 true (bit flag로 뱉어줌)
		if (::GetAsyncKeyState(key) & 0x8000)
		{
			// 이전의 key값의 상태를 받아옴. 참조자로
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			// 이전 프레임에 키를 한번도 누르지 않은 NONE이라면
			else
				state = KEY_STATE::DOWN;
		}
		// 해당하는 키를 누르지 않았음을 의미함.
		else
		{
			// 이전의 key값의 상태를 받아옴. 참조자로
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}
}