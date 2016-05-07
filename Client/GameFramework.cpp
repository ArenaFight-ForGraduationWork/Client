#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	m_pd3dDevice = NULL;
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDeviceContext = NULL;

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

	// Direct3D 디바이스, 디바이스 컨텍스트, 스왑 체인 등을 생성하는 함수를 호출한다
	if (!CreateDirect3DDisplay()) return false;

	// 오브젝트 매니저를 초기화한다
	m_pObjectManager = CObjectManager::GetSingleton();
	m_pObjectManager->Initialize(m_pd3dDevice);

	// 카메라 매니저를 초기화한다
	m_pCameraManager = CCameraManager::GetSingleton();
	m_pCameraManager->Initialize(m_pd3dDevice);

	// 씬 매니저를 초기화한다
	m_pSceneManager = CSceneManager::GetSingleton();
	m_pSceneManager->Initialize();
	m_pSceneManager->Change(CSceneManager::eSceneType::FIRST);
	m_pSceneManager->GetNowScene()->BuildObjects(m_pd3dDevice);

	return true;
}

bool CGameFramework::CreateRenderTargetDepthStencilView()
{
	HRESULT hResult = S_OK;

	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return false;
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return false;
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
	if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return false;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return false;

	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);

	return true;
}


bool CGameFramework::CreateRenderTargetView()
{
	HRESULT hResult = S_OK;

	//스왑 체인의 후면버퍼에 대한 렌더 타겟 뷰를 생성한다.
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return false;
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return false;
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	//렌더 타겟 뷰를 생성하고 출력-병합 단계에 연결한다.
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, NULL);

	return true;
}

bool CGameFramework::CreateDirect3DDisplay()
{
	RECT rcClient;
	::GetClientRect(m_hWnd, &rcClient);
	m_nWndClientWidth = rcClient.right - rcClient.left;
	m_nWndClientHeight = rcClient.bottom - rcClient.top;

	UINT dwCreateDeviceFlags = 0;
#ifdef _DEBUG
	dwCreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서를 나타낸다.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//디바이스를 생성하기 위하여 시도할 특성 레벨의 순서를 나타낸다.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//생성할 스왑 체인을 서술하는 구조체이다.
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
	//디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성한다.
	// 고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성한다.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, 
			dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, 
			&m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return false;

	//렌더 타겟 뷰를 생성하는 함수를 호출한다.
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

		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		m_pDXGISwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

		CreateRenderTargetDepthStencilView();

		m_pCameraManager->GetNowCamera()->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);

		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		m_pSceneManager->GetNowScene()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam, m_GameTimer.GetTimeElapsed());
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		m_pSceneManager->GetNowScene()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam, m_GameTimer.GetTimeElapsed());
		break;
	}
	return(0);
}

//void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	switch (m_OperationMode)
//	{
//	case MODE_MOUSE:
//	{
//		switch (nMessageID)
//		{
//		case WM_MOUSEMOVE:
//			m_ptOldCursorPos.x = m_ptNewCursorPos.x;
//			m_ptOldCursorPos.y = m_ptNewCursorPos.y;
//			m_ptNewCursorPos.x = LOWORD(lParam);
//			m_ptNewCursorPos.y = HIWORD(lParam);
//
//			if (m_ptNewCursorPos.x > m_ptOldCursorPos.x)
//				m_pCamera->RotatebyYaw(-150 * m_GameTimer.GetTimeElapsed());
//			else
//				m_pCamera->RotatebyYaw(150 * m_GameTimer.GetTimeElapsed());
//			break;
//		case WM_MOUSEWHEEL:
//			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
//				m_pCamera->Zoom(200 * m_GameTimer.GetTimeElapsed());
//			else
//				m_pCamera->Zoom(-200 * m_GameTimer.GetTimeElapsed());
//			break;
//		default: break;
//		}
//	}	break;
//	case MODE_KEYBOARD:
//	{
//	}	break;
//	default: break;
//	}
//}

