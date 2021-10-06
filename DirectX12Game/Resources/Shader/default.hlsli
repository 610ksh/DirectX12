#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "params.hlsli"

/////////////////////////////////////////////

struct VS_IN
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL; // added for Light
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : TEXCOORD;
	float3 viewPos : POSITION; // added for Light
	float3 viewNormal : NORMAL; // added for Light
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
	output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz); // ���⺤�ʹ� �������� 0�� �־������. �׷��� translation�� ���� �ȵ�.

	/// 3. �� �������� �����Ͷ������� �� ������ �޾Ƽ� ��հ����� �߰� �ȼ����� �����ؼ� ��������. �װ� PS�� ����
	return output;
}

// PS = Pixel Shader
float4 PS_Main(VS_OUT input) : SV_Target
{
	// tex_0�� ������ t0 �������� ������ �ǹ���.
	// t0�� �ִ� (u,v)�� � ��å����(Sampler) ������ �������� ��������.
	// ��å�� �̹� sam_0 ������ �� �ִ� ����.
	// Sample�̶�� �Լ��� ���� ���� uv ���� �����Ű�°���.
	float4 color = tex_0.Sample(g_sam_0, input.uv);
	return color;
}

#endif