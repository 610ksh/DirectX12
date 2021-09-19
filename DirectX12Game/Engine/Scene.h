#pragma once

class GameObject; // forward declaration

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

private:
	// 게임 오브젝트를 들고 있음.
	vector<shared_ptr<GameObject>> _gameObjects;
};

