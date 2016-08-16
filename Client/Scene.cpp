#include "stdafx.h"
#include "Scene.h"
#include "ResourceManager.h"



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

	m_pInterface = nullptr;
	m_pd3dcbCamera = nullptr;
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

	m_pInterface = new CUserInterface();
}

void CScene::ReleaseObjects()
{
	m_vShaders.clear();
}

void CScene::AnimateObjectsAndRender3D(ID3D11DeviceContext *pd3dDeviceContext, float time)
{
	m_pLight->UpdateLights(pd3dDeviceContext);

	for (unsigned int i = 0; i < m_vShaders.size() - 1; ++i)
	{
		m_vShaders[i]->AnimateObjectAndRender(pd3dDeviceContext, time);
	}
	//for (auto shader : m_vShaders)
	//{
	//	shader->AnimateObjectAndRender(pd3dDeviceContext, time);
	//}
}
void CScene::AnimateObjectsAndRender2D(ID3D11DeviceContext *pd3dDeviceContext, float time)
{
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_vShaders[m_vShaders.size() - 1]->UpdateShaderVariables(pd3dDeviceContext, &matrix);

	for (unsigned int i = 0; i < m_pInterface->GetTexture()->GetNumOfTextures(); ++i)
	{
		m_vShaders[m_vShaders.size() - 1]->UpdateShaderVariables(pd3dDeviceContext, m_pInterface->GetTexture());
		m_vShaders[m_vShaders.size() - 1]->AnimateObjectAndRender(pd3dDeviceContext, static_cast<float>(m_pInterface->GetIndexCount()));
	}
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
	packet_player_move *move_packet;
	player_attack* attack_packet;

	switch (nMessageID)
	{
	case WM_KEYDOWN:
	{
		//switch (wParam)
		//{
		//case VK_UP:
		//{
		//	if (server_on) {
		//		move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
		//		move_packet->size = sizeof(*move_packet);
		//		move_packet->type = PLAYER_MOV;
		//		move_packet->move_type = W;
		//		move_packet->direction = m_pCameraManager->GetNowCamera()->GetYaw();
		//		send(sock, (char*)move_packet, sizeof(*move_packet), 0);
		//		m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Move);
		//	}
		//}break;
		//default:break;
		//}
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

		//case VK_UP:
		//{
		//	move_packet = reinterpret_cast<packet_player_move*>(send_buffer);
		//	move_packet->size = sizeof(*move_packet);
		//	move_packet->type = PLAYER_MOV_END;
		//	move_packet->move_type = W;
		//	move_packet->direction = 0;
		//	send(sock, (char*)move_packet, sizeof(*move_packet), 0);
		//	if (GetKeyboardState(pKeyBuffer))
		//	{
		//		if (!((pKeyBuffer[VK_UP] & 0xF0) || (pKeyBuffer[VK_DOWN] & 0xF0)
		//			|| (pKeyBuffer[VK_LEFT] & 0xF0) || (pKeyBuffer[VK_RIGHT] & 0xF0)))
		//		{
		//			m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);
		//		}
		//	}
		//}break;

		//case VK_SPACE:
		//{
		//	attack_packet = reinterpret_cast<player_attack*>(send_buffer);
		//	attack_packet->id = myID;
		//	attack_packet->size = sizeof(*attack_packet);
		//	attack_packet->type = PLAYER_ATTACK;
		//	attack_packet->attack_type = NORMAL_ATTACK;
		//	send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		//}break;
		//case 0x31:	// 1
		//{
		//	attack_packet = reinterpret_cast<player_attack*>(send_buffer);
		//	attack_packet->id = myID;
		//	attack_packet->size = sizeof(*attack_packet);
		//	attack_packet->type = PLAYER_ATTACK;
		//	attack_packet->attack_type = PLAYER_SKILL1;
		//	send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		//}break;
		//case 0x32:	// 2
		//{
		//	attack_packet = reinterpret_cast<player_attack*>(send_buffer);
		//	attack_packet->id = myID;
		//	attack_packet->size = sizeof(*attack_packet);
		//	attack_packet->type = PLAYER_ATTACK;
		//	attack_packet->attack_type = PLAYER_SKILL2;
		//	send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		//}break;
		//case 0x33:	// 3
		//{
		//	attack_packet = reinterpret_cast<player_attack*>(send_buffer);
		//	attack_packet->id = myID;
		//	attack_packet->size = sizeof(*attack_packet);
		//	attack_packet->type = PLAYER_ATTACK;
		//	attack_packet->attack_type = PLAYER_SKILL3;
		//	send(sock, (char*)attack_packet, sizeof(*attack_packet), 0);
		//}break;

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

				if ((!m_dwDirectionPrev) && (m_dwDirectionNow))
				{
					/* send move_start_packet */
				}
				else if ((m_dwDirectionPrev) && (!m_dwDirectionNow))
				{
					/* send move_end_packet */
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
				}
				else
					m_pObjectManager->FindObject(myID)->PlayAnimation(CObject::eAnimationType::Idle);
			}
		}
		// �¿�ȸ��
		if (m_pKeyBuffer[0x51] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTimeElapsed);	// Q
		if (m_pKeyBuffer[0x45] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTimeElapsed);	// E
		// ��
		if (m_pKeyBuffer[0x5A] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(-100 * fTimeElapsed);			// Z
		if (m_pKeyBuffer[0x58] & 0xF0) m_pCameraManager->GetNowCamera()->Zoom(100 * fTimeElapsed);			// X
	}

	if (pPlayer)
		m_pCameraManager->GetNowCamera()->Update(pPlayer->GetPosition());
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
	m_pFog->Initialize(pd3dDevice);

	// create interface object
	m_pInterface = new CUserInterface();
	m_pInterface->Initialize(pd3dDevice, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT);
	m_pInterface->SetTexture(pd3dDevice, L"./Data/UI/health.png", 20, 20);	// ũ��

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
}

