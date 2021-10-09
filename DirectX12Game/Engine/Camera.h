#pragma once
#include "Component.h"
#include "Frustum.h"

// ī�޶� ���
enum class PROJECTION_TYPE
{
	PERSPECTIVE, // ���� ����
	ORTHOGRAPHIC, // ���� ����
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	void Render();

private:
	// ī�޶� type ����
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	// ī�޶� ����
	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f; // 180/4 = 45��
	float _scale = 1.f; // ���������� ���� ����

	// ����� �ʱ�ȭ�� ����.
	Matrix _matView = {};
	Matrix _matProjection = {};

	Frustum _frustum;

public:
	// TEMP (�ӽ������� static ���)
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

