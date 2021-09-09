
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

struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float4 color : COLOR;
};

// VS = Vertex Shader
VS_OUT VS_Main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	
	output.pos = float4(input.pos, 1.f);
	output.pos += offset0;
	output.color = input.color;
	output.color += offset1;

	return output;
}

// PS = Pixel Shader
float4 PS_Main(VS_OUT input) : SV_Target
{
	return input.color;
}