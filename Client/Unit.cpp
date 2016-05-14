#include "stdafx.h"
#include "Unit.h"


CUnit::CUnit()
{
	m_pObject = nullptr;

<<<<<<< HEAD:Client/Player.cpp
	m_fSpeed = 300;
	m_fHP = 100;
}
=======
	m_fStrikingPower = 10;
	m_fDefensivePower = 10;
>>>>>>> master:Client/Unit.cpp

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
<<<<<<< HEAD:Client/Player.cpp
	m_pObject->MoveForward(m_fSpeed * fTimeElapsed);


	// �浹�� �ƴҰ�� Move�� �ϰ� �ص����� ĳ��������� �ٲ��. �� �ٲ������ ��������ش�.
	// ���� �ٿ����ķ� ���� �ʿ䰡 ���°�, setBoundingBox()�� Text�� �� �о�� ��ü ���� ��� �����ִ°Ŵ�!
	// �̸� �浹üũ�� ���� �������״� �ٿ���ڽ��� ���庯ȯ����� ĳ������ ���庯ȯ��ķ� �ٲ��ش�.....��� ������ �ٵ� �ȵʤ�
	m_pObject->SetBoundingBox();		//���� ���� �� ������� �ϰ� �ǰŵ�, setboundingbox�� �صθ� ĳ���Ϳ��庯ȯ����� �������� �� �ִ�.
	m_pObject->MoveAndRotatingHitBox();

}

void CPlayer::boundingBoxMove(const float cameraYaw, const DWORD dwDirection, const float fTimeElapsed)
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

	m_pObject->BoundingRotateAbsolute(&D3DXVECTOR3(0, cameraYaw + fAngle, 0));
	m_pObject->BoundingMoveForward(m_fSpeed * fTimeElapsed);


	m_pObject->MoveAndRotatingBoundingBox();
=======
	m_pObject->MoveForward(m_fMovingSpeed * fTimeElapsed);
>>>>>>> master:Client/Unit.cpp
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
	// ������Ʈ�� ������ �ش� ������Ʈ�� ��ġ�� ��ȯ�ϰ�, ������ ������ ��ȯ�Ѵ�
	if (m_pObject)
	{
		return m_pObject->GetPosition();
	}

	return &D3DXVECTOR3(0, 0, 0);
}
<<<<<<< HEAD:Client/Player.cpp
=======

void CUnit::AnimateRender(int StateNum, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed)
{
	m_pObject->SetPlayAnimationState(StateNum);
	m_pObject->Animate(StateNum, pd3dDeviceContext, fTimeElapsed);
}




CPlayer::CPlayer()
{}
CPlayer::CPlayer(CPlayer& copy)
{}
CPlayer::~CPlayer()
{}








CMonster::CMonster()
{}
CMonster::CMonster(CMonster& copy)
{}
CMonster::~CMonster()
{}




>>>>>>> master:Client/Unit.cpp
