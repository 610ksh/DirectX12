#include "pch.h"
#include "RootSignature.h"


void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	// Root parameter를 2개 생성함.
	CD3DX12_ROOT_PARAMETER param[2];
	param[0].InitAsConstantBufferView(0); // 0번 -> b0
	param[1].InitAsConstantBufferView(1); // 1번 -> b1

	// 루트 시그니처도 제작하기 위해서 설명서를 추가로 만들고 있음.
	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param); // C가 들어갔으니, 헬퍼 클래스를 이용함.
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}