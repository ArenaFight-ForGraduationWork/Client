#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <SpriteBatch.h>




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

	//m_pInterface = nullptr;
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

	//m_pInterface = new CUserInterface();
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
	//for (auto shader : m_vShaders)
	//{
	//	shader->AnimateObjectAndRender(pd3dDeviceContext, time);
	//}
}
void CScene::AnimateObjectsAndRender2D(float time)
{
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_vShaders[m_vShaders.size() - 1]->UpdateShaderVariables(&matrix);

	//for (unsigned int i = 0; i < m_pInterface->GetTexture()->GetNumOfTextures(); ++i)
	//{
	//	m_vShaders[m_vShaders.size() - 1]->UpdateShaderVariables(m_pInterface->GetTexture());
	//	m_vShaders[m_vShaders.size() - 1]->AnimateObjectAndRender(static_cast<float>(m_pInterface->GetIndexCount()));
	//}
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


		// 기본공격(space) : VK_SPACE
		// 스킬 : 0x31, 0x32, 0x33


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
		m_dwDirectionPrev = m_dwDirectionNow;
		m_dwDirectionNow = 0;

		if (GetKeyboardState(m_pKeyBuffer))
		{
			if (m_pKeyBuffer[VK_SPACE] & 0xF0)	// attack
			{
				pPlayer->PlayAnimation(CObject::eAnimationType::Attack);
			}
			else if (m_pKeyBuffer[0x31] & 0xF0)	// skill1
			{
				pPlayer->PlayAnimation(CObject::eAnimationType::Skill1);
			}
			else if (m_pKeyBuffer[0x32] & 0xF0)	// skill2
			{
				pPlayer->PlayAnimation(CObject::eAnimationType::Skill2);
			}
			else if (m_pKeyBuffer[0x33] & 0xF0)	// skill3
			{
				pPlayer->PlayAnimation(CObject::eAnimationType::Skill3);
			}
			else
			{
				// 1) 카메라가 바라보는 방향 + 입력받은 방향 = fAngle를 Yaw값으로 회전
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

				if (D3DXVECTOR2(0, 0) != inputAngle) // inputAngle==(0,0)이면 어느 방향으로든 움직이지 않는다 => 이동 계산X
				{
					D3DXVECTOR2 defaultAngle(0, 1);	// X, Z
					float fAngle = acosf(D3DXVec2Dot(&defaultAngle, &inputAngle) / (D3DXVec2Length(&defaultAngle) * D3DXVec2Length(&inputAngle)));
					fAngle = static_cast<float>(D3DXToDegree(fAngle));
					fAngle = ((defaultAngle.x* inputAngle.y - defaultAngle.y*inputAngle.x) > 0.0f) ? fAngle : -fAngle;

					pPlayer->SetDirectionAbsolute(&D3DXVECTOR3(0, m_pCameraManager->GetNowCamera()->GetYaw() + fAngle, 0));

					// 2) 로컬 z축으로 속도 * 시간만큼 이동
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
					else
					{	/* if ((m_dwDirectionPrev) && (!m_dwDirectionNow)) 
							this client's player was moving, but now doesn't move */
						if (m_dwDirectionPrev)
						{
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
				else
					m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);
			}
		}
		// 카메라 좌우회전
		if (m_pKeyBuffer[0x51] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTimeElapsed);	// Q
		if (m_pKeyBuffer[0x45] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTimeElapsed);	// E
		// 카메라 줌
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

	///* 아이템 설정 */
	//for (int i = 0; i < 10; ++i)
	//	m_pObjectManager->Insert(i + 2000, eResourceType::Item_HP, D3DXVECTOR3(i * 100, 50, i * 100));
	//for (int i = 20; i < 30; ++i)
	//	m_pObjectManager->Insert(i + 2000, eResourceType::Item_Buff, D3DXVECTOR3((i-19)*-100, 50, (i-19)*-100));
	/* check : 여기 이상하게 rand()에서 i가 터진다. 지금 이렇게 해놔서 벡터 assert가 안 뜨는데, 원래 rand()로 좌표 찍던 때는 계속 터짐*/

	/* 맵 꾸미기 */
	{
		// 바닥
		m_pObjectManager->Insert(3000, eResourceType::Floor, D3DXVECTOR3(0, 0, 0));

		// 벽. 한 면에 두개씩 들어간다
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
				m_pObjectManager->Insert(4000 + i, eResourceType::Wall1, WallPos[i]);	// ㅡ
			else
				m_pObjectManager->Insert(4000 + i, eResourceType::Wall1, WallPos[i], D3DXVECTOR3(0, 90, 0));  // ㅣ
		}

		// 나무
		m_pObjectManager->Insert(4010, eResourceType::Tree, D3DXVECTOR3(-2000, 0, 1900), D3DXVECTOR3(0, 90, 0));
		m_pObjectManager->Insert(4011, eResourceType::Tree, D3DXVECTOR3(-1800, 0, -1900));
		m_pObjectManager->Insert(4012, eResourceType::Tree, D3DXVECTOR3(2200, 0, 1900), D3DXVECTOR3(0, 60, 0));
		m_pObjectManager->Insert(4013, eResourceType::Tree, D3DXVECTOR3(2100, 0, -1900));

		for (short i = 0; i < 20; ++i)
			m_pObjectManager->Insert(4020 + i, eResourceType::grass, D3DXVECTOR3(static_cast<float>(rand() % 2000 - 1000), 0, static_cast<float>(rand() % 2000 - 1000)));
	}

	m_pFog = new CFog();
	m_pFog->Initialize();

	//// create interface object
	//m_pInterface = new CUserInterface();
	//m_pInterface->Initialize(FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	//m_pInterface->SetTexture(L"./Data/UI/health.png", 20, 20);	// 크기

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
	//gpCommonState->TurnZBufferOff();

	//if(m_pObjectManager->FindObject(myID))
	//m_pInterface->ChangeSize(FRAME_BUFFER_WIDTH / 20, m_pObjectManager->FindObject(myID)->GetComponent()->GetHealthPoint());
	//else
	//	m_pInterface->ChangeSize(FRAME_BUFFER_WIDTH / 20, 0);
	//m_pInterface->Render(FRAME_BUFFER_WIDTH / 20 * 18, FRAME_BUFFER_HEIGHT / 20);	// 위치

	//// set view + projection matrix buffer
	//D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	//gpCommonState->m_pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	//VS_CB_VIEWPROJECTION_MATRIX *pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX *)d3dMappedResource.pData;
	//D3DXMATRIX matrix;	D3DXMatrixIdentity(&matrix);
	//D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &matrix);
	//D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, m_pCameraManager->GetNowCamera()->GetOrthoMatrix());
	//gpCommonState->m_pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);
	//gpCommonState->m_pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbCamera);

	//CScene::AnimateObjectsAndRender2D(time);

	//gpCommonState->TurnZBufferOn();
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


