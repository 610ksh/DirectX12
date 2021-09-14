#pragma once

// [계약서 / 결재]

// CPU [         ]		GPU [         ]
// 한국 [         ]		베트남 [        ]


class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	// 샘플러 만듦
	void CreateSamplerDesc();
	// 루트 시그니처 만듦
	void CreateRootSignature();

private:
	ComPtr<ID3D12RootSignature> _signature;

	// 나중에는 배열 형태임. 지금은 딱 1개만 만들 예정.
	D3D12_STATIC_SAMPLER_DESC _samplerDesc;
};

