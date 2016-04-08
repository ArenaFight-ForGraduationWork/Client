#include "stdafx.h"
#include "Fog.h"


CFog::CFog()
{
	m_pd3dcbFog = nullptr;
}
CFog::~CFog()
{
}

void CFog::Initialize(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(VS_CB_FOG);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &m_pd3dcbFog);

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	ID3D11DeviceContext *pDeviceContext;
	pd3dDevice->GetImmediateContext(&pDeviceContext);
	pDeviceContext->Map(m_pd3dcbFog, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_FOG *pcbFog = (VS_CB_FOG *)d3dMappedResource.pData;
	pcbFog->fogStart = 300;
	pcbFog->fogEnd = 500;
	pDeviceContext->Unmap(m_pd3dcbFog, 0);
	pDeviceContext->VSSetConstantBuffers(VS_SLOT_FOG, 1, &m_pd3dcbFog);
}

void CFog::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbFog, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_FOG *pcbFog = (VS_CB_FOG *)d3dMappedResource.pData;
	pcbFog->fogStart = 300;
	pcbFog->fogEnd = 500;
	//memcpy(pcbFog, pMaterial->GetMaterial(), sizeof(VS_CB_FOG));
	pd3dDeviceContext->Unmap(m_pd3dcbFog, 0);
	pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_MATERIAL, 1, &m_pd3dcbFog);
}
