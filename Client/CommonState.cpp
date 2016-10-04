#include "stdafx.h"
#include "CommonState.h"


CCommonState *gpCommonState;

CCommonState::CCommonState()
{
	m_pd3dDevice = nullptr;
	m_pd3dDeviceContext = nullptr;

	m_pd3dDepthStencilDefault = nullptr;
	m_pd3dDepthStencilDisable = nullptr;

	m_pTimer = new CGameTimer();
}
CCommonState::~CCommonState()
{
	//Destroy();
}

//void CCommonState::Destroy()
//{
//	if (m_pd3dDeviceContext)
//	{
//		m_pd3dDeviceContext->ClearState();
//		m_pd3dDeviceContext->Release();
//	}
//	if (m_pd3dDevice) m_pd3dDevice->Release();
//}

void CCommonState::TurnZBufferOn()
{
	m_pd3dDeviceContext->OMSetDepthStencilState(gpCommonState->m_pd3dDepthStencilDefault, 1);
	return;
}

void CCommonState::TurnZBufferOff()
{
	m_pd3dDeviceContext->OMSetDepthStencilState(gpCommonState->m_pd3dDepthStencilDisable, 1);
	return;
}


