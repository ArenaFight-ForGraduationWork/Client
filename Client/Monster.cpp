#include "stdafx.h"
#include "Monster.h"


CMonster::CMonster()
{
	m_pObject = nullptr;
	m_fSpeed = 100;
}


CMonster::~CMonster()
{
	m_pObject = nullptr;
}

void CMonster::SetObject(CObject *pObject)
{
	m_pObject = pObject;
}
void CMonster::ReleaseObject()
{
	m_pObject = nullptr;
}

void CMonster::Move(const float cameraYaw, const DWORD dwDirection, const float fTimeElapsed)
{
	// 1) 카메라가 바라보는 방향 + 입력받은 방향 = fAngle를 Yaw값으로 회전
	D3DXVECTOR3 defaultAngle = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 inputAngle = D3DXVECTOR3(0, 0, 0);
	if (dwDirection)
	{
		if (dwDirection & DIR_FORWARD)  inputAngle.z += 1;
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
	m_pObject->MoveForward(m_fSpeed * fTimeElapsed);
}

void CMonster::MoveRelative(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->MoveRelative(x, y, z);
}
void CMonster::MoveRelative(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->MoveRelative(vec);
}
void CMonster::MoveAbsolute(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->MoveAbsolute(x, y, z);
}
void CMonster::MoveAbsolute(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->MoveAbsolute(vec);
}

void CMonster::RotateRelative(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->RotateRelative(x, y, z);
}
void CMonster::RotateRelative(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->RotateRelative(vec);
}
void CMonster::RotateAbsolute(const float x, const float y, const float z)
{
	if (m_pObject)
		m_pObject->RotateAbsolute(x, y, z);
}
void CMonster::RotateAbsolute(const D3DXVECTOR3 *vec)
{
	if (m_pObject)
		m_pObject->RotateAbsolute(vec);
}

const D3DXVECTOR3* CMonster::GetPosition()
{
	// 오브젝트가 있으면 해당 오브젝트의 위치를 반환하고, 없으면 원점을 반환한다
	if (m_pObject)
	{
		return m_pObject->GetPosition();
	}

	return &D3DXVECTOR3(0, 0, 0);
}

