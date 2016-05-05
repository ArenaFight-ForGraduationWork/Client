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
			// 이동
			if (pKeyBuffer[0x41] & 0xF0) dwDirection |= DIR_LEFT;		// A
			if (pKeyBuffer[0x44] & 0xF0) dwDirection |= DIR_RIGHT;		// D
			if (pKeyBuffer[0x57] & 0xF0) dwDirection |= DIR_FORWARD;	// W
			if (pKeyBuffer[0x53] & 0xF0) dwDirection |= DIR_BACKWARD;	// S
		}

		m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::IDLE);

		//if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObject(20000)))		//m_pPlayer->GetObject()->Collision( 플레이어 제외 아이템,몬스터), id로 확실히 구분시켜야할듯.
		//{
		//	//m_pPlayer->GetObjects()->Collison(m_pPlayer->GetObjects()/*m_pPlayer[i]*/)
		//	//지금 문제가 뭐냐면, (0,0,0) 기준으로 보면 바로 충돌임ㅇㅇ, 근데 insert할때 얘네 초기 위치를 내가 바꿔줬었음(몬스터를 x축으로 100만큼 옮겨줬음)
		//	// 그러니까 몬스터 충돌체크의 m_MinVer,m_MaxVer.x의 값이 100만큼 증가해있어야했음. 그걸 안해줌ㅇㅇ 고치자  [完] moveAbsolute안에 moveBoundingbox를 추가해줬음.
		//	cout << "충돌했다!" << endl;
		//}
		///* player state 변경, 방향이 있을경우 달린다.*/
		//if (dwDirection != 0)
		//{
		//	m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::RUN);
		//	//cout<< "움직일때 바운딩 움직이나? "<< m_pObjectManager->FindObject(30000)->GetMaxVer().x<< endl;
		//}

		//if (is_Attack)
		//{
		//	m_pObjectManager->FindObject(30000)->SetPlayAnimationState(ePLAYER_STATE::ATTACK);

		//	//애니메이션이 한바퀴 돌아서 0이 되면, 공격상태를 멈춘다.
		//	if (m_pObjectManager->FindObject(30000)->m_fAnimationPlaytime == 0.0f)
		//		is_Attack = false;
		//}
	}
	break;

	case MODE_KEYBOARD:
	{
		if (GetKeyboardState(pKeyBuffer))
		{
			// 이동
			if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
			if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
			if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
			if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
			//// 좌우회전
			//if (pKeyBuffer[0x51] & 0xF0) m_pCamera->RotatebyYaw(100 * m_GameTimer.GetTimeElapsed());		// Q
			//if (pKeyBuffer[0x45] & 0xF0) m_pCamera->RotatebyYaw(-100 * m_GameTimer.GetTimeElapsed());		// E
			//// 줌
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

	//// 4) 플레이어 위치에 따라 카메라 update
	//m_pCamera->Update(m_pPlayer->GetPosition());

	////서버_좌표 보내기 : 이동하고나서 좌표를 보내면 되지않을까해서 여기에 넣어둠.
	////const D3DXVECTOR3 *pos = m_pPlayer->GetPosition();
	////cout << "보내려는 좌표 : " << pos->x << "," << pos->y << "," << pos->z << endl;
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



