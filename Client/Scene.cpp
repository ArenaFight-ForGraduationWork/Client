#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <SpriteBatch.h>
#include <DDSTextureLoader.h>




CScene::CScene()
{
	m_pLight = new CLight();

	m_OperationMode = MODE_KEYBOARD;

	m_pCameraManager = nullptr;

	m_ptOldCursorPos.x = 0;
	m_ptOldCursorPos.y = 0;
	m_ptNewCursorPos.x = 0;
	m_ptNewCursorPos.y = 0;

	m_pObjectManager = CObjectManager::GetSingleton();

	m_pSpriteBatch = nullptr;
	m_pTexture = nullptr;

	m_pd3dcbCamera = nullptr;
}
CScene::~CScene()
{
}

void CScene::BuildObjects()
{
	CResourceManager *pResourceManager = CResourceManager::GetSingleton();
	for (BYTE i = (BYTE)CResourceManager::eShaderType::START; i < (BYTE)CResourceManager::eShaderType::END; ++i)
	{
		m_vShaders.push_back(pResourceManager->GetShaderByShaderType((CResourceManager::eShaderType)i));
	}

	m_pLight->BuildLights();

 	m_pSpriteBatch.reset(new DirectX::SpriteBatch(gpCommonState->m_pd3dDeviceContext));
}

void CScene::ReleaseObjects()
{
	m_vShaders.clear();
}

void CScene::AnimateObjectsAndRender3D(float time)
{
	m_pLight->UpdateLights();

	for (unsigned int i = 0; i < m_vShaders.size() - 1; ++i)
	{
		m_vShaders[i]->AnimateObjectAndRender(time);
	}
}
void CScene::AnimateObjectsAndRender2D(float time)
{
}





CFirstScene::CFirstScene()
{
	m_dwDirectionPrev = 0;
	m_dwDirectionNow = 0;

	m_pFog = nullptr;
}
CFirstScene::~CFirstScene()
{
}

void CFirstScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
	{
	}break;
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
			m_pFog->Expand(&D3DXVECTOR3(0, 0, 0));
			break;
		case VK_F4:
			m_pFog->Contract();
			break;


		// �⺻����(space) : VK_SPACE
		// ��ų : 0x31, 0x32, 0x33
		case VK_SPACE :
		{
			CObject *pPlayer = m_pObjectManager->FindObject(myID);
			if (pPlayer)
			{
				if ((CObject::eAnimationType::Idle == pPlayer->GetNowAnimation()) | (CObject::eAnimationType::Move == pPlayer->GetNowAnimation()))
				{
					cout << "send attack packet" << endl;

					pPlayer->PlayAnimation(CObject::eAnimationType::Attack);
					player_attack  *pp = reinterpret_cast<player_attack *>(send_buffer);

					pp->size = sizeof(*pp);
					pp->type = PLAYER_ATTACK;
					pp->id = myID;
					pp->attack_type = NORMAL_ATTACK;
					if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
						printf("event data send ERROR\n");
				}
			}
			pPlayer = nullptr;
		} break;
		case 0x31:
		{
			CObject *pPlayer = m_pObjectManager->FindObject(myID);
			if (pPlayer)
			{
				if ((CObject::eAnimationType::Idle == pPlayer->GetNowAnimation()) | (CObject::eAnimationType::Move == pPlayer->GetNowAnimation()))
				{
					cout << "send 1 packet" << endl;

					pPlayer->PlayAnimation(CObject::eAnimationType::Skill1);
					player_attack  *pp = reinterpret_cast<player_attack *>(send_buffer);

					pp->size = sizeof(*pp);
					pp->type = PLAYER_ATTACK;
					pp->id = myID;
					pp->attack_type = PLAYER_SKILL1;
					if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
						printf("event data send ERROR\n");
				}
			}
			pPlayer = nullptr;
		} break;
		case 0x32:
		{
			CObject *pPlayer = m_pObjectManager->FindObject(myID);
			if (pPlayer)
			{
				if ((CObject::eAnimationType::Idle == pPlayer->GetNowAnimation()) | (CObject::eAnimationType::Move == pPlayer->GetNowAnimation()))
				{
					cout << "send 2 packet" << endl;

					pPlayer->PlayAnimation(CObject::eAnimationType::Skill2);
					player_attack  *pp = reinterpret_cast<player_attack *>(send_buffer);

					pp->size = sizeof(*pp);
					pp->type = PLAYER_ATTACK;
					pp->id = myID;
					pp->attack_type = PLAYER_SKILL2;
					if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
						printf("event data send ERROR\n");
				}
			}
			pPlayer = nullptr;
		} break;
		case 0x33:
		{
			CObject *pPlayer = m_pObjectManager->FindObject(myID);
			if (pPlayer)
			{
				if ((CObject::eAnimationType::Idle == pPlayer->GetNowAnimation()) | (CObject::eAnimationType::Move == pPlayer->GetNowAnimation()))
				{
					cout << "send 3 packet" << endl;

					pPlayer->PlayAnimation(CObject::eAnimationType::Skill3);
					player_attack  *pp = reinterpret_cast<player_attack *>(send_buffer);

					pp->size = sizeof(*pp);
					pp->type = PLAYER_ATTACK;
					pp->id = myID;
					pp->attack_type = PLAYER_SKILL3;
					if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
						printf("event data send ERROR\n");
				}
			}
			pPlayer = nullptr;
		} break;

		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		default:break;
		}
		break;
	default:break;
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
				m_pCameraManager->GetNowCamera()->RotatebyYaw(-350 * fTimeElapsed);
			else
				m_pCameraManager->GetNowCamera()->RotatebyYaw(350 * fTimeElapsed);
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
	CObject *pPlayer = m_pObjectManager->FindObject(myID);

	if (pPlayer)
	{
		if (GetKeyboardState(m_pKeyBuffer))
		{
			m_dwDirectionPrev = m_dwDirectionNow;
			m_dwDirectionNow = 0;

			//if (m_pKeyBuffer[VK_SPACE] & 0xF0)	// attack
			//{
			//	pPlayer->PlayAnimation(CObject::eAnimationType::Attack);
			//}
			//else if (m_pKeyBuffer[0x31] & 0xF0)	// skill1
			//{
			//	pPlayer->PlayAnimation(CObject::eAnimationType::Skill1);
			//}
			//else if (m_pKeyBuffer[0x32] & 0xF0)	// skill2
			//{
			//	pPlayer->PlayAnimation(CObject::eAnimationType::Skill2);
			//}
			//else if (m_pKeyBuffer[0x33] & 0xF0)	// skill3
			//{
			//	pPlayer->PlayAnimation(CObject::eAnimationType::Skill3);
			//}
			//else
			//if ((static_cast<BYTE>(CObject::eAnimationType::Idle) == pPlayer->GetNowAnimation())
			//	| (static_cast<BYTE>(CObject::eAnimationType::Move) == pPlayer->GetNowAnimation()))
			if ((CObject::eAnimationType::Idle == pPlayer->GetNowAnimation()) | (CObject::eAnimationType::Move == pPlayer->GetNowAnimation()))
			{
				// 1) ī�޶� �ٶ󺸴� ���� + �Է¹��� ���� = fAngle�� Yaw������ ȸ��
				D3DXVECTOR2 inputAngle(0, 0);
				if (m_pKeyBuffer[VK_UP] & 0xF0)
				{
					m_dwDirectionNow |= DIR_FORWARD;
					inputAngle.y += 1;
				}
				if (m_pKeyBuffer[VK_DOWN] & 0xF0)
				{
					m_dwDirectionNow |= DIR_BACKWARD;
					inputAngle.y -= 1;
				}
				if (m_pKeyBuffer[VK_LEFT] & 0xF0)
				{
					m_dwDirectionNow |= DIR_LEFT;
					inputAngle.x += 1;
				}
				if (m_pKeyBuffer[VK_RIGHT] & 0xF0)
				{
					m_dwDirectionNow |= DIR_RIGHT;
					inputAngle.x -= 1;
				}

				if (D3DXVECTOR2(0, 0) != inputAngle) // inputAngle==(0,0)�̸� ��� �������ε� �������� �ʴ´� => �̵� ���X
				{
					D3DXVECTOR2 defaultAngle(0, 1);	// X, Z
					float fAngle = acosf(D3DXVec2Dot(&defaultAngle, &inputAngle) / (D3DXVec2Length(&defaultAngle) * D3DXVec2Length(&inputAngle)));
					fAngle = static_cast<float>(D3DXToDegree(fAngle));
					fAngle = ((defaultAngle.x* inputAngle.y - defaultAngle.y*inputAngle.x) > 0.0f) ? fAngle : -fAngle;

					pPlayer->SetDirectionAbsolute(&D3DXVECTOR3(0, m_pCameraManager->GetNowCamera()->GetYaw() + fAngle, 0));

					// 2) ���� z������ �ӵ� * �ð���ŭ �̵�
					pPlayer->MoveForward(fTimeElapsed);
					pPlayer->PlayAnimation(CObject::eAnimationType::Move);

					if (m_dwDirectionNow)
					{	/* this client's player is moving */
						player_position *pp = reinterpret_cast<player_position*>(send_buffer);

						pp->size = sizeof(*pp);
						pp->type = PLAYER_MOV;	// herehere
						pp->id = myID;
						pp->x = pPlayer->GetPosition()->x;
						pp->z = pPlayer->GetPosition()->z;
						pp->direction = pPlayer->GetDirection()->y;
						pp->isMoving = true;
						if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
							printf("event data send ERROR\n");
					}
				}
				else
				{
					if (m_dwDirectionPrev)
					{	/* if ((m_dwDirectionPrev) && (!m_dwDirectionNow))
					this client's player was moving, but now doesn't move */
						if (!m_dwDirectionNow)
						{
							m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);

							player_position *pp = reinterpret_cast<player_position*>(send_buffer);

							pp->size = sizeof(*pp);
							pp->type = PLAYER_MOV;	// herehere
							pp->id = myID;
							pp->x = pPlayer->GetPosition()->x;
							pp->z = pPlayer->GetPosition()->z;
							pp->direction = pPlayer->GetDirection()->y;
							pp->isMoving = false;
							if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
								printf("event data send ERROR\n");
						}
					}
				}
			}
		}

		// ī�޶� �¿�ȸ��
		if (m_pKeyBuffer[0x51] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTimeElapsed);	// Q
		if (m_pKeyBuffer[0x45] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTimeElapsed);	// E
		// ī�޶� ��
		if (m_pKeyBuffer[0x5A] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(-100 * fTimeElapsed);			// Z
		if (m_pKeyBuffer[0x58] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(100 * fTimeElapsed);			// X
	}

	if (pPlayer)
		m_pCameraManager->GetNowCamera()->Update(pPlayer->GetPosition());
	else
		m_pCameraManager->GetNowCamera()->Update(new D3DXVECTOR3(0, 0, 0));
}

void CFirstScene::BuildObjects()
{
	CScene::BuildObjects();

	gpCommonState->m_pd3dDevice->GetImmediateContext(&gpCommonState->m_pd3dDeviceContext);

	m_pCameraManager = CCameraManager::GetSingleton();
	if (m_pObjectManager->FindObject(myID))
		m_pCameraManager->GetNowCamera()->SetLookAt(m_pObjectManager->FindObject(myID)->GetPosition());
	else
		m_pCameraManager->GetNowCamera()->SetLookAt(new D3DXVECTOR3(0, 0, 0));

	///* ������ ���� */
	//for (int i = 0; i < 10; ++i)
	//	m_pObjectManager->Insert(i + 2000, eResourceType::Item_HP, D3DXVECTOR3(i * 100, 50, i * 100));
	//for (int i = 20; i < 30; ++i)
	//	m_pObjectManager->Insert(i + 2000, eResourceType::Item_Buff, D3DXVECTOR3((i-19)*-100, 50, (i-19)*-100));
	/* check : ���� �̻��ϰ� rand()���� i�� ������. ���� �̷��� �س��� ���� assert�� �� �ߴµ�, ���� rand()�� ��ǥ ��� ���� ��� ����*/

	/* �� �ٹ̱� */
	{
		// �ٴ�
		m_pObjectManager->Insert(3000, eResourceType::Floor, D3DXVECTOR3(0, 0, 0));

		// ��. �� �鿡 �ΰ��� ����
		D3DXVECTOR3 WallPos[8];
		WallPos[0] = D3DXVECTOR3(-1250, 500, 2500);
		WallPos[1] = D3DXVECTOR3(1250, 500, 2500);
		WallPos[2] = D3DXVECTOR3(-1250, 500, -2500);
		WallPos[3] = D3DXVECTOR3(1250, 500, -2500);

		WallPos[4] = D3DXVECTOR3(2500, 500, 1250);
		WallPos[5] = D3DXVECTOR3(2500, 500, -1250);
		WallPos[6] = D3DXVECTOR3(-2500, 500, 1250);
		WallPos[7] = D3DXVECTOR3(-2500, 500, -1250);
		for (int i = 0; i < 8; ++i)
		{
			if (i < 4)
				m_pObjectManager->Insert(4000 + i, eResourceType::Wall1, WallPos[i]);	// ��
			else
				m_pObjectManager->Insert(4000 + i, eResourceType::Wall1, WallPos[i], D3DXVECTOR3(0, 90, 0));  // ��
		}

		// ����
		m_pObjectManager->Insert(4010, eResourceType::Tree, D3DXVECTOR3(-2000, 0, 1900), D3DXVECTOR3(0, 90, 0));
		m_pObjectManager->Insert(4011, eResourceType::Tree, D3DXVECTOR3(-1800, 0, -1900));
		m_pObjectManager->Insert(4012, eResourceType::Tree, D3DXVECTOR3(2200, 0, 1900), D3DXVECTOR3(0, 60, 0));
		m_pObjectManager->Insert(4013, eResourceType::Tree, D3DXVECTOR3(2100, 0, -1900));

		for (short i = 0; i < 20; ++i)
			m_pObjectManager->Insert(4020 + i, eResourceType::grass, D3DXVECTOR3(static_cast<float>(rand() % 2000 - 1000), 0, static_cast<float>(rand() % 2000 - 1000)));
	}

	m_pFog = new CFog();
	m_pFog->Initialize();

	m_pSpriteBatch.reset(new DirectX::SpriteBatch(gpCommonState->m_pd3dDeviceContext));
	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/frame.dds", nullptr, &m_pTexture);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gpCommonState->m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
}

void CFirstScene::AnimateObjectsAndRender3D(float time)
{
	if (m_pFog->IsInUse())
		m_pFog->Update();

	CScene::AnimateObjectsAndRender3D(time);
}

void CFirstScene::AnimateObjectsAndRender2D(float time)
{	
	gpCommonState->TurnZBufferOff();
	m_pSpriteBatch->Begin();

	RECT a;
	a.left = 0;	a.top = 0;	a.right = FRAME_BUFFER_WIDTH;	a.bottom = FRAME_BUFFER_HEIGHT - 20;
	m_pSpriteBatch->Draw(m_pTexture, a);

	m_pSpriteBatch->End();
	gpCommonState->TurnZBufferOn();
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