void CFirstScene::AnimateObjectsAndRender3D(ID3D11DeviceContext *pd3dDeviceContext, float time)
{
	if (m_pFog->IsInUse())
		m_pFog->Update(pd3dDeviceContext);

	CScene::AnimateObjectsAndRender3D(pd3dDeviceContext, time);
}

void CFirstScene::AnimateObjectsAndRender2D(ID3D11DeviceContext *pd3dDeviceContext, float time)
{	
	// turn off the z buffer ** first **

	m_pInterface->ChangeSize(FRAME_BUFFER_WIDTH / 20, m_pObjectManager->FindObject(myID)->GetComponent()->GetHealthPoint());
	m_pInterface->Render(pd3dDeviceContext, FRAME_BUFFER_WIDTH / 20 * 18, FRAME_BUFFER_HEIGHT / 20);	// ��ġ

	// set view + projection matrix buffer
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_VIEWPROJECTION_MATRIX *pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX *)d3dMappedResource.pData;
	D3DXMATRIX matrix;	D3DXMatrixIdentity(&matrix);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, &matrix);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, m_pCameraManager->GetNowCamera()->GetOrthoMatrix());
	pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);
	pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbCamera);

	CScene::AnimateObjectsAndRender2D(pd3dDeviceContext, time);

	// turn ** on ** the z buffer
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
//			printf(" ĳ���� : %.2f, %.2f\n", m_pPlayer->GetObjects()->m_MaxVer.x, m_pPlayer->GetObjects()->m_MaxVer.z);
//			printf(" ���� : %.2f, %.2f\n", m_pMonster->GetObjects()->m_MaxVer.x, m_pMonster->GetObjects()->m_MaxVer.z);
//			break;
//
//		case VK_SPACE:	//��Ÿ
//			PressSkillNum = 0;
//			Player_Attack_number = 3;
//			m_pPlayer->SetIsAttack(true);
//			m_pMonster->SetIsAttack(true);
//			m_pMonster->GetObjects()->SetPlayAnimationState(eUNIT_STATE::ATTACK);
//			break;
//		case 0x31:			//1��
//			cout << "1��" << endl;
//			PressSkillNum = 1;
//			Player_Attack_number = 4;
//			m_pPlayer->SetIsAttack(true);
//			m_pMonster->SetIsAttack(true);
//			m_pMonster->GetObjects()->SetPlayAnimationState(eUNIT_STATE::SKILL1);
//			break;
//		case 0x32:			//2��
//			cout << "2��" << endl;
//			PressSkillNum = 2;
//			Player_Attack_number = 5;
//			m_pPlayer->SetIsAttack(true);
//			m_pMonster->SetIsAttack(true);
//			m_pMonster->GetObjects()->SetPlayAnimationState(eUNIT_STATE::SKILL2);
//			break;
//		case 0x33:			//3��
//			cout << "3��" << endl;
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
//			// �̵�
//			if (pKeyBuffer[0x41] & 0xF0) dwDirection |= DIR_LEFT;		// A
//			if (pKeyBuffer[0x44] & 0xF0) dwDirection |= DIR_RIGHT;		// D
//			if (pKeyBuffer[0x57] & 0xF0) dwDirection |= DIR_FORWARD;	// W
//			if (pKeyBuffer[0x53] & 0xF0) dwDirection |= DIR_BACKWARD;	// S
//		}
//
//		/* player state ����, ������ ������� �޸���.*/
//		if (dwDirection != 0)
//			m_pPlayer->GetObjects()->SetPlayAnimationState(eUNIT_STATE::MOVE);
//		else
//			m_pPlayer->GetObjects()->SetPlayAnimationState(eUNIT_STATE::IDLE);
//
//		if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObjectInCategory(CObjectManager::eObjectType::BUFF_CRYSTAL)))		//������ HP �浹üũ
//		{
//			//cout << "���� �ε��� ������Ʈ ��ȣ : " << m_pPlayer->GetObjects()->CollOtherID << endl;
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
//			cout << "���� ü�� : " << m_pPlayer->GetHP() << endl;
//			cout << "���� ���ǵ� : " << m_pPlayer->GetSpeed() << endl;
//		}
//
//
//		if (m_pPlayer->GetIsAttack())		//���� ���� �� 
//		{
//			m_pPlayer->GetObjects()->SetPressSkill(PressSkillNum);	//��� ��ų �������� �˷��ְ�
//			m_pPlayer->GetObjects()->SetPlayAnimationState((eUNIT_STATE)Player_Attack_number);
//
//			if (m_pPlayer->GetObjects()->MyHitAndEnemyBound(m_pMonster->GetObjects()))		//�� ��Ʈ�ڽ� + ��� �浹üũ�ڽ� Ȯ��
//			{
//				//cout << "������ �����ߴ�!!  ü�� : "<<m_pObjectManager->FindObject(20000)->GetHP() << endl;
//				cout << "���� ���ȴ�!" << endl;
//				//is_Coll = true;	//�ϴ� �浹�ߴٴ� �͸� �˷��ֱ� ���ؼ�
//			}
//
//			if (m_pPlayer->GetObjects()->m_fAnimationPlaytime == 0.0f)	//�ִϸ��̼��� �ѹ��� ���Ƽ� 0�� �Ǹ�, ���ݻ��¸� �����.
//			{
//				//is_Coll = false;			//���� �浹üũ�� ���ؼ� false
//				m_pPlayer->SetIsAttack(false);		//�ѹ��� ���Ƽ� ���ݳ�������, ���ݸ���� �����ֱ� ���� false
//			}
//		}
//
//		if (m_pMonster->GetIsAttack())
//		{
//			if (m_pMonster->GetObjects()->m_fAnimationPlaytime == 0.0f)	//�ִϸ��̼��� �ѹ��� ���Ƽ� 0�� �Ǹ�, ���ݻ��¸� �����.
//			{
//				//is_Coll = false;			//���� �浹üũ�� ���ؼ� false
//				m_pMonster->SetIsAttack(false);		//�ѹ��� ���Ƽ� ���ݳ�������, ���ݸ���� �����ֱ� ���� false
//			}
//		}
//	}
//	break;
//
//	case MODE_KEYBOARD:
//	{
//		if (GetKeyboardState(pKeyBuffer))
//		{
//			// �̵�
//			if (pKeyBuffer[VK_UP] & 0xF0)		dwDirection |= DIR_FORWARD;
//			if (pKeyBuffer[VK_DOWN] & 0xF0)		dwDirection |= DIR_BACKWARD;
//			if (pKeyBuffer[VK_LEFT] & 0xF0)		dwDirection |= DIR_LEFT;
//			if (pKeyBuffer[VK_RIGHT] & 0xF0)	dwDirection |= DIR_RIGHT;
//			// �¿�ȸ��
//			if (pKeyBuffer[0x51] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(100 * fTimeElapsed);			// Q
//			if (pKeyBuffer[0x45] & 0xF0) m_pCameraManager->GetNowCamera()->RotatebyYaw(-100 * fTimeElapsed);		// E
//			// ��
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
//	m_pPlayer->boundingBoxMove(m_pCameraManager->GetNowCamera()->GetYaw(), dwDirection, fTimeElapsed);		//�ٿ���ڽ� ���� �̵��ؼ�
//
//	if (m_pPlayer->GetObjects()->Collison(m_pObjectManager->FindObjectInCategory(CObjectManager::eObjectType::NATURAL_FEATURE)))		//�浹üũ�� �� ����
//	{
//		iscoll = true;
//		//cout << "���� ����" << endl;
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
//	//if (!iscoll)		// �浹�� �ƴϸ� �����δ�!.
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
//	// * NATURAL_FEATURE : ��, ����, �� ���	    �浹üũo, ����x, �̵� ���ϰ� ��.		0~999
//	//	* BUFF_CRYSTAL :								�浹üũo, ����o,							1000~1999
//	//	* MONSTER : ���� = ����						�浹üũo, ��Ʈ�ڽ�o,						2000~2999
//	//	* PLAYER :											�浹üũo, ��Ʈ�ڽ�o,						3000~3999
//	//	* LAND : �ٴ�.										�浹üũx, ����x                            4000~
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
//	/* ������ ���� */
//	for (int i = 0; i < 6; ++i)
//		m_pObjectManager->Insert(i + 1000, eResourceType::Item_HP, D3DXVECTOR3(rand() % 400 * i, 50, (rand() % 500 + 10)*i));
//	for (int i = 6; i < 11; ++i)
//		m_pObjectManager->Insert(i + 1000, eResourceType::Item_Buff, D3DXVECTOR3((rand() % 400 - 300) * i, 50, (rand() % 200 - 150)*i));
//
//
//	/*================ �ٹ̱� =========================*/
//	m_pObjectManager->Insert(4000, eResourceType::Floor, D3DXVECTOR3(0, 0, 0));		//�ٴ�
//
//	/*�� ����*/
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
//	for (int i = 0; i < 9; ++i)		//��
//	{
//		if (i < 5)
//			m_pObjectManager->Insert(i, eResourceType::Wall1, WallPos[i - 1]);	// ��
//		else
//			m_pObjectManager->Insert(i, eResourceType::Wall1, WallPos[i - 1], D3DXVECTOR3(0, 90, 0));  // ��
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


