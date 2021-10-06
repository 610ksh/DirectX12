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

	/// ����
	void Render();

private:
	// �� ������ GPU ������ �־��ִ� �Լ�
	void PushLightData();

public:
	void AddGameObject(shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	// ���� �Ҽӵ� ��� gameObject���� �����.
	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }

private:
	// ���� ������Ʈ�� ��� ����.
	vector<shared_ptr<GameObject>> _gameObjects;
};

