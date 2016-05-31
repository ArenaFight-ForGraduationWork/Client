#include "stdafx.h"
#include "Object.h"

CMaterial::CMaterial()
{
	m_nReferences = 1;
	m_pMaterial = new MATERIAL();
}
CMaterial::~CMaterial()
{
}

void CMaterial::AddRef()
{
	m_nReferences++;
}

void CMaterial::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences == 0) delete this;
}


CTexture::CTexture(int nTextures)
{
	m_nReferences = 1;
	m_nTextures = nTextures;

	m_ppd3dsrvTextures = new ID3D11ShaderResourceView*[m_nTextures];
	for (int i = 0; i < m_nTextures; ++i) m_ppd3dsrvTextures[i] = nullptr;
	m_ppd3dSamplerStates = new ID3D11SamplerState*[m_nTextures];
	for (int i = 0; i < m_nTextures; ++i) m_ppd3dSamplerStates[i] = nullptr;
}

CTexture::~CTexture()
{
	if (m_ppd3dsrvTextures) delete[] m_ppd3dsrvTextures;
	if (m_ppd3dSamplerStates) delete[] m_ppd3dSamplerStates;
}

void CTexture::AddRef()
{
	m_nReferences++;
	for (int i = 0; i < m_nTextures; ++i)
	{
		if (m_ppd3dsrvTextures[i]) m_ppd3dsrvTextures[i]->AddRef();
		if (m_ppd3dSamplerStates[i]) m_ppd3dSamplerStates[i]->AddRef();
	}
}

void CTexture::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	for (int i = 0; i < m_nTextures; ++i)
	{
		if (m_ppd3dsrvTextures[i]) m_ppd3dsrvTextures[i]->Release();
		if (m_ppd3dSamplerStates[i]) m_ppd3dSamplerStates[i]->Release();
	}
	if (m_nReferences == 0) delete this;
}

void CTexture::SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture, ID3D11SamplerState *pd3dSamplerState)
{
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = pd3dsrvTexture;
	m_ppd3dSamplerStates[nIndex] = pd3dSamplerState;
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->AddRef();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->AddRef();
}

CObject::CObject(UINT id)
{
	m_pd3dxWorldMatrix = new D3DXMATRIX();
	D3DXMatrixIdentity(m_pd3dxWorldMatrix);

	m_boundingWorldMatrix = new D3DXMATRIX();
	D3DXMatrixIdentity(m_boundingWorldMatrix);

	m_pMesh = nullptr;
	m_pMaterial = nullptr;
	m_pTexture = nullptr;

	m_id = id;

	g_pd3dcbBoneMatrix = nullptr;
	g_pcbBoneMatrix = nullptr;

	for (int i = 0; i < ATTACK_COUNT; ++i){
		m_HitMaxVer[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_HitMinVer[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int i = 0; i < ANIMATION_COUNT; ++i)
		m_AniMaxTime[i] = 0.0f;
}

void CObject::SetResourceType(int type)
{
	eSourceType = type;
}

CObject::~CObject()
{
	if (m_pMesh) m_pMesh->Release();
	if (m_pMaterial) m_pMaterial->Release();
	if (m_pTexture) m_pTexture->Release();

}

void CObject::SetMesh(CMesh *pMesh)
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = pMesh;
	if (m_pMesh) m_pMesh->AddRef();
}
void CObject::SetMaterial(CMaterial *pMaterial)
{
	if (m_pMaterial) m_pMaterial->Release();
	m_pMaterial = pMaterial;
	if (m_pMaterial) m_pMaterial->AddRef();
}

void CObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}

