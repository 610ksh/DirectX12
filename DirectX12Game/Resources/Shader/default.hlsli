/////////////////////////////////////////////

// for Transform

// 4바이트 위치 값을 담당
cbuffer TEST_B0 : register(b0)
{
	// x,y,z 3개와 1개더 4개의 float
	float4 offset0;
};

/////////////////////////////////////////////

// for Material

// MaterialParams, _params형태로 통째로 넘어감
cbuffer MATERIAL_PARAMS : register(b1)
{
	int int_0;
	int int_1;
	int int_2;
	int int_3;
	int int_4;
	float float_0; // 사용
	float float_1; // 사용
	float float_2; // 사용
	float float_3;
	float float_4;
};

// Texture
Texture2D tex_0 : register(t0); // 사용
Texture2D tex_1 : register(t1);
Texture2D tex_2 : register(t2);
Texture2D tex_3 : register(t3);
Texture2D tex_4 : register(t4);

/////////////////////////////////////////////

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

/////////////////////////////////////////////

// Main함수

// VS = Vertex Shader
VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	/*output.pos = float4(input.pos, 1.f);
	output.pos += offset0;
	output.color = input.color;
	output.color += offset1;*/

	// 일단 그냥 있는 그대로 넘김
	output.pos = float4(input.pos, 1.f);

	// b1 레지스터에 있는 float 변수를 이용해서 값을 바꿈!
	output.pos.x += float_0;
	output.pos.y += float_1;
	output.pos.z += float_2;

	// b0 레지스터로 위치값 바꿈. 키보드 입력
	output.pos += offset0; 

	output.color = input.color;
	output.uv = input.uv;

	return output;
}

// PS = Pixel Shader
float4 PS_Main(VS_OUT input) : SV_Target
{
	// tex_0는 위에서 t0 레지스터 변수를 의미함.
	// t0에 있는 (u,v)를 어떤 정책으로(Sampler) 색상을 결정할지 설정해줌.
	// 정책은 이미 sam_0 변수에 들어가 있는 상태.
	// Sample이라는 함수를 통해 들어온 uv 값을 변경시키는거임.
	float4 color = tex_0.Sample(sam_0, input.uv);
	return color;
}