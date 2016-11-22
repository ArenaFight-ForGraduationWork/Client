#include "stdafx.h"
#include "Scene.h"
#include <DDSTextureLoader.h>





//
//	Scene
//
CScene::CScene()
{
	m_pLight = new CLight();

	m_ptOldCursorPos.x = 0;
	m_ptOldCursorPos.y = 0;
	m_ptNewCursorPos.x = 0;
	m_ptNewCursorPos.y = 0;

	m_pCameraManager = nullptr;
	m_pObjectManager = CObjectManager::GetSingleton();

	m_pSpriteBatch = nullptr;
	m_vTextures.clear();
	m_pSpriteFont = nullptr;
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

	m_pSpriteBatch.reset(new SpriteBatch(gpCommonState->GetDeviceContext()));
}

void CScene::ReleaseObjects()
{
	m_vShaders.clear();
}

void CScene::AnimateObjectsAndRender()
{
	m_pLight->UpdateLights();

	for (unsigned int i = 0; i < m_vShaders.size(); ++i)
	{
		m_vShaders[i]->AnimateObjectAndRender();
	}
}





//
//	Intro Scene
//
CIntroScene::CIntroScene()
{
	m_bButton = 0;

	m_vStrings.clear();
	m_pTempString = new string();
	m_pTempString->clear();
}
CIntroScene::~CIntroScene()
{
}

void CIntroScene::ChangeState()
{
	m_vStrings.clear();
	m_pTempString->clear();

	m_bButton = 0;
}

void CIntroScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (player_status)
	{
	case static_cast<BYTE>(ePlayer_State::eLOBBY) :
		_KeyboardMessageInLobby(hWnd, nMessageID, wParam, lParam);
		break;
	case static_cast<BYTE>(ePlayer_State::eROOM) :
		_KeyboardMessageInRoom(hWnd, nMessageID, wParam, lParam);
		break;
	default: break;
	}
}
void CIntroScene::_KeyboardMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
	{
		switch (wParam)
		{
			// case0x30~case0x39: 0~9, case0x41~case0x5A: A~Z
		case 0x30:case 0x31:case 0x32:case 0x33:case 0x34:case 0x35:case 0x36:case 0x37:case 0x38:case 0x39:
		{
			char temp[2] = { static_cast<char>(wParam) };
			m_pTempString->append(temp);
		}break;
		case 0x41:case 0x42:case 0x43:case 0x44:case 0x45:case 0x46:case 0x47:case 0x48:case 0x49:case 0x4A:case 0x4B:case 0x4C:case 0x4D:
		case 0x4E:case 0x4F:case 0x50:case 0x51:case 0x52:case 0x53:case 0x54:case 0x55:case 0x56:case 0x57:case 0x58:case 0x59:case 0x5A:
		{
			if ((1 == m_bButton) | (3 == m_bButton))	// inserting room-name
			{
				char temp[2] = { static_cast<char>(wParam) };
				m_pTempString->append(temp);
			}
		}break;
		default:break;
		}
	}break;
	default:break;
	}
}
void CIntroScene::_KeyboardMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CIntroScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (player_status)
	{
	case static_cast<BYTE>(ePlayer_State::eLOBBY) :
		_MouseMessageInLobby(hWnd, nMessageID, wParam, lParam);
		break;
	case static_cast<BYTE>(ePlayer_State::eROOM) :
		_MouseMessageInRoom(hWnd, nMessageID, wParam, lParam);
		break;
	default: break;
	}
}
void CIntroScene::_MouseMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (WM_LBUTTONUP == nMessageID)
	{
		m_ptNewCursorPos.x = LOWORD(lParam);
		m_ptNewCursorPos.y = HIWORD(lParam);

		switch (m_bButton)
		{
		case 0:
		{	// 버튼 아무것도 안 눌림

			// 방 생성 버튼 or 방 입장 버튼 눌렸는지 확인
			// 눌렀으면 1 or 3으로 bbutton 변경

			// check create-room button
			if (_CheckDestination(&m_ptNewCursorPos, &rButton1pos))
			{
				m_bButton = 1;
				break;
			}

			// check enter-room button
			if (_CheckDestination(&m_ptNewCursorPos, &rButton2pos))
			{
				m_bButton = 3;
				break;
			}
		}break;
		case 1:
		{	// create room > insert room-name

			// 확인 버튼 눌렸는지 확인
			// 눌렀으면 2로 bbutton 변경

			if (_CheckDestination(&m_ptNewCursorPos, &rInputWindowButtonPos))
			{
				if (m_pTempString->length() > 0)
				{	// 입력이 뭔가 들어와있긴 함
					m_vStrings.push_back(*m_pTempString);
					m_pTempString = new string();
					m_pTempString->clear();

					m_bButton = 2;
				}
				break;
			}
		}break;
		case 2:
		{	// create room > insert stage-number

			// 확인 버튼 눌렸는지 확인
			// 눌렀으면 방 생성 패킷 보냄
			// changestate()

			if (_CheckDestination(&m_ptNewCursorPos, &rInputWindowButtonPos))
			{
				if (m_pTempString->length() > 0)
				{
					m_vStrings.push_back(*m_pTempString);
					m_pTempString = new string();
					m_pTempString->clear();

					create_room* pp = reinterpret_cast<create_room*>(send_buffer);
					pp->type = CREATE_ROOM;
					pp->id = myID;
					strcpy_s(pp->room_name, sizeof(pp->room_name), m_vStrings[0].c_str());
					pp->stage = static_cast<BYTE>(atoi(m_vStrings[1].c_str()));
					pp->size = sizeof(*pp);
					if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
						printf("send ERROR\n");
					Sleep(100);

					ChangeState();
				}
				break;
			}
		}break;
		case 3:
		{	// enter room > insert room-name

			// 확인 버튼 눌렸는지 확인
			// 방 입장 패킷 보냄
			// changestate()
			if (_CheckDestination(&m_ptNewCursorPos, &rInputWindowButtonPos))
			{
				if (m_pTempString->length() > 0)
				{
					m_vStrings.push_back(*m_pTempString);
					m_pTempString = new string();
					m_pTempString->clear();

					join_room* pp = reinterpret_cast<join_room*>(send_buffer);
					pp->type = JOIN_ROOM;
					pp->id = myID;
					strcpy_s(pp->room_name, sizeof(pp->room_name), m_vStrings[0].c_str());
					pp->size = sizeof(*pp);
					if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
						printf("send ERROR\n");
					Sleep(100);

					ChangeState();
				}
			}
		}break;
		default:break;
		}
	}
}
void CIntroScene::_MouseMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (WM_LBUTTONUP == nMessageID)
	{
		m_ptNewCursorPos.x = LOWORD(lParam);
		m_ptNewCursorPos.y = HIWORD(lParam);

		// check game-start button
		if (_CheckDestination(&m_ptNewCursorPos, &rButton2pos))
		{
			game_start* pp = reinterpret_cast<game_start*>(send_buffer);
			pp->type = GAME_START;
			pp->id = myID;
			pp->size = sizeof(*pp);
			if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
				printf("send ERROR\n");
		}
	}
}
bool CIntroScene::_CheckDestination(POINT *pMousePos, const RECT *pDestination)
{
	if (pMousePos->x >= pDestination->left)
	{
		if (pMousePos->x <= pDestination->right)
		{
			if (pMousePos->y >= pDestination->top)
			{
				if (pMousePos->y <= pDestination->bottom)
				{
					return true;
				}
			}
		}
	}

	return false;
}

void CIntroScene::BuildObjects()
{
	CScene::BuildObjects();

	m_pSpriteBatch.reset(new  SpriteBatch(gpCommonState->GetDeviceContext()));
	m_vTextures.clear();
	ID3D11ShaderResourceView *pTexture;

	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/frame.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/rbutton1.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/rbutton2.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/InputWindow.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/rbutton3.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/background.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/rbutton4.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	m_pSpriteFont.reset(new  SpriteFont(gpCommonState->GetDevice(), L"./Data/Font/Arial18.spritefont"));
}

