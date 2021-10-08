#pragma once
#include "Object.h"

enum class RASTERIZER_TYPE
{
	CULL_NONE, // 모두다 연산한다는 의미
	CULL_FRONT, // 시계방향을 컬링한다.
	CULL_BACK, // 반시계를 무시한다. default 옵션
	WIREFRAME,
};

enum class DEPTH_STENCIL_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
};

struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_TYPE depthStencilType = DEPTH_STENCIL_TYPE::LESS;
};

// [일감 기술서] 외주 인력들이 뭘 해야할지 기술
class Shader : public Object
{
public:
	Shader();
	virtual ~Shader();

	// 최초 쉐이더 생성
	void Init(const wstring& path, ShaderInfo info = ShaderInfo());
	void Update();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	// ComPtr은 GPU쪽에서 생성되는 데이터에 대한 자료형
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	// 생성하게될 PipelineState 변수. Init 함수에서 초기화 예정
	ComPtr<ID3D12PipelineState>			_pipelineState;
	// 위의 pipelineState를 초기화하기 위해서 필요한 설명서 변수
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

