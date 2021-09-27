#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	/// View 행렬
	// 카메라의 Transform에 접근하여 월드행렬을 가져옴. 그것의 역행렬을 계산함
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	// 현재 화면 비율의 값을 가져옴
	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	/// Projection 행렬
	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height /*aspect ratio*/, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);

	/// 임시 static 변수에 넣어줌
	S_MatView = _matView;
	S_MatProjection = _matProjection;
}

void Camera::Render()
{
	// 현재 카메라가 소속된 전체 하나의 Scene을 가져옴 (싱글톤 이용)
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();

	// TODO : Layer 구분
	// 씬에 소속된 모든 게임 오브젝트들을 담은 벡터를 가져온다.
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		gameObject->GetMeshRenderer()->Render();
	}
}