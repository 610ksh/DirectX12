#pragma once

// [��༭ / ����]

// CPU [         ]		GPU [         ]
// �ѱ� [         ]		��Ʈ�� [        ]


class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	// ���÷� ����
	void CreateSamplerDesc();
	// ��Ʈ �ñ״�ó ����
	void CreateRootSignature();

private:
	ComPtr<ID3D12RootSignature> _signature;

	// ���߿��� �迭 ������. ������ �� 1���� ���� ����.
	D3D12_STATIC_SAMPLER_DESC _samplerDesc;
};