void CObject::MoveRelative(const float fx, const float fy, const float fz)
{
	D3DXVECTOR3 d3dxvPosition = *GetPosition();

	D3DXVECTOR3 d3dxvRight = *GetRight();
	D3DXVECTOR3 d3dxvUp = *GetUp();
	D3DXVECTOR3 d3dxvLookAt = *GetLookAt();

	d3dxvPosition += fx * d3dxvRight;
	d3dxvPosition += fy * d3dxvUp;
	d3dxvPosition += fz * d3dxvLookAt;

	MoveAbsolute(&d3dxvPosition);
}
void CObject::MoveRelative(const D3DXVECTOR3 *d3dxVec)
{
	D3DXVECTOR3 d3dxvPosition = *GetPosition();

	D3DXVECTOR3 d3dxvRight = *GetRight();
	D3DXVECTOR3 d3dxvUp = *GetUp();
	D3DXVECTOR3 d3dxvLookAt = *GetLookAt();

	d3dxvPosition += d3dxVec->x * d3dxvRight;
	d3dxvPosition += d3dxVec->y * d3dxvUp;
	d3dxvPosition += d3dxVec->z * d3dxvLookAt;

	MoveAbsolute(&d3dxvPosition);
}
void CObject::MoveAbsolute(const float fx, const float fy, const float fz)
{
	m_pd3dxWorldMatrix->_41 = fx;
	m_pd3dxWorldMatrix->_42 = fy;
	m_pd3dxWorldMatrix->_43 = fz;
}
void CObject::MoveAbsolute(const D3DXVECTOR3 *d3dxVec)
{
	m_pd3dxWorldMatrix->_41 = d3dxVec->x;
	m_pd3dxWorldMatrix->_42 = d3dxVec->y;
	m_pd3dxWorldMatrix->_43 = d3dxVec->z;

}

void CObject::MoveForward(const float fDistance)
{
	D3DXVECTOR3 d3dxvPosition = *GetPosition();
	D3DXVECTOR3 d3dxvLookAt = *GetLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	MoveAbsolute(&d3dxvPosition);
}



void CObject::BoundingMoveForward(const float fDistance)
{
	D3DXVECTOR3 d3dxvTempPosition = *GetPosition();
	D3DXVECTOR3 d3dxvTempLookAt = *GetLookAt();
	d3dxvTempPosition += fDistance * d3dxvTempLookAt;
	//cout << d3dxvTempPosition.x << ", " << d3dxvTempPosition.z << endl;
	BoundingMoveAbsolute(&d3dxvTempPosition);
}

void CObject::BoundingMoveAbsolute(const D3DXVECTOR3 *d3dxVec)
{
	m_boundingWorldMatrix->_41 = d3dxVec->x;
	m_boundingWorldMatrix->_42 = d3dxVec->y;
	m_boundingWorldMatrix->_43 = d3dxVec->z;
	//printf(" 바운딩 : %f, %f\n", m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_43);
}

void CObject::BoundingRotateAbsolute(const D3DXVECTOR3 *d3dxVec)
{
	// 1) 회전각을 0,0,0으로 되돌리기 = 현재 회전행렬 얻어오기 > 행렬을 역행렬로 바꾸기 > 역행렬을 현재 월드변환행렬에 곱해주기
	D3DXMATRIX mtxPresentRotation = (*_GetBoundingRotationMatrix());
	D3DXMatrixInverse(&mtxPresentRotation, 0, &mtxPresentRotation);
	(*m_boundingWorldMatrix) = mtxPresentRotation * (*m_boundingWorldMatrix);

	// 2) d3dxVec만큼 회전하기
	BoundingRotateRelative(d3dxVec);
}
void CObject::BoundingRotateRelative(const D3DXVECTOR3 *d3dxVec)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(d3dxVec->y), (float)D3DXToRadian(d3dxVec->x), (float)D3DXToRadian(d3dxVec->z));
	(*m_boundingWorldMatrix) = mtxRotate * (*m_boundingWorldMatrix);
}

const D3DXMATRIX* CObject::_GetBoundingRotationMatrix()
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixIdentity(&mtxRotate);

	m_boundingWorldMatrix->_11 = mtxRotate._11;
	m_boundingWorldMatrix->_12 = mtxRotate._12;
	m_boundingWorldMatrix->_13 = mtxRotate._13;
	m_boundingWorldMatrix->_21 = mtxRotate._21;
	m_boundingWorldMatrix->_22 = mtxRotate._22;
	m_boundingWorldMatrix->_23 = mtxRotate._23;
	m_boundingWorldMatrix->_31 = mtxRotate._31;
	m_boundingWorldMatrix->_32 = mtxRotate._32;
	m_boundingWorldMatrix->_33 = mtxRotate._33;

	return &mtxRotate;
}

