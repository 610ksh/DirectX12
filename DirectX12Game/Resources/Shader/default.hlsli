
// 4����Ʈ ��ġ ���� ���
cbuffer TEST_B0 : register(b0)
{
	float4 offset0;
};

// 4����Ʈ ���� ���� ���
cbuffer TEST_B1 : register(b1)
{
	float4 offset1;
};

// t0 �������͸� �̿��ϰڴٰ� ����.
Texture2D tex_0 : register(t0);

// SamplerState
SamplerState sam_0 : register(s0);

struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float4 color : COLOR;
	float2 uv : TEXCOORD;
};

// VS = Vertex Shader
VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	/*output.pos = float4(input.pos, 1.f);
	output.pos += offset0;
	output.color = input.color;
	output.color += offset1;*/

	// �ϴ� �׳� �ִ� �״�� �ѱ�
	output.pos = float4(input.pos, 1.f);
	output.color = input.color;
	output.uv = input.uv;

	return output;
}

// PS = Pixel Shader
float4 PS_Main(VS_OUT input) : SV_Target
{
	// tex_0�� ������ t0 �������� ������ �ǹ���.
	// t0�� �ִ� (u,v)�� � ��å����(Sampler) ������ �������� ��������.
	// ��å�� �̹� sam_0 ������ �� �ִ� ����.
	// Sample�̶�� �Լ��� ���� ���� uv ���� �����Ű�°���.
	float4 color = tex_0.Sample(sam_0, input.uv);
	return color;
}