#ifndef _DEFAULT_FX_
#define _DEFAULT_FX_

#include "params.fx"
#include "utils.fx" // for light

/////////////////////////////////////////////

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL; // added for Light
	float3 tangent : TANGENT;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
	float3 viewPos : POSITION; // added for Light
	float3 viewNormal : NORMAL; // added for Light
	float3 viewTangent : TANGENT;
	float3 viewBinormal : BINORMAL;
};

/////////////////////////////////////////////

// Main�Լ�

// VS = Vertex Shader
VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	/// 1. �Ѿ�� ���� b0 ���������� WVP ��İ� �������ش�.
	output.pos = mul(float4(input.pos, 1.f), g_matWVP); // ������ǥ��� �Ѿ
	output.uv = input.uv;

	/// 2. �� ó���� ���� viewPos, viewNormal. ���� ������ ����ϰ� ����.
	// ������ǥ�� P�� ���� �ʰ� WV������ ���ؼ� View������ Pos�� ����. �� ����� ����.
	output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz; // pos�� �������� 1�� �����ؾ���.
	
	// ���������� Normal�� �Ȱ��� View������ ����. �� ����� ����.
	// ���⺤�ʹ� �������� 0�� �־������. �׷��� translation�� ���� �ȵ�.
	output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz); 

	// tangent���� ó���� �Ѿ�ö��� Local ��ǥ��� WV�� �����ָ� viwe ��ǥ��� �ٲܼ� �ִ�.
	output.viewTangent = normalize(mul(float4(input.tangent, 0.f), g_matWV).xyz);
	// tangent�� normal���� �������ָ� binormal�� �ȴ�.
	// �޼���ǥ���̴�. ���� ��������. tangent�� ������. (T, B, N). normal�� �������ָ� �ݴ�������� ���´�
	output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));

	/// 3. �� �������� �����Ͷ������� �� ������ �޾Ƽ� ��հ����� �߰� �ȼ����� �����ؼ� ��������. �װ� PS�� ����
	return output;
}

// PS = Pixel Shader
float4 PS_Main(VS_OUT input) : SV_Target
{
	// ��� ������
	float4 color = float4(1.f, 1.f, 1.f, 1.f);
	// �ؽ�ó 0���� �����Ѵٸ�
	if (g_tex_on_0)
		// ���� ������ �ؽ�ó �������� �ٲ���.
		color = g_tex_0.Sample(g_sam_0, input.uv);
	
	// ó���� ������ ����Ʈ Normal �� �ʱ�ȭ
	float3 viewNormal = input.viewNormal;

	// ���� �븻���� �����Ѵٸ�(1��)
	if (g_tex_on_1)
	{
		// �븻�ʿ��� ���÷��� ���� u,v�� ����
		// [0,255] �������� [0,1]�� ��ȯ��(�����Ͷ������� ������)
		float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
		// [0,1] �������� [-1,1]�� ��ȯ
		tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
		// ��ȯ��� �����. TS -> VS�� �ٲٴ� �����. (viewT, viewB, viewN)�� ����. float3
		float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
		// tangentSpaceNormal�� TBN ����� ���ϸ� view��ǥ�� ������ Normal���� ���� �� �ְԵ�.
		viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
	}

	// tex_0�� ������ t0 �������� ������ �ǹ���.
	// t0�� �ִ� (u,v)�� � ��å����(Sampler) ������ �������� ��������.
	// ��å�� �̹� sam_0 ������ �� �ִ� ����.
	// Sample�̶�� �Լ��� ���� ���� uv ���� �����Ű�°���.
	//float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	
	// ���� ��ü�� ������ ������� ������Ŵ(RGBA)
	//float4 color = float4(1.f, 1.f, 1.f, 1.f);
	
	// LightColor ���� 0���� �ʱ�ȭ (diffuse, ambient, specular)
	LightColor totalColor = (LightColor)0.f;

	// ��ġ�� ���� ������ŭ ���鼭 ó��
	for (int i = 0; i < g_lightCount; ++i)
	{
		// utils.hlsli�� �ִ� �Լ� ���. diffuse, ambient, specular�� �������.
		LightColor color = CalculateLightColor(i, viewNormal, input.viewPos);
		totalColor.diffuse += color.diffuse;
		totalColor.ambient += color.ambient;
		totalColor.specular += color.specular;
	}

	// Į�� ���� ����
	color.xyz = (totalColor.diffuse.xyz * color.xyz)
		+ totalColor.ambient.xyz * color.xyz
		+ totalColor.specular.xyz;

	return color;
}

// [Texture Shader]
// g_tex_0 : Output Texture
// AlphaBlend : true
struct VS_TEX_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEX_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
};

VS_TEX_OUT VS_Tex(VS_TEX_IN input)
{
	VS_TEX_OUT output = (VS_TEX_OUT)0;

	output.pos = mul(float4(input.pos, 1.f), g_matWVP);
	output.uv = input.uv;

	return output;
}

float4 PS_Tex(VS_TEX_OUT input) : SV_Target
{
	float4 color = float4(1.f, 1.f, 1.f, 1.f);
	if (g_tex_on_0)
		color = g_tex_0.Sample(g_sam_0, input.uv);

	return color;
}

#endif