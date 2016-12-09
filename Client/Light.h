#pragma once

#include "ConstantBuffers.h"
#include <DirectXMath.h>
using namespace DirectX;

#define POINT_LIGHT			1.0f
#define SPOT_LIGHT			2.0f
#define DIRECTIONAL_LIGHT	3.0f

/*
	���� ������ �ִ� 16��.
	���� ���� ����� MAX_LIGHTS�� ���� ��,
	��� ���̴� ���ϵ��� MAX_LIGHTS�� �����ϰ� �ٲ���� �Ѵ�.
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


