////////////////////////////////////////////////////////
//													  //
//					Vertex Buffer					  //
//													  //
////////////////////////////////////////////////////////

#define MAX_MATRIX 128

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
	uint4 Bones1 : BONES0;
	uint4 Bones2 : BONES1;
	float4 Weights1 : WEIGHTS0;
	float4 Weights2 : WEIGHTS1;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
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
cbuffer cbBoneWorldMatrix : register(b2)
{
	row_major matrix gmtxBone[MAX_MATRIX] : packoffset(c0);
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	float4 Pos;
	Pos = float4(input.position, 1);

	uint iBone0 = input.Bones1.r;
	uint iBone1 = input.Bones1.g;
	uint iBone2 = input.Bones1.b;
	uint iBone3 = input.Bones1.a;
	uint iBone4 = input.Bones2.r;
	uint iBone5 = input.Bones2.g;
	uint iBone6 = input.Bones2.b;
	uint iBone7 = input.Bones2.a;

	float fWeight0 = input.Weights1.r;
	float fWeight1 = input.Weights1.g;
	float fWeight2 = input.Weights1.b;
	float fWeight3 = input.Weights1.a;
	float fWeight4 = input.Weights2.r;
	float fWeight5 = input.Weights2.g;
	float fWeight6 = input.Weights2.b;
	float fWeight7 = input.Weights2.a;

	matrix m0 = gmtxBone[iBone0];
	matrix m1 = gmtxBone[iBone1];
	matrix m2 = gmtxBone[iBone2];
	matrix m3 = gmtxBone[iBone3];
	matrix m4 = gmtxBone[iBone4];
	matrix m5 = gmtxBone[iBone5];
	matrix m6 = gmtxBone[iBone6];
	matrix m7 = gmtxBone[iBone7];

	if (fWeight0 > 0) output.position += fWeight0 * mul(Pos, m0);
	if (fWeight1 > 0) output.position += fWeight1 * mul(Pos, m1);
	if (fWeight2 > 0) output.position += fWeight2 * mul(Pos, m2);
	if (fWeight3 > 0) output.position += fWeight3 * mul(Pos, m3);
	if (fWeight4 > 0) output.position += fWeight4 * mul(Pos, m4);
	if (fWeight5 > 0) output.position += fWeight5 * mul(Pos, m5);
	if (fWeight6 > 0) output.position += fWeight6 * mul(Pos, m6);
	if (fWeight7 > 0) output.position += fWeight7 * mul(Pos, m7);

	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(Pos, (float3x3)gmtxWorld);
	output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);

	matrix mtxWorldViewProjection = mul(gmtxWorld, gmtxView);
	mtxWorldViewProjection = mul(mtxWorldViewProjection, gmtxProjection);
	output.position = mul(output.position, mtxWorldViewProjection);

	output.tex2dcoord = input.tex2dcoord;

	return output;
}





////////////////////////////////////////////////////////
//													  //
//					Pixel Buffer					  //
//													  //
////////////////////////////////////////////////////////

#define MAX_LIGHTS		4 
#define POINT_LIGHT		1.0f
#define SPOT_LIGHT		2.0f
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
	float m_fTheta; //cos(m_fTheta)
	float m_fPhi; //cos(m_fPhi)
	float m_bEnable;
	float padding;
};
struct LIGHTEDCOLOR
{
	float4 m_cAmbient;
	float4 m_cDiffuse;
	float4 m_cSpecular;
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
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

/*���⼺ ������ ȿ���� ����ϴ� �Լ��̴�.
���⼺ ������ ��������� �Ÿ��� ���� ������ ���� ������ �ʴ´�.*/
LIGHTEDCOLOR DirectionalLight(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = -gLights[i].m_vDirection;
		float fDiffuseFactor = dot(vToLight, vNormal);
	//������ ������ ���� ���Ϳ� �̷�� ������ ������ �� ���� ������ ������ ����Ѵ�.
	if (fDiffuseFactor > 0.0f)
	{
		//������ ����ŧ�� �Ŀ��� 0�� �ƴ� ���� ����ŧ�� ������ ������ ����Ѵ�.
		if (gMaterial.m_cSpecular.a != 0.0f)
		{
#ifdef _WITH_REFLECT
			float3 vReflect = reflect(-vToLight, vNormal);
				float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
			float3 vHalf = normalize(vToCamera + vToLight);
#else
			float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
				float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
			output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
		}
		output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
	}
	output.m_cAmbient = gMaterial.m_cAmbient * gLights[i].m_cAmbient;
	return(output);
}

//�� ������ ȿ���� ����ϴ� �Լ��̴�.
LIGHTEDCOLOR PointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	//��������� �Ÿ��� ������ ��ȿ�Ÿ����� ���� ���� ������ ������ ����Ѵ�.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//������ ������ ���� ���Ϳ� �̷�� ������ ������ �� ���� ������ ������ ����Ѵ�.
		if (fDiffuseFactor > 0.0f)
		{
			//������ ����ŧ�� �Ŀ��� 0�� �ƴ� ���� ����ŧ�� ������ ������ ����Ѵ�.
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
					float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
		//��������� �Ÿ��� ���� ������ ������ ����Ѵ�.
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;
	}
	return(output);
}

