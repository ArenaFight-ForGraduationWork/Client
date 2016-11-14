#include "stdafx.h"
#include "Light.h"





//
//	Light
//
CLight::CLight()
{
	m_pLights = nullptr;
	m_pd3dcbLights = nullptr;
}
CLight::~CLight()
{
}

void CLight::BuildLights()
{
	m_pLights = new PS_CB_LIGHT();
	ZeroMemory(m_pLights, sizeof(PS_CB_LIGHT));

	// 게임 월드 전체를 비추는 주변조명을 설정한다.
	m_pLights->m_f4GlobalAmbient[0] = 1.0f;
	m_pLights->m_f4GlobalAmbient[1] = 1.0f;
	m_pLights->m_f4GlobalAmbient[2] = 1.0f;
	m_pLights->m_f4GlobalAmbient[3] = 1.0f;

	//3개의 조명(점 광원, 스팟 광원, 방향성 광원)을 설정한다.
	{
		m_pLights->m_pLights[0].m_bEnable = FALSE;
		m_pLights->m_pLights[0].m_nType = POINT_LIGHT;
		m_pLights->m_pLights[0].m_fRange = 40.0f;
		// ambient
		m_pLights->m_pLights[0].m_f4Ambient[0] = 0.8f;
		m_pLights->m_pLights[0].m_f4Ambient[1] = 0.5f;
		m_pLights->m_pLights[0].m_f4Ambient[2] = 0.5f;
		m_pLights->m_pLights[0].m_f4Ambient[3] = 1.0f;
		// diffuse
		m_pLights->m_pLights[0].m_f4Diffuse[0] = 0.3f;
		m_pLights->m_pLights[0].m_f4Diffuse[1] = 0.3f;
		m_pLights->m_pLights[0].m_f4Diffuse[2] = 0.0f;
		m_pLights->m_pLights[0].m_f4Diffuse[3] = 1.0f;
		// specular
		m_pLights->m_pLights[0].m_f4Specular[0] = 0.1f;
		m_pLights->m_pLights[0].m_f4Specular[1] = 0.1f;
		m_pLights->m_pLights[0].m_f4Specular[2] = 0.1f;
		m_pLights->m_pLights[0].m_f4Specular[3] = 0.0f;
		// position
		m_pLights->m_pLights[0].m_f3Position[0] = 0.0f;
		m_pLights->m_pLights[0].m_f3Position[1] = 200.0f;
		m_pLights->m_pLights[0].m_f3Position[2] = 0.0f;
		// direction
		m_pLights->m_pLights[0].m_f3Direction[0] = 0.0f;
		m_pLights->m_pLights[0].m_f3Direction[1] = 0.0f;
		m_pLights->m_pLights[0].m_f3Direction[2] = 0.0f;
		// attenuation
		m_pLights->m_pLights[0].m_f3Attenuation[0] = 1.0f;
		m_pLights->m_pLights[0].m_f3Attenuation[1] = 0.001f;
		m_pLights->m_pLights[0].m_f3Attenuation[2] = 0.0001f;
	}
	{
		m_pLights->m_pLights[1].m_bEnable = FALSE;
		m_pLights->m_pLights[1].m_nType = SPOT_LIGHT;
		m_pLights->m_pLights[1].m_fRange = 100.0f;
		// ambient
		m_pLights->m_pLights[0].m_f4Ambient[0] = 0.1f;
		m_pLights->m_pLights[0].m_f4Ambient[1] = 0.1f;
		m_pLights->m_pLights[0].m_f4Ambient[2] = 0.1f;
		m_pLights->m_pLights[0].m_f4Ambient[3] = 1.0f;
		// diffuse
		m_pLights->m_pLights[0].m_f4Diffuse[0] = 0.3f;
		m_pLights->m_pLights[0].m_f4Diffuse[1] = 0.3f;
		m_pLights->m_pLights[0].m_f4Diffuse[2] = 0.0f;
		m_pLights->m_pLights[0].m_f4Diffuse[3] = 1.0f;
		// specular
		m_pLights->m_pLights[0].m_f4Specular[0] = 0.1f;
		m_pLights->m_pLights[0].m_f4Specular[1] = 0.1f;
		m_pLights->m_pLights[0].m_f4Specular[2] = 0.1f;
		m_pLights->m_pLights[0].m_f4Specular[3] = 0.0f;
		// position
		m_pLights->m_pLights[0].m_f3Position[0] = 0.0f;
		m_pLights->m_pLights[0].m_f3Position[1] = 0.0f;
		m_pLights->m_pLights[0].m_f3Position[2] = 0.0f;
		// direction
		m_pLights->m_pLights[0].m_f3Direction[0] = 0.0f;
		m_pLights->m_pLights[0].m_f3Direction[1] = 0.0f;
		m_pLights->m_pLights[0].m_f3Direction[2] = 1.0f;
		// attenuation
		m_pLights->m_pLights[0].m_f3Attenuation[0] = 1.0f;
		m_pLights->m_pLights[0].m_f3Attenuation[1] = 0.01f;
		m_pLights->m_pLights[0].m_f3Attenuation[2] = 0.0001f;
		m_pLights->m_pLights[1].m_fFalloff = 8.0f;
		m_pLights->m_pLights[1].m_fPhi = cos(XMConvertToRadians(40.0f));
		m_pLights->m_pLights[1].m_fTheta = cos(XMConvertToRadians(20.0f));
	}
	{
		m_pLights->m_pLights[2].m_bEnable = TRUE;
		m_pLights->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
		// ambient
		m_pLights->m_pLights[0].m_f4Ambient[0] = 0.6f;
		m_pLights->m_pLights[0].m_f4Ambient[1] = 1.0f;
		m_pLights->m_pLights[0].m_f4Ambient[2] = 0.6f;
		m_pLights->m_pLights[0].m_f4Ambient[3] = 1.0f;
		// diffuse
		m_pLights->m_pLights[0].m_f4Diffuse[0] = 0.5f;
		m_pLights->m_pLights[0].m_f4Diffuse[1] = 0.5f;
		m_pLights->m_pLights[0].m_f4Diffuse[2] = 0.5f;
		m_pLights->m_pLights[0].m_f4Diffuse[3] = 1.0f;
		// specular
		m_pLights->m_pLights[0].m_f4Specular[0] = 0.0f;
		m_pLights->m_pLights[0].m_f4Specular[1] = 0.0f;
		m_pLights->m_pLights[0].m_f4Specular[2] = 0.0f;
		m_pLights->m_pLights[0].m_f4Specular[3] = 0.0f;
		// direction
		m_pLights->m_pLights[0].m_f3Direction[0] = 0.0f;
		m_pLights->m_pLights[0].m_f3Direction[1] = -1.0f;
		m_pLights->m_pLights[0].m_f3Direction[2] = 0.0f;
	}

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(PS_CB_LIGHT);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = m_pLights;
	gpCommonState->m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);
}

void CLight::ReleaseLights()
{
	if (m_pLights) delete m_pLights;
	if (m_pd3dcbLights) m_pd3dcbLights->Release();
}

void CLight::UpdateLights()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	gpCommonState->m_pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	{
		PS_CB_LIGHT *pcbLight = (PS_CB_LIGHT*)d3dMappedResource.pData;
		memcpy(pcbLight, m_pLights, sizeof(PS_CB_LIGHT));
	}
	gpCommonState->m_pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
	gpCommonState->m_pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_LIGHT, 1, &m_pd3dcbLights);
}