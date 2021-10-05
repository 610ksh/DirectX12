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
	/// 1. CBV 업로드
	// Constant Buffer를 가져와서 값을 넣어 CBV를 만들고 있다.
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	/// 2. SRV 업로드
	// SRV를 만들고 있다. 텍스처.
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		// 텍스처 레지스터 번호 가져오기
		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		// SetSRV
		GEngine->GetTableDescHeap()->SetSRV(_textures[i]->GetCpuHandle(), reg);
	}

	/// 3. 파이프라인 세팅
	_shader->Update();
}
