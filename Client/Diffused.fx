struct VS_DIFFUSED_INPUT
{
	float3 position: POSITION;
	float4 color: COLOR0;
};
struct VS_DIFFUSED_OUTPUT
{
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

// ī�޶� ��ȯ ��İ� ���� ��ȯ ����� ���� ���̴� ����(���� 0�� ���)
cbuffer cbViewProjectionMatrix : register(b0)
{
	matrix gmtxView: packoffset(c0);
	matrix gmtxProjection: packoffset(c4);
};
// ���� ��ȯ ����� ���� ���̴� ������ �����Ѵ�(���� 1�� ���)
cbuffer cbWorldMatrix : register(b1)
{
	matrix gmtxWorld : packoffset(c0);
};




/* ������ ������� �ʴ� ���� ���̴� */
VS_DIFFUSED_OUTPUT VS_Diffused(VS_DIFFUSED_INPUT input)
{
	VS_DIFFUSED_OUTPUT output = (VS_DIFFUSED_OUTPUT)0;

	output.position = mul(float4(input.position, 1.0f), gmtxWorld);			/* ���庯ȯ */
	output.position = mul(output.position, gmtxView);				/* ī�޶�ȯ(�亯ȯ) */
	output.position = mul(output.position, gmtxProjection);		/* ������ȯ */

	output.color = input.color;

	return output;
}

/* ������ ������ ������� �ʴ� �ȼ� ���̴� �Լ� */
float4 PS_Diffused(VS_DIFFUSED_OUTPUT input) : SV_Target
{
	return input.color;
}