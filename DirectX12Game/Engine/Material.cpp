#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{

}

Material::~Material()
{

}

void Material::PushData()
{
	/// 1. CBV ���ε�
	// Constant Buffer�� �����ͼ� ���� �־� CBV�� ����� �ִ�.
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	/// 2. SRV ���ε�
	// SRV�� ����� �ִ�. �ؽ�ó.
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		// �ؽ�ó �������� ��ȣ ��������
		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		// SetSRV
		GEngine->GetTableDescHeap()->SetSRV(_textures[i]->GetCpuHandle(), reg);
	}

	/// 3. ���������� ����
	_shader->Update();
}
