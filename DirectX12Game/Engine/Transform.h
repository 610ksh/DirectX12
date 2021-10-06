#pragma once
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	virtual void FinalUpdate() override;
	void PushData();

public:
	// Parent 기준의 SRT를 의미함.
	const Vec3& GetLocalPosition() { return _localPosition; }
	const Vec3& GetLocalRotation() { return _localRotation; }
	const Vec3& GetLocalScale() { return _localScale; }

	const Matrix& GetLocalToWorldMatrix() { return _matWorld; }
	const Vec3 GetWorldPosition() { return _matWorld.Translation(); }

	Vec3 GetRight() { return _matWorld.Right(); }
	Vec3 GetUp() { return _matWorld.Up(); }
	Vec3 GetLook() { return _matWorld.Backward(); }

	void SetLocalPosition(const Vec3& position) { _localPosition = position; }
	void SetLocalRotation(const Vec3& rotation) { _localRotation = rotation; }
	void SetLocalScale(const Vec3& scale) { _localScale = scale; }

public:
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	weak_ptr<Transform> GetParent() { return _parent; }

private:
	/// TODO : World 위치 관련 SRT
	Vec3 _localPosition = {}; // T
	Vec3 _localRotation = {}; // R
	Vec3 _localScale = { 1.f,1.f,1.f }; // S

	Matrix _matLocal = {}; // 내 부모님 기준 local
	Matrix _matWorld = {}; // 월드 행렬

	weak_ptr<Transform> _parent; // 순환 막기위한 weak
};

