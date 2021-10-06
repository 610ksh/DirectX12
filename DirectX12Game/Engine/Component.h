#pragma once
#include "Object.h"

// Component���� Type
enum class COMPONENT_TYPE : uint8
{
	TRANSFORM, // 0
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	// ...

	// ����� �������̺� �������� �����ؾ���.
	MONO_BEHAVIOUR, 
	END,
};

enum
{
	// ��� �����̺� �� ����
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component : public Object
{
public:
	// � Ÿ������ ����
	Component(COMPONENT_TYPE type);
	// �Ҹ��ڴ� �θ� Ŭ�����̱⿡ virtual�� �ٿ���.
	virtual ~Component();

public:
	// ������ �и��ߴٴ°� Ư¡
	virtual void Awake() { } // Start ������ ���� �ʱ�ȭ�Ǿ�� �ϴ°�
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { } // Update ������ ó���Ǿ���� �κ�
	// ������ ���������� ���������� ó���ϴ� �κ�.
	virtual void FinalUpdate() { } // ���������� ����� ����.

public:
	COMPONENT_TYPE GetType() { return _type; }
	// ����ִ��� �׾����� üũ. weak_ptr�� expired�� Ȯ�� ����.
	bool IsValid() { return _gameObject.expired() == false; }

	// ����Ƽ �ڵ忡���� �ִ� ����.
	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	// GameObject Ŭ���������� �̰��� �����ϱ� �ٶ�⿡ private���� �߰�,
	// friend class GameObject�� ��������.
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	// ������ ���� enum clas ����.
	COMPONENT_TYPE _type;

	/*
		weak_ptr�� ������, GameObject������ �� Ŭ���� Component�� ��� �����ǵ�,
		�̷��� ������Ʈ�� ���� ����Ű�� �ִ� ���·� �������.
		Component�� ������ GameObject�� ����Ű��, GameObject������ ������� Component�� ����Ŵ.
		�̶� shared_ptr�� ����� ��ȯ������ �Ǿ ������ �� => reference count�� �پ���� ����.
		�Ҹ���� ����. ����, shared_ptr�� ���� �ذ��� ���� �ֱ���. ������� ���ϼ� ����.
		�ᱹ, shared_ptr�� ���� �� ��� weak_ptr�� �ߴٴ� �ǹ�.
	*/
	weak_ptr<GameObject> _gameObject;
};

