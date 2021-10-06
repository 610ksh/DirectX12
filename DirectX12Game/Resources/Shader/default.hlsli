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
	output.viewNormal = normalize(mul(float4(input.normal, 0.f), g_matWV).xyz); // 방향벡터는 마지막에 0을 넣어줘야함. 그래야 translation이 적용 안됨.

	/// 3. 이 다음으로 레스터라이저가 이 값들을 받아서 평균값으로 중간 픽셀들을 보간해서 셋팅해줌. 그걸 PS가 받음
	return output;
}

// PS = Pixel Shader
float4 PS_Main(VS_OUT input) : SV_Target
{
	// tex_0는 위에서 t0 레지스터 변수를 의미함.
	// t0에 있는 (u,v)를 어떤 정책으로(Sampler) 색상을 결정할지 설정해줌.
	// 정책은 이미 sam_0 변수에 들어가 있는 상태.
	// Sample이라는 함수를 통해 들어온 uv 값을 변경시키는거임.
	float4 color = tex_0.Sample(g_sam_0, input.uv);
	return color;
}

#endif