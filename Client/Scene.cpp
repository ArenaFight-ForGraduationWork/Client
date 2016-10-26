#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"

#include <SpriteBatch.h>
#include <DDSTextureLoader.h>
#include <SpriteFont.h>

#include "Effects.h"
#include "InputLayout.h"
#include "RenderStates.h"
//#include "ParticleSystem.h"




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
	m_vTextures.clear();
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

void CScene::AnimateObjectsAndRender(float time)
{
	m_pLight->UpdateLights();

	for (unsigned int i = 0; i < m_vShaders.size() - 1; ++i)
	{
		m_vShaders[i]->AnimateObjectAndRender(time);
	}
}





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

void CIntroScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
{
	switch (player_status)
	{
	case static_cast<BYTE>(ePlayer_State::eLOBBY) :
		KeyboardMessageInLobby(hWnd, nMessageID, wParam, lParam, fTimeElapsed);
		break;
	case static_cast<BYTE>(ePlayer_State::eROOM) :
		KeyboardMessageInRoom(hWnd, nMessageID, wParam, lParam, fTimeElapsed);
		break;
	default: break;
	}
}
void CIntroScene::KeyboardMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
{
	switch (nMessageID)
	{
	case WM_KEYUP:
	{
		switch (wParam)
		{
		//case VK_RETURN:
		//{
		//	if (1==m_bButton)
		//	{	// create room > insert room-name
		//		if (m_pTempString->length() > 0)
		//		{	// 입력이 뭔가 들어와있긴 함
		//			m_vStrings.push_back(*m_pTempString);
		//			m_pTempString = new string();
		//			m_pTempString->clear();

		//			m_bButton = 2;
		//		}
		//	}
		//	else if (2==m_bButton)
		//	{	// create room > insert stage-number
		//		if (m_pTempString->length() > 0)
		//		{
		//			m_vStrings.push_back(*m_pTempString);
		//			m_pTempString = new string();
		//			m_pTempString->clear();

		//			create_room* pp = reinterpret_cast<create_room*>(send_buffer);
		//			pp->type = CREATE_ROOM;
		//			pp->id = myID;
		//			//gets_s(pp->room_name);
		//			//gets_s(pp->room_name);
		//			//printf("입력한방이름:%s\n", pp->room_name);
		//			strcpy_s(pp->room_name, sizeof(pp->room_name), m_vStrings[0].c_str());
		//			//printf("플레이할 스테이지를 입력해주세요\n");
		//			//scanf("%hhd", &pp->stage);
		//			//printf("입력한스테이지:%d\n", pp->stage);
		//			pp->stage = static_cast<BYTE>(atoi(m_vStrings[1].c_str()));
		//			pp->size = sizeof(*pp);
		//			printf("전송하는사이즈:%d\n", pp->size);
		//			if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
		//				printf("send ERROR\n");

		//			Sleep(100);
		//		}
		//	}
		//}break;
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
void CIntroScene::KeyboardMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
{
}

void CIntroScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
{ 
	switch (player_status)
	{
	case static_cast<BYTE>(ePlayer_State::eLOBBY) :
		MouseMessageInLobby(hWnd, nMessageID, wParam, lParam, fTimeElapsed);
		break;
	case static_cast<BYTE>(ePlayer_State::eROOM) :
		MouseMessageInRoom(hWnd, nMessageID, wParam, lParam, fTimeElapsed);
		break;
	default: break;
	}
}
void CIntroScene::MouseMessageInLobby(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
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
void CIntroScene::MouseMessageInRoom(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
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
bool CIntroScene::_CheckDestination(POINT * pMousePos, const RECT* pDestination)
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

	m_pSpriteBatch.reset(new DirectX::SpriteBatch(gpCommonState->m_pd3dDeviceContext));
	m_vTextures.clear();
	ID3D11ShaderResourceView *pTexture;

	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/frame.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/rbutton1.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/rbutton2.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/InputWindow.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/rbutton3.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/background.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/rbutton4.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	m_pSpriteFont.reset(new DirectX::SpriteFont(gpCommonState->m_pd3dDevice, L"./Data/Font/Arial18.spritefont"));
}

void CIntroScene::AnimateObjectsAndRender(float time)
{
	// 3D
	CScene::AnimateObjectsAndRender(time);

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

		//m_pSpriteFont->DrawString(m_pSpriteBatch.get(), L"Create Room",
		//	DirectX::XMFLOAT2(static_cast<float>(rButton1pos.left), static_cast<float>(rButton1pos.top)));

		switch (m_bButton)
		{
		case 1:
		{
			m_pSpriteBatch->Draw(m_vTextures[3], rInputWindowPos);
			m_pSpriteBatch->Draw(m_vTextures[4], rInputWindowButtonPos);

			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), L"Input Room Name",
				DirectX::XMFLOAT2(static_cast<float>(rInputWindowPos.left) + 10, static_cast<float>(rInputWindowPos.top) + 10));
			wstring wstr = wstring(m_pTempString->begin(), m_pTempString->end());
			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(FRAME_BUFFER_WIDTH / 4 + 50, FRAME_BUFFER_HEIGHT / 2 - 20));

		}break;
		case 2:
		{
			m_pSpriteBatch->Draw(m_vTextures[3], rInputWindowPos);
			m_pSpriteBatch->Draw(m_vTextures[4], rInputWindowButtonPos);

			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), L"Input Stage Number (1 ~ 3)",
				DirectX::XMFLOAT2(static_cast<float>(rInputWindowPos.left) + 10, static_cast<float>(rInputWindowPos.top) + 10));
			wstring wstr = wstring(m_pTempString->begin(), m_pTempString->end());
			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(FRAME_BUFFER_WIDTH / 4 + 50, FRAME_BUFFER_HEIGHT / 2 - 20));
		}break;
		case 3:
		{
			m_pSpriteBatch->Draw(m_vTextures[3], rInputWindowPos);
			m_pSpriteBatch->Draw(m_vTextures[4], rInputWindowButtonPos);

			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), L"Input Room Name",
				DirectX::XMFLOAT2(static_cast<float>(rInputWindowPos.left) + 10, static_cast<float>(rInputWindowPos.top) + 10));
			wstring wstr = wstring(m_pTempString->begin(), m_pTempString->end());
			m_pSpriteFont->DrawString(m_pSpriteBatch.get(), wstr.c_str(), DirectX::XMFLOAT2(FRAME_BUFFER_WIDTH / 4 + 50, FRAME_BUFFER_HEIGHT / 2 - 20));
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