//void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	D3DXVECTOR3 temp;
//	switch (nMessageID)
//	{
//	case WM_KEYUP:
//		switch (wParam)
//		{
//		case VK_F1:
//			m_OperationMode = MODE_MOUSE;
//			break;
//		case VK_F2:
//			m_OperationMode = MODE_KEYBOARD;
//			break;
//
//		case VK_F5:
//			if (m_pObjectManager->FindObject(30000))
//			{
//				m_pObjectManager->DeleteObject(30000);
//				m_pPlayer->SetObject(m_pObjectManager->Insert(30000, eResourceType::MonB, m_pd3dDevice, m_pd3dDeviceContext, 1, 3, D3DXVECTOR3(200, 0, 0), D3DXVECTOR3(0, 0, 0)));
//			}
//			break;
//
//		case VK_F6:
//			if (m_pObjectManager->FindObject(2))
//				m_pObjectManager->DeleteObject(2);
//			else
//				m_pObjectManager->Insert(2, eResourceType::ITME_HP, D3DXVECTOR3(-50, 0, 0), D3DXVECTOR3(0, 0, 0));
//			break;
//		case VK_F3:
//			for (int i = 0; i < m_pObjectManager->FindObjectInCategory(0).size(); ++i)
//			{
//				printf(" 저장번호: %d ID: %d\t %f %f \n",i, m_pObjectManager->FindObjectInCategory(0)[i]->GetId(), m_pObjectManager->FindObjectInCategory(0)[i]->m_MaxVer.x, m_pObjectManager->FindObjectInCategory(0)[i]->m_MaxVer.z);
//			}
//			break;
//		case VK_F4:
//			temp = m_pObjectManager->FindObject(0)->m_MaxVer;
//			printf(" 아이템: %f %f\n", temp.x, temp.z);
//			printf("유저 : %f %f\n", m_pPlayer->GetObjects()->m_MaxVer.x, m_pPlayer->GetObjects()->m_MaxVer.z);
//			break;
//		case VK_F7:
//			m_pPlayer->GetObjects()->SetPosition(&D3DXVECTOR3(-30, 0, 100));
//			//m_pObjectManager->FindObject(2)->SetPosition(&D3DXVECTOR3(0, 0, 300));
//			break;
//		case VK_SPACE:
//			is_Attack = true;
//			Press_SkillNum = 0;
//			break;
//
//		case VK_ESCAPE:
//			::PostQuitMessage(0);
//			break;
//		default:
//			break;
//		}
//		break;
//	default:
//		break;
//	}
//}