//���� ������ ȿ���� ����ϴ� �Լ��̴�.
LIGHTEDCOLOR SpotLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	//��������� �Ÿ��� ������ ��ȿ�Ÿ����� ���� ���� ������ ������ ����Ѵ�.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//������ ������ ���� ���Ϳ� �̷�� ������ ������ �� ���� ������ ������ ����Ѵ�.
		if (fDiffuseFactor > 0.0f)
		{
			//������ ����ŧ�� �Ŀ��� 0�� �ƴ� ���� ����ŧ�� ������ ������ ����Ѵ�.
			if (gMaterial.m_cSpecular.a != 0.0f)
			{
#ifdef _WITH_REFLECT
				float3 vReflect = reflect(-vToLight, vNormal);
					float fSpecularFactor = pow(max(dot(vReflect, vToCamera), 0.0f), gMaterial.m_cSpecular.a);
#else
#ifdef _WITH_LOCAL_VIEWER_HIGHLIGHTING
				float3 vHalf = normalize(vToCamera + vToLight);
#else
				float3 vHalf = float3(0.0f, 1.0f, 0.0f);
#endif
					float fSpecularFactor = pow(max(dot(vHalf, vNormal), 0.0f), gMaterial.m_cSpecular.a);
#endif
				output.m_cSpecular = gMaterial.m_cSpecular * (gLights[i].m_cSpecular * fSpecularFactor);
			}
			output.m_cDiffuse = gMaterial.m_cDiffuse * (gLights[i].m_cDiffuse * fDiffuseFactor);
		}
#ifdef _WITH_THETA_PHI_CONES
		float fAlpha = max(dot(-vToLight, gLights[i].m_vDirection), 0.0f);
		float fSpotFactor = pow(max(((fAlpha - gLights[i].m_fPhi) / (gLights[i].m_fTheta - gLights[i].m_fPhi)), 0.0f), gLights[i].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), gLights[i].m_fFalloff);
#endif
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor * fSpotFactor);
		output.m_cDiffuse *= fAttenuationFactor * fSpotFactor;
		output.m_cSpecular *= fAttenuationFactor * fSpotFactor;
	}
	return(output);
}

// ������ ������ ����ϱ� ���� �Լ��̴�.
float4 Lighting(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
		float3 vToCamera = normalize(vCameraPosition - vPosition);
		LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (i = 0; i < MAX_LIGHTS; i++)
		{
			//Ȱ��ȭ�� ���� ���Ͽ� ������ ������ ����Ѵ�.
			if (gLights[i].m_bEnable == 1.0f)
			{
				//������ ������ ���� ������ ������ ����Ѵ�.
				if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
				{
					LightedColor = DirectionalLight(i, vNormal, vToCamera);
					cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
				}
				if (gLights[i].m_nType == POINT_LIGHT)
				{
					LightedColor = PointLight(i, vPosition, vNormal, vToCamera);
					cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
				}
				if (gLights[i].m_nType == SPOT_LIGHT)
				{
					LightedColor = SpotLight(i, vPosition, vNormal, vToCamera);
					cColor += (LightedColor.m_cAmbient + LightedColor.m_cDiffuse + LightedColor.m_cSpecular);
				}
			}
		}
	//�۷ι� �ֺ� ������ ������ ���� ���� ���Ѵ�.
	cColor += (gcLightGlobalAmbient * gMaterial.m_cAmbient);
	//���� ������ ���İ��� ������ ��ǻ�� ������ ���İ����� �����Ѵ�.
	cColor.a = gMaterial.m_cDiffuse.a;
	return(cColor);
}

float4 PS(PS_INPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination;
	cIllumination = Lighting(input.positionW, input.normalW);

	float4 cColor;
	cColor = gtxtTexture.Sample(gSamplerState, input.tex2dcoord)* cIllumination;

	return(cColor);
}