#pragma once

class Scene; // forward declaration

/*
	������ Scene�� ��� �ִ� Manager Ŭ����
*/
class SceneManager
{
	// �̱��� �߰�
	DECLARE_SINGLE(SceneManager);
public:
	void Update();
	void LoadScene(wstring sceneName); // ���� �ε�

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
private:
	shared_ptr<Scene> LoadTestScene();
private:
	shared_ptr<Scene> _activeScene;
};

