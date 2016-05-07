#include "stdafx.h"
#include "Player.h"


CUnit::CUnit()
{
	m_pObject = nullptr;

	m_fStrikingPower = 10;
	m_fDefensivePower = 10;

	m_fMovingSpeed = 100;

	m_fHp = 100;
}
CUnit::CUnit(CUnit& copy)
{
}
CUnit::~CUnit()
{
	m_pObject = nullptr;
}

void CUnit::SetObject(CObject *pObject)
{
	m_pObject = pObject;
}
void CUnit::ReleaseObject()
{
	m_pObject = nullptr;
}

void CUnit::Move(const float cameraYaw, const DWORD dwDirection, const float fTimeElapsed)
{
	// 1) 카메라가 바라보는 방향 + 입력받은 방향 = fAngle를 Yaw값으로 회전
	D3DXVECTOR3 defaultAngle = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 inputAngle = D3DXVECTOR3(0, 0, 0);
	if (dwDirection)
	{
		if (dwDirection & DIR_FORWARD)  inputAngle.z += 1;	//boundingbox의 m_MinVer, m_MaxVer+=1;을 해주던지 해야함.
		if (dwDirection & DIR_BACKWARD)	inputAngle.z -= 1;
		if (dwDirection & DIR_LEFT)	inputAngle.x += 1;
		if (dwDirection & DIR_RIGHT) inputAngle.x -= 1;
	}
	if (D3DXVECTOR3(0, 0, 0) == inputAngle) return;	// inputAngle==(0,0,0)이면 어느 방향으로든 움직이지 않는다 => 함수 종료
	float fAngle = acosf(D3DXVec3Dot(&defaultAngle, &inputAngle) / (D3DXVec3Length(&defaultAngle) * D3DXVec3Length(&inputAngle)));
	fAngle = D3DXToDegree(fAngle);
	fAngle = ((defaultAngle.x* inputAngle.z - defaultAngle.z*inputAngle.x) > 0.0f) ? fAngle : -fAngle;

	m_pObject->RotateAbsolute(&D3DXVECTOR3(0, cameraYaw + fAngle, 0));

	// 2) 로컬 z축으로 속도 * 시간만큼 이동
	m_pObject->MoveForward(m_fMovingSpeed * fTimeElapsed);
}

void CUnit::MoveRelative(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->MoveRelative(x, y, z);
}
void CUnit::MoveRelative(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->MoveRelative(vec);
}
void CUnit::MoveAbsolute(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->MoveAbsolute(x, y, z);
}
void CUnit::MoveAbsolute(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->MoveAbsolute(vec);
}

void CUnit::RotateRelative(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->RotateRelative(x, y, z);
}
void CUnit::RotateRelative(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->RotateRelative(vec);
}
void CUnit::RotateAbsolute(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->RotateAbsolute(x, y, z);
}
void CUnit::RotateAbsolute(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->RotateAbsolute(vec);
}

const D3DXVECTOR3* CUnit::GetPosition()
{
	// 오브젝트가 있으면 해당 오브젝트의 위치를 반환하고, 없으면 원점을 반환한다
	if (m_pObject)
	{
		return m_pObject->GetPosition();
	}

	return &D3DXVECTOR3(0, 0, 0);
}

void CUnit::AnimateRender(int StateNum, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed)
{
	m_pObject->SetPlayAnimationState(StateNum);
	m_pObject->Animate(StateNum, pd3dDeviceContext, fTimeElapsed);
}




CPlayer::CPlayer()
{}
CPlayer::CPlayer(CPlayer& copy)
{}
CPlayer::CPlayer()
{}








CMonster::CMonster()
{}
CMonster::CMonster(CMonster& copy)
{}
CMonster::~CMonster()
{}




