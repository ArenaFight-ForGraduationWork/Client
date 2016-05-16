#include "stdafx.h"
#include "Unit.h"


CUnit::CUnit()
{
	m_pObject = nullptr;

	m_fSpeed = 500;
	m_fHP = 100;
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
	//m_pObject->SetBoundingBoxMatrix();
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


	// 충돌이 아닐경우 Move를 하게 해뒀으니 캐릭터행렬은 바뀐다. 그 바뀐행렬을 적용시켜준다.
	// 기존 바운딩행렬로 돌릴 필요가 없는게, setBoundingBox()는 Text의 그 읽어온 자체 값에 계속 곱해주는거다!
	// 미리 충돌체크를 위해 움직여뒀던 바운딩박스용 월드변환행렬을 캐릭터의 월드변환행렬로 바꿔준다.....라고 생각함 근데 안됨ㅎ
	//만약 위에 내 생각대로 하게 되거든, setboundingbox를 해두면 캐릭터월드변환행렬을 적용해줄 수 있다. 라고 생각했지만 안됨.
	//m_pObject->SetBoundingBox();

	m_pObject->MoveAndRotatingHitBox();

}

void CUnit::boundingBoxMove(const float cameraYaw, const DWORD dwDirection, const float fTimeElapsed)
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

	m_pObject->BoundingRotateAbsolute(&D3DXVECTOR3(0, cameraYaw + fAngle, 0));
	m_pObject->BoundingMoveForward(m_fSpeed * fTimeElapsed);

	m_pObject->MoveAndRotatingBoundingBox();
	//printf("현재 바운딩 좌표 : %f %f\n", m_pObject->m_MaxVer.x, m_pObject->m_MaxVer.z);
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
