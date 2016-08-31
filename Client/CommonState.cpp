#include "stdafx.h"
#include "CommonState.h"


CCommonState *gpCommonState;

CCommonState::CCommonState()
{
	m_pd3dDevice = nullptr;
	m_pd3dDeviceContext = nullptr;
}
CCommonState::~CCommonState()
{
	Destroy();
}

void CCommonState::Destroy()
{
	if (m_pd3dDeviceContext)
	{
		m_pd3dDeviceContext->ClearState();
		m_pd3dDeviceContext->Release();
	}
	if (m_pd3dDevice) m_pd3dDevice->Release();
}