#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

void RootSignature::Init()
{
	CreateSamplerDesc();
	CreateRootSignature();
}

void RootSignature::CreateRootSignature()
{
	// descriptor table�� ���ΰ� ������� �Ǿ��ִ��� range�� �����������. (���빰 ����)
	CD3DX12_DESCRIPTOR_RANGE ranges[] = // �迭 ���¶� �� �߰��� �� �ִ�.
	{
		// (1.�뵵 : TYPE_CBV ����, 2.��� Ȱ���ϰ� ������, 3.������� �������͸� ���� ������)
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT - 1, 1), // b1~b4
		// SRV ����.
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4 
	};

	// Root parameter�� 2�� ������.
	CD3DX12_ROOT_PARAMETER param[2];
	// root CBV ������� �߰�. b0 �������͸� ���. �������� ����� �༮�� ������ ��
	param[0].InitAsConstantBufferView(static_cast<uint32>(CBV_REGISTER::b0)); // b0
	// �������Ϳ� Descriptor table ����. ��ü ũ��(����)�� �����ּҸ� �Ѱ������.
	param[1].InitAsDescriptorTable(_countof(ranges), ranges);

	// ConstantBufferView(CBV) ���·� �������Ϳ� �ʱ�ȭ�ϰڴٴ� ��ɾ�.
	//param[0].InitAsConstantBufferView(0); // 0�� -> b0 �������� ���
	//param[1].InitAsConstantBufferView(1); // 1�� -> b1 �������� ���

	// ��Ʈ �ñ״�ó�� �����ϱ� ���ؼ� ������ �߰��� ����� ����. C�� ������, ���� Ŭ������ �̿���.
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc); // ���÷� �߰���
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // �Է� ������ �ܰ�

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}

void RootSignature::CreateSamplerDesc()
{
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}