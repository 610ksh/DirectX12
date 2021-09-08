#include "pch.h" // precompiled header 설정에 의해서 자동으로 추가된 헤더
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo & info)
{
	GEngine->Init(info);

	vector<Vertex> vec(3);
	vec[0].pos = Vec3(0.f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	// 메시 초기화
	mesh->Init(vec);

	// shader 초기화
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");
	
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	// TODO!
	shader->Update(); // 쉐이더를 먼저 업데이트
	mesh->Render(); // 메시를 Render

	GEngine->RenderEnd();
}
