#include "pch.h"
#include "Shader.h"
#include "Engine.h"

void Shader::Init(const wstring& path)
{
	// VertexShader ����
	CreateVertexShader(path, "VS_Main", "vs_5_0"); // �����Լ� ����
	// PixelShader ����
	CreatePixelShader(path, "PS_Main", "ps_5_0"); // �����Լ� ����

	// pipeline�� ���� ���� ������ ���� ���
	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		// ��ġ���� ������ ������� ���� ���� �������ְ� ����. (0, 12, 28�� offset��)
		// pos�� float 3��. 12����Ʈ
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		// color�� rgba�� float 4��. 16����Ʈ. -> 12 + 16 = 28 ��������.
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		// texture�� 28���� ������. 32��Ʈ 2�� ����� (R32, G32)
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	// pipeline�� ���� ���� ���� ����
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

	// GraphicsPipelineState ����
	DEVICE->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState));
}

void Shader::Update()
{
	// Init���� ������� pipelineState�� ������ ������������ ���Ӱ� ����.
	CMD_LIST->SetPipelineState(_pipelineState.Get()); // GPU���� �ϰ��� ����.
}


/// ���� �Լ����� Init �Լ����� ȣ��Ǵ� �Լ���.

void Shader::CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	uint32 compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// ���̴� ������ �о�� �Ľ��Ѵ�. �� �о��ٸ� �Ʒ��� ������ �޽��� �ڽ��� ȣ����.
	if (FAILED(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, name.c_str(), version.c_str(), compileFlag, 0, &blob, &_errBlob)))
	{
		::MessageBoxA(nullptr, "Shader Create Failed !", nullptr, MB_OK);
	}

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

void Shader::CreateVertexShader(const wstring& path, const string& name, const string& version)
{
	// pipelineDesc ������ VS �κ�����
	CreateShader(path, name, version, _vsBlob, _pipelineDesc.VS);
}

void Shader::CreatePixelShader(const wstring& path, const string& name, const string& version)
{
	// pipelineDesc ������ PS �κ�����
	CreateShader(path, name, version, _psBlob, _pipelineDesc.PS);
}