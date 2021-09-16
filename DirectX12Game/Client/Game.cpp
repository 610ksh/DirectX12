#include "pch.h" // precompiled header 설정에 의해서 자동으로 추가된 헤더
#include "Game.h"
#include "Engine.h"

// 하나의 메시와 쉐이더 객체를 하나 생성함
shared_ptr<Mesh> mesh = make_shared<Mesh>(); // 메시를 딱 1개만 생성함
shared_ptr<Shader> shader = make_shared<Shader>();

// Texture도 1개만 생성해봄.
shared_ptr<Texture> texture = make_shared<Texture>();


void Game::Init(const WindowInfo & info)
{
	// 엔진 초기화
	GEngine->Init(info);

	/// 정점 정보
	// 지금은 정점을 하나만 넘기는 형태. 메시에 넘기는 데이터
	vector<Vertex> vec(4);
	vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
	vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
	vec[0].uv = Vec2(0.f, 0.f); // (0,0)
	vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
	vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[1].uv = Vec2(1.f, 0.f); // (1,0)
	vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
	vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
	vec[2].uv = Vec2(1.f, 1.f); // (1,1)
	vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
	vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
	vec[3].uv = Vec2(0.f, 1.f); // (0,1)

	vector<uint32> indexVec;
	{
		indexVec.push_back(0);
		indexVec.push_back(1);
		indexVec.push_back(2);
	}
	{
		indexVec.push_back(0);
		indexVec.push_back(2);
		indexVec.push_back(3);
	}

	// 메시 초기화
	mesh->Init(vec, indexVec);

	// shader 초기화 -> HLSL 파일 인식. (VertexShader, PixelShader 생성)
	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	// texture 초기화
	texture->Init(L"..\\Resources\\Texture\\veigar.jpg");
	
	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	// 맨앞에 일단 넣음.
	GEngine->Update();

	// 전체 렌더링 초기화 및 사전 준비단계.
	// 렌더링 관련 부분에서 초기화가 필요한 작업을 RenderBegin에서 한다.
	// 현재는 _cmdQueue->RenderBegin(&_viewport, &_scissorRect) 코드 뿐이다.
	GEngine->RenderBegin();

	// 쉐이더를 먼저 업데이트
	// 한 프레임에 하나의 전체 렌더링 파이프라인을 만듦.
	// 매 프레임마다 새롭게 생성하는 구조임.
	shader->Update();
	
	/// TODO!
	
	/*
		<Notice>
		만약 위치값과 색상값이 다르게 적용되길 바란다면,
		Transform을 2개로 따로 만들어서 Mesh에서 다르게 설정하고
		그 상태로 PushData를 하여 Render하였으면 가능하다.
		아래의 예제는 하나의 Transform을 이용하여 위치와 색상값
		모두를 바꾸는거라는걸 주의하도록 하자.
	*/
	{
		/// 변수 지정 및 설정값 추가
		static Transform t = {}; // 한번 선언하면 끝까지 유지됨.
		
		// 키를 누르면 이동하게됨. 1씩.
		// 근데 만약에 우리가 W를 1초간 눌렀고, 1초동안 프레임이 100번 돌아가면
		// 갑자기 100씩 이동하게 된다.
		if (INPUT->GetButton(KEY_TYPE::W))
			t.offset.y += 1.f * DELTA_TIME;
		if (INPUT->GetButton(KEY_TYPE::S))
			t.offset.y -= 1.f * DELTA_TIME;
		if (INPUT->GetButton(KEY_TYPE::A))
			t.offset.x -= 1.f * DELTA_TIME;
		if (INPUT->GetButton(KEY_TYPE::D))
			t.offset.x += 1.f * DELTA_TIME;

		mesh->SetTransform(t);
		mesh->SetTexture(texture); // texture 지정

		/// 최종적으로 Render
		mesh->Render();
	}

	/*{
		/// 변수 지정 및 설정값 추가
		Transform t;
		t.offset = Vec4(0.25f, 0.25f, 0.f, 0.f);
		mesh->SetTransform(t);
		mesh->SetTexture(texture); // texture 지정

		/// 최종적으로 Render
		mesh->Render();
	}*/

	// 전체 렌더링 작업 완료.
	// 내부적으로 다음 프레임으로 가기전에 끝나기를 기다린다. WaitSync();
	// 현재는_cmdQueue->RenderEnd() 코드 뿐이다.
	GEngine->RenderEnd();
}
