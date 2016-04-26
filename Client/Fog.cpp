#include "stdafx.h"
#include "Fog.h"


CFog::CFog()
{
	m_pd3dcbFogCenter = nullptr;
	m_pd3dcbFogRange = nullptr;
}
CFog::~CFog()
{
}

void CFog::Initialize(ID3D11Device *pd3dDevice, const D3DXVECTOR3 *pd3dxvCenter, const float fRange)
{
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
	VS_CB_FOG_CENTER *pcbFogCenter = (VS_CB_FOG_CENTER *)d3dMappedResource.pData;
	pcbFogCenter->m_d3dxvCenter = *pd3dxvCenter;
	pDeviceContext->Unmap(m_pd3dcbFogCenter, 0);
	pDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG_CENTER, 1, &m_pd3dcbFogCenter);

	/* VS_CB_FOG_RANGE */
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_FOG_RANGE);
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbFogRange);

	pDeviceContext->Map(m_pd3dcbFogRange, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_FOG_RANGE *pcbFogRange = (VS_CB_FOG_RANGE *)d3dMappedResource.pData;
	pcbFogRange->m_fRange = fRange;
	pDeviceContext->Unmap(m_pd3dcbFogRange, 0);
	pDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG_RANGE, 1, &m_pd3dcbFogRange);
}

//void CFog::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, const D3DXVECTOR3 *pd3dxvCenter)
void CFog::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext, D3DXVECTOR3 pd3dxvCenter)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;

	pd3dDeviceContext->Map(m_pd3dcbFogCenter, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_FOG_CENTER *pcbFogCenter = (VS_CB_FOG_CENTER *)d3dMappedResource.pData;
	//pcbFogCenter->m_d3dxvCenter = *pd3dxvCenter;
	pcbFogCenter->m_d3dxvCenter = pd3dxvCenter;
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
	m_pd3dcbFogCenter->Release();
	m_pd3dcbFogRange->Release();
}
