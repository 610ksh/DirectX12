#include "pch.h"
#include "Shader.h"
#include "Engine.h"

void Shader::Init(const wstring& path)
{
	// VertexShader 생성
	CreateVertexShader(path, "VS_Main", "vs_5_0"); // 메인함수 생성
	// PixelShader 생성
	CreatePixelShader(path, "PS_Main", "ps_5_0"); // 메인함수 생성

	// pipeline에 대한 설명서 제작을 위한 편수
	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		// 위치값과 색상이 어떤식으로 들어가게 될지 설정해주고 있음. (0, 12, 28은 offset임)
		// pos는 float 3개. 12바이트
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		// color는 rgba라서 float 4개. 16바이트. -> 12 + 16 = 28 이전까지.
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		// texture는 28부터 시작함. 32비트 2개 사용함 (R32, G32)
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	// pipeline에 대한 설명서 내용 기입
	_pipelineDesc.InputLayout = { desc, _countof(desc) };
	_pipelineDesc.pRootSignature = ROOT_SIGNATURE.Get();

	_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_pipelineDesc.DepthStencilState.DepthEnable = FALSE;
	_pipelineDesc.DepthStencilState.StencilEnable = FALSE;
	_pipelineDesc.SampleMask = UINT_MAX;
	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_pipelineDesc.NumRenderTargets = 1;
	_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_pipelineDesc.SampleDesc.Count = 1;

	// GraphicsPipelineState 생성
	DEVICE->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));
}

void Shader::Update()
{
	// Init에서 만들었던 pipelineState를 가지고 파이프라인을 새롭게 설정.
	CMD_LIST->SetPipelineState(_pipelineState.Get()); // GPU에게 일감을 던짐.
}


/// 밑의 함수들은 Init 함수에서 호출되는 함수들.

void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// 쉐이더 파일을 읽어와 파싱한다. 못 읽었다면 아래의 내용의 메시지 박스를 호출함.
	if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str(), version.c_str(), compileFlag, 0, &blob, &_errBlob)))
	{
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	// pipelineDesc 내용의 VS 부분으로
	CreateShader(path, name, version, _vsBlob, _pipelineDesc.VS);
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	// pipelineDesc 내용의 PS 부분으로
	CreateShader(path, name, version, _psBlob, _pipelineDesc.PS);
}