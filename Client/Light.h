#ifndef LIGHT_H_
#define LIGHT_H_

#include "stdafx.h"

#define MAX_LIGHTS			4
#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f

struct LIGHT
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular;
	D3DXVECTOR3 m_d3dxvPosition;
	float m_fRange;
	D3DXVECTOR3 m_d3dxvDirection;
	float m_nType;
	D3DXVECTOR3 m_d3dxvAttenuation;
	float m_fFalloff;
	float m_fTheta; //cos(m_fTheta)
	float m_fPhi;	//cos(m_fPhi)
	float m_bEnable;
	float padding;
};

struct LIGHTS
{
	// 상수 버퍼는 크기가 16의 배수가 되어야 한다. 
	LIGHT m_pLights[MAX_LIGHTS];
	D3DXCOLOR m_d3dxcGlobalAmbient;
	D3DXVECTOR4 m_d3dxvCameraPosition;
};



class CLight
{
public:
	CLight();
	~CLight();

	void BuildLights(ID3D11Device *pd3dDevice);
	void UpdateLights(ID3D11DeviceContext *pd3dDeviceContext);
	void ReleaseLights();

private:
	LIGHTS *m_pLights;
	ID3D11Buffer *m_pd3dcbLights;
};





/* 
	조명 개수는 최대 16개.
	조명 개수 변경시 MAX_LIGHTS를 변경 후, 
	모든 셰이더 파일들의 MAX_LIGHTS도 동일하게 바꿔줘야 한다.
*/
#endif