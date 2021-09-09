#pragma once

// [�ϰ� �����] ���� �ηµ��� �� �ؾ����� ���
class Shader
{
public:
	// ���� ���̴� ����
	void Init(const wstring& path);
	void Update();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	// ComPtr�� GPU�ʿ��� �����Ǵ� �����Ϳ� ���� �ڷ���
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	// �����ϰԵ� PipelineState ����. Init �Լ����� �ʱ�ȭ ����
	ComPtr<ID3D12PipelineState>			_pipelineState;
	// ���� pipelineState�� �ʱ�ȭ�ϱ� ���ؼ� �ʿ��� ���� ����
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};
};

