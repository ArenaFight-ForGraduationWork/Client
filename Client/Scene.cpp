#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"



CScene::CScene()
{
	m_pLight = new CLight();

	m_OperationMode = MODE_KEYBOARD;

	m_pPlayer = nullptr;
	m_pCameraManager = nullptr;

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

void CFirstScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
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
		case VK_F3:
			printf(" ĳ���� : %.2f, %.2f\n", m_pPlayer->GetObjects()->m_MaxVer.x, m_pPlayer->GetObjects()->m_MaxVer.z);
			printf(" ���� : %.2f, %.2f\n", m_pMonster->GetObjects()->m_MaxVer.x, m_pMonster->GetObjects()->m_MaxVer.z);
			break;

		case VK_SPACE:	//��Ÿ
			PressSkillNum = 0;
			Player_Attack_number = 3;
			m_pPlayer->SetIsAttack(true);
			break;
		case 0x31:			//1��
			cout << "1��" << endl;
			PressSkillNum = 1;
			Player_Attack_number = 4;
			m_pPlayer->SetIsAttack(true);
			break;
		case 0x32:			//2��
			cout << "2��" << endl;
			PressSkillNum = 2;
			Player_Attack_number = 5;
			m_pPlayer->SetIsAttack(true);
			break;
		case 0x33:			//3��
			cout << "3��" << endl;
			PressSkillNum = 3;
			Player_Attack_number = 6;
			m_pPlayer->SetIsAttack(true);
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
void CFirstScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
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
				m_pCameraManager->GetNowCamera()->RotatebyYaw(-150 * fTimeElapsed);
			else
				m_pCameraManager->GetNowCamera()->RotatebyYaw(150 * fTimeElapsed);
			break;
		case WM_MOUSEWHEEL:
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
				m_pCameraManager->GetNowCamera()->Zoom(200 * fTimeElapsed);
			else
				m_pCameraManager->GetNowCamera()->Zoom(-200 * fTimeElapsed);
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
void CFirstScene::ProcessInput(float fTimeElapsed)
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

		/* player state ����, ������ ������� �޸���.*/
		if (dwDirection != 0)
			m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::RUN);
		else
			m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::IDLE);



		if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObjectInCategory(CObjectManager::eObjectType::BUFF_CRYSTAL)))
		{
			cout << "���� �ε��� ������Ʈ ��ȣ : " << m_pPlayer->GetObjects()->CollOtherID << endl;
			m_pObjectManager->DeleteObject(m_pPlayer->GetObjects()->CollOtherID);
			m_pPlayer->SetHP();
			m_pPlayer->SetSpeed();
			cout << "���� ü�� : " << m_pPlayer->GetHP() << endl;
			cout << "���� ���ǵ� : " << m_pPlayer->GetSpeed() << endl;
		}

		if (m_pPlayer->GetIsAttack())		//���� ���� �� 
		{
			m_pPlayer->GetObjects()->SetPressSkill(PressSkillNum);	//��� ��ų �������� �˷��ְ�
			m_pPlayer->GetObjects()->SetPlayAnimationState((ePLAYER_STATE)Player_Attack_number);

			if (m_pPlayer->GetObjects()->MyHitAndEnemyBound(m_pMonster->GetObjects()))		//�� ��Ʈ�ڽ� + ��� �浹üũ�ڽ� Ȯ��
			{
				//cout << "������ �����ߴ�!!  ü�� : "<<m_pObjectManager->FindObject(20000)->GetHP() << endl;
				cout << "���� ���ȴ�!" << endl;
				//is_Coll = true;	//�ϴ� �浹�ߴٴ� �͸� �˷��ֱ� ���ؼ�
			}

			if (m_pPlayer->GetObjects()->m_fAnimationPlaytime == 0.0f)	//�ִϸ��̼��� �ѹ��� ���Ƽ� 0�� �Ǹ�, ���ݻ��¸� �����.
			{
				//if (is_Coll) m_pObjectManager->FindObject(20000)->SetHP();	// �� �ִϸ��̼��� �ѹ��� ����, �����߿� �浹�ߴٰ� �ٲ���°� ������ �׶� �Ǹ� �����.
				//cout << "ü��: " << m_pObjectManager->FindObject(20000)->GetHP() << endl;
				//is_Coll = false;			//���� �浹üũ�� ���ؼ� false
				m_pPlayer->SetIsAttack(false);		//�ѹ��� ���Ƽ� ���ݳ�������, ���ݸ���� �����ֱ� ���� false
			}
		}
	}
	break;

	case MODE_KEYBOARD:
	{
		if (GetKeyboardState(pKeyBuffer))
		{
			// �̵�
			if (pKeyBuffer[VK_UP] & 0xF0)		dwDirection |= DIR_FORWARD;
			if (pKeyBuffer[VK_DOWN] & 0xF0)		dwDirection |= DIR_BACKWARD;
			if (pKeyBuffer[VK_LEFT] & 0xF0)		dwDirection |= DIR_LEFT;
			if (pKeyBuffer[VK_RIGHT] & 0xF0)	dwDirection |= DIR_RIGHT;
			// �¿�ȸ��
			if (pKeyBuffer[0x51] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTimeElapsed);			// Q
			if (pKeyBuffer[0x45] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTimeElapsed);		// E
			// ��
			if (pKeyBuffer[0x5A] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(-100 * fTimeElapsed);				// Z
			if (pKeyBuffer[0x58] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(100 * fTimeElapsed);				// X
		}
		if (dwDirection != 0)
			m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::RUN);
		else
			m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::IDLE);
	}
	break;
	}


	if (dwDirection && false == m_pPlayer->GetIsAttack()) m_pPlayer->Move(m_pCameraManager->GetNowCamera()->GetYaw(), dwDirection, fTimeElapsed);

	m_pCameraManager->GetNowCamera()->Update(m_pPlayer->GetPosition());
}

void CFirstScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	CScene::BuildObjects(pd3dDevice);

	ID3D11DeviceContext *pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

	//*LAND : ����.�ٴ�, ��, ����, �� ��� = >  �浹üũo, ����x, �̵� ���ϰ� ��.		0~999
	//	* BUFF_CRYSTAL :							�浹üũo, ����o, 1000~1999
	//	* MONSTER : ���� = ����					 �浹üũo, ��Ʈ�ڽ�o, 2000~2999
	//	* PLAYER :										�浹üũo, ��Ʈ�ڽ�o, 3000~3999
	
	m_pPlayer = new CPlayer();
	m_pPlayer->SetObject(m_pObjectManager->Insert(3000, eResourceType::User, pd3dDevice, pd3dDeviceContext, 0, 3, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0)));
	m_pPlayer->GetObjects()->SetPlayAnimationState(ePLAYER_STATE::IDLE);

	m_pMonster = new CMonster();
	m_pMonster->SetObject(m_pObjectManager->Insert(2000, eResourceType::Item_HP, D3DXVECTOR3(-300, 50, 100)));

	m_pCameraManager = CCameraManager::GetSingleton();
	m_pCameraManager->GetNowCamera()->SetLookAt(m_pPlayer->GetPosition());

	m_pObjectManager->Insert(0, eResourceType::Floor, D3DXVECTOR3(0, 0, 0));
	for (int i = 0; i < 6; ++i)
		m_pObjectManager->Insert(i+1000, eResourceType::Item_HP, D3DXVECTOR3(120 * i, 0, 100));
}






CSceneManager::CSceneManager()
{
	m_eNow = eSceneType::FIRST;
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
	m_mScenes[eSceneType::FIRST] = new CFirstScene();
	m_mScenes[eSceneType::SECOND] = new CFirstScene();
}

void CSceneManager::Destroy()
{
}

void CSceneManager::Change(eSceneType eType)
{
	m_eNow = eType;
}

CScene* CSceneManager::GetNowScene()
{
	return m_mScenes[m_eNow];
}