CFirstScene::CFirstScene()
{
	m_dwDirectionPrev = 0;
	m_dwDirectionNow = 0;

	m_pFog = nullptr;

	mFire = nullptr;
	mRain = nullptr;
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
			m_pFog->Expand(0, 0, 0);
			break;
		case VK_F4:
			m_pFog->Contract();
			break;

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

			if ((CObject::eAnimationType::Idle == pPlayer->GetNowAnimation()) | (CObject::eAnimationType::Move == pPlayer->GetNowAnimation()))
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

					XMFLOAT3 cameraAngle(0, m_pCameraManager->GetNowCamera()->GetYaw() + fAngle, 0);
					pPlayer->SetDirectionAbsolute(XMLoadFloat3(&cameraAngle));

					// 2) 로컬 z축으로 속도 * 시간만큼 이동
					pPlayer->MoveForward(fTimeElapsed);
					pPlayer->PlayAnimation(CObject::eAnimationType::Move);

					if (m_dwDirectionNow)
					{	/* this client's player is moving */
						player_position *pp = reinterpret_cast<player_position*>(send_buffer);

						pp->size = sizeof(*pp);
						pp->type = PLAYER_MOV;	// herehere
						pp->id = myID;
						pp->x = XMVectorGetX(pPlayer->GetPosition());
						pp->z = XMVectorGetZ(pPlayer->GetPosition());
						pp->direction = XMVectorGetY(pPlayer->GetDirection());
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
							pp->x = XMVectorGetX(pPlayer->GetPosition());
							pp->z = XMVectorGetZ(pPlayer->GetPosition());
							pp->direction = XMVectorGetY(pPlayer->GetDirection());
							pp->isMoving = false;
							if (SOCKET_ERROR == send(sock, (char*)pp, sizeof(*pp), 0))
								printf("event data send ERROR\n");
						}
					}
				}
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
	{
		D3DXVECTOR3 position;
		position.x = XMVectorGetX(pPlayer->GetPosition());
		position.y = XMVectorGetY(pPlayer->GetPosition());
		position.z = XMVectorGetZ(pPlayer->GetPosition());
		m_pCameraManager->GetNowCamera()->Update(&position);
	}
	else
		m_pCameraManager->GetNowCamera()->Update(new D3DXVECTOR3(0, 0, 0));
}

