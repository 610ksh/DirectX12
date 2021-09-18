#include "pch.h" // precompiled header 설정에 의해서 자동으로 추가된 헤더
#include "Game.h"
#include "Engine.h"
#include "Material.h"

/// Component 헤더
#include "GameObject.h"
#include "MeshRenderer.h"

/// GameObject 선언
shared_ptr<GameObject> gameObject = make_shared<GameObject>();


void Game::Init(const WindowInfo & info)
{
	/// 1. 엔진 초기화
	/*
		1. 전체 엔진 사용을 위한 모든 클래스 변수 생성 및 초기화
		2. 2개의 Constant Buffer 생성
		- Transform을 위한 b0 레지스터 사용. 연결
		- Material을 위한 b1 레지스터 사용. 연결
	*/
	GEngine->Init(info);

	/// 2. Vertex 정보 생성
	// 메시 하나가 사용할 정점 정보 생성.
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

	/// 3. Index 정보 생성
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

	/// 4. GameObject 초기화
	gameObject->Init(); // Transform 생성

	/// 5. MeshRenderer 생성
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();

	/// 6. Mesh 초기화 및 meshRenderer 설정 (정점, 인덱스 정보 필요)
	{
		// 1개의 메시를 생성. 전역변수. (하나의 오브젝트 정보)
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVec);
		meshRenderer->SetMesh(mesh); // 메시 렌더러에서 메시를 설정
	}

	{
		/// 7. Shader, Texture 생성 및 초기화
		// 세이더, 텍스처 변수 선언
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		// shader, texture 초기화, HLSL 파일 인식. (VertexShader, PixelShader 생성)
		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\veigar.jpg");
		
		/// 6. Material 생성
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader); // 사용할 세이더 설정
		material->SetFloat(0, 0.3f); // b1 정보의 float_0
		material->SetFloat(1, 0.4f); // b1 정보의 float_1
		material->SetFloat(2, 0.3f); // b1 정보의 float_2
		material->SetTexture(0, texture); // tex_0(t0)에 베이가 텍스처를 넘김
		
		/// 7. 최종적으로 위에서 만든 Material을 meshRenderer에 지정함
		meshRenderer->SetMaterial(material);
	}

	/// 8. GameObject에 meshRenderer 컴포넌트 추가.
	gameObject->AddComponent(meshRenderer);

	/// 9. 위의 내용이 전부다 완료될때까지 잠시 대기. 
	// CPU와 GPU의 설정상 동기화 수행
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

	// GameObject의 Update함수 호출
	// 내부적으로 gameObject가 들고 있는 모든 컴포넌트의 Update 호출함
	gameObject->Update();

	/*
	
	{
		/// 변수 지정 및 설정값 추가
		static Transform t = {}; // 한번 선언하면 끝까지 유지됨.
		
		/// 키보드 입력값 처리 (x,y 값만)
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

		/// Transform 설정 (최종 결정)
		mesh->SetTransform(t);
		
		/// 최종적으로 Render
		// Transform 정보와 Material 정보가 모두 들어감.
		mesh->Render(); 
	}
	*/

	// 전체 렌더링 작업 완료.
	// 내부적으로 다음 프레임으로 가기전에 끝나기를 기다린다. WaitSync();
	// 현재는_cmdQueue->RenderEnd() 코드 뿐이다.
	GEngine->RenderEnd();
}
