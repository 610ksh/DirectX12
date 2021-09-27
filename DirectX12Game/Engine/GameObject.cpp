#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

#include "MonoBehaviour.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

// 이 GameObject가 들고 있는 컴포넌트와 스크립트의 Awake 함수를 호출
void GameObject::Awake()
{
	// 내가 들고 있는 모든 component의 Awake 함수를 호출한다.
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	// 내가 들고 있는 모든 MonoBehaviour script의 Awake 함수를 호출한다.
	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->Update();
	}
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehaviour>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FinalUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FinalUpdate();
	}

	// 스크립트에서는 FinalUpdate를 사용하지 못하도록 약속하자.
}


shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

// 이 GameObject가 들고있는 Transform 정보르 넘김
shared_ptr<Transform> GameObject::GetTransform()
{
	//uint8 index = static_cast<uint8>(COMPONENT_TYPE::TRANSFORM);
	// 스마트 포인터끼리의 casting이라서 static_pointer_cast
	//return static_pointer_cast<Transform>(_components[index]);

	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::TRANSFORM);
	return static_pointer_cast<Transform>(component);
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::MESH_RENDERER);
	return static_pointer_cast<MeshRenderer>(component);
}

shared_ptr<Camera> GameObject::GetCamera()
{
	shared_ptr<Component> component = GetFixedComponent(COMPONENT_TYPE::CAMERA);
	return static_pointer_cast<Camera>(component);
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	/*
		 해당 컴포넌트에 접근하여 GameObject를 설정.
		 너의 GameObject 대상은 바로 나다, 라는 것을 지정
		component->SetGameObject(make_shared<GameObject>(this)); 로 만들면 안될까?
		-> 또다른 스마트 포인터가 생성됨.
		-> 이미 만들어져 있는 대상을 기준으로 또 다른 객체가 생성됨.
		-> 우리는 또 다른 객체를 만들지 않고, 만들어져있는 놈을 스마트 포인터로 만들고 싶은거임.

		this가 아니라 자기자신에 대한 스마트 포인터를 넘기는 방법임. weak_ptr로 넘김
		C++ 표준임.
	*/
	component->SetGameObject(shared_from_this()); // 셋팅
	// 컴포넌트의 type을 가져옴. 너 뭐하는 놈인데? (transform, meshRenderer 등등)
	uint8 index = static_cast<uint8>(component->GetType());
	// 기본 형태의 Component라면
	if (index < FIXED_COMPONENT_COUNT)
	{
		// _components 배열에 해당 컴포넌트 자체를 추가시킴. 1:1 대응
		_components[index] = component;
	}
	else
	{
		// 기본 Component가 아니라면, MonoBehaviour를 상속한 우리의 커스텀 스크립트이므로 아래처럼 추가
		// dynamic_pointer_cast란 c++의 dynamic_cast와 비슷함.
		// 스마트끼리의 작업에서는 dynamic_pointer_cast를 사용
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}