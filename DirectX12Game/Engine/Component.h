#pragma once
#include "Object.h"

// Component들의 Type
enum class COMPONENT_TYPE : uint8
{
	TRANSFORM, // 0
	MESH_RENDERER,
	CAMERA,
	LIGHT,
	// ...

	// 참고로 모노비헤이비어가 마지막에 등장해야함.
	MONO_BEHAVIOUR, 
	END,
};

enum
{
	// 모노 비헤이비어를 뺀 개수
	FIXED_COMPONENT_COUNT = static_cast<uint8>(COMPONENT_TYPE::END) - 1
};

class GameObject;
class Transform;

class Component : public Object
{
public:
	// 어떤 타입인지 받음
	Component(COMPONENT_TYPE type);
	// 소멸자는 부모 클래스이기에 virtual을 붙여줌.
	virtual ~Component();

public:
	// 시점을 분리했다는게 특징
	virtual void Awake() { } // Start 이전에 먼저 초기화되어야 하는것
	virtual void Start() { }
	virtual void Update() { }
	virtual void LateUpdate() { } // Update 다음에 처리되어야할 부분
	// 순서를 최종적으로 마지막으로 처리하는 부분.
	virtual void FinalUpdate() { } // 엔진에서만 사용할 예정.

public:
	COMPONENT_TYPE GetType() { return _type; }
	// 살아있는지 죽었는지 체크. weak_ptr의 expired로 확인 가능.
	bool IsValid() { return _gameObject.expired() == false; }

	// 유니티 코드에서도 있는 내용.
	shared_ptr<GameObject> GetGameObject();
	shared_ptr<Transform> GetTransform();

private:
	// GameObject 클래스에서만 이것을 실행하길 바라기에 private으로 했고,
	// friend class GameObject를 선언했음.
	friend class GameObject;
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	// 위에서 따로 enum clas 만듦.
	COMPONENT_TYPE _type;

	/*
		weak_ptr인 이유는, GameObject에서도 이 클래스 Component를 들고 있을건데,
		이렇게 컴포넌트와 서로 가리키고 있는 형태로 만들거임.
		Component의 주인인 GameObject를 가리키고, GameObject에서는 사용중인 Component를 가리킴.
		이때 shared_ptr로 만들면 순환구조가 되어서 문제가 됨 => reference count가 줄어들지 못함.
		소멸되지 않음. 물론, shared_ptr로 만들어서 해결할 수도 있긴함. 명시적인 줄일수 있음.
		결국, shared_ptr로 만들 수 없어서 weak_ptr로 했다는 의미.
	*/
	weak_ptr<GameObject> _gameObject;
};

