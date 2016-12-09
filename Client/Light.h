#pragma once

#include "ConstantBuffers.h"
#include <DirectXMath.h>
using namespace DirectX;

#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f

/*
	조명 개수는 최대 16개.
	조명 개수 변경시 MAX_LIGHTS를 변경 후,
	모든 셰이더 파일들의 MAX_LIGHTS도 동일하게 바꿔줘야 한다.
*/





//
//	Light
//
class CLight
{
public:
	CLight();
	~CLight();

	void BuildLights();
	void UpdateLights();
	void ReleaseLights();

private:
	PS_CB_LIGHT *m_pLights;
	ID3D11Buffer *m_pd3dcbLights;
};


