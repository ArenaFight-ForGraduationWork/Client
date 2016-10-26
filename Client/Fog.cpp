#include "stdafx.h"
#include "Fog.h"

#include "ConstantBuffers.h"



CFog::CFog()
{
	m_eState = eState::DISABLE;

	m_f3Center = XMFLOAT3(0, 0, 0);

	m_fNowRange = 0;
	m_fMinRange = 100;
	m_fMaxRange = 1000;

	m_pd3dcbFog = nullptr;
}
CFog::~CFog()
{
}

void CFog::Initialize()
{
	this->Destroy();

	D3D11_BUFFER_DESC d3dBufferDesc;
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	gpCommonState->m_pd3dDevice->GetImmediateContext(&gpCommonState->m_pd3dDeviceContext);

	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_FOG);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	gpCommonState->m_pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbFog);

	gpCommonState->m_pd3dDeviceContext->Map(m_pd3dcbFog, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	{
		VS_CB_FOG *pcbFog = (VS_CB_FOG *)d3dMappedResource.pData;
		pcbFog->m_fRange = -1;		// if m_fRange < 0, fog is Enable.
	}
	gpCommonState->m_pd3dDeviceContext->Unmap(m_pd3dcbFog, 0);
	gpCommonState->m_pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG, 1, &m_pd3dcbFog);
}

void CFog::Update()
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

	UpdateShaderVariables();
}
void CFog::Expand(float x, float y, float z)
{
	if (eState::DISABLE == m_eState)
	{
		m_eState = eState::EXPAND;
		m_f3Center = XMFLOAT3(x, y, z);
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

void CFog::UpdateShaderVariables()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	gpCommonState->m_pd3dDeviceContext->Map(m_pd3dcbFog, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	{
		VS_CB_FOG *pcbFog = (VS_CB_FOG *)d3dMappedResource.pData;
		pcbFog->m_fCenter[0] = m_f3Center.x;
		pcbFog->m_fCenter[1] = m_f3Center.y;
		pcbFog->m_fCenter[2] = m_f3Center.z;
		pcbFog->m_fRange = m_fNowRange;
	}
	gpCommonState->m_pd3dDeviceContext->Unmap(m_pd3dcbFog, 0);
	gpCommonState->m_pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG, 1, &m_pd3dcbFog);
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