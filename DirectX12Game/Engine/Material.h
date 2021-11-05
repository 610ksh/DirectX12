#pragma once
#include "Object.h"

// 전방선언
class Shader;
class Texture;

// 전체 개수를 위한 enum
enum
{
	MATERIAL_INT_COUNT = 4,
	MATERIAL_FLOAT_COUNT = 4,
	MATERIAL_TEXTURE_COUNT = 4,
	MATERIAL_VECTOR2_COUNT = 4,
};

struct MaterialParams
{
	// 직접 배열에 접근해서 넣기보다 함수로 넣어주도록 만듦.
	void SetInt(uint8 index, int32 value) { intParams[index] = value; }
	void SetFloat(uint8 index, float value) { floatParams[index] = value; }
	void SetTexOn(uint8 index, int32 value) { texOnParams[index] = value; }
	void SetVec2(uint8 index, Vec2 value) { vec2Params[index] = value; }

	// std::array 선언, 범위체크를 자동으로 하는 기능이 내장되어있다.
	// 일반 배열보다 권장됨. 안정성 체크.
	// 벡터와 유사하지만 길이가 고정된 배열
	// 아래의 배열은 세이더에 그대로 전달되고 있다. Material의 Update함수에 의해
	array<int32, MATERIAL_INT_COUNT> intParams; // 5개 생성
	array<float, MATERIAL_FLOAT_COUNT> floatParams; // 5개 생성
	array<int32, MATERIAL_TEXTURE_COUNT> texOnParams; // 텍스처를 사용하는지 여부 변수
	array<Vec2, MATERIAL_VECTOR2_COUNT> vec2Params;
};

/// Material
class Material : public Object
{
public:
	Material();
	virtual ~Material();

	shared_ptr<Shader> GetShader() { return _shader; }

	// setter
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
	void SetInt(uint8 index, int32 value) { _params.SetInt(index, value); }
	void SetFloat(uint8 index, float value) { _params.SetFloat(index, value); }
	void SetTexture(uint8 index, shared_ptr<Texture> texture)
	{
		_textures[index] = texture;
		// 텍스처 정보가 nullptr이면 0 아니면 1로 설정. 3항연산자
		_params.SetTexOn(index, (texture == nullptr ? 0 : 1));
	}

	void SetVec2(uint8 index, Vec2 value) { _params.SetVec2(index, value); }

	// 데이터를 밀언허는것.
	void PushData();

private:
	// Shader (세이더)
	shared_ptr<Shader>	_shader;
	// Material 파라미터 (위에 따로 구조체로 만듦)
	MaterialParams		_params;
	// Texture (텍스처), 5개를 미리 생성해놈.
	array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> _textures;
};

