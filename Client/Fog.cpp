#include "stdafx.h"
#include "Fog.h"


CFog::CFog()
{
	m_eState = eState::DISABLE;

	m_pd3dxvCenter = new D3DXVECTOR3(0, 0, 0);

	m_fNowRange = 0;
	m_fMinRange = 100;
	m_fMaxRange = 1000;

	m_pd3dcbFog = nullptr;
}
CFog::~CFog()
{
}

void CFog::Initialize(ID3D11Device *pd3dDevice)
{
	this->Destroy();

	D3D11_BUFFER_DESC d3dBufferDesc;
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	ID3D11DeviceContext *pDeviceContext;
	pd3dDevice->GetImmediateContext(&pDeviceContext);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_FOG);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbFog);

	pDeviceContext->Map(m_pd3dcbFog, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	{
		VS_CB_FOG *pcbFog = (VS_CB_FOG *)d3dMappedResource.pData;
		pcbFog->m_fRange = -1;		// if m_fRange < 0, fog is Enable.
	}
	pDeviceContext->Unmap(m_pd3dcbFog, 0);
	pDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG, 1, &m_pd3dcbFog);
}

void CFog::Update(ID3D11DeviceContext *pd3dDeviceContext)
{
	switch (m_eState)
	{
	case eState::EXPAND:
	{
		if (m_fNowRange < m_fMaxRange)
			m_fNowRange += 20;
		else
		{
			m_fNowRange = m_fMaxRange;
			m_eState = eState::KEEP;
		}
	} break;
	case eState::CONTRACT:
	{
		if (m_fNowRange > m_fMinRange)
			m_fNowRange -= 20;
		else
		{
			m_fNowRange = -1;
			m_eState = eState::DISABLE;
		}
	} break;
	default:
		break;
	}

	UpdateShaderVariables(pd3dDeviceContext);
}
void CFog::Expand(D3DXVECTOR3 *pd3dvCenter)
{
	if (eState::DISABLE == m_eState)
	{
		m_eState = eState::EXPAND;
		m_pd3dxvCenter = pd3dvCenter;
		m_fNowRange = m_fMinRange;
	}
}
void CFog::Contract()
{
	if (eState::KEEP == m_eState)
	{
		m_eState = eState::CONTRACT;
		m_fNowRange = m_fMaxRange;
	}
}

void CFog::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(m_pd3dcbFog, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	{
		VS_CB_FOG *pcbFog = (VS_CB_FOG *)d3dMappedResource.pData;
		pcbFog->m_d3dxvCenter = *m_pd3dxvCenter;
		pcbFog->m_fRange = m_fNowRange;
	}
	pd3dDeviceContext->Unmap(m_pd3dcbFog, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG, 1, &m_pd3dcbFog);
}

void CFog::Destroy()
{
	if (m_pd3dcbFog)
		m_pd3dcbFog->Release();
}

bool CFog::IsInUse()
{
	if (eState::DISABLE == m_eState)
		return false;
	else
		return true;
}