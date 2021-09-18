#pragma once
#include "Component.h" // for FIXED_COMPONENT_COUNT

class Transform;
class MeshRenderer;
class MonoBehaviour;

/*
	public enable_shared_from_this<GameObject> ����̶�
	AddComponent �Լ����� shared_from_this() �Լ��� ����ϱ� ���ؼ�.
	�ڱ��ڽ��� ����Ʈ �����ͷ� �ѱ�� ������ ������ ������ this�ۿ� ������.
	component->SetGameObject(make_shared<GameObject>(this)) �� ����� ū�ϳ�
	-> �Ǵٸ� ����Ʈ �����Ͱ� ������.
	-> �̹� ������� �ִ� ����� �������� �� �ٸ� ��ü�� ������.
	-> �츮�� �� �ٸ� ��ü�� ������ �ʰ�, ��������ִ� ���� ����Ʈ �����ͷ� ����� ��������.
	component->SetGameObject(shared_from_this()); �� ����ؾ���
*/
class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Init();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	shared_ptr<Transform> GetTransform();

	void AddComponent(shared_ptr<Component> component);

private:
	// component�� array�� ��� ����.
	// [0] : transform, [1] : mesh renderer �̷������� ������ ����������. �������� 2�� �� �� ����.
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// ������ ���߿� MonoBehaviour��� ��ũ��Ʈ�� ��ӹ޾� Ŀ�������� ���� ������Ʈ�� ���� ����
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

