#include "pch.h"
#include "Input.h"


void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	// ���͸� ó���� KEY_TYPE_COUNT(255)�� ��ŭ�� �ʱ���·� ������
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

/// �� �����Ӹ��� Input�� Update�� ȣ���Ѵ�.
// ȣ���Ͽ� Ű���� ��Ȳ�� üũ�Ѵ�.
// �� ���� ��, ������ ������ �������� ��� Ű���� �����ϴٴ°� �����ؾ��Ѵ�.
// ������ �׻� �� ���������� �Ѵ�.
// �������� �����̶�� �� �������� �ſ� ���� �����̱� ������,
// �츮�� �����ϴ� ���� ª�� ������ Ű�� ����(�������� ��Ʈ�� ����)�� �Ƹ��� �������� ���̴�.
void Input::Update()
{
	// ���� Active�� ������ �޾ƿ�.
	HWND hwnd = ::GetActiveWindow();
	// Active �����찡 �ٸ��ٸ�, ��� ������ Ű���� NONE���� �о����.
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;

		return;
	}

	// ��ü Ű���� üũ��
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// Ű�� ������ ���� ������ true (bit flag�� �����)
		if (::GetAsyncKeyState(key) & 0x8000)
		{
			// ������ key���� ���¸� �޾ƿ�. �����ڷ�
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			// ���� �����ӿ� Ű�� �ѹ��� ������ ���� NONE�̶��
			else
				state = KEY_STATE::DOWN;
		}
		// �ش��ϴ� Ű�� ������ �ʾ����� �ǹ���.
		else
		{
			// ������ key���� ���¸� �޾ƿ�. �����ڷ�
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::UP;
			else
				state = KEY_STATE::NONE;
		}
	}
}