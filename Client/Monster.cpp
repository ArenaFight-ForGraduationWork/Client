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
	// 1) ī�޶� �ٶ󺸴� ���� + �Է¹��� ���� = fAngle�� Yaw������ ȸ��
	D3DXVECTOR3 defaultAngle = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 inputAngle = D3DXVECTOR3(0, 0, 0);
	if (dwDirection)
	{
		if (dwDirection & DIR_FORWARD)  inputAngle.z += 1;
		if (dwDirection & DIR_BACKWARD)	inputAngle.z -= 1;
		if (dwDirection & DIR_LEFT)	inputAngle.x += 1;
		if (dwDirection & DIR_RIGHT) inputAngle.x -= 1;
	}
	if (D3DXVECTOR3(0, 0, 0) == inputAngle) return;	// inputAngle==(0,0,0)�̸� ��� �������ε� �������� �ʴ´� => �Լ� ����
	float fAngle = acosf(D3DXVec3Dot(&defaultAngle, &inputAngle) / (D3DXVec3Length(&defaultAngle) * D3DXVec3Length(&inputAngle)));
	fAngle = D3DXToDegree(fAngle);
	fAngle = ((defaultAngle.x* inputAngle.z - defaultAngle.z*inputAngle.x) > 0.0f) ? fAngle : -fAngle;


	m_pObject->RotateAbsolute(&D3DXVECTOR3(0, cameraYaw + fAngle, 0));

	// 2) ���� z������ �ӵ� * �ð���ŭ �̵�
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
	// ������Ʈ�� ������ �ش� ������Ʈ�� ��ġ�� ��ȯ�ϰ�, ������ ������ ��ȯ�Ѵ�
	if (m_pObject)
	{
		return m_pObject->GetPosition();
	}

	return &D3DXVECTOR3(0, 0, 0);
}

