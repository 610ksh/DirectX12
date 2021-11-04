#ifndef _DEFAULT_HLSLI_
#define _DEFAULT_HLSLI_

#include "params.fx"


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

struct PS_OUT
{
	float4 position : SV_Target0;
	float4 normal : SV_Target1;
	float4 color : SV_Target2;
};

// PS = Pixel Shader
PS_OUT PS_Main(VS_OUT input)
{
	PS_OUT output = (PS_OUT)0;

	float4 color = float4(1.f, 1.f, 1.f, 1.f);
	if (g_tex_on_0)
		color = g_tex_0.Sample(g_sam_0, input.uv);

	float3 viewNormal = input.viewNormal;
	if (g_tex_on_1)
	{
		// [0,255] �������� [0,1]�� ��ȯ
		float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
		// [0,1] �������� [-1,1]�� ��ȯ
		tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
		float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
		viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
	}

	// �߰� ���幰�̾��� ������ ������� ������
	output.position = float4(input.viewPos.xyz, 0.f);
	output.normal = float4(viewNormal.xyz, 0.f);
	output.color = color;

	return output;
}

#endif