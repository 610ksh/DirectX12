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

// PS = Pixel Shader
float4 PS_Main(VS_OUT input) : SV_Target
{
	// 흰색 고정값
	float4 color = float4(1.f, 1.f, 1.f, 1.f);
	// 텍스처 0번이 존재한다면
	if (g_tex_on_0)
		// 기존 색깔을 텍스처 색상으로 바꿔줌.
		color = g_tex_0.Sample(g_sam_0, input.uv);
	
	// 처음에 설정한 디폴트 Normal 값 초기화
	float3 viewNormal = input.viewNormal;

	// 만약 노말맵이 존재한다면(1번)
	if (g_tex_on_1)
	{
		// 노말맵에서 샘플러를 통해 u,v를 추출
		// [0,255] 범위에서 [0,1]로 변환됨(레스터라이저가 해줬음)
		float3 tangentSpaceNormal = g_tex_1.Sample(g_sam_0, input.uv).xyz;
		// [0,1] 범위에서 [-1,1]로 변환
		tangentSpaceNormal = (tangentSpaceNormal - 0.5f) * 2.f;
		// 변환행렬 만들기. TS -> VS로 바꾸는 행렬임. (viewT, viewB, viewN)을 넣음. float3
		float3x3 matTBN = { input.viewTangent, input.viewBinormal, input.viewNormal };
		// tangentSpaceNormal에 TBN 행렬을 곱하면 view좌표계 기준의 Normal값을 구할 수 있게됨.
		viewNormal = normalize(mul(tangentSpaceNormal, matTBN));
	}

	// tex_0는 위에서 t0 레지스터 변수를 의미함.
	// t0에 있는 (u,v)를 어떤 정책으로(Sampler) 색상을 결정할지 설정해줌.
	// 정책은 이미 sam_0 변수에 들어가 있는 상태.
	// Sample이라는 함수를 통해 들어온 uv 값을 변경시키는거임.
	//float4 color = g_tex_0.Sample(g_sam_0, input.uv);
	
	// 현재 물체의 색상을 흰색으로 고정시킴(RGBA)
	//float4 color = float4(1.f, 1.f, 1.f, 1.f);
	
	// LightColor 변수 0으로 초기화 (diffuse, ambient, specular)
	LightColor totalColor = (LightColor)0.f;

	// 설치한 빛의 개수만큼 돌면서 처리
	for (int i = 0; i < g_lightCount; ++i)
	{
		// utils.hlsli에 있는 함수 사용. diffuse, ambient, specular를 계산해줌.
		LightColor color = CalculateLightColor(i, viewNormal, input.viewPos);
		totalColor.diffuse += color.diffuse;
		totalColor.ambient += color.ambient;
		totalColor.specular += color.specular;
	}

	// 칼라 최종 결정
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