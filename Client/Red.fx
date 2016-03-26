#define MAX_LIGHTS			4 
#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES

struct MATERIAL
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular; //a = power
	float4 m_cEmissive;
};

struct LIGHT
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular;
	float3 m_vPosition;
	float m_fRange;
	float3 m_vDirection;
	float m_nType;
	float3 m_vAttenuation;
	float m_fFalloff;
	float m_fTheta;	 //cos(m_fTheta)
	float m_fPhi;	 //cos(m_fPhi)
	float m_bEnable;
	float padding;
};

struct LIGHTEDCOLOR
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular;
};

// 텍스처와 조명을 사용하는 경우의, 정점쉐이더 입력-출력을 위한 구조체
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
};
struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
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

cbuffer cbLight : register(b0)
{
	LIGHT gLights[MAX_LIGHTS];
	float4 gcLightGlobalAmbient;
	float4 gvCameraPosition;
};

cbuffer cbMaterial : register(b1)
{
	MATERIAL gMaterial;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.position = mul(float4(input.position, 1.0f), gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);

	output.positionW = mul(input.position, (float3x3)gmtxWorld);
	output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);

	output.normalW = mul(input.normal, (float3x3)gmtxWorld);

	output.tex2dcoord = input.tex2dcoord;

	return output;
}

Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 PS(VS_OUTPUT input) : SV_Target
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
