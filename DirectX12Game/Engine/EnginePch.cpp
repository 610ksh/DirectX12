#include "pch.h"
#include "EnginePch.h"
#include "Engine.h" // Engine Ŭ���� ������ �������� �����ϱ� ���ؼ� �ʿ���

// ���� Ŭ������ ����Ʈ������ ���·� ����.
// EnginePch.h�� �����ϴ� ��� ������ Engine ��ü�� ���ٰ���.
// �� �ѹ��� �����ǰ� ��� �������.
unique_ptr<Engine> GEngine = make_unique<Engine>();