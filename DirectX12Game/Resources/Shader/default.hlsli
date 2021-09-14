
// 4바이트 위치 값을 담당
cbuffer TEST_B0 : register(b0)
{
	float4 offset0;
};

// 4바이트 색상 쪽을 담당
cbuffer TEST_B1 : register(b1)
{
	float4 offset1;
};

// t0 레지스터를 이용하겠다고 선언.
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

	// 일단 그냥 있는 그대로 넘김
	output.pos = float4(input.pos, 1.f);
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