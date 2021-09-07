#include "pch.h"
#include "EnginePch.h"
#include "Engine.h" // Engine 클래스 변수를 전역으로 생성하기 위해서 필요함

// 전역 클래스를 스마트포인터 형태로 만듦.
// EnginePch.h를 포함하는 모든 곳에서 Engine 객체에 접근가능.
// 단 한번만 생성되고 계속 살아있음.
unique_ptr<Engine> GEngine = make_unique<Engine>();