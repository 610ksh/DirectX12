#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	// 현재 씬의 Update들을 돌림
	_activeScene->Update();
	_activeScene->LateUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : 기존 Scene 정리
	// TODO : 파일에서 Scene 정보 로드

	// 새로운 로드 씬
	_activeScene = LoadTestScene();

	// 최초의 씬 Awake 호출, Start도.
	_activeScene->Awake();
	_activeScene->Start();
}

// 테스트로 하는 씬
shared_ptr<Scene> SceneManager::LoadTestScene()
{
	// 기존의 테스트 코드를 복붙하자. 최초의 초기화 부분

	/// 1. 먼저 새로운 씬을 하나 만듦
	shared_ptr<Scene> scene = make_shared<Scene>();

	/// 2. TestObject, 1개만 만듦
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();

	/// 3. 정점 정보 만듦
	vector<Vertex> vec(4);
	{
		vec[0].pos = Vec3(-0.5f, 0.5f, 0.5f);
		vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);
		vec[0].uv = Vec2(0.f, 0.f);
		vec[1].pos = Vec3(0.5f, 0.5f, 0.5f);
		vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);
		vec[1].uv = Vec2(1.f, 0.f);
		vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
		vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
		vec[2].uv = Vec2(1.f, 1.f);
		vec[3].pos = Vec3(-0.5f, -0.5f, 0.5f);
		vec[3].color = Vec4(0.f, 1.f, 0.f, 1.f);
		vec[3].uv = Vec2(0.f, 1.f);
	}

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

	/// 4. gameObject 초기화
	gameObject->Init(); // Transform

	/// 5. 메시 렌더러 생성
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();

	/// 6. 메시 생성 및 설정
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVec);
		meshRenderer->SetMesh(mesh);
	}

	/// 7. 세이더, 텍스처, 머티리얼 생성 및 설정
	{
		shared_ptr<Shader> shader = make_shared<Shader>();
		shared_ptr<Texture> texture = make_shared<Texture>();
		shader->Init(L"..\\Resources\\Shader\\default.hlsli");
		texture->Init(L"..\\Resources\\Texture\\veigar.jpg");
		shared_ptr<Material> material = make_shared<Material>();
		material->SetShader(shader);
		material->SetFloat(0, 0.3f);
		material->SetFloat(1, 0.4f);
		material->SetFloat(2, 0.3f);
		material->SetTexture(0, texture);
		meshRenderer->SetMaterial(material);
	}

	/// 8. 메시 렌더러를 오브젝트에 적용
	gameObject->AddComponent(meshRenderer);

	/// 9. 현재 씬에 이 gameObject를 벡터에 추가함
	scene->AddGameObject(gameObject);

	return scene;
}