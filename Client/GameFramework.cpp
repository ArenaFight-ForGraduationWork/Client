#include "stdafx.h"
#include "GameFramework.h"





//
//	Game Framework
//
CGameFramework::CGameFramework()
{
	m_pDXGISwapChain = NULL;
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

//다음 함수는 응용 프로그램이 실행되면 호출된다는 것에 유의하라. 
bool CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	gpCommonState = CCommonState::GetSingleton();

	// Direct3D 디바이스, 디바이스 컨텍스트, 스왑 체인 등을 생성하는 함수를 호출한다
	if (!CreateDirect3DDisplay()) return false;

	// 오브젝트 매니저를 초기화한다
	m_pObjectManager = CObjectManager::GetSingleton();
	m_pObjectManager->Initialize();

	// 카메라 매니저를 초기화한다
	m_pCameraManager = CCameraManager::GetSingleton();
	m_pCameraManager->Initialize();

	// 씬 매니저를 초기화한다
	m_pSceneManager = CSceneManager::GetSingleton();
	m_pSceneManager->Initialize();
	m_pSceneManager->Change(CSceneManager::eSceneType::INTRO);

	return true;
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return false;
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

	// Create first depth stencil state(using depth buffer)
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	if (FAILED(hResult = gpCommonState->m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &gpCommonState->m_pd3dDepthStencilDefault)))	return false;
	gpCommonState->m_pd3dDeviceContext->OMSetDepthStencilState(gpCommonState->m_pd3dDepthStencilDefault, 1);

	// Create second depth stencil state(unusing depth buffer)
	depthStencilDesc.DepthEnable = false;/// <<<	only defference from other depth stencil view
	if (FAILED(hResult = gpCommonState->m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &gpCommonState->m_pd3dDepthStencilDisable)))	return false;

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

	//스왑 체인의 후면버퍼에 대한 렌더 타겟 뷰를 생성한다.
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return false;
	if (FAILED(hResult = gpCommonState->m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return false;
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	//렌더 타겟 뷰를 생성하고 출력-병합 단계에 연결한다.
	gpCommonState->m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, NULL);

	return true;
}

bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	UINT dwCreateDeviceFlags = 0;

	// 디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//디바이스를 생성하기 위하여 시도할 특성 레벨의 순서
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	// 생성할 스왑 체인을 서술하는 구조체
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = m_nWndClientWidth;
	dxgiSwapChainDesc.BufferDesc.Height = m_nWndClientHeight;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

	D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hResult = S_OK;
	// 디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성
	// 고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL,
			dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc,
			&m_pDXGISwapChain, &gpCommonState->m_pd3dDevice, &nd3dFeatureLevel, &gpCommonState->m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !gpCommonState->m_pd3dDevice || !gpCommonState->m_pd3dDeviceContext) return false;

	// 렌더 타겟 뷰를 생성하는 함수를 호출
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

		m_pDXGISwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

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

// 다음 함수는 응용 프로그램이 종료될 때 호출된다는 것에 유의하라. 
void CGameFramework::OnDestroy()
{
	m_pObjectManager->DeleteObjectAll();

	if (gpCommonState->m_pd3dDeviceContext) gpCommonState->m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (gpCommonState->m_pd3dDepthStencilDefault) gpCommonState->m_pd3dDepthStencilDefault->Release();
	if (gpCommonState->m_pd3dDepthStencilDisable) gpCommonState->m_pd3dDepthStencilDisable->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
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

	m_pDXGISwapChain->Present(0, 0);

	gpCommonState->m_pTimer->GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}



