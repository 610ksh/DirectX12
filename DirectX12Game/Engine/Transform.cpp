#include "pch.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}

void Transform::FinalUpdate()
{
	/// 1. W(월드) 행렬을 만듦
	Matrix matScale = Matrix::CreateScale(_localScale); // S
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x); // Rx
	matRotation *= Matrix::CreateRotationY(_localRotation.y); // Ry
	matRotation *= Matrix::CreateRotationZ(_localRotation.z); // Rz
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition); // T

	/// 2. SRT 곱으로 월드행렬 완성
	_matLocal = matScale * matRotation * matTranslation; // Local에 넣음.
	_matWorld = _matLocal; // 부모가 없다면 Local 자체가 World를 의미함.

	/// 3. 부모를 가지고 있다면 World 행렬을 바꿔준다.
	shared_ptr<Transform> parent = GetParent().lock(); // 부모를 받아옴
	if (parent != nullptr)
	{
		// 부모의 최종 월드 행렬을 받아와서 곱해주고 있다. SRT * SRT 느낌
		_matWorld *= parent->GetLocalToWorldMatrix(); // 부모님의 로컬로
	}
}


void Transform::PushData()
{
	// WVP를 만들어서 곱해줘서 넘겨줘야함.
	// 현재는 [W] 까지 위에서 만들어놨음.
	TransformParams transformParams = {};
	transformParams.matWorld = _matWorld;
	transformParams.matView = Camera::S_MatView;
	transformParams.matProjection = Camera::S_MatProjection;
	transformParams.matWV = _matWorld * Camera::S_MatView;
	// W V P 순서대로 곱해주자.
	transformParams.matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;

	// 이미 만들어놓고 예약 지정해둔 상수버퍼(b0)에 WVP행렬을 밀어넣어줌. 실제로 들어감.
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&transformParams, sizeof(transformParams));
}
