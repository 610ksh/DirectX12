#pragma once
#include "Component.h"

enum class LIGHT_TYPE : uint8
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

struct LightColor
{
	Vec4 diffuse; // RGB + 패딩4byte = V
	Vec4 ambient;
	Vec4 specular;
};

// 빛이 들고 있는 정보
struct LightInfo
{
	LightColor	color; // DAS
	Vec4		position; // 빛의 위치
	Vec4		direction; // 빛이 쏘는 방향
	int32 		lightType; // uint8이 아니라 32로 한 이유는 세이더쪽과 패딩을 맞춰주기 위해서임
	float 		range; // 빛의 최대 범위
	float 		angle; // 빛이 쏘는 각도 spot에서 사용
	int32		padding; // 아무 의미x, 데이터 사이즈를 16바이트의 배수를 위해 필요.
};

// 최종적으로 세이더쪽으로 넘겨줄 빛의 정보(Params)를 담고 있음.
struct LightParams
{
	uint32		lightCount; // 조명의 개수
	Vec3 		padding; // 패딩값
	LightInfo	lights[50]; // 조명 최대 50개를 넣어줌.
};

class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void FinalUpdate() override;

public:
	/// 최종적인 getter
	const LightInfo& GetLightInfo() { return _lightInfo; }
	/// LightInfo에 대한 setter
	void SetLightDirection(const Vec3& direction) { _lightInfo.direction = direction; }

	void SetDiffuse(const Vec3& diffuse) { _lightInfo.color.diffuse = diffuse; }
	void SetAmbient(const Vec3& ambient) { _lightInfo.color.ambient = ambient; }
	void SetSpecular(const Vec3& specular) { _lightInfo.color.specular = specular; }

	void SetLightType(LIGHT_TYPE type) { _lightInfo.lightType = static_cast<int32>(type); }
	void SetLightRange(float range) { _lightInfo.range = range; }
	void SetLightAngle(float angle) { _lightInfo.angle = angle; }

private:
	LightInfo _lightInfo = {};
};

