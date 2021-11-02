#pragma once

class Scene; // forward declaration

enum
{
	MAX_LAYER = 32
};


/*
	������ Scene�� ��� �ִ� Manager Ŭ����
*/
class SceneManager
{
	// �̱��� �߰�
	DECLARE_SINGLE(SceneManager);
public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName); // ���� �ε�

	/// for layer
	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }
private:
	shared_ptr<Scene> LoadTestScene();
private:
	shared_ptr<Scene> _activeScene;

	/// for layer
	array<wstring, MAX_LAYER> _layerNames;
	map<wstring, uint8> _layerIndex;
};

