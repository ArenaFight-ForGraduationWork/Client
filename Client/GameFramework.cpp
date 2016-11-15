#include "stdafx.h"
#include "GameFramework.h"





//
//	Game Framework
//
CGameFramework::CGameFramework()
{
	m_pd3dRenderTargetView = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pSceneManager = nullptr;
	_tcscpy_s(m_pszBuffer, _T("LapProject ("));

	m_pd3dDepthStencilBuffer = NULL;
	m_pd3dDepthStencilView = NULL;
}

CGameFramework::~CGameFramework()
{
}

//���� �Լ��� ���� ���α׷��� ����Ǹ� ȣ��ȴٴ� �Ϳ� �����϶�. 
bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	gpCommonState = CCommonState::GetSingleton();
	gpCommonState->Initialize(hMainWnd);

	// Direct3D ����̽�, ����̽� ���ؽ�Ʈ, ���� ü�� ���� �����ϴ� �Լ��� ȣ���Ѵ�
	if (!CreateDirect3DDisplay()) return false;

	// ������Ʈ �Ŵ����� �ʱ�ȭ�Ѵ�
	m_pObjectManager = CObjectManager::GetSingleton();
	m_pObjectManager->Initialize();

	// ī�޶� �Ŵ����� �ʱ�ȭ�Ѵ�
	m_pCameraManager = CCameraManager::GetSingleton();
	m_pCameraManager->Initialize();

	// �� �Ŵ����� �ʱ�ȭ�Ѵ�
	m_pSceneManager = CSceneManager::GetSingleton();
	m_pSceneManager->Initialize();
	m_pSceneManager->Change(CSceneManager::eSceneType::INTRO);

	return true;
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = gpCommonState->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return false;
	if (FAILED(hResult = gpCommonState->m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return false;
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;
	ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dDepthStencilBufferDesc.Width = m_nWndClientWidth;
	d3dDepthStencilBufferDesc.Height = m_nWndClientHeight;
	d3dDepthStencilBufferDesc.MipLevels = 1;
	d3dDepthStencilBufferDesc.ArraySize = 1;
	d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
	d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
	d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dDepthStencilBufferDesc.CPUAccessFlags = 0;
	d3dDepthStencilBufferDesc.MiscFlags = 0;
	if (FAILED(hResult = gpCommonState->m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return false;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = gpCommonState->m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return false;

	gpCommonState->m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return true;
}


bool CGameFramework::CreateRenderTargetView()
{
	HRESULT hResult = S_OK;

	//���� ü���� �ĸ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = gpCommonState->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return false;
	if (FAILED(hResult = gpCommonState->m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return false;
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	//���� Ÿ�� �並 �����ϰ� ���-���� �ܰ迡 �����Ѵ�.
	gpCommonState->m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, NULL);

	return true;
}

bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	// ���� Ÿ�� �並 �����ϴ� �Լ��� ȣ��
	if (!CreateRenderTargetDepthStencilView()) return false;

	return true;
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
	{
		m_nWndClientWidth = LOWORD(lParam);
		m_nWndClientHeight = HIWORD(lParam);

		gpCommonState->m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		gpCommonState->GetSwapChain()->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		CreateRenderTargetDepthStencilView();

		m_pCameraManager->GetNowCamera()->SetViewport(0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);

		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		m_pSceneManager->GetNowScene()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		m_pSceneManager->GetNowScene()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

// ���� �Լ��� ���� ���α׷��� ����� �� ȣ��ȴٴ� �Ϳ� �����϶�. 
void CGameFramework::OnDestroy()
{
	m_pObjectManager->DeleteObjectAll();

	if (gpCommonState->m_pd3dDeviceContext) gpCommonState->m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (gpCommonState->GetDepthStencilDefault()) gpCommonState->GetDepthStencilDefault()->Release();
	if (gpCommonState->GetDepthStencilDisable()) gpCommonState->GetDepthStencilDisable()->Release();
	if (gpCommonState->GetSwapChain()) gpCommonState->GetSwapChain()->Release();
	if (gpCommonState->m_pd3dDeviceContext) gpCommonState->m_pd3dDeviceContext->Release();
	if (gpCommonState->m_pd3dDevice) gpCommonState->m_pd3dDevice->Release();
}

void CGameFramework::FrameAdvance()
{
	gpCommonState->m_pTimer->Tick(60.0f);

	float fClearColor[4] = { COLORRGB(250), COLORRGB(250), COLORRGB(250), 1.0f };
	if (m_pd3dRenderTargetView) gpCommonState->m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) gpCommonState->m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (m_pCameraManager)
		m_pCameraManager->GetNowCamera()->UpdateShaderVariables();

	if (m_pSceneManager)
	{
		m_pSceneManager->GetNowScene()->ProcessInput();
		m_pSceneManager->GetNowScene()->AnimateObjectsAndRender();
	}

	gpCommonState->GetSwapChain()->Present(0, 0);

	gpCommonState->m_pTimer->GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}