const D3DXVECTOR3* CObject::GetBoundingPosition()
{
	return &D3DXVECTOR3(m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_42, m_boundingWorldMatrix->_43);
}
const D3DXVECTOR3* CObject::GetBoundingLookAt()
{
	D3DXVECTOR3 d3dxvLookAt(m_boundingWorldMatrix->_31, m_boundingWorldMatrix->_32, m_boundingWorldMatrix->_33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return &d3dxvLookAt;
}


void CObject::RotateRelative(const float fPitch, const float fYaw, const float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	(*m_pd3dxWorldMatrix) = mtxRotate * (*m_pd3dxWorldMatrix);
}
void CObject::RotateRelative(const D3DXVECTOR3 *d3dxVec)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(d3dxVec->y), (float)D3DXToRadian(d3dxVec->x), (float)D3DXToRadian(d3dxVec->z));
	(*m_pd3dxWorldMatrix) = mtxRotate * (*m_pd3dxWorldMatrix);
}
void CObject::RotateRelative(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	(*m_pd3dxWorldMatrix) = mtxRotate * (*m_pd3dxWorldMatrix);
}
void CObject::RotateAbsolute(const float fPitch, const float fYaw, const float fRoll)
{
	// 1) 회전각을 0,0,0으로 되돌리기 = 현재 회전행렬 얻어오기 > 행렬을 역행렬로 바꾸기 > 역행렬을 현재 월드변환행렬에 곱해주기
	D3DXMATRIX mtxPresentRotation = (*_GetRotationMatrix());
	D3DXMatrixInverse(&mtxPresentRotation, 0, &mtxPresentRotation);
	(*m_pd3dxWorldMatrix) = mtxPresentRotation * (*m_pd3dxWorldMatrix);

	// 2) fPitch, fYaw, fRoll로 회전하기
	RotateRelative(fPitch, fYaw, fRoll);
}
void CObject::RotateAbsolute(const D3DXVECTOR3 *d3dxVec)  // 0, x, 0    y축기준
{
	// 1) 회전각을 0,0,0으로 되돌리기 = 현재 회전행렬 얻어오기 > 행렬을 역행렬로 바꾸기 > 역행렬을 현재 월드변환행렬에 곱해주기
	D3DXMATRIX mtxPresentRotation = (*_GetRotationMatrix());
	D3DXMatrixInverse(&mtxPresentRotation, 0, &mtxPresentRotation);
	(*m_pd3dxWorldMatrix) = mtxPresentRotation * (*m_pd3dxWorldMatrix);

	// 2) d3dxVec만큼 회전하기
	RotateRelative(d3dxVec);
}
void CObject::RotateAbsolute(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle)
{
	// 1) 회전각을 0,0,0으로 되돌리기 = 현재 회전행렬 얻어오기 > 행렬을 역행렬로 바꾸기 > 역행렬을 현재 월드변환행렬에 곱해주기
	D3DXMATRIX mtxPresentRotation = (*_GetRotationMatrix());
	D3DXMatrixInverse(&mtxPresentRotation, 0, &mtxPresentRotation);
	(*m_pd3dxWorldMatrix) = mtxPresentRotation * (*m_pd3dxWorldMatrix);

	// 2) fPitch, fYaw, fRoll로 회전하기
	RotateRelative(pd3dxvAxis, fAngle);
}

const D3DXVECTOR3* CObject::GetPosition()
{
	return &D3DXVECTOR3(m_pd3dxWorldMatrix->_41, m_pd3dxWorldMatrix->_42, m_pd3dxWorldMatrix->_43);
}