void CFirstScene::BuildObjects()
{
	CScene::BuildObjects();

	gpCommonState->m_pd3dDevice->GetImmediateContext(&gpCommonState->m_pd3dDeviceContext);

	m_pCameraManager = CCameraManager::GetSingleton();
	if (m_pObjectManager->FindObject(myID))
	{
		D3DXVECTOR3 d3dxPos;
		XMVECTOR vPos;
		vPos = m_pObjectManager->FindObject(myID)->GetPosition();
		d3dxPos = D3DXVECTOR3(XMVectorGetX(vPos), XMVectorGetY(vPos), XMVectorGetZ(vPos));

		m_pCameraManager->GetNowCamera()->SetLookAt(&d3dxPos);
	}
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

	m_pSpriteBatch.reset(new DirectX::SpriteBatch(gpCommonState->m_pd3dDeviceContext));
	m_vTextures.clear();
	ID3D11ShaderResourceView *pTexture;

	// 0: frame
	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/frame.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	// 1: hp
	DirectX::CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, L"./Data/UI/hp.dds", nullptr, &pTexture);
	m_vTextures.push_back(pTexture);
	pTexture = nullptr;

	Effects::InitAll(gpCommonState->m_pd3dDevice);
	InputLayouts::InitAll(gpCommonState->m_pd3dDevice);
	RenderStates::InitAll(gpCommonState->m_pd3dDevice);

	mRandomTexSRV = d3dHelper::CreateRandomTexture1DSRV(gpCommonState->m_pd3dDevice);

	std::vector<std::wstring> flares;
	flares.push_back(L"Textures\\flare0.dds");
	flares.push_back(L"./Textures/flare0.dds");
	mFlareTexSRV = d3dHelper::CreateTexture2DArraySRV(gpCommonState->m_pd3dDevice, gpCommonState->m_pd3dDeviceContext, flares);

	mFire = new ParticleSystem();
	mFire->Init(gpCommonState->m_pd3dDevice, Effects::FireFX, mFlareTexSRV, mRandomTexSRV, 500);
	mFire->SetEmitPos(XMFLOAT3(0.0f, 100.0f, 0.0f));
}

void CFirstScene::AnimateObjectsAndRender(float time)
{
	// 3D
	if (m_pFog->IsInUse())
		m_pFog->Update();

	CScene::AnimateObjectsAndRender(time);

	static int aa = 0;
	if (aa > 1)
	{
		int b = 1 + 1;
	}
	else
		aa += 1;

	if (mFire)
	{
		float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		mFire->Update(gpCommonState->m_pTimer->GetTimeElapsed(), gpCommonState->m_pTimer->GetNowTime());

		XMFLOAT3 cameraPos;
		XMStoreFloat3(&cameraPos, m_pCameraManager->GetNowCamera()->GetPosition());
		XMMATRIX cameraViewProjection;
		cameraViewProjection = XMMatrixMultiply(m_pCameraManager->GetNowCamera()->GetViewMatrix(), m_pCameraManager->GetNowCamera()->GetProjectionMatrix());

		mFire->SetEyePos(cameraPos);
		//mFire->Draw(gpCommonState->m_pd3dDeviceContext, cameraViewProjection);
		gpCommonState->m_pd3dDeviceContext->OMSetBlendState(0, blendFactor, 0xffffffff); // restore default
	}

	// 2D
	gpCommonState->TurnZBufferOff();
	m_pSpriteBatch->Begin();

	// frame
	m_pSpriteBatch->Draw(m_vTextures[0], rFramePos);

	// hp
	CObject *pObject = m_pObjectManager->FindObject(myID);
	if (pObject)
	{
		rHpPos.right = rHpPos.left + pObject->GetComponent()->GetHealthPoint() * 3;
		m_pSpriteBatch->Draw(m_vTextures[1], rHpPos);
	}




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
	m_mScenes[eSceneType::INTRO] = new CIntroScene();
	m_mScenes[eSceneType::FIRST] = new CFirstScene();
}

void CSceneManager::Destroy()
{
}

void CSceneManager::Change(eSceneType eType)
{
	//m_mScenes[m_eNow]->Destroy();
	m_eNow = eType;
	m_mScenes[m_eNow]->BuildObjects();
}

CScene* CSceneManager::GetNowScene()
{
	return m_mScenes[m_eNow];
}


