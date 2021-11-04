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

// Main함수

// VS = Vertex Shader
VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	/// 1. 넘어온 값을 b0 레지스터의 WVP 행렬과 연산해준다.
	output.pos = mul(float4(input.pos, 1.f), g_matWVP); // 투영좌표계로 넘어감
	output.uv = input.uv;

	/// 2. 빛 처리를 위한 viewPos, viewNormal. 정점 단위로 계산하고 있음.
	// 투영좌표계 P로 가지 않고 WV까지만 곱해서 View에서의 Pos를 구함. 빛 계산을 위해.
	output.viewPos = mul(float4(input.pos, 1.f), g_matWV).xyz; // pos는 마지막을 1로 셋팅해야함.
	
	// 마찬가지로 Normal도 똑같이 View까지만 구함. 빛 계산을 위해.
	// 방향벡터는 마지막에 0을 넣어줘야함. 그래야 translation이 적용 안됨.
	output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz); 

	// tangent값이 처음에 넘어올때는 Local 좌표계라서 WV로 곱해주면 viwe 좌표계로 바꿀수 있다.
	output.viewTangent = normalize(mul(float4(input.tangent, 0.f), g_matWV).xyz);
	// tangent와 normal값을 외적해주면 binormal이 된다.
	// 왼손좌표계이다. 순서 주의하자. tangent가 먼저다. (T, B, N). normal을 먼저해주면 반대방향으로 나온다
	output.viewBinormal = normalize(cross(output.viewTangent, output.viewNormal));

	/// 3. 이 다음으로 레스터라이저가 이 값들을 받아서 평균값으로 중간 픽셀들을 보간해서 셋팅해줌. 그걸 PS가 받음
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
		// [0,255] 범위에서 [0,1]로 변환
		float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
		// [0,1] 범위에서 [-1,1]로 변환
		tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
		float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
		viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
	}

	// 중간 저장물이었던 값들을 결과물에 저장함
	output.position = float4(input.viewPos.xyz, 0.f);
	output.normal = float4(viewNormal.xyz, 0.f);
	output.color = color;

	return output;
}

#endif