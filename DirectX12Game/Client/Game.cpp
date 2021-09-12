#include "pch.h" // precompiled header 설정에 의해서 자동으로 추가된 헤더
#include "Game.h"
#include "Engine.h"

// 하나의 메시와 쉐이더 객체를 하나 생성함
shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo & info)
{
	// 엔진 초기화
	GEngine->Init(info);

	/// 3개의 정점 정보
	// 지금은 정점을 하나만 넘기는 형태.
	vector<Vertex> vec(3);
	vec[0].pos = Vec3(0.f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[1].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.0f, 0.f, 1.f);
	vec[2].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);

	// 메시 초기화
	mesh->Init(vec);

	// shader 초기화 -> HLSL 파일 인식. (VertexShader, PixelShader 생성)
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");
	
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	GEngine->RenderBegin();

	/// TODO!
	
	// 쉐이더를 먼저 업데이트
	// 한 프레임에 하나의 렌더링파이프라인을 만드는듯.
	// 한 프레임이 끝나면 다시 하나의 렌더링 파이프라인을 만들고.
	shader->Update();
	
	/*
		<Notice>
		만약 위치값과 색상값이 다르게 적용되길 바란다면,
		Transform을 2개로 따로 만들어서 Mesh에서 다르게 설정하고
		그 상태로 PushData를 하여 Render하였으면 가능하다.
		아래의 예제는 하나의 Transform을 이용하여 위치와 색상값
		모두를 바꾸는거라는걸 주의하도록 하자.
	*/
	{
		// Vec4 짜리 offset 변수 1개있는 구조체
		Transform t;
		// 삼각형의 x값 위치에 0.75 칼라 red값에 0.75가 더해질거임
		t.offset = Vec4(0.75f, 0.f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->Render();
	}


	{
		// Vec4 짜리 offset 변수 1개있는 구조체
		Transform t;
		// 삼각형의 y값 위치에 0.75 칼라 green값에 0.75가 더해질거임
		t.offset = Vec4(0.0f, 0.75f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->Render();
	}

	GEngine->RenderEnd();
}
