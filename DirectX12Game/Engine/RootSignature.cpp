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
	// descriptor table의 내부가 어떤식으로 되어있는지 range를 설정해줘야함. (내용물 설명서)
	CD3DX12_DESCRIPTOR_RANGE ranges[] = // 배열 형태라서 더 추가할 수 있다.
	{
		// (1.용도 : TYPE_CBV 형태, 2.몇개를 활용하고 싶은지, 3.몇번부터 레지스터를 쓰고 싶은지)
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT - 1, 1), // b1~b4
		// SRV 전용.
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0~t4 
	};

	// Root parameter를 2개 생성함.
	CD3DX12_ROOT_PARAMETER param[2];
	// root CBV 방식으로 추가. b0 레지스터를 사용. 전역으로 사용할 녀석을 선언한 셈
	param[0].InitAsConstantBufferView(static_cast<uint32>(CBV_REGISTER::b0)); // b0
	// 레지스터에 Descriptor table 생성. 전체 크기(개수)와 시작주소를 넘겨줘야함.
	param[1].InitAsDescriptorTable(_countof(ranges), ranges);

	// ConstantBufferView(CBV) 형태로 레지스터에 초기화하겠다는 명령어.
	//param[0].InitAsConstantBufferView(0); // 0번 -> b0 레지스터 사용
	//param[1].InitAsConstantBufferView(1); // 1번 -> b1 레지스터 사용

	// 루트 시그니처도 제작하기 위해서 설명서를 추가로 만들고 있음. C가 들어갔으니, 헬퍼 클래스를 이용함.
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc); // 샘플러 추가됨
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}

void RootSignature::CreateSamplerDesc()
{
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}