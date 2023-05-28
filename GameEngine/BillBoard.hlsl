Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

//’¸“_î•ñˆÈŠO‚Ìî•ñ
cbuffer global
{
	float4x4 matWVP;
	float4   color;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD;
};

VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD)
{
	VS_OUT outData;
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;

	return outData;
}

float4 PS(VS_OUT inData) : SV_Target
{
	float4 outColor = g_texture.Sample(g_sampler,inData.uv)*color;
	//return float4(1, 1, 1, 1);
	return outColor;
}