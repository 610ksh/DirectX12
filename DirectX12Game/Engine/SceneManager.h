#pragma once

class Scene; // forward declaration

/*
	현재의 Scene을 들고 있는 Manager 클래스
*/
class SceneManager
{
	// 싱글톤 추가
	DECLARE_SINGLE(SceneManager);
public:
	void Update();
	void LoadScene(wstring sceneName); // 씬을 로드

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
private:
	shared_ptr<Scene> LoadTestScene();
private:
	shared_ptr<Scene> _activeScene;
};

