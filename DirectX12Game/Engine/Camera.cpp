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
	/// View ���
	// ī�޶��� Transform�� �����Ͽ� ��������� ������. �װ��� ������� �����
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	// ���� ȭ�� ������ ���� ������
	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	/// Projection ���
	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height /*aspect ratio*/, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);

	/// �ӽ� static ������ �־���
	S_MatView = _matView;
	S_MatProjection = _matProjection;
}

void Camera::Render()
{
	// ���� ī�޶� �Ҽӵ� ��ü �ϳ��� Scene�� ������ (�̱��� �̿�)
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();

	// TODO : Layer ����
	// ���� �Ҽӵ� ��� ���� ������Ʈ���� ���� ���͸� �����´�.
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	for (auto& gameObject : gameObjects)
	{
		if (gameObject->GetMeshRenderer() == nullptr)
			continue;

		gameObject->GetMeshRenderer()->Render();
	}
}