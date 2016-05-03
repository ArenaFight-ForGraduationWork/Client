#include "stdafx.h"
#include "Fog.h"


CFog::CFog()
{
	m_eState = eState::DISABLE;

	m_pd3dxvCenter = new D3DXVECTOR3(0, 0, 0);

	m_fNowRange = 0;

	m_fMinRange = 100;
	m_fMaxRange = 1000;

	m_pd3dcbFogCenter = nullptr;
	m_pd3dcbFogRange = nullptr;
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

	/* VS_CB_FOG_CENTER */
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_FOG_CENTER);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbFogCenter);

	pDeviceContext->Map(m_pd3dcbFogCenter, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	{
		VS_CB_FOG_CENTER *pcbFogCenter = (VS_CB_FOG_CENTER *)d3dMappedResource.pData;
		pcbFogCenter->m_fEnable = false;
		pcbFogCenter->m_d3dxvCenter = *m_pd3dxvCenter;
	}
	pDeviceContext->Unmap(m_pd3dcbFogCenter, 0);
	pDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG_CENTER, 1, &m_pd3dcbFogCenter);

	/* VS_CB_FOG_RANGE */
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_FOG_RANGE);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbFogRange);
}

void CFog::Update(ID3D11Device *pd3dDevice)
{
	if (m_eState != eState::DISABLE)
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
				m_fNowRange = m_fMinRange;
				m_eState = eState::DISABLE;
			}
		} break;
		default:
			break;
		}

		ID3D11DeviceContext *pDeviceContext;
		pd3dDevice->GetImmediateContext(&pDeviceContext);
		if (m_eState != eState::DISABLE)
			UpdateShaderVariables(pDeviceContext, m_fNowRange);
		else
			UpdateShaderVariables(pDeviceContext, D3DXVECTOR3(0, 0, 0));
	}
}
void CFog::Expand(ID3D11Device *pd3dDevice, const D3DXVECTOR3 *pd3dvCenter)
{
	if (eState::DISABLE == m_eState)
	{
		m_eState = eState::EXPAND;
		m_fNowRange = m_fMinRange;

		ID3D11DeviceContext *pDeviceContext;
		pd3dDevice->GetImmediateContext(&pDeviceContext);

		UpdateShaderVariables(pDeviceContext, *pd3dvCenter);
	}
}
void CFog::Contract(ID3D11Device *pd3dDeivce)
{
	if (eState::KEEP == m_eState)
	{
		m_eState = eState::CONTRACT;
		m_fNowRange = m_fMaxRange;
	}
}

//void CFog::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const D3DXVECTOR3 *pd3dxvCenter)
void CFog::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 pd3dxvCenter)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(m_pd3dcbFogCenter, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_FOG_CENTER *pcbFogCenter = (VS_CB_FOG_CENTER *)d3dMappedResource.pData;
	//pcbFogCenter->m_d3dxvCenter = *pd3dxvCenter;
	pcbFogCenter->m_d3dxvCenter = pd3dxvCenter;
	if (m_eState != eState::DISABLE)
		pcbFogCenter->m_fEnable = true;
	else
		pcbFogCenter->m_fEnable = false;
	pd3dDeviceContext->Unmap(m_pd3dcbFogCenter, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG_CENTER, 1, &m_pd3dcbFogCenter);
}

void CFog::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const float fRange)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(m_pd3dcbFogRange, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_FOG_RANGE *pcbFogRange = (VS_CB_FOG_RANGE *)d3dMappedResource.pData;
	pcbFogRange->m_fRange = fRange;
	pd3dDeviceContext->Unmap(m_pd3dcbFogRange, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG_RANGE, 1, &m_pd3dcbFogRange);
}

void CFog::Destroy()
{
	if (m_pd3dcbFogCenter)
		m_pd3dcbFogCenter->Release();
	if (m_pd3dcbFogRange)
		m_pd3dcbFogRange->Release();
}

bool CFog::IsInUse()
{
	if (eState::DISABLE == m_eState)
		return false;
	else
		return true;
}