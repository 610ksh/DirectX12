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

// �� GameObject�� ��� �ִ� ������Ʈ�� ��ũ��Ʈ�� Awake �Լ��� ȣ��
void GameObject::Awake()
{
	// ���� ��� �ִ� ��� component�� Awake �Լ��� ȣ���Ѵ�.
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	// ���� ��� �ִ� ��� MonoBehaviour script�� Awake �Լ��� ȣ���Ѵ�.
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

	// ��ũ��Ʈ������ FinalUpdate�� ������� ���ϵ��� �������.
}


shared_ptr<Component> GameObject::GetFixedComponent(COMPONENT_TYPE type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

// �� GameObject�� ����ִ� Transform ������ �ѱ�
shared_ptr<Transform> GameObject::GetTransform()
{
	//uint8 index = static_cast<uint8>(COMPONENT_TYPE::TRANSFORM);
	// ����Ʈ �����ͳ����� casting�̶� static_pointer_cast
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
		 �ش� ������Ʈ�� �����Ͽ� GameObject�� ����.
		 ���� GameObject ����� �ٷ� ����, ��� ���� ����
		component->SetGameObject(make_shared<GameObject>(this)); �� ����� �ȵɱ�?
		-> �Ǵٸ� ����Ʈ �����Ͱ� ������.
		-> �̹� ������� �ִ� ����� �������� �� �ٸ� ��ü�� ������.
		-> �츮�� �� �ٸ� ��ü�� ������ �ʰ�, ��������ִ� ���� ����Ʈ �����ͷ� ����� ��������.

		this�� �ƴ϶� �ڱ��ڽſ� ���� ����Ʈ �����͸� �ѱ�� �����. weak_ptr�� �ѱ�
		C++ ǥ����.
	*/
	component->SetGameObject(shared_from_this()); // ����
	// ������Ʈ�� type�� ������. �� ���ϴ� ���ε�? (transform, meshRenderer ���)
	uint8 index = static_cast<uint8>(component->GetType());
	// �⺻ ������ Component���
	if (index < FIXED_COMPONENT_COUNT)
	{
		// _components �迭�� �ش� ������Ʈ ��ü�� �߰���Ŵ. 1:1 ����
		_components[index] = component;
	}
	else
	{
		// �⺻ Component�� �ƴ϶��, MonoBehaviour�� ����� �츮�� Ŀ���� ��ũ��Ʈ�̹Ƿ� �Ʒ�ó�� �߰�
		// dynamic_pointer_cast�� c++�� dynamic_cast�� �����.
		// ����Ʈ������ �۾������� dynamic_pointer_cast�� ���
		_scripts.push_back(dynamic_pointer_cast<MonoBehaviour>(component));
	}
}