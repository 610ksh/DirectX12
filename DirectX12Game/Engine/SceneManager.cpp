#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Engine.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Light.h"

#include "TestCameraScript.h"
#include "Resources.h"

void SceneManager::Update()
{
	if (_activeScene == nullptr)
		return;

	// ���� ���� Update���� ����
	_activeScene->Update();
	_activeScene->LateUpdate();
	_activeScene->FinalUpdate();
}

// TEMP �ӽÿ���. 
void SceneManager::Render()
{
	if (_activeScene)
		_activeScene->Render();
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


void SceneManager::SetLayerName(uint8 index, const wstring& name)
{
	// ���� ������ ����
	const wstring& prevName = _layerNames[index];
	_layerIndex.erase(prevName);

	_layerNames[index] = name;
	_layerIndex[name] = index;
}

uint8 SceneManager::LayerNameToIndex(const wstring& name)
{
	auto findIt = _layerIndex.find(name);
	if (findIt == _layerIndex.end())
		return 0;

	return findIt->second;
}



// �׽�Ʈ�� �ϴ� ��
shared_ptr<Scene> SceneManager::LoadTestScene()
{
	// ������ �׽�Ʈ �ڵ带 ��������. ������ �ʱ�ȭ �κ�

	/// 1. ���̾� ����
#pragma region LayerMask
	SetLayerName(0, L"Default");
	SetLayerName(1, L"UI");
#pragma endregion

	/// 2. ���� ���ο� ���� �ϳ� ����
	shared_ptr<Scene> scene = make_shared<Scene>();

#pragma region TestObject
/*
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

	/// 4. Transform Component �߰�
	gameObject->AddComponent(make_shared<Transform>());

	shared_ptr<Transform> transform = gameObject->GetTransform();
	transform->SetLocalPosition(Vec3(0.f, 100.f, 200.f));
	transform->SetLocalScale(Vec3(100.f, 100.f, 1.f));

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
*/

#pragma endregion

#pragma region Camera
	{
		// ī�޶� ������Ʈ ����
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Main_Camera");
		// ī�޶� ������Ʈ �߰� (Transform, Camera)
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, FOV=45��
		// ī�޶� �̵� ��ũ��Ʈ �߰�. ��������Ʈ ������ �������� ��ġ�
		camera->AddComponent(make_shared<TestCameraScript>());
		// ī�޶� ��ġ ����
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		// UI Layer�� �ø� ����
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, true); // UI�� �� ����
		// ���� ī�޶� �־��ֱ�
		scene->AddGameObject(camera);
	}
#pragma endregion

#pragma region UI_Camera
	{
		shared_ptr<GameObject> camera = make_shared<GameObject>();
		camera->SetName(L"Orthographic_Camera");
		camera->AddComponent(make_shared<Transform>());
		camera->AddComponent(make_shared<Camera>()); // Near=1, Far=1000, 800*600
		camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		camera->GetCamera()->SetProjectionType(PROJECTION_TYPE::ORTHOGRAPHIC);
		uint8 layerIndex = GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI");
		camera->GetCamera()->SetCullingMaskAll(); // �� ����
		camera->GetCamera()->SetCullingMaskLayerOnOff(layerIndex, false); // UI�� ����
		scene->AddGameObject(camera);
	}
#pragma endregion


#pragma region Sphere
	/*
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(150.f, -100.f, 200.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = make_shared<Shader>();
			shared_ptr<Texture> texture = make_shared<Texture>();
			shared_ptr<Texture> texture2 = make_shared<Texture>(); // normal
			shader->Init(L"..\\Resources\\Shader\\default.fx");
			texture->Init(L"..\\Resources\\Texture\\Wallpaper.jpg");
			texture2->Init(L"..\\Resources\\Texture\\Wallpaper_Normal.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			material->SetTexture(1, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		scene->AddGameObject(sphere);
	}
	*/
#pragma endregion

#pragma region SkyBox
	{
		shared_ptr<GameObject> skybox = make_shared<GameObject>();
		skybox->AddComponent(make_shared<Transform>()); // transform�� ���� ��������. ����Ʈ��.
		skybox->SetCheckFrustum(false);
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh(); // ���� ���
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			// high resolution space hdri
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Skybox");
			
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Sky01", L"..\\Resources\\Texture\\Sky01.jpg");

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		skybox->AddComponent(meshRenderer);
		scene->AddGameObject(skybox);
	}
#pragma endregion
#pragma region Object
	{
		shared_ptr<GameObject> obj = make_shared<GameObject>();
		obj->AddComponent(make_shared<Transform>());
		obj->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 150.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadSphereMesh();
			meshRenderer->SetMesh(sphereMesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Deferred");
			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"Stone", L"..\\Resources\\Texture\\Stone.jpg");
			shared_ptr<Texture> texture2 = GET_SINGLE(Resources)->Load<Texture>(L"Stone_Normal", L"..\\Resources\\Texture\\Stone_Normal.jpg");
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			material->SetTexture(1, texture2);
			meshRenderer->SetMaterial(material);
		}
		obj->AddComponent(meshRenderer);
		scene->AddGameObject(obj);
	}
#pragma endregion

#pragma region UI_Test
	for (int32 i = 0; i < 5; i++)
	{
		shared_ptr<GameObject> sphere = make_shared<GameObject>();
		sphere->SetLayerIndex(GET_SINGLE(SceneManager)->LayerNameToIndex(L"UI")); // UI
		sphere->AddComponent(make_shared<Transform>());
		sphere->GetTransform()->SetLocalScale(Vec3(100.f, 100.f, 100.f));
		sphere->GetTransform()->SetLocalPosition(Vec3(-350.f + (i * 160), 250.f, 500.f));
		shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
		{
			shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadRectangleMesh();
			meshRenderer->SetMesh(mesh);
		}
		{
			shared_ptr<Shader> shader = GET_SINGLE(Resources)->Get<Shader>(L"Texture");

			shared_ptr<Texture> texture;
			if (i < 3)
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::G_BUFFER)->GetRTTexture(i);
			else
				texture = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::LIGHTING)->GetRTTexture(i - 3);

			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(shader);
			material->SetTexture(0, texture);
			meshRenderer->SetMaterial(material);
		}
		sphere->AddComponent(meshRenderer);
		scene->AddGameObject(sphere);
	}
#pragma endregion


#pragma region Directional Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		//light->GetTransform()->SetLocalPosition(Vec3(0.f, 150.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(0, 0, 1.f));
		light->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(1.f, 0.f, 0.f));
		light->GetLight()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.2f, 0.2f, 0.2f));

		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Point Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 150.f));
		light->AddComponent(make_shared<Light>());
		//light->GetLight()->SetLightDirection(Vec3(-1.f, -1.f, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::POINT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.5f, 0.0f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.3f, 0.0f));
		light->GetLight()->SetLightRange(200.f);

		scene->AddGameObject(light);
	}
#pragma endregion

#pragma region Spot Light
	{
		shared_ptr<GameObject> light = make_shared<GameObject>();
		light->AddComponent(make_shared<Transform>());
		light->GetTransform()->SetLocalPosition(Vec3(75.f, 0.f, 150.f));
		light->AddComponent(make_shared<Light>());
		light->GetLight()->SetLightDirection(Vec3(-1.f, 0, 0));
		light->GetLight()->SetLightType(LIGHT_TYPE::SPOT_LIGHT);
		light->GetLight()->SetDiffuse(Vec3(0.0f, 0.f, 0.5f));
		light->GetLight()->SetAmbient(Vec3(0.0f, 0.0f, 0.1f));
		light->GetLight()->SetSpecular(Vec3(0.0f, 0.0f, 0.1f));
		light->GetLight()->SetLightRange(200.f);
		light->GetLight()->SetLightAngle(3.14f / 2);

		scene->AddGameObject(light);
	}
#pragma endregion

	return scene;
}