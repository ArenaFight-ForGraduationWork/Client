#ifndef LIGHT_H_
#define LIGHT_H_

#include "stdafx.h"

#include <DirectXMath.h>
using namespace DirectX;

#define MAX_LIGHTS			4
#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f

struct LIGHT
{
	float m_f4Ambient[4];
	float m_f4Diffuse[4];
	float m_f4Specular[4];
	float m_f3Position[3];
	float m_fRange;
	float m_f3Direction[3];
	float m_nType;
	float m_f3Attenuation[3];
	float m_fFalloff;
	float m_fTheta; // cos(m_fTheta)
	float m_fPhi;	// cos(m_fPhi)
	float m_bEnable;
	float padding;
};

struct LIGHTS
{
	// ��� ���۴� ũ�Ⱑ 16�� ����� �Ǿ�� �Ѵ�. 
	LIGHT m_pLights[MAX_LIGHTS];
	float m_f4GlobalAmbient[4];
	float m_f4CameraPosition[4];
};



class CLight
{
public:
	CLight();
	~CLight();

	void BuildLights();
	void UpdateLights();
	void ReleaseLights();

private:
	LIGHTS *m_pLights;
	ID3D11Buffer *m_pd3dcbLights;
};





/* 
	���� ������ �ִ� 16��.
	���� ���� ����� MAX_LIGHTS�� ���� ��, 
	��� ���̴� ���ϵ��� MAX_LIGHTS�� �����ϰ� �ٲ���� �Ѵ�.
*/
#endif