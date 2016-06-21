#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"



CScene::CScene()
{
	m_pLight = new CLight();

	m_OperationMode = MODE_KEYBOARD;

	//m_pPlayer = nullptr;
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
	m_pFog = nullptr;
}
CFirstScene::~CFirstScene()
{
}

void CFirstScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
{
	static UCHAR pKeyBuffer[256];
	packet_player_move *move_packet;
	player_attack* attack_packet;

	switch (nMessageID)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_UP:
		{
			if (server_on) {
				move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV;
				move_packet->move_type = W;
				move_packet->direction = m_pCameraManager->GetNowCamera()->GetYaw();
				send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Move);
			}
		}break;
		case VK_DOWN:
		{
			if (server_on) {
				move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV;
				move_packet->move_type = S;
				move_packet->direction = m_pCameraManager->GetNowCamera()->GetYaw();
				send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Move);
			}
		}break;
		case VK_LEFT:
		{
			if (server_on) {
				move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV;
				move_packet->move_type = A;
				move_packet->direction = m_pCameraManager->GetNowCamera()->GetYaw();
				send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Move);
			}
		}break;
		case VK_RIGHT:
		{
			if (server_on) {
				move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
				move_packet->size = sizeof(*move_packet);
				move_packet->type = PLAYER_MOV;
				move_packet->move_type = D;
				move_packet->direction = m_pCameraManager->GetNowCamera()->GetYaw();
				send(sock, (char*)move_packet, sizeof(*move_packet), 0);
				m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Move);
			}
		}break;
		default:break;
		}
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

		case VK_UP:
		{
			move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
			move_packet->size = sizeof(*move_packet);
			move_packet->type = PLAYER_MOV_END;
			move_packet->move_type = W;
			move_packet->direction = 0;
			send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			if (GetKeyboardState(pKeyBuffer))
			{
				if (!((pKeyBuffer[VK_UP] & 0xF0) || (pKeyBuffer[VK_DOWN] & 0xF0)
					|| (pKeyBuffer[VK_LEFT] & 0xF0) || (pKeyBuffer[VK_RIGHT] & 0xF0)))
				{
					m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);
				}
			}
		}break;
		case VK_LEFT:
		{
			move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
			move_packet->size = sizeof(*move_packet);
			move_packet->type = PLAYER_MOV_END;
			move_packet->move_type = A;
			move_packet->direction = 0;
			send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			if (GetKeyboardState(pKeyBuffer))
			{
				if (!((pKeyBuffer[VK_UP] & 0xF0) || (pKeyBuffer[VK_DOWN] & 0xF0)
					|| (pKeyBuffer[VK_LEFT] & 0xF0) || (pKeyBuffer[VK_RIGHT] & 0xF0)))
				{
					m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);
				}
			}
		}break;
		case VK_RIGHT:
		{
			move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
			move_packet->size = sizeof(*move_packet);
			move_packet->type = PLAYER_MOV_END;
			move_packet->move_type = D;
			move_packet->direction = 0;
			send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			if (GetKeyboardState(pKeyBuffer))
			{
				if (!((pKeyBuffer[VK_UP] & 0xF0) || (pKeyBuffer[VK_DOWN] & 0xF0)
					|| (pKeyBuffer[VK_LEFT] & 0xF0) || (pKeyBuffer[VK_RIGHT] & 0xF0)))
				{
					m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);
				}
			}
		}break;
		case VK_DOWN:
		{
			move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
			move_packet->size = sizeof(*move_packet);
			move_packet->type = PLAYER_MOV_END;
			move_packet->move_type = S;
			move_packet->direction = 0;
			send(sock, (char*)move_packet, sizeof(*move_packet), 0);
			if (GetKeyboardState(pKeyBuffer))
			{
				if (!((pKeyBuffer[VK_UP] & 0xF0) || (pKeyBuffer[VK_DOWN] & 0xF0)
					|| (pKeyBuffer[VK_LEFT] & 0xF0) || (pKeyBuffer[VK_RIGHT] & 0xF0)))
				{
					m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);
				}
			}
		}break;

		case VK_SPACE:
		{
			attack_packet = reinterpret_cast<player_attack*>(send_buffer);
			attack_packet->id = myID;
			attack_packet->size = sizeof(*attack_packet);
			attack_packet->type = PLAYER_ATTACK;
			attack_packet->attack_type = NORMAL_ATTACK;
			send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		}break;
		case 0x31:
		{
			attack_packet = reinterpret_cast<player_attack*>(send_buffer);
			attack_packet->id = myID;
			attack_packet->size = sizeof(*attack_packet);
			attack_packet->type = PLAYER_ATTACK;
			attack_packet->attack_type = PLAYER_SKILL1;
			send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		}break;
		case 0x32:
		{
			attack_packet = reinterpret_cast<player_attack*>(send_buffer);
			attack_packet->id = myID;
			attack_packet->size = sizeof(*attack_packet);
			attack_packet->type = PLAYER_ATTACK;
			attack_packet->attack_type = PLAYER_SKILL2;
			send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		}break;
		case 0x33:
		{
			attack_packet = reinterpret_cast<player_attack*>(send_buffer);
			attack_packet->id = myID;
			attack_packet->size = sizeof(*attack_packet);
			attack_packet->type = PLAYER_ATTACK;
			attack_packet->attack_type = PLAYER_SKILL3;
			send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		}break;

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
	static UCHAR pKeyBuffer[256];
	if (GetKeyboardState(pKeyBuffer))
	{
		if (pKeyBuffer[0x51] & 0xf0)
			m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTimeElapsed);
		if (pKeyBuffer[0x45] & 0xf0)
			m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTimeElapsed);
	}

	if (m_pObjectManager->FindObject(myID))
		m_pCameraManager->GetNowCamera()->Update(m_pObjectManager->FindObject(myID)->GetPosition());
	else
		m_pCameraManager->GetNowCamera()->Update(new D3DXVECTOR3(0, 0, 0));
}

void CFirstScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	CScene::BuildObjects(pd3dDevice);

	ID3D11DeviceContext *pd3dDeviceContext;
	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);

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
	m_pFog->Initialize(pd3dDevice);
}

void CFirstScene::AnimateObjectsAndRender(ID3D11DeviceContext *pd3dDeviceContext, float time)
{
	if (m_pFog->IsInUse())
		m_pFog->Update(pd3dDeviceContext);

	CScene::AnimateObjectsAndRender(pd3dDeviceContext, time);
}






//CFirstScene::CFirstScene()
//{
//}
//CFirstScene::~CFirstScene()
//{
//}
//
//void CFirstScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
//{
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
//		case VK_F3:
//			printf(" 캐릭터 : %.2f, %.2f\n", m_pPlayer->GetObjects()->m_MaxVer.x, m_pPlayer->GetObjects()->m_MaxVer.z);
//			printf(" 몬스터 : %.2f, %.2f\n", m_pMonster->GetObjects()->m_MaxVer.x, m_pMonster->GetObjects()->m_MaxVer.z);
//			break;
//
//		case VK_SPACE:	//평타
//			PressSkillNum = 0;
//			Player_Attack_number = 3;
//			m_pPlayer->SetIsAttack(true);
//			m_pMonster->SetIsAttack(true);
//			m_pMonster->GetObjects()->SetPlayAnimationState(eUNIT_STATE::ATTACK);
//			break;
//		case 0x31:			//1번
//			cout << "1번" << endl;
//			PressSkillNum = 1;
//			Player_Attack_number = 4;
//			m_pPlayer->SetIsAttack(true);
//			m_pMonster->SetIsAttack(true);
//			m_pMonster->GetObjects()->SetPlayAnimationState(eUNIT_STATE::SKILL1);
//			break;
//		case 0x32:			//2번
//			cout << "2번" << endl;
//			PressSkillNum = 2;
//			Player_Attack_number = 5;
//			m_pPlayer->SetIsAttack(true);
//			m_pMonster->SetIsAttack(true);
//			m_pMonster->GetObjects()->SetPlayAnimationState(eUNIT_STATE::SKILL2);
//			break;
//		case 0x33:			//3번
//			cout << "3번" << endl;
//			PressSkillNum = 3;
//			Player_Attack_number = 6;
//			m_pPlayer->SetIsAttack(true);
//			m_pMonster->SetIsAttack(true);
//			m_pMonster->GetObjects()->SetPlayAnimationState(eUNIT_STATE::SKILL3);
//			break;
//		case VK_ESCAPE:
//			::PostQuitMessage(0);
//			break;
//		default:
//			break;
//		}
//		break;
//
//	default:
//		break;
//	}
//}
//void CFirstScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam, float fTimeElapsed)
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
//				m_pCameraManager->GetNowCamera()->RotatebyYaw(-150 * fTimeElapsed);
//			else
//				m_pCameraManager->GetNowCamera()->RotatebyYaw(150 * fTimeElapsed);
//			break;
//		case WM_MOUSEWHEEL:
//			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
//				m_pCameraManager->GetNowCamera()->Zoom(200 * fTimeElapsed);
//			else
//				m_pCameraManager->GetNowCamera()->Zoom(-200 * fTimeElapsed);
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
//void CFirstScene::ProcessInput(float fTimeElapsed)
//{
//	static UCHAR pKeyBuffer[256];
//	DWORD dwDirection = 0;
//	DWORD tempDirection = 0;
//
//	iscoll = false;
//
//	switch (m_OperationMode)
//	{
//	case MODE_MOUSE:			// F1
//	{
//		if (GetKeyboardState(pKeyBuffer))
//		{
//			// 이동
//			if (pKeyBuffer[0x41] & 0xF0) dwDirection |= DIR_LEFT;		// A
//			if (pKeyBuffer[0x44] & 0xF0) dwDirection |= DIR_RIGHT;		// D
//			if (pKeyBuffer[0x57] & 0xF0) dwDirection |= DIR_FORWARD;	// W
//			if (pKeyBuffer[0x53] & 0xF0) dwDirection |= DIR_BACKWARD;	// S
//		}
//
//		/* player state 변경, 방향이 있을경우 달린다.*/
//		if (dwDirection != 0)
//			m_pPlayer->GetObjects()->SetPlayAnimationState(eUNIT_STATE::MOVE);
//		else
//			m_pPlayer->GetObjects()->SetPlayAnimationState(eUNIT_STATE::IDLE);
//
//		if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObjectInCategory(CObjectManager::eObjectType::BUFF_CRYSTAL)))		//아이템 HP 충돌체크
//		{
//			//cout << "나랑 부딪힌 오브젝트 번호 : " << m_pPlayer->GetObjects()->CollOtherID << endl;
//			int tempId = m_pPlayer->GetObjects()->CollOtherID;
//			if (m_pObjectManager->FindObject(tempId)->GetResourceType() == (int)eResourceType::Item_HP)
//			{
//				m_pPlayer->SetHP();
//			}
//
//			else if (m_pObjectManager->FindObject(tempId)->GetResourceType() == (int)eResourceType::Item_Buff)
//			{
//				m_pPlayer->SetSpeed();
//			}
//			m_pObjectManager->DeleteObject(m_pPlayer->GetObjects()->CollOtherID);
//
//			cout << "현재 체력 : " << m_pPlayer->GetHP() << endl;
//			cout << "현재 스피드 : " << m_pPlayer->GetSpeed() << endl;
//		}
//
//
//		if (m_pPlayer->GetIsAttack())		//공격 했을 때 
//		{
//			m_pPlayer->GetObjects()->SetPressSkill(PressSkillNum);	//몇번 스킬 눌렀는지 알려주고
//			m_pPlayer->GetObjects()->SetPlayAnimationState((eUNIT_STATE)Player_Attack_number);
//
//			if (m_pPlayer->GetObjects()->MyHitAndEnemyBound(m_pMonster->GetObjects()))		//내 히트박스 + 상대 충돌체크박스 확인
//			{
//				//cout << "적에게 적중했다!!  체력 : "<<m_pObjectManager->FindObject(20000)->GetHP() << endl;
//				cout << "적을 때렸다!" << endl;
//				//is_Coll = true;	//일단 충돌했다는 것만 알려주기 위해서
//			}
//
//			if (m_pPlayer->GetObjects()->m_fAnimationPlaytime == 0.0f)	//애니메이션이 한바퀴 돌아서 0이 되면, 공격상태를 멈춘다.
//			{
//				//is_Coll = false;			//다음 충돌체크를 위해서 false
//				m_pPlayer->SetIsAttack(false);		//한바퀴 돌아서 공격끝났으니, 공격모션을 끝내주기 위해 false
//			}
//		}
//
//		if (m_pMonster->GetIsAttack())
//		{
//			if (m_pMonster->GetObjects()->m_fAnimationPlaytime == 0.0f)	//애니메이션이 한바퀴 돌아서 0이 되면, 공격상태를 멈춘다.
//			{
//				//is_Coll = false;			//다음 충돌체크를 위해서 false
//				m_pMonster->SetIsAttack(false);		//한바퀴 돌아서 공격끝났으니, 공격모션을 끝내주기 위해 false
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
//			if (pKeyBuffer[VK_UP] & 0xF0)		dwDirection |= DIR_FORWARD;
//			if (pKeyBuffer[VK_DOWN] & 0xF0)		dwDirection |= DIR_BACKWARD;
//			if (pKeyBuffer[VK_LEFT] & 0xF0)		dwDirection |= DIR_LEFT;
//			if (pKeyBuffer[VK_RIGHT] & 0xF0)	dwDirection |= DIR_RIGHT;
//			// 좌우회전
//			if (pKeyBuffer[0x51] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTimeElapsed);			// Q
//			if (pKeyBuffer[0x45] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTimeElapsed);		// E
//			// 줌
//			if (pKeyBuffer[0x5A] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(-100 * fTimeElapsed);				// Z
//			if (pKeyBuffer[0x58] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(100 * fTimeElapsed);				// X
//		}
//		if (dwDirection != 0)
//			m_pPlayer->GetObjects()->SetPlayAnimationState(eUNIT_STATE::MOVE);
//		else
//			m_pPlayer->GetObjects()->SetPlayAnimationState(eUNIT_STATE::IDLE);
//	}
//	break;
//	}
//
//	m_pPlayer->boundingBoxMove(m_pCameraManager->GetNowCamera()->GetYaw(), dwDirection, fTimeElapsed);		//바운딩박스 먼저 이동해서
//
//	if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObjectInCategory(CObjectManager::eObjectType::NATURAL_FEATURE)))		//충돌체크를 한 다음
//	{
//		iscoll = true;
//		//cout << "벽에 닿음" << endl;
//		m_pPlayer->GetObjects()->SetBoundingBox();
//		m_pPlayer->GetObjects()->SetBoundingBoxMatrix();
//
//	}
//
//	if (dwDirection && false == m_pPlayer->GetIsAttack() && !iscoll)
//	{
//		m_pPlayer->Move(m_pCameraManager->GetNowCamera()->GetYaw(), dwDirection, fTimeElapsed);
//	}
//
//	//if (!iscoll)		// 충돌이 아니면 움직인다!.
//	//{
//	//	m_pPlayer->Move(m_pCameraManager->GetNowCamera()->GetYaw(), dwDirection, fTimeElapsed);
//	//}
//	//	
//	m_pCameraManager->GetNowCamera()->Update(m_pPlayer->GetPosition());
//}
//
//void CFirstScene::BuildObjects(ID3D11Device *pd3dDevice)
//{
//	CScene::BuildObjects(pd3dDevice);
//
//	ID3D11DeviceContext *pd3dDeviceContext;
//	pd3dDevice->GetImmediateContext(&pd3dDeviceContext);
//
//	// * NATURAL_FEATURE : 벽, 나무, 돌 등등	    충돌체크o, 삭제x, 이동 못하게 함.		0~999
//	//	* BUFF_CRYSTAL :								충돌체크o, 삭제o,							1000~1999
//	//	* MONSTER : 몬스터 = 보스						충돌체크o, 히트박스o,						2000~2999
//	//	* PLAYER :											충돌체크o, 히트박스o,						3000~3999
//	//	* LAND : 바닥.										충돌체크x, 삭제x                            4000~
//
//	m_pPlayer = new CPlayer();
//	m_pPlayer->SetObject(m_pObjectManager->Insert(3000, eResourceType::User, pd3dDevice, pd3dDeviceContext, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0)));
//	m_pPlayer->GetObjects()->SetPlayAnimationState(eUNIT_STATE::IDLE);
//
//	m_pMonster = new CMonster();
//	m_pMonster->SetObject(m_pObjectManager->Insert(2000, eResourceType::Monster1, pd3dDevice, pd3dDeviceContext, D3DXVECTOR3(-400, 0, 0), D3DXVECTOR3(0, 0, 0)));
//	m_pMonster->GetObjects()->SetPlayAnimationState(0);
//
//	m_pCameraManager = CCameraManager::GetSingleton();
//	m_pCameraManager->GetNowCamera()->SetLookAt(m_pPlayer->GetPosition());
//
//	m_pObjectManager->Insert(10, eResourceType::Floor, D3DXVECTOR3(0, -100, 0));
//
//	/* 아이템 설정 */
//	for (int i = 0; i < 6; ++i)
//		m_pObjectManager->Insert(i + 1000, eResourceType::Item_HP, D3DXVECTOR3(rand() % 400 * i, 50, (rand() % 500 + 10)*i));
//	for (int i = 6; i < 11; ++i)
//		m_pObjectManager->Insert(i + 1000, eResourceType::Item_Buff, D3DXVECTOR3((rand() % 400 - 300) * i, 50, (rand() % 200 - 150)*i));
//
//
//	/*================ 꾸미기 =========================*/
//	m_pObjectManager->Insert(4000, eResourceType::Floor, D3DXVECTOR3(0, 0, 0));		//바닥
//
//	/*맵 설정*/
//	D3DXVECTOR3 WallPos[8];
//	WallPos[0] = D3DXVECTOR3(-1250, 500, 2500);
//	WallPos[1] = D3DXVECTOR3(1250, 500, 2500);
//	WallPos[2] = D3DXVECTOR3(-1250, 500, -2500);
//	WallPos[3] = D3DXVECTOR3(1250, 500, -2500);
//
//	WallPos[4] = D3DXVECTOR3(2500, 500, 1250);
//	WallPos[5] = D3DXVECTOR3(2500, 500, -1250);
//	WallPos[6] = D3DXVECTOR3(-2500, 500, 1250);
//	WallPos[7] = D3DXVECTOR3(-2500, 500, -1250);
//
//	for (int i = 0; i < 9; ++i)		//벽
//	{
//		if (i < 5)
//			m_pObjectManager->Insert(i, eResourceType::Wall1, WallPos[i - 1]);	// ㅡ
//		else
//			m_pObjectManager->Insert(i, eResourceType::Wall1, WallPos[i - 1], D3DXVECTOR3(0, 90, 0));  // ㅣ
//	}
//	m_pObjectManager->Insert(9, eResourceType::Tree, D3DXVECTOR3(-2000, 0, 1900), D3DXVECTOR3(0, 90, 0));
//	m_pObjectManager->Insert(10, eResourceType::Tree, D3DXVECTOR3(-1800, 0, -1900));
//	m_pObjectManager->Insert(11, eResourceType::Tree, D3DXVECTOR3(2200, 0, 1900), D3DXVECTOR3(0, 60, 0));
//	m_pObjectManager->Insert(12, eResourceType::Tree, D3DXVECTOR3(2100, 0, -1900));
//}
//
//void CFirstScene::AnimateObjectsAndRender(ID3D11DeviceContext *pd3dDeviceContext, float time)
//{
//	CScene::AnimateObjectsAndRender(pd3dDeviceContext, time);
//}





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


