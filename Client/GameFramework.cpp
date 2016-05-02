#include "stdafx.h"
#include "GameFramework.h"

CGameFramework::CGameFramework()
{
	m_pPlayer = nullptr;
	m_pCamera = nullptr;

	m_OperationMode = MODE_KEYBOARD;

	m_ptOldCursorPos.x = 0;
	m_ptOldCursorPos.y = 0;
	m_ptNewCursorPos.x = 0;
	m_ptNewCursorPos.y = 0;

	m_pd3dDevice = NULL;
	m_pDXGISwapChain = NULL;
	m_pd3dRenderTargetView = NULL;
	m_pd3dDeviceContext = NULL;

	m_nWndClientWidth = FRAME_BUFFER_WIDTH;
	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	m_pScene = NULL;
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

	//Direct3D ����̽�, ����̽� ���ؽ�Ʈ, ���� ü�� ���� �����ϴ� �Լ��� ȣ���Ѵ�. 
	if (!CreateDirect3DDisplay()) return false;

	// ������Ʈ �Ŵ����� �ʱ�ȭ�Ѵ�
	m_pObjectManager = CObjectManager::GetSingleton(m_pd3dDevice);

	//�������� ��ü(���� ���� ��ü)�� �����Ѵ�. 
	BuildObjects();

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

	//���� ü���� �ĸ���ۿ� ���� ���� Ÿ�� �並 �����Ѵ�.
	ID3D11Texture2D *pd3dBackBuffer;
	if (FAILED(hResult = m_pDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return false;
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return false;
	if (pd3dBackBuffer) pd3dBackBuffer->Release();

	//���� Ÿ�� �並 �����ϰ� ���-���� �ܰ迡 �����Ѵ�.
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

	//����̽��� �����ϱ� ���Ͽ� �õ��� ����̹� ������ ������ ��Ÿ����.
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//����̽��� �����ϱ� ���Ͽ� �õ��� Ư�� ������ ������ ��Ÿ����.
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	//������ ���� ü���� �����ϴ� ����ü�̴�.
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
	//����̽��� ����̹� ������ Ư�� ������ �����ϴ� ����̽��� ���� ü���� �����Ѵ�.
	// ������� ����̽� ������ �õ��ϰ� �����ϸ� ���� ������ ����̽��� �����Ѵ�.
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(hResult = D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL, dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext))) break;
	}
	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext) return false;

	//���� Ÿ�� �並 �����ϴ� �Լ��� ȣ���Ѵ�.
	if (!CreateRenderTargetDepthStencilView()) return false;

	//SetViewport();

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

		m_pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, m_nWndClientWidth, m_nWndClientHeight, 0.0f, 1.0f);

		break;
	}
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (m_OperationMode)
	{
	case MODE_MOUSE:
	{
		switch (nMessageID)
		{
		case WM_MOUSEMOVE:
			m_ptOldCursorPos.x = m_ptNewCursorPos.x;
			m_ptOldCursorPos.y = m_ptNewCursorPos.y;
			m_ptNewCursorPos.x = LOWORD(lParam);
			m_ptNewCursorPos.y = HIWORD(lParam);

			if (m_ptNewCursorPos.x > m_ptOldCursorPos.x)
				m_pCamera->RotatebyYaw(-150 * m_GameTimer.GetTimeElapsed());
			else
				m_pCamera->RotatebyYaw(150 * m_GameTimer.GetTimeElapsed());
			break;
		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				m_pCamera->Zoom(200 * m_GameTimer.GetTimeElapsed());
			else
				m_pCamera->Zoom(-200 * m_GameTimer.GetTimeElapsed());
			break;
		default: break;
		}
	}	break;
	case MODE_KEYBOARD:
	{
	}	break;
	default: break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	D3DXVECTOR3 temp;
	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_F1:
			m_OperationMode = MODE_MOUSE;
			break;
		case VK_F2:
			m_OperationMode = MODE_KEYBOARD;
			break;

		case VK_F5:
			if (m_pObjectManager->FindObject(30000))
			{
				m_pObjectManager->DeleteObject(30000);
				m_pPlayer->SetObject(m_pObjectManager->Insert(30000, eResourceType::MonB, m_pd3dDevice, m_pd3dDeviceContext, 1, 3, D3DXVECTOR3(200, 0, 0), D3DXVECTOR3(0, 0, 0)));
			}
			break;

		case VK_F6:
			if (m_pObjectManager->FindObject(2))
				m_pObjectManager->DeleteObject(2);
			else
				m_pObjectManager->Insert(2, eResourceType::ITME_HP, D3DXVECTOR3(-50, 0, 0), D3DXVECTOR3(0, 0, 0));
			break;
		case VK_F3:
			for (int i = 0; i < m_pObjectManager->FindObjectInCategory(0).size(); ++i)
			{
				printf(" �����ȣ: %d ID: %d\t %f %f \n",i, m_pObjectManager->FindObjectInCategory(0)[i]->GetId(), m_pObjectManager->FindObjectInCategory(0)[i]->m_MaxVer.x, m_pObjectManager->FindObjectInCategory(0)[i]->m_MaxVer.z);
			}
			break;
		case VK_F4:
			temp = m_pObjectManager->FindObject(0)->m_MaxVer;
			printf(" ������: %f %f\n", temp.x, temp.z);
			printf("���� : %f %f\n", m_pPlayer->GetObjects()->m_MaxVer.x, m_pPlayer->GetObjects()->m_MaxVer.z);
			break;
		case VK_F7:
			m_pPlayer->GetObjects()->SetPosition(&D3DXVECTOR3(-30, 0, 100));
			//m_pObjectManager->FindObject(2)->SetPosition(&D3DXVECTOR3(0, 0, 300));
			break;
		case VK_SPACE:
			is_Attack = true;
			Press_SkillNum = 0;
			break;

		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}


void CGameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	DWORD dwDirection = 0;

	switch (m_OperationMode)
	{
	case MODE_MOUSE:			// F1
	{

		if (GetKeyboardState(pKeyBuffer))
		{

			// �̵�
			if (pKeyBuffer[0x41] & 0xF0) dwDirection |= DIR_LEFT;		// A
			if (pKeyBuffer[0x44] & 0xF0) dwDirection |= DIR_RIGHT;		// D
			if (pKeyBuffer[0x57] & 0xF0) dwDirection |= DIR_FORWARD;	// W
			if (pKeyBuffer[0x53] & 0xF0) dwDirection |= DIR_BACKWARD;	// S
		
		}
		m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::IDLE);

		/* player state ����, ������ ������� �޸���.*/
		if (dwDirection != 0)
		{
			m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::RUN);
		}
		
		if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObjectInCategory(0)))		
		{
			cout << "���� �ε��� ������Ʈ ��ȣ : " << m_pPlayer->GetObjects()->CollOtherID << endl;
			m_pObjectManager->DeleteObject(m_pPlayer->GetObjects()->CollOtherID);
		}


		if (is_Attack)
		{			
			m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::ATTACK);		//���ݻ��¶�� �� �˷��ְ�
			m_pObjectManager->FindObject(30000)->SetPressSkill(Press_SkillNum);										//��� ��ų �������� �˷��ְ�
			
			if (m_pObjectManager->FindObject(30000)->MyHitAndEnemyBound(m_pMonster->GetObjects()))		//�� ��Ʈ�ڽ� + ��� �浹üũ�ڽ� Ȯ��
			{
				//cout << "������ �����ߴ�!!  ü�� : "<<m_pObjectManager->FindObject(20000)->GetHP() << endl;
				cout << "���� ���ȴ�!" << endl;
				is_Coll = true;	//�ϴ� �浹�ߴٴ� �͸� �˷��ֱ� ���ؼ�
			}
		
			if (m_pObjectManager->FindObject(30000)->m_fAnimationPlaytime == 0.0f)	//�ִϸ��̼��� �ѹ��� ���Ƽ� 0�� �Ǹ�, ���ݻ��¸� �����.
			{
				//if (is_Coll) m_pObjectManager->FindObject(20000)->SetHP();	// �� �ִϸ��̼��� �ѹ��� ����, �����߿� �浹�ߴٰ� �ٲ���°� ������ �׶� �Ǹ� �����.
				//cout << "ü��: " << m_pObjectManager->FindObject(20000)->GetHP() << endl;
				is_Coll = false;			//���� �浹üũ�� ���ؼ� false
				is_Attack = false;		//�ѹ��� ���Ƽ� ���ݳ�������, ���ݸ���� �����ֱ� ���� false
			}
			
		}
	}
	break;

	case MODE_KEYBOARD:
	{
		if (GetKeyboardState(pKeyBuffer))
		{
			// �̵�
			if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
			if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
			if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
			if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
			// �¿�ȸ��
			if (pKeyBuffer[0x51] & 0xF0) m_pCamera->RotatebyYaw(100 * m_GameTimer.GetTimeElapsed());		// Q
			if (pKeyBuffer[0x45] & 0xF0) m_pCamera->RotatebyYaw(-100 * m_GameTimer.GetTimeElapsed());		// E
			// ��
			if (pKeyBuffer[0x5A] & 0xF0) m_pCamera->Zoom(-100 * m_GameTimer.GetTimeElapsed());				// Z
			if (pKeyBuffer[0x58] & 0xF0) m_pCamera->Zoom(100 * m_GameTimer.GetTimeElapsed());				// X
		}
		//m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::IDLE);
		//if (dwDirection != 0)
		//	m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::RUN);
	}
	break;

	}

	if (dwDirection) m_pPlayer->Move(m_pCamera->GetYaw(), dwDirection, m_GameTimer.GetTimeElapsed());
	m_pPlayer->GetObjects()->MoveAndRotatingHitBox();		// ȸ������ ���� ����Ǵ� ��Ʈ�ڽ��� ��������.
	m_pPlayer->GetObjects()->MoveAndRotatingBoundingBox();	//Move�Ҷ� ����� �����Ʈ������ �ʿ��ؼ� �ؿ��ٰ� ����!!

	// 4) �÷��̾� ��ġ�� ���� ī�޶� update
	m_pCamera->Update(m_pPlayer->GetPosition());
	
	if (m_GameTimer.flow1second()) cout << "tlrks : " << m_GameTimer.GetNowTime() << endl;

	//����_��ǥ ������ : �̵��ϰ��� ��ǥ�� ������ �����������ؼ� ���⿡ �־��.
	//const D3DXVECTOR3 *pos = m_pPlayer->GetPosition();
	//cout << "�������� ��ǥ : " << pos->x << "," << pos->y << "," << pos->z << endl;
	//SendPosPacket(pos->x, pos->y, pos->z);
}

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


void CGameFramework::BuildObjects()
{
	m_pScene = new CScene();
	if (m_pScene) m_pScene->BuildObjects(m_pd3dDevice);

	/* �÷��̾� ����*/
	m_pPlayer = new CPlayer();
	m_pPlayer->SetObject(m_pObjectManager->Insert(30000, eResourceType::MonA, m_pd3dDevice, m_pd3dDeviceContext, 0, 3,D3DXVECTOR3(0, 0, -300), D3DXVECTOR3(0, 0, 0)));
	
	/* ���� ���� */
	m_pMonster = new CMonster();
	m_pMonster->SetObject(m_pObjectManager->Insert(20050, eResourceType::MonB, m_pd3dDevice, m_pd3dDeviceContext, 1, 3, D3DXVECTOR3(0, 0, 50), D3DXVECTOR3(0, 0, 0)));
	//m_ppMonster = new CMonster*[2];
	//m_ppMonster[0]->SetObject(m_pObjectManager->Insert(20000, eResourceType::MonB, m_pd3dDevice, m_pd3dDeviceContext, 1, 3, D3DXVECTOR3(0, 0, 50), D3DXVECTOR3(0, 0, 0)));
	//m_ppMonster[1]->SetObject(m_pObjectManager->Insert(20001, eResourceType::MonB, m_pd3dDevice, m_pd3dDeviceContext, 1, 3, D3DXVECTOR3(0, 0, 250), D3DXVECTOR3(0, 0, 0)));

	//m_pObjectManager->Insert(20000, eResourceType::MonB, m_pd3dDevice, m_pd3dDeviceContext, 1, 3, D3DXVECTOR3(0, 0, 50), D3DXVECTOR3(0, 0, 0));
	//m_pObjectManager->Insert(20001, eResourceType::User, m_pd3dDevice, m_pd3dDeviceContext, 1, 3, D3DXVECTOR3(100, 0, 0), D3DXVECTOR3(0, 0, 0));

	/* �� ���� & ������ ���� */
	for (int i = 0; i < 6; ++i)
		m_pObjectManager->Insert(i, eResourceType::ITME_HP, D3DXVECTOR3(120 * i , 0, 100));

	// 1) ī�޶� init
	m_pCamera = new CThirdPersonCamera();
	m_pCamera->CreateShaderVariables(m_pd3dDevice);

	// 2) ī�޶� update
	m_pCamera->SetLookAt(m_pPlayer->GetPosition());
	m_pCamera->RegenerateViewMatrix();

	// 3) ī�޶� set ����
	m_pCamera->SetViewport(m_pd3dDeviceContext, 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	m_pCamera->GenerateProjectionMatrix(1.01f, 5000.0f, ASPECT_RATIO, 60.0f);
	m_pCamera->GenerateViewMatrix();
}


void CGameFramework::ReleaseObjects()
{
	if (m_pScene) m_pScene->ReleaseObjects();
	if (m_pScene) delete m_pScene;

	if (m_pPlayer)	delete m_pPlayer;
}



void CGameFramework::AnimateObjects()
{
	if (m_pScene) m_pScene->AnimateObjects(0, m_pd3dDeviceContext,m_GameTimer.GetTimeElapsed());					
}

void CGameFramework::FrameAdvance()
{
	m_GameTimer.Tick(60.0f);

	ProcessInput();

	float fClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	if (m_pd3dRenderTargetView) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);
	if (m_pd3dDepthStencilView) m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// 5) ī�޶� ���̴� update
	if (m_pCamera) m_pCamera->UpdateShaderVariables(m_pd3dDeviceContext);

	if (m_pScene) m_pScene->AnimateObjectsAndRender(m_pd3dDeviceContext, m_GameTimer.GetTimeElapsed());
	
	m_pDXGISwapChain->Present(0, 0);

	m_GameTimer.GetFrameRate(m_pszBuffer + 12, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}
