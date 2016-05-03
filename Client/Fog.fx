////////////////////////////////////////////////////////
//													  //
//					Vertex Buffer					  //
//													  //
////////////////////////////////////////////////////////

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
};
struct PS_INPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 tex2dcoord : TEXCOORD0;
	float fogFactor : FOG;
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
cbuffer cbFogCenter : register(b2)
{
	float gfFogEnable;
	float3 gf3FogCenter;
}
cbuffer cbFogRange : register(b3)
{
	float gfFogRange;
}

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	output.position = mul(float4(input.position, 1.0f), gmtxWorld);
	output.position = mul(output.position, gmtxView);
	output.position = mul(output.position, gmtxProjection);

	output.positionW = mul(input.position, (float3x3)gmtxWorld);
	output.positionW += float3(gmtxWorld._41, gmtxWorld._42, gmtxWorld._43);

	output.normalW = mul(input.normal, (float3x3)gmtxWorld);

	output.tex2dcoord = input.tex2dcoord;

	if (gfFogEnable)
	{
		float4 inputVertex;
		inputVertex = mul(float4(input.position, 1.0f), gmtxWorld);
		float distance;
		distance = sqrt(((gf3FogCenter.x - inputVertex.x)*(gf3FogCenter.x - inputVertex.x))
			+ ((gf3FogCenter.z - inputVertex.z)*(gf3FogCenter.z - inputVertex.z))
			);
		output.fogFactor = saturate(distance / gfFogRange);
	}
	else
		output.fogFactor = -1.0f;

	return output;
}




////////////////////////////////////////////////////////
//													  //
//					Pixel Buffer					  //
//													  //
////////////////////////////////////////////////////////


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


/*방향성 조명의 효과를 계산하는 함수이다.
방향성 조명은 조명까지의 거리에 따라 조명의 양이 변하지 않는다.*/
LIGHTEDCOLOR DirectionalLight(int i, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = -gLights[i].m_vDirection;
		float fDiffuseFactor = dot(vToLight, vNormal);
	//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
	if (fDiffuseFactor > 0.0f)
	{
		//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
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
	return output;
}

//점 조명의 효과를 계산하는 함수이다.
LIGHTEDCOLOR PointLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;

	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	//조명까지의 거리가 조명의 유효거리보다 작을 때만 조명의 영향을 계산한다.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
		if (fDiffuseFactor > 0.0f)
		{
			//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
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
		//조명까지의 거리에 따라 조명의 영향을 계산한다.
		float fAttenuationFactor = 1.0f / dot(gLights[i].m_vAttenuation, float3(1.0f, fDistance, fDistance*fDistance));
		output.m_cAmbient = gMaterial.m_cAmbient * (gLights[i].m_cAmbient * fAttenuationFactor);
		output.m_cDiffuse *= fAttenuationFactor;
		output.m_cSpecular *= fAttenuationFactor;
	}
	return output;
}

//스팟 조명의 효과를 계산하는 함수이다.
LIGHTEDCOLOR SpotLight(int i, float3 vPosition, float3 vNormal, float3 vToCamera)
{
	LIGHTEDCOLOR output = (LIGHTEDCOLOR)0;
	float3 vToLight = gLights[i].m_vPosition - vPosition;
		float fDistance = length(vToLight);
	//조명까지의 거리가 조명의 유효거리보다 작을 때만 조명의 영향을 계산한다.
	if (fDistance <= gLights[i].m_fRange)
	{
		vToLight /= fDistance;
		float fDiffuseFactor = dot(vToLight, vNormal);
		//조명의 방향이 법선 벡터와 이루는 각도가 예각일 때 직접 조명의 영향을 계산한다.
		if (fDiffuseFactor > 0.0f)
		{
			//물질의 스펙큘러 파워가 0이 아닐 때만 스펙큘러 조명의 영향을 계산한다.
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
	return output;
}

// 조명의 영향을 계산하기 위한 함수이다.
float4 Lighting(float3 vPosition, float3 vNormal)
{
	int i;
	float3 vCameraPosition = float3(gvCameraPosition.x, gvCameraPosition.y, gvCameraPosition.z);
		float3 vToCamera = normalize(vCameraPosition - vPosition);
		LIGHTEDCOLOR LightedColor = (LIGHTEDCOLOR)0;
	float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		for (i = 0; i < MAX_LIGHTS; i++)
		{
			//활성화된 조명에 대하여 조명의 영향을 계산한다.
			if (gLights[i].m_bEnable == 1.0f)
			{
				//조명의 유형에 따라 조명의 영향을 계산한다.
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
	//글로벌 주변 조명의 영향을 최종 색상에 더한다.
	cColor += (gcLightGlobalAmbient * gMaterial.m_cAmbient);
	//최종 색상의 알파값은 물질의 디퓨즈 색상의 알파값으로 설정한다.
	cColor.a = gMaterial.m_cDiffuse.a;
	return cColor;
}

Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

float4 PS(PS_INPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination;
	cIllumination = Lighting(input.positionW, input.normalW);

	float4 cColor;
	cColor = gtxtTexture.Sample(gSamplerState, input.tex2dcoord) * cIllumination;

	if (input.fogFactor >= 0)
	{
		float4 cfogColor;
		cfogColor = float4(0.2706, 0.1098, 0.6392, 1.0);	// purple = RGB(69, 28, 163)

		cColor = (input.fogFactor * cColor) + (1.0f - input.fogFactor) * cfogColor;
	}

	return cColor;
}

