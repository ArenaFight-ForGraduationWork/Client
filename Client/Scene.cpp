#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"



CScene::CScene()
{
	m_pLight = new CLight();

	m_OperationMode = MODE_KEYBOARD;

	m_ptOldCursorPos.x = 0;
	m_ptOldCursorPos.y = 0;
	m_ptNewCursorPos.x = 0;
	m_ptNewCursorPos.y = 0;

	m_pObjectManager = CObjectManager::GetSingleton();
}
CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	CResourceManager *pResourceManager = CResourceManager::GetSingleton(pd3dDevice);
	for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
	{
		m_vShaders.push_back(pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i));
	}

	m_pLight->BuildLights(pd3dDevice);
}

void CScene::ReleaseObjects()
{
	m_vShaders.clear();
}

void CScene::AnimateObjects(int StateCnt, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed)
{
	for (auto shader : m_vShaders)
	{
		shader->AnimateObjects(StateCnt, pd3dDeviceContext, fTimeElapsed);
	}
}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext)
{
	m_pLight->UpdateLights(pd3dDeviceContext);

	for (auto shader : m_vShaders)
	{
		shader->Render(pd3dDeviceContext);
	}
}

void CScene::AnimateObjectsAndRender(ID3D11DeviceContext *pd3dDeviceContext, float time)
{
	m_pLight->UpdateLights(pd3dDeviceContext);

	for (auto shader : m_vShaders)
	{
		shader->AnimateObjectAndRender(pd3dDeviceContext, time);
	}
}









CFirstScene::CFirstScene()
{
}
CFirstScene::~CFirstScene()
{
}

void CFirstScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
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
		default:
			break;
		}
		break;
	default:
		break;
	}
}
void CFirstScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

			//if (m_ptNewCursorPos.x > m_ptOldCursorPos.x)
			//	m_pCamera->RotatebyYaw(-150 * m_GameTimer.GetTimeElapsed());
			//else
			//	m_pCamera->RotatebyYaw(150 * m_GameTimer.GetTimeElapsed());
			//break;
		case WM_MOUSEWHEEL:
			//if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			//	m_pCamera->Zoom(200 * m_GameTimer.GetTimeElapsed());
			//else
			//	m_pCamera->Zoom(-200 * m_GameTimer.GetTimeElapsed());
			//break;
		default: break;
		}
	}	break;
	case MODE_KEYBOARD:
	{
	}	break;
	default: break;
	}
}
void CFirstScene::ProcessInput()
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

		//if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObject(20000)))		//m_pPlayer->GetObject()->Collision( �÷��̾� ���� ������,����), id�� Ȯ���� ���н��Ѿ��ҵ�.
		//{
		//	//m_pPlayer->GetObjects()->Collison(m_pPlayer->GetObjects()/*m_pPlayer[i]*/)
		//	//���� ������ ���ĸ�, (0,0,0) �������� ���� �ٷ� �浹�Ӥ���, �ٵ� insert�Ҷ� ��� �ʱ� ��ġ�� ���� �ٲ������(���͸� x������ 100��ŭ �Ű�����)
		//	// �׷��ϱ� ���� �浹üũ�� m_MinVer,m_MaxVer.x�� ���� 100��ŭ �������־������. �װ� �����ܤ��� ��ġ��  [��] moveAbsolute�ȿ� moveBoundingbox�� �߰�������.
		//	cout << "�浹�ߴ�!" << endl;
		//}
		///* player state ����, ������ ������� �޸���.*/
		//if (dwDirection != 0)
		//{
		//	m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::RUN);
		//	//cout<< "�����϶� �ٿ�� �����̳�? "<< m_pObjectManager->FindObject(30000)->GetMaxVer().x<< endl;
		//}

		//if (is_Attack)
		//{
		//	m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::ATTACK);

		//	//�ִϸ��̼��� �ѹ��� ���Ƽ� 0�� �Ǹ�, ���ݻ��¸� �����.
		//	if (m_pObjectManager->FindObject(30000)->m_fAnimationPlaytime == 0.0f)
		//		is_Attack = false;
		//}
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
			//// �¿�ȸ��
			//if (pKeyBuffer[0x51] & 0xF0) m_pCamera->RotatebyYaw(100 * m_GameTimer.GetTimeElapsed());		// Q
			//if (pKeyBuffer[0x45] & 0xF0) m_pCamera->RotatebyYaw(-100 * m_GameTimer.GetTimeElapsed());		// E
			//// ��
			//if (pKeyBuffer[0x5A] & 0xF0) m_pCamera->Zoom(-100 * m_GameTimer.GetTimeElapsed());				// Z
			//if (pKeyBuffer[0x58] & 0xF0) m_pCamera->Zoom(100 * m_GameTimer.GetTimeElapsed());				// X
		}
		m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::IDLE);
		if (dwDirection != 0)
			m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::RUN);
	}
	break;

	}

	//if (dwDirection) m_pPlayer->Move(m_pCamera->GetYaw(), dwDirection, m_GameTimer.GetTimeElapsed());

	////m_pplater -> movebounding

	//// 4) �÷��̾� ��ġ�� ���� ī�޶� update
	//m_pCamera->Update(m_pPlayer->GetPosition());

	////����_��ǥ ������ : �̵��ϰ��� ��ǥ�� ������ �����������ؼ� ���⿡ �־��.
	////const D3DXVECTOR3 *pos = m_pPlayer->GetPosition();
	////cout << "�������� ��ǥ : " << pos->x << "," << pos->y << "," << pos->z << endl;
	////SendPosPacket(pos->x, pos->y, pos->z);
}







CSceneManager::CSceneManager()
{
}
CSceneManager::~CSceneManager()
{
}
CSceneManager* CSceneManager::GetSingleton()
{
	static CSceneManager instance;
	return &instance;
}

void CSceneManager::Initialize()
{
	m_mScenes[eSceneType::FISRT] = new CFirstScene();
	m_mScenes[eSceneType::SECOND] = new CFirstScene();
}

void CSceneManager::Destroy()
{
}

void CSceneManager::Change(eSceneType eType)
{
	m_eNow = eType;
}



