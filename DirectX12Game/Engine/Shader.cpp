#include "pch.h"
#include "Shader.h"
#include "Engine.h"

Shader::Shader() : Object(OBJECT_TYPE::SHADER)
{

}

Shader::~Shader()
{

}

void Shader::Init(const wstring& path, ShaderInfo info)
{
	_info = info;

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
		//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		// texture�� 28���� ������. 32��Ʈ 2�� ����� (R32, G32)
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		/// normal, tangent
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	// pipeline�� ���� ���� ���� ����
	_pipelineDesc.InputLayout = { desc, _countof(desc) };
	_pipelineDesc.pRootSignature = ROOT_SIGNATURE.Get();

	_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//_pipelineDesc.DepthStencilState.DepthEnable = TRUE; // Depth ���
	//_pipelineDesc.DepthStencilState.StencilEnable = FALSE; // Stencil ���x
	_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); // DepthStencil �ѹ��� ó��
	_pipelineDesc.SampleMask = UINT_MAX;
	_pipelineDesc.PrimitiveTopologyType = info.topologyType;
	_pipelineDesc.NumRenderTargets = 1;
	_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_pipelineDesc.SampleDesc.Count = 1;
	_pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // DSVFormat �ʱ�ȭ

	switch (info.shaderType)
	{
	case SHADER_TYPE::DEFERRED:
		_pipelineDesc.NumRenderTargets = RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT;
		_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT; // POSITION
		_pipelineDesc.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT; // NORMAL
		_pipelineDesc.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // COLOR
		break;
	case SHADER_TYPE::FORWARD:
		_pipelineDesc.NumRenderTargets = 1;
		_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	}


	/// ShaderInfo ����. rasterizerType
	switch (info.rasterizerType)
	{
	case RASTERIZER_TYPE::CULL_BACK:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_NONE:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIZER_TYPE::WIREFRAME:
		_pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		_pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}
	/// ShaderInfo ����. depthStencilType
	switch (info.depthStencilType)
	{
	case DEPTH_STENCIL_TYPE::LESS:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_TYPE::LESS_EQUAL:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_TYPE::GREATER:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_TYPE::GREATER_EQUAL:
		_pipelineDesc.DepthStencilState.DepthEnable = TRUE;
		_pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	}

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