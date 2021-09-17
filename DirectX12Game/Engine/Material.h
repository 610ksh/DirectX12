#pragma once

// ���漱��
class Shader;
class Texture;

// ��ü ������ ���� enum
enum
{
	MATERIAL_INT_COUNT = 5,
	MATERIAL_FLOAT_COUNT = 5,
	MATERIAL_TEXTURE_COUNT = 5,
};

struct MaterialParams
{
	// ���� �迭�� �����ؼ� �ֱ⺸�� �Լ��� �־��ֵ��� ����.
	void SetInt(uint8 index, int32 value) { intParams[index] = value; }
	void SetFloat(uint8 index, float value) { floatParams[index] = value; }

	// std::array ����, ����üũ�� �ڵ����� �ϴ� ����� ����Ǿ��ִ�.
	// �Ϲ� �迭���� �����. ������ üũ.
	// ���Ϳ� ���������� ���̰� ������ �迭
	array<int32, MATERIAL_INT_COUNT> intParams;
	array<float, MATERIAL_FLOAT_COUNT> floatParams;
};

/// Material
class Material
{
public:
	shared_ptr<Shader> GetShader() { return _shader; }

	// setter
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
	void SetInt(uint8 index, int32 value) { _params.SetInt(index, value); }
	void SetFloat(uint8 index, float value) { _params.SetFloat(index, value); }
	void SetTexture(uint8 index, shared_ptr<Texture> texture) { _textures[index] = texture; }

	void Update();

private:
	// Shader (���̴�)
	shared_ptr<Shader>	_shader;
	// Material �Ķ���� (���� ���� ����ü�� ����)
	MaterialParams		_params;
	// Texture (�ؽ�ó)
	array<shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> _textures;
};

