#include "pch.h" // precompiled header 설정에 의해서 자동으로 추가된 헤더
#include "Game.h"
#include "Engine.h"

void Game::Init(const WindowInfo & info)
{
	GEngine->Init(info);
}

void Game::Update()
{
	GEngine->Render();
}
