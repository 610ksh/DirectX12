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
	/// 1. W(����) ����� ����
	Matrix matScale = Matrix::CreateScale(_localScale); // S
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x); // Rx
	matRotation *= Matrix::CreateRotationY(_localRotation.y); // Ry
	matRotation *= Matrix::CreateRotationZ(_localRotation.z); // Rz
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition); // T

	/// 2. SRT ������ ������� �ϼ�
	_matLocal = matScale * matRotation * matTranslation; // Local�� ����.
	_matWorld = _matLocal; // �θ� ���ٸ� Local ��ü�� World�� �ǹ���.

	/// 3. �θ� ������ �ִٸ� World ����� �ٲ��ش�.
	shared_ptr<Transform> parent = GetParent().lock(); // �θ� �޾ƿ�
	if (parent != nullptr)
	{
		// �θ��� ���� ���� ����� �޾ƿͼ� �����ְ� �ִ�. SRT * SRT ����
		_matWorld *= parent->GetLocalToWorldMatrix(); // �θ���� ���÷�
	}
}


void Transform::PushData()
{
	// WVP�� ���� �����༭ �Ѱ������.
	// ����� [W] ���� ������ ��������.
	TransformParams transformParams = {};
	transformParams.matWorld = _matWorld;
	transformParams.matView = Camera::S_MatView;
	transformParams.matProjection = Camera::S_MatProjection;
	transformParams.matWV = _matWorld * Camera::S_MatView;
	// W V P ������� ��������.
	transformParams.matWVP = _matWorld * Camera::S_MatView * Camera::S_MatProjection;

	// �̹� �������� ���� �����ص� �������(b0)�� WVP����� �о�־���. ������ ��.
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&transformParams, sizeof(transformParams));
}