const D3DXVECTOR3* CObject::GetRight()
{
	D3DXVECTOR3 d3dxvRight(m_pd3dxWorldMatrix->_11, m_pd3dxWorldMatrix->_12, m_pd3dxWorldMatrix->_13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	return &d3dxvRight;
}

const D3DXVECTOR3* CObject::GetUp()
{
	D3DXVECTOR3 d3dxvUp(m_pd3dxWorldMatrix->_21, m_pd3dxWorldMatrix->_22, m_pd3dxWorldMatrix->_23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return &d3dxvUp;
}

const D3DXVECTOR3* CObject::GetLookAt()
{
	D3DXVECTOR3 d3dxvLookAt(m_pd3dxWorldMatrix->_31, m_pd3dxWorldMatrix->_32, m_pd3dxWorldMatrix->_33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return &d3dxvLookAt;
}

void CObject::SetTexture(CTexture *pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

const D3DXMATRIX* CObject::_GetRotationMatrix()
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixIdentity(&mtxRotate);

	m_pd3dxWorldMatrix->_11 = mtxRotate._11;
	m_pd3dxWorldMatrix->_12 = mtxRotate._12;
	m_pd3dxWorldMatrix->_13 = mtxRotate._13;
	m_pd3dxWorldMatrix->_21 = mtxRotate._21;
	m_pd3dxWorldMatrix->_22 = mtxRotate._22;
	m_pd3dxWorldMatrix->_23 = mtxRotate._23;
	m_pd3dxWorldMatrix->_31 = mtxRotate._31;
	m_pd3dxWorldMatrix->_32 = mtxRotate._32;
	m_pd3dxWorldMatrix->_33 = mtxRotate._33;

	return &mtxRotate;
}

void CObject::SetPosition(D3DXVECTOR3* pos)
{
	m_pd3dxWorldMatrix->_41 = pos->x;
	m_pd3dxWorldMatrix->_42 = pos->y;
	m_pd3dxWorldMatrix->_43 = pos->z;
}

//===========================================================
//		충돌박스, 히트박스를 위한 함수들
//		1) MyHitAndEnemyBound : 내 히트박스 + 몬스터 충돌박스 = 몬스터 체력 감소
//		2) EnemyHitAndMyBound : 적 히트박스 + 내 충돌박스 = 내 체력 감소
//		3) Collison : 내 충돌박스 + 아이템일때만 충돌박스 = 아이템 효과 발동
//===========================================================
void CObject::SetBoundingBox()
{
	m_MaxVer = this->m_pMesh->GetMaxVer();
	m_MinVer = this->m_pMesh->GetMinVer();

	//위치값 변환
	D3DXVec3TransformCoord(&m_MaxVer, &m_MaxVer, GetWorldMatrix());		// 연산의 결과가 저장될 벡터의 주소값, 연산을 수행할 3차원 벡터값, 연산을 수행할 4X4행렬의 주소값
	D3DXVec3TransformCoord(&m_MinVer, &m_MinVer, GetWorldMatrix());

	if (m_MaxVer.x < m_MinVer.x)
	{
		float temp;
		temp = m_MaxVer.x;
		m_MaxVer.x = m_MinVer.x;
		m_MinVer.x = temp;
	}
	if (m_MaxVer.y < m_MinVer.y)
	{
		float temp;
		temp = m_MaxVer.y;
		m_MaxVer.y = m_MinVer.y;
		m_MinVer.y = temp;
	}
	if (m_MaxVer.z < m_MinVer.z)
	{
		float temp;
		temp = m_MaxVer.z;
		m_MaxVer.z = m_MinVer.z;
		m_MinVer.z = temp;
	}

	//printf("ID : %d, max : %f, %f, %f\n", this->GetId(), m_MaxVer.x, m_MaxVer.y, m_MaxVer.z);
	//printf("ID : %d, min : %f, %f, %f\n", this->GetId(), m_MinVer.x, m_MinVer.y, m_MinVer.z);

}
void CObject::SetBoundingBoxMatrix()
{
	//printf(" 설정 전 월드변환 : %f, %f\n", m_pd3dxWorldMatrix->_41, m_pd3dxWorldMatrix->_43);
	m_boundingWorldMatrix->_11 = m_pd3dxWorldMatrix->_11;
	m_boundingWorldMatrix->_12 = m_pd3dxWorldMatrix->_12;
	m_boundingWorldMatrix->_13 = m_pd3dxWorldMatrix->_13;
	m_boundingWorldMatrix->_21 = m_pd3dxWorldMatrix->_21;
	m_boundingWorldMatrix->_22 = m_pd3dxWorldMatrix->_22;
	m_boundingWorldMatrix->_23 = m_pd3dxWorldMatrix->_23;
	m_boundingWorldMatrix->_31 = m_pd3dxWorldMatrix->_31;
	m_boundingWorldMatrix->_32 = m_pd3dxWorldMatrix->_32;
	m_boundingWorldMatrix->_33 = m_pd3dxWorldMatrix->_33;
	m_boundingWorldMatrix->_41 = m_pd3dxWorldMatrix->_41;
	m_boundingWorldMatrix->_42 = m_pd3dxWorldMatrix->_42;
	m_boundingWorldMatrix->_43 = m_pd3dxWorldMatrix->_43;
	//printf(" 설정 후 바운딩 월드변환 : %f, %f\n", m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_43);
}
void CObject::SetHitBox()
{
	/*
	공격별로 히트박스 크기가 다 다름. 그래서 m_HitMaxVer[공격모션개수] 이런식으로 만들어줌
	1) 내 히트박스 + 몬스터 충돌박스 = 몬스터 체력 감소
	2) 몬스터 히트박스 + 내 충돌박스 = 내 체력 감소
	3) 내 충돌박스 + 아이템 충돌박스 = 아이템 삭제, 효과 적용
	*/
	for (int i = 0; i < ATTACK_COUNT; ++i)
	{
		m_HitMaxVer[i] = this->m_pMesh->m_HitMaxVer[i];
		m_HitMinVer[i] = this->m_pMesh->m_HitMinVer[i];
		D3DXVec3TransformCoord(&m_HitMaxVer[i], &m_HitMaxVer[i], GetWorldMatrix());
		D3DXVec3TransformCoord(&m_HitMinVer[i], &m_HitMinVer[i], GetWorldMatrix());

		if (m_HitMaxVer[i].x < m_HitMinVer[i].x)
		{
			float temp;
			temp = m_HitMaxVer[i].x;
			m_HitMaxVer[i].x = m_HitMinVer[i].x;
			m_HitMinVer[i].x = temp;
		}
		if (m_HitMaxVer[i].y < m_HitMinVer[i].y)
		{
			float temp;
			temp = m_HitMaxVer[i].y;
			m_HitMaxVer[i].y = m_HitMinVer[i].y;
			m_HitMinVer[i].y = temp;
		}
		if (m_HitMaxVer[i].z < m_HitMinVer[i].z)
		{
			float temp;
			temp = m_HitMaxVer[i].z;
			m_HitMaxVer[i].z = m_HitMinVer[i].z;
			m_HitMinVer[i].z = temp;
		}
	}

	//printf(" Set() max X : %f   Y: %f   Z: %f\n", m_HitMaxVer[0].x, m_HitMaxVer[0].y, m_HitMaxVer[0].z);
	//printf(" Set() min X : %f   Y: %f   Z: %f\n", m_HitMinVer[0].x, m_HitMinVer[0].y, m_HitMinVer[0].z);
}

bool CObject::MyHitAndEnemyBound(CObject *pObject)
{
	/*
	내가 공격시에만 확인하면 됨. 공격 스킬일때만(attack, skill1, skill2, skill3)
	PressSkillNum = 내가 누른 스킬 번호 (≠ animation_state와는 다름)
	0부터 ATTACK1 .. etc
	*/
	if (m_HitMinVer[PressSkillNum].x > pObject->m_MaxVer.x || m_HitMaxVer[PressSkillNum].x < pObject->m_MinVer.x)
	{
		return false;
	}

	if (m_HitMinVer[PressSkillNum].z > pObject->m_MaxVer.z || m_HitMaxVer[PressSkillNum].z < pObject->m_MinVer.z)
	{
		return false;
	}

	//pObject->HP -= 1;
	
	return true;
}

bool CObject::EnemyHitAndMyBound(CObject *pObject)
{
	/*
		적이 날 때렸는지 확인하는 함수. 항상 작동되고 있어야함! 언제 때릴지 모르니까
		날 때리는게!!!!! 몬스터인지!!!! 꼭 id로 확인이 필요한 부분임.. ㅠㅠㅠ 이거 정말 중요★
		우리팀이 날 때렸는데 피가 깎이게 할 수는 없잖아? 
	*/
	if (pObject->m_HitMinVer[PressSkillNum].x > m_MaxVer.x || pObject->m_HitMaxVer[PressSkillNum].x < m_MinVer.x)
	{
		return false;
	}

	if (pObject->m_HitMinVer[PressSkillNum].z > m_MaxVer.z || pObject->m_HitMaxVer[PressSkillNum].z < m_MinVer.z)
	{
		return false;
	}
	
	return true;
}

void CObject::MoveAndRotatingHitBox()
{

	D3DXMATRIX NowMatrix = *GetWorldMatrix();

	for (int i = 0; i < ATTACK_COUNT; ++i)
	{
		m_HitMaxVer[i] = this->m_pMesh->m_HitMaxVer[i];
		m_HitMinVer[i] = this->m_pMesh->m_HitMinVer[i];
		D3DXVec3TransformCoord(&m_HitMaxVer[i], &m_HitMaxVer[i], &NowMatrix);
		D3DXVec3TransformCoord(&m_HitMinVer[i], &m_HitMinVer[i], &NowMatrix);

		if (m_HitMaxVer[i].x < m_HitMinVer[i].x)
		{
			float temp;
			temp = m_HitMaxVer[i].x;
			m_HitMaxVer[i].x = m_HitMinVer[i].x;
			m_HitMinVer[i].x = temp;
		}
		if (m_HitMaxVer[i].y < m_HitMinVer[i].y)
		{
			float temp;
			temp = m_HitMaxVer[i].y;
			m_HitMaxVer[i].y = m_HitMinVer[i].y;
			m_HitMinVer[i].y = temp;
		}
		if (m_HitMaxVer[i].z < m_HitMinVer[i].z)
		{
			float temp;
			temp = m_HitMaxVer[i].z;
			m_HitMaxVer[i].z = m_HitMinVer[i].z;
			m_HitMinVer[i].z = temp;
		}
	}

	//printf("  max X : %f   Y: %f   Z: %f\n", m_HitMaxVer[0].x, m_HitMaxVer[0].y, m_HitMaxVer[0].z);
	//printf("  min X : %f   Y: %f   Z: %f\n", m_HitMinVer[0].x, m_HitMinVer[0].y, m_HitMinVer[0].z);
}

void CObject::MoveAndRotatingBoundingBox()
{
	D3DXMATRIX NowMatrix = *GetBoundingWorldMatrix();

	m_MaxVer = this->m_pMesh->GetMaxVer();
	m_MinVer = this->m_pMesh->GetMinVer();

	//위치값 변환
	D3DXVec3TransformCoord(&m_MaxVer, &m_MaxVer, &NowMatrix);		// 연산의 결과가 저장될 벡터의 주소값, 연산을 수행할 3차원 벡터값, 연산을 수행할 4X4행렬의 주소값
	D3DXVec3TransformCoord(&m_MinVer, &m_MinVer, &NowMatrix);

	if (m_MaxVer.x < m_MinVer.x)
	{
		float temp;
		temp = m_MaxVer.x;
		m_MaxVer.x = m_MinVer.x;
		m_MinVer.x = temp;
	}
	if (m_MaxVer.y < m_MinVer.y)
	{
		float temp;
		temp = m_MaxVer.y;
		m_MaxVer.y = m_MinVer.y;
		m_MinVer.y = temp;
	}
	if (m_MaxVer.z < m_MinVer.z)
	{
		float temp;
		temp = m_MaxVer.z;
		m_MaxVer.z = m_MinVer.z;
		m_MinVer.z = temp;
	}

	//printf("ID : %d, max : %f, %f, %f\n", this->GetId(), m_MaxVer.x, m_MaxVer.y, m_MaxVer.z);
	//printf("ID : %d, min : %f, %f, %f\n", this->GetId(), m_MinVer.x, m_MinVer.y, m_MinVer.z);

}

bool CObject::Collison(CObject *pObject)
{

	// 현재 메쉬 최대 < 타겟 메쉬 최소 , 현재 메쉬 최소 > 타겟 메쉬 최대면 충돌이 아님
	if (m_MinVer.x > pObject->m_MaxVer.x || m_MaxVer.x < pObject->m_MinVer.x)  ///여기에 nowMinver.를 쓰셈
	{
		return false;
	}

	if (m_MinVer.z > pObject->m_MaxVer.z || m_MaxVer.z < pObject->m_MinVer.z)
	{
		return false;
	}

	return true;		//y축은 할 필요가 없다.
}


bool CObject::Collison(vector<CObject*> &vObject)
{
	/*
		카테고리별로 전체 충돌체크를 해버리자!
	*/
	bool isColl = false;

	int Cnt = vObject.size();

	for (int i = 0; i < Cnt; ++i)
	{
		if (m_MinVer.x > vObject[i]->m_MaxVer.x || m_MaxVer.x < vObject[i]->m_MinVer.x)
		{
			continue;
		}

		if (m_MinVer.z > vObject[i]->m_MaxVer.z || m_MaxVer.z < vObject[i]->m_MinVer.z)
		{
			continue;
		}
		else  //continue에 안걸리고 여기까지 왔으면 충돌한거임!!
		{
			CollOtherID = vObject[i]->m_id;	//충돌했을때 충돌한 것의 아이디를 넣어줌.
			isColl = true;
		}	
	}

	if (isColl)   
		return true;
	else 
		return false;
}

//===========================================================
//
//		애니메이션 재생에 필요한 함수들
//
//===========================================================
void CObject::SetResult(XMFLOAT4X4*** result)
{
	m_pppResult = result;
}

void CObject::SetAniIndexCount(int count)
{
	m_AnimationIndexCount = count;
}

void CObject::SetTime(long long* time)
{
	for (int i = 0; i < ANIMATION_COUNT; ++i)
	{
		m_AniMaxTime[i] = time[i];
		cout << i << "번째 maxtime:" << m_AniMaxTime[i] << endl;
	}
}

void CObject::SetPlayAnimationState(int state)
{
	Animation_state = state;
}

void CObject::SetConstantBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext)
{
	//애니메이션을 위한 본 매트릭스 상수버퍼 생성
	D3D11_BUFFER_DESC BD;
	::ZeroMemory(&BD, sizeof(D3D11_BUFFER_DESC));
	BD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	BD.ByteWidth = sizeof(VS_CB_BONE_MATRIX);
	BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	pd3dDevice->CreateBuffer(&BD, NULL, &g_pd3dcbBoneMatrix);
	pd3dDeviceContext->Map(g_pd3dcbBoneMatrix, NULL, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &g_d3dMappedResource);
	{
		g_pcbBoneMatrix = (VS_CB_BONE_MATRIX *)g_d3dMappedResource.pData;
		for (int i = 0; i < 128; ++i)
			g_pcbBoneMatrix->m_XMmtxBone[i] = XMMatrixIdentity();
	}
	pd3dDeviceContext->Unmap(g_pd3dcbBoneMatrix, NULL);
}


void CObject::AnimateObjectAndRender(ID3D11DeviceContext* pd3dDeviceContext, float time)
{
	if (isAnimating)		//애니메이션이 있을 경우 이 부분을 거쳐감. 아이템같은경우는 애니메이션이 없으므로 이부분이 필요 없음.
	{
		if (PreState != Animation_state)	//다른 애니메이션으로 바꼈을때
		{
			PreState = Animation_state;
			m_fAnimationPlaytime = 0.0f;
		}
		m_fAnimationPlaytime += 0.01f;				// 공격속도 제어 가능. 파닥파닥
		NowTime = m_fAnimationPlaytime * 1000;

		if ((NowTime / 10) >= (m_AniMaxTime[Animation_state] / 10))		//애니메이션 초기화되는 부분.
		{
			NowTime -= m_AniMaxTime[Animation_state];
			m_fAnimationPlaytime = 0;
		}

		for (int i = 0; i < m_AnimationIndexCount; ++i)	//128
		{
			XMMATRIX ResultMatrix = XMLoadFloat4x4(&m_pppResult[Animation_state][NowTime / 10][i]);
			g_pcbBoneMatrix->m_XMmtxBone[i] = ResultMatrix;
		}

		if (g_pd3dcbBoneMatrix != nullptr)
		{
			pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_BONE_MATRIX, 1, &g_pd3dcbBoneMatrix);
		}
	}
	
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);				//애니메이션이 없는 그냥 오브젝트는 이 Render에만 들어감
}












CUnit::CUnit(UINT id) : CObject(id)
{
}
CUnit::~CUnit()
{
}


