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

	// ���� ���� Update���� ����
	_activeScene->Update();
	_activeScene->LateUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// TODO : ���� Scene ����
	// TODO : ���Ͽ��� Scene ���� �ε�

	// ���ο� �ε� ��
	_activeScene = LoadTestScene();

	// ������ �� Awake ȣ��, Start��.
	_activeScene->Awake();
	_activeScene->Start();
}

// �׽�Ʈ�� �ϴ� ��
shared_ptr<Scene> SceneManager::LoadTestScene()
{
	// ������ �׽�Ʈ �ڵ带 ��������. ������ �ʱ�ȭ �κ�

	/// 1. ���� ���ο� ���� �ϳ� ����
	shared_ptr<Scene> scene = make_shared<Scene>();

	/// 2. TestObject, 1���� ����
	shared_ptr<GameObject> gameObject = make_shared<GameObject>();

	/// 3. ���� ���� ����
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

	/// 4. gameObject �ʱ�ȭ
	gameObject->Init(); // Transform

	/// 5. �޽� ������ ����
	shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();

	/// 6. �޽� ���� �� ����
	{
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->Init(vec, indexVec);
		meshRenderer->SetMesh(mesh);
	}

	/// 7. ���̴�, �ؽ�ó, ��Ƽ���� ���� �� ����
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

	/// 8. �޽� �������� ������Ʈ�� ����
	gameObject->AddComponent(meshRenderer);

	/// 9. ���� ���� �� gameObject�� ���Ϳ� �߰���
	scene->AddGameObject(gameObject);

	return scene;
}