void CIntroScene::AnimateObjectsAndRender()
{
	// 3D
	CScene::AnimateObjectsAndRender();

	// 2D
	gpCommonState->TurnZBufferOff();
	m_pSpriteBatch->Begin();

	m_pSpriteBatch->Draw(m_vTextures[5], rFramePos);
	m_pSpriteBatch->Draw(m_vTextures[0], rFramePos);
	switch (player_status)
	{
	case static_cast<BYTE>(ePlayer_State::eLOBBY) :
	{
		m_pSpriteBatch->Draw(m_vTextures[1], rButton1pos);
		m_pSpriteBatch->Draw(m_vTextures[2], rButton2pos);

		switch (m_bButton)
		{
		case 1:
		{
			m_pSpriteBatch->Draw(m_vTextures[3], rInputWindowPos);
			m_pSpriteBatch->Draw(m_vTextures[4], rInputWindowButtonPos);

			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), L"Input Room Name",
				XMFLOAT2(static_cast<float>(rInputWindowPos.left) + 10, static_cast<float>(rInputWindowPos.top) + 10));
			wstring wstr = wstring(m_pTempString->begin(), m_pTempString->end());
			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), wstr.c_str(), XMFLOAT2(FRAME_BUFFER_WIDTH / 4 + 50, FRAME_BUFFER_HEIGHT / 2 - 20));
		}break;
		case 2:
		{
			m_pSpriteBatch->Draw(m_vTextures[3], rInputWindowPos);
			m_pSpriteBatch->Draw(m_vTextures[4], rInputWindowButtonPos);

			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), L"Input Stage Number (1 ~ 3)",
				XMFLOAT2(static_cast<float>(rInputWindowPos.left) + 10, static_cast<float>(rInputWindowPos.top) + 10));
			wstring wstr = wstring(m_pTempString->begin(), m_pTempString->end());
			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), wstr.c_str(), XMFLOAT2(FRAME_BUFFER_WIDTH / 4 + 50, FRAME_BUFFER_HEIGHT / 2 - 20));
		}break;
		case 3:
		{
			m_pSpriteBatch->Draw(m_vTextures[3], rInputWindowPos);
			m_pSpriteBatch->Draw(m_vTextures[4], rInputWindowButtonPos);

			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), L"Input Room Name",
				XMFLOAT2(static_cast<float>(rInputWindowPos.left) + 10, static_cast<float>(rInputWindowPos.top) + 10));
			wstring wstr = wstring(m_pTempString->begin(), m_pTempString->end());
			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), wstr.c_str(), XMFLOAT2(FRAME_BUFFER_WIDTH / 4 + 50, FRAME_BUFFER_HEIGHT / 2 - 20));
		}break;
		default:break;
		}
	}break;
	case static_cast<BYTE>(ePlayer_State::eROOM) :
	{
		m_pSpriteBatch->Draw(m_vTextures[6], rButton2pos);
	}break;
	default: break;
	}

	m_pSpriteBatch->End();
	gpCommonState->TurnZBufferOn();
}





//
//	First Scene
//
CFirstScene::CFirstScene()
{
	m_dwDirectionPrev = 0;
	m_dwDirectionNow = 0;

	m_pFog = nullptr;

	// Particle
	m_FireParticle = new CParticle();

	isFireParticle = false;
}
CFirstScene::~CFirstScene()
{
}

void CFirstScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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
			m_pFog->Expand(0, 0, 0);
			break;
		case VK_F2:
			m_pFog->Contract();
			break;

		case VK_SPACE:
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

			if (isFireParticle)
			{
				isFireParticle = false;
			}
			else
			{
				m_FireParticle->Reset();
				isFireParticle = true;
			}
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
void CFirstScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	//switch (nMessageID)
	//{
	//case WM_MOUSEMOVE:
	//	m_ptOldCursorPos.x = m_ptNewCursorPos.x;
	//	m_ptOldCursorPos.y = m_ptNewCursorPos.y;
	//	m_ptNewCursorPos.x = LOWORD(lParam);
	//	m_ptNewCursorPos.y = HIWORD(lParam);

	//	if (m_ptNewCursorPos.x > m_ptOldCursorPos.x)
	//		m_pCameraManager->GetNowCamera()->RotatebyYaw(-350 * fTimeElapsed);
	//	else
	//		m_pCameraManager->GetNowCamera()->RotatebyYaw(350 * fTimeElapsed);
	//	break;
	//case WM_MOUSEWHEEL:
	//	if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
	//		m_pCameraManager->GetNowCamera()->Zoom(200 * fTimeElapsed);
	//	else
	//		m_pCameraManager->GetNowCamera()->Zoom(-200 * fTimeElapsed);
	//	break;
	//default: break;
	//}
}
void CFirstScene::ProcessInput()
{
	CObject *pPlayer = m_pObjectManager->FindObject(myID);
	float fTime = gpCommonState->GetTimer()->GetTimeElapsed();

	if (pPlayer)
	{
		if (GetKeyboardState(m_pKeyBuffer))
		{
			m_dwDirectionPrev = m_dwDirectionNow;
			m_dwDirectionNow = 0;

			if ((CObject::eAnimationType::Idle == pPlayer->GetNowAnimation()) | (CObject::eAnimationType::Move == pPlayer->GetNowAnimation()))
			{
				// 1) 카메라가 바라보는 방향 + 입력받은 방향 = fAngle를 Yaw값으로 회전
				XMFLOAT2 f2InputAngle = XMFLOAT2(0, 0);
				XMVECTOR vInputAngle;

				if (m_pKeyBuffer[VK_UP] & 0xF0)
				{
					m_dwDirectionNow |= DIR_FORWARD;
					f2InputAngle.y += 1;
				}
				if (m_pKeyBuffer[VK_DOWN] & 0xF0)
				{
					m_dwDirectionNow |= DIR_BACKWARD;
					f2InputAngle.y -= 1;
				}
				if (m_pKeyBuffer[VK_LEFT] & 0xF0)
				{
					m_dwDirectionNow |= DIR_LEFT;
					f2InputAngle.x += 1;
				}
				if (m_pKeyBuffer[VK_RIGHT] & 0xF0)
				{
					m_dwDirectionNow |= DIR_RIGHT;
					f2InputAngle.x -= 1;
				}

				vInputAngle = XMLoadFloat2(&f2InputAngle);
				if (!XMVector2Equal(vInputAngle, XMVectorZero()))
				{
					XMFLOAT2 f2DefaultAngle = XMFLOAT2(0, 1);	// X, Z
					XMVECTOR vDefaultAngle;
					vDefaultAngle = XMLoadFloat2(&f2DefaultAngle);

					float dotDI = XMVectorGetX(XMVector2Dot(vDefaultAngle, vInputAngle));
					float lengthD = XMVectorGetX(XMVector2Length(vDefaultAngle));
					float lengthI = XMVectorGetX(XMVector2Length(vInputAngle));
					float fAngle = acosf(dotDI / (lengthD * lengthI));
					fAngle = XMConvertToDegrees(fAngle);
					fAngle = (((XMVectorGetX(vDefaultAngle) * XMVectorGetY(vInputAngle)) - (XMVectorGetY(vDefaultAngle) * XMVectorGetX(vInputAngle))) > 0.0f) ? fAngle : -fAngle;

					XMFLOAT3 cameraAngle(0, m_pCameraManager->GetNowCamera()->GetYaw() + fAngle, 0);
					pPlayer->SetDirectionAbsolute(XMLoadFloat3(&cameraAngle));

					// 2) 로컬 z축으로 속도 * 시간만큼 이동
					pPlayer->MoveForward(fTime);
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

		// 카메라 좌우회전
		if (m_pKeyBuffer[0x51] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTime);	// Q
		if (m_pKeyBuffer[0x45] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTime);	// E
		// 카메라 줌
		if (m_pKeyBuffer[0x5A] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(-100 * fTime);		// Z
		if (m_pKeyBuffer[0x58] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(100 * fTime);			// X
	}

	if (pPlayer)
	{
		XMVECTOR position = XMLoadFloat3(pPlayer->GetPosition());
		m_pCameraManager->GetNowCamera()->Update(position);
	}
	else
	{
		XMFLOAT3 position = XMFLOAT3(0, 0, 0);
		m_pCameraManager->GetNowCamera()->Update(XMLoadFloat3(&position));
	}
}

void CFirstScene::BuildObjects()
{
	CScene::BuildObjects();

	m_pCameraManager = CCameraManager::GetSingleton();
	if (m_pObjectManager->FindObject(myID))
	{
		XMVECTOR vPos;
		vPos = XMLoadFloat3(m_pObjectManager->FindObject(myID)->GetPosition());

		m_pCameraManager->GetNowCamera()->SetLookAt(vPos);
	}
	else
	{
		XMFLOAT3 lookAt = XMFLOAT3(0, 0, 0);
		m_pCameraManager->GetNowCamera()->SetLookAt(XMLoadFloat3(&lookAt));
	}

	///* 아이템 설정 */
	//for (int i = 0; i < 10; ++i)
	//	m_pObjectManager->Insert(i + 2000, eResourceType::Item_HP, D3DXVECTOR3(i * 100, 50, i * 100));
	//for (int i = 20; i < 30; ++i)
	//	m_pObjectManager->Insert(i + 2000, eResourceType::Item_Buff, D3DXVECTOR3((i-19)*-100, 50, (i-19)*-100));
	/* check : 여기 이상하게 rand()에서 i가 터진다. 지금 이렇게 해놔서 벡터 assert가 안 뜨는데, 원래 rand()로 좌표 찍던 때는 계속 터짐*/

	/* 맵 꾸미기 */
	{
		XMFLOAT3 f3VectorPos, f3VectorDir;

		// 바닥
		m_pObjectManager->Insert(3000, eResourceType::Floor, XMVectorZero(), XMVectorZero());

		// 벽. 한 면에 두개씩 들어간다
		XMFLOAT3 WallPos[8];
		XMFLOAT3 f3WallDir;
		WallPos[0] = XMFLOAT3(-1250, 500, 2500);
		WallPos[1] = XMFLOAT3(1250, 500, 2500);
		WallPos[2] = XMFLOAT3(-1250, 500, -2500);
		WallPos[3] = XMFLOAT3(1250, 500, -2500);
		WallPos[4] = XMFLOAT3(2500, 500, 1250);
		WallPos[5] = XMFLOAT3(2500, 500, -1250);
		WallPos[6] = XMFLOAT3(-2500, 500, 1250);
		WallPos[7] = XMFLOAT3(-2500, 500, -1250);
		for (int i = 0; i < 8; ++i)
		{
			if (i < 4)
			{
				m_pObjectManager->Insert(4000 + i, eResourceType::Wall1, XMLoadFloat3(&WallPos[i]), XMVectorZero());	// ㅡ
			}
			else
			{
				f3WallDir = XMFLOAT3(0, 90, 0);
				m_pObjectManager->Insert(4000 + i, eResourceType::Wall1, XMLoadFloat3(&WallPos[i]), XMLoadFloat3(&f3WallDir));  // ㅣ
			}
		}

		// 나무
		f3VectorPos = XMFLOAT3(-2000, 0, 1900); f3VectorDir = XMFLOAT3(0, 90, 0);
		m_pObjectManager->Insert(4010, eResourceType::Tree, XMLoadFloat3(&f3VectorPos), XMLoadFloat3(&f3VectorDir));
		f3VectorPos = XMFLOAT3(-1800, 0, -1900); f3VectorDir = XMFLOAT3(0, 0, 0);
		m_pObjectManager->Insert(4011, eResourceType::Tree, XMLoadFloat3(&f3VectorPos), XMLoadFloat3(&f3VectorDir));
		f3VectorPos = XMFLOAT3(2200, 0, 1900); f3VectorDir = XMFLOAT3(0, 60, 0);
		m_pObjectManager->Insert(4012, eResourceType::Tree, XMLoadFloat3(&f3VectorPos), XMLoadFloat3(&f3VectorDir));
		f3VectorPos = XMFLOAT3(2100, 0, -1900); f3VectorDir = XMFLOAT3(0, 0, 0);
		m_pObjectManager->Insert(4013, eResourceType::Tree, XMLoadFloat3(&f3VectorPos), XMLoadFloat3(&f3VectorDir));

		m_pObjectManager->Insert(4020, eResourceType::grass, XMVectorZero(), XMVectorZero());


		//for (short i = 0; i < 20; ++i)
		//{
		//	f3VectorPos = XMFLOAT3(static_cast<float>(rand() % 2000 - 1000), 0, static_cast<float>(rand() % 2000 - 1000));
		//	f3VectorDir = XMFLOAT3(0, 0, 0);
		//	m_pObjectManager->Insert(4020 + i, eResourceType::grass, XMLoadFloat3(&f3VectorPos), XMLoadFloat3(&f3VectorDir));
		//}
	}

	m_pFog = new CFog();
	m_pFog->Initialize();

	m_pSpriteBatch.reset(new  SpriteBatch(gpCommonState->GetDeviceContext()));
	m_vTextures.clear();
	ID3D11ShaderResourceView *pTexture;

	// 0: frame
	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/frame.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	// 1: hp
	CreateDDSTextureFromFile(gpCommonState->GetDevice(), L"./Data/UI/hp.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	m_FireParticle->Initialize("Fire.fxo", L"Data/Effect/SpectacularBurst3.dds", 3);
	//m_FireParticle->Initialize("Fire.fxo", L"Data/Effect/flare0.dds", 10000);
	XMFLOAT3 cameralook;
	XMStoreFloat3(&cameralook, m_pCameraManager->GetNowCamera()->GetLookVector());
	cameralook.x *= -1;	cameralook.y *= -1;	cameralook.z *= -1;
	m_FireParticle->SetEmitDir(XMLoadFloat3(&cameralook));
}

void CFirstScene::AnimateObjectsAndRender()
{
	// 3D
	{
		//if (m_pFog->IsInUse())
		//	m_pFog->Update();

		CScene::AnimateObjectsAndRender();
	}

	// particle
	{
		gpCommonState->EnableAlphaBlending();
		RenderParticle();
		gpCommonState->DisableAlphaBlending();
	}

	// 2D
	{
		gpCommonState->TurnZBufferOff();
		m_pSpriteBatch->Begin();
		{
			// frame
			m_pSpriteBatch->Draw(m_vTextures[0], rFramePos);
			// hp
			CObject *pObject = m_pObjectManager->FindObject(myID);
			if (pObject)
			{
				rHpPos.right = rHpPos.left + static_cast<LONG>(pObject->GetComponent()->GetHealthPoint()) * 3;
				m_pSpriteBatch->Draw(m_vTextures[1], rHpPos);
			}
		}
		m_pSpriteBatch->End();
		gpCommonState->TurnZBufferOn();
	}
}

void CFirstScene::RenderParticle()
{
	if (isFireParticle)
	{
		FireParticleTime += 2.0f;
		if (FireParticleTime <= 200.0f)
		{
			m_FireParticle->Update(gpCommonState->GetTimer()->GetTimeElapsed(), gpCommonState->GetTimer()->GetTimeElapsed());
			XMVECTOR vEyePosition;
			if (m_pCameraManager->GetNowCamera())
				vEyePosition = XMLoadFloat3(m_pCameraManager->GetNowCamera()->GetPosition());
			else
				vEyePosition = XMVectorZero();
			m_FireParticle->SetEyePos(vEyePosition);

			XMVECTOR vPlayerPosition;
			if (m_pObjectManager->FindObject(myID))
				vPlayerPosition = XMLoadFloat3(m_pObjectManager->FindObject(myID)->GetPosition());
			else
				vPlayerPosition = XMVectorZero();
			m_FireParticle->SetEmitPos(vPlayerPosition);

			XMMATRIX V = XMLoadFloat4x4(m_pCameraManager->GetNowCamera()->GetViewMatrix());
			XMMATRIX P = XMLoadFloat4x4(m_pCameraManager->GetNowCamera()->GetProjectionMatrix());
			m_FireParticle->Draw(V, P);
		}
		else
		{
			FireParticleTime = 0.0f;
			isFireParticle = false;
			m_FireParticle->Reset();
		}
	}
}





//
//	Scene Manager
//
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
	m_mScenes[eSceneType::INTRO] = new CIntroScene();
	m_mScenes[eSceneType::FIRST] = new CFirstScene();
}

void CSceneManager::Destroy()
{
}

void CSceneManager::Change(eSceneType eType)
{
	m_eNow = eType;
	m_mScenes[m_eNow]->BuildObjects();
}

CScene* CSceneManager::GetNowScene()
{
	return m_mScenes[m_eNow];
}


