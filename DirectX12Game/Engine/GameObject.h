#pragma once
#include "Component.h" // for FIXED_COMPONENT_COUNT
#include "Object.h"

class Transform;
class MeshRenderer;
class Camera;
class Light;
class MonoBehaviour;

/*
	public enable_shared_from_this<GameObject> 상속이란
	AddComponent 함수에서 shared_from_this() 함수를 사용하기 위해서.
	자기자신을 스마트 포인터로 넘기고 싶은데 기존의 문법은 this밖에 없었음.
	component->SetGameObject(make_shared<GameObject>(this)) 로 만들면 큰일남
	-> 또다른 스마트 포인터가 생성됨.
	-> 이미 만들어져 있는 대상을 기준으로 또 다른 객체가 생성됨.
	-> 우리는 또 다른 객체를 만들지 않고, 만들어져있는 놈을 스마트 포인터로 만들고 싶은거임.
	component->SetGameObject(shared_from_this()); 를 사용해야함
*/
class GameObject : public Object, public enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	// 공용 코드
	shared_ptr<Component> GetFixedComponent(COMPONENT_TYPE type);

	// 개별 요소를 반환
	shared_ptr<Transform> GetTransform();
	shared_ptr<MeshRenderer> GetMeshRenderer();
	shared_ptr<Camera> GetCamera();
	shared_ptr<Light> GetLight();


	void AddComponent(shared_ptr<Component> component);

private:
	// component를 array로 들고 있음.
	// [0] : transform, [1] : mesh renderer 이런식으로 순서가 정해져있음. 같은것은 2개 올 수 없음.
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	// 유저가 나중에 MonoBehaviour라는 스크립트를 상속받아 커스텀으로 만든 컴포넌트는 따로 관리
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

