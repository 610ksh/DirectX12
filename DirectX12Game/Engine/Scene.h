#pragma once

class GameObject; // forward declaration

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	// 씬에 소속된 모든 gameObject들을 뱉어줌.
	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

private:
	// 게임 오브젝트를 들고 있음.
	vector<shared_ptr<GameObject>> _gameObjects;
};

