////////////////////////////////////////////////////////
//													  //
//					Vertex Buffer					  //
//													  //
////////////////////////////////////////////////////////

struct VS_INPUT
{
	float3 position : POSITION;
	float2 tex2dcoord : TEXCOORD0;
};
struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex2dcoord : TEXCOORD0;
};

cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView: packoffset(c0);
	matrix gmtxProjection: packoffset(c4);
};
cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};



PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.position = mul(float4(input.position, 1.0f), gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);

	output.tex2dcoord = input.tex2dcoord;

	return output;
}





////////////////////////////////////////////////////////
//													  //
//					Pixel Buffer					  //
//													  //
////////////////////////////////////////////////////////

Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);



float4 PS(PS_INPUT input) : SV_Target
{
	float4 resultColor;
	resultColor = gtxtTexture.Sample(gSamplerState, input.tex2dcoord);

	//if (0 == resultColor.x)
	//{
	//	if (1 == resultColor.y)
	//	{
	//		if (1 == resultColor.z)
	//		{
	//			resultColor.w = 0;
	//		}
	//	}
	//}
	//if (0.0f == resultColor.r)
	//	resultColor.a = 0.0f;
	//else
	//	resultColor.a = 1.0f;

	resultColor.a = 0.0f;

	return resultColor;
}

