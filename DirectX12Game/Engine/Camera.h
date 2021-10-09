#pragma once
#include "Component.h"
#include "Frustum.h"

// 카메라 방식
enum class PROJECTION_TYPE
{
	PERSPECTIVE, // 원근 투영
	ORTHOGRAPHIC, // 직교 투영
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	void Render();

private:
	// 카메라 type 변수
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	// 카메라 변수
	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f; // 180/4 = 45도
	float _scale = 1.f; // 직교투영을 위한 변수

	// 선언과 초기화를 해줌.
	Matrix _matView = {};
	Matrix _matProjection = {};

	Frustum _frustum;

public:
	// TEMP (임시적으로 static 사용)
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

