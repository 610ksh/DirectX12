#include "pch.h"
#include "EnginePch.h"
#include "Engine.h"

// ���� Ŭ������ ����Ʈ������ ���·� ����.
// EnginePch.h�� �����ϴ� ��� ������ Engine ��ü�� ���ٰ���.
// �� �ѹ��� �����ǰ� ��� �������.
unique_ptr<Engine> GEngine = make_unique<Engine>();