//void CGameFramework::ProcessInput()
//{
//	static UCHAR pKeyBuffer[256];
//	DWORD dwDirection = 0;
//
//	switch (m_OperationMode)
//	{
//	case MODE_MOUSE:			// F1
//	{
//
//		if (GetKeyboardState(pKeyBuffer))
//		{
//
//			// 이동
//			if (pKeyBuffer[0x41] & 0xF0) dwDirection |= DIR_LEFT;		// A
//			if (pKeyBuffer[0x44] & 0xF0) dwDirection |= DIR_RIGHT;		// D
//			if (pKeyBuffer[0x57] & 0xF0) dwDirection |= DIR_FORWARD;	// W
//			if (pKeyBuffer[0x53] & 0xF0) dwDirection |= DIR_BACKWARD;	// S
//		
//		}
//		m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::IDLE);
//		/* player state 변경, 방향이 있을경우 달린다.*/
//		if (dwDirection != 0)
//		{
//			m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::RUN);
//		}
//		
//		if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObjectInCategory(0)))		
//		{
//			cout << "나랑 부딪힌 오브젝트 번호 : " << m_pPlayer->GetObjects()->CollOtherID << endl;
//			m_pObjectManager->DeleteObject(m_pPlayer->GetObjects()->CollOtherID);
//			m_pPlayer->SetHP();
//			m_pPlayer->SetSpeed();
//			cout << "현재 체력 : " << m_pPlayer->GetHP() << endl;
//			cout << "현재 스피드 : " << m_pPlayer->GetSpeed() << endl;
//		}
//
//		if (is_Attack)
//		{			
//			m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::ATTACK);		//공격상태라는 걸 알려주고
//			m_pPlayer->GetObjects()->SetPressSkill(Press_SkillNum);										//몇번 스킬 눌렀는지 알려주고
//			
//			//if (m_pPlayer->GetObjects()->MyHitAndEnemyBound(m_pMonster->GetObjects()))		//내 히트박스 + 상대 충돌체크박스 확인
//			//{
//			//	//cout << "적에게 적중했다!!  체력 : "<<m_pObjectManager->FindObject(20000)->GetHP() << endl;
//			//	cout << "적을 때렸다!" << endl;
//			//	is_Coll = true;	//일단 충돌했다는 것만 알려주기 위해서
//			//}
//		
//			if (m_pPlayer->GetObjects()->m_fAnimationPlaytime == 0.0f)	//애니메이션이 한바퀴 돌아서 0이 되면, 공격상태를 멈춘다.
//			{
//				//if (is_Coll) m_pObjectManager->FindObject(20000)->SetHP();	// 그 애니메이션이 한바퀴 돌고, 돌던중에 충돌했다고 바뀐상태가 있으면 그때 피를 깎아줌.
//				//cout << "체력: " << m_pObjectManager->FindObject(20000)->GetHP() << endl;
//				is_Coll = false;			//다음 충돌체크를 위해서 false
//				is_Attack = false;		//한바퀴 돌아서 공격끝났으니, 공격모션을 끝내주기 위해 false
//			}
//		}
//	}
//	break;
//
//	case MODE_KEYBOARD:
//	{
//		if (GetKeyboardState(pKeyBuffer))
//		{
//			// 이동
//			if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
//			if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
//			if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
//			if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
//			// 좌우회전
//			if (pKeyBuffer[0x51] & 0xF0) m_pCamera->RotatebyYaw(100 * m_GameTimer.GetTimeElapsed());		// Q
//			if (pKeyBuffer[0x45] & 0xF0) m_pCamera->RotatebyYaw(-100 * m_GameTimer.GetTimeElapsed());		// E
//			// 줌
//			if (pKeyBuffer[0x5A] & 0xF0) m_pCamera->Zoom(-100 * m_GameTimer.GetTimeElapsed());				// Z
//			if (pKeyBuffer[0x58] & 0xF0) m_pCamera->Zoom(100 * m_GameTimer.GetTimeElapsed());				// X
//		}
//	}
//	break;
//
//	}
//
//	if (dwDirection) m_pPlayer->Move(m_pCamera->GetYaw(), dwDirection, m_GameTimer.GetTimeElapsed());
//
//	// 4) 플레이어 위치에 따라 카메라 update
//	m_pCamera->Update(m_pPlayer->GetPosition());
//	
//	if (m_GameTimer.flow1second()) cout << "tlrks : " << m_GameTimer.GetNowTime() << endl;
//
//}


// 다음 함수는 응용 프로그램이 종료될 때 호출된다는 것에 유의하라. 
void CGameFramework::OnDestroy()
{
	ReleaseObjects();

	m_pObjectManager->DeleteObjectAll();

	if (m_pd3dDeviceContext) m_pd3dDeviceContext->ClearState();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();
	if (m_pDXGISwapChain) m_pDXGISwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
}

void CGameFramework::ReleaseObjects()
{
}

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(60.0f);

	float fClearColor[4] = { COLORRGB(69), COLORRGB(28), COLORRGB(163), 1.0f };
	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	if (m_pCameraManager)
		m_pCameraManager->GetNowCamera()->UpdateShaderVariables(m_pd3dDeviceContext);

	if (m_pSceneManager)
	{
		m_pSceneManager->GetNowScene()->ProcessInput(m_GameTimer.GetTimeElapsed());
		m_pSceneManager->GetNowScene()->AnimateObjectsAndRender(m_pd3dDeviceContext, m_GameTimer.GetTimeElapsed());
	}

	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}
