struct VS_INPUT
{
	float3 position: POSITION;
	float4 color: COLOR0;
};
struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

// 카메라 변환 행렬과 투영 변환 행렬을 위한 쉐이더 변수(슬롯 0을 사용)
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView: packoffset(c0);
	matrix gmtxProjection: packoffset(c4);
};
// 월드 변환 행렬을 위한 쉐이더 변수를 선언한다(슬롯 1을 사용)
cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};




/* 조명을 사용하지 않는 정점 쉐이더 */
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(float4(input.position, 1.0f), gmtxWorld);	/* 월드변환 */
	output.position = mul(output.position, gmtxView);				/* 카메라변환(뷰변환) */
	output.position = mul(output.position, gmtxProjection);			/* 투영변환 */

	output.color = input.color;

	return output;
}

/* 조명의 영향을 계산하지 않는 픽셀 쉐이더 함수 */
float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color;
}