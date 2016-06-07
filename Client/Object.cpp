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

	m_pd3dcbBoneMatrix = nullptr;
	m_pcbBoneMatrix = nullptr;

	m_fAnimationPlaytime = 0.0f;

	//for (int i = 0; i < ATTACK_COUNT; ++i)
	//{
	//	m_HitMaxVer[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//	m_HitMinVer[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//}
	for (int i = 0; i < ANIMATION_COUNT; ++i)
		m_fAniMaxTime[i] = 0.0f;

	m_eAnimationType = eAnimationType::Idle;
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



//void CObject::BoundingMoveForward(const float fDistance)
//{
//	D3DXVECTOR3 d3dxvTempPosition = *GetPosition();
//	D3DXVECTOR3 d3dxvTempLookAt = *GetLookAt();
//	d3dxvTempPosition += fDistance * d3dxvTempLookAt;
//	//cout << d3dxvTempPosition.x << ", " << d3dxvTempPosition.z << endl;
//	BoundingMoveAbsolute(&d3dxvTempPosition);
//}
//
//void CObject::BoundingMoveAbsolute(const D3DXVECTOR3 *d3dxVec)
//{
//	m_boundingWorldMatrix->_41 = d3dxVec->x;
//	m_boundingWorldMatrix->_42 = d3dxVec->y;
//	m_boundingWorldMatrix->_43 = d3dxVec->z;
//	//printf(" 바운딩 : %f, %f\n", m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_43);
//}
//
//void CObject::BoundingRotateAbsolute(const D3DXVECTOR3 *d3dxVec)
//{
//	// 1) 회전각을 0,0,0으로 되돌리기 = 현재 회전행렬 얻어오기 > 행렬을 역행렬로 바꾸기 > 역행렬을 현재 월드변환행렬에 곱해주기
//	D3DXMATRIX mtxPresentRotation = (*_GetBoundingRotationMatrix());
//	D3DXMatrixInverse(&mtxPresentRotation, 0, &mtxPresentRotation);
//	(*m_boundingWorldMatrix) = mtxPresentRotation * (*m_boundingWorldMatrix);
//
//	// 2) d3dxVec만큼 회전하기
//	BoundingRotateRelative(d3dxVec);
//}
//void CObject::BoundingRotateRelative(const D3DXVECTOR3 *d3dxVec)
//{
//	D3DXMATRIX mtxRotate;
//	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(d3dxVec->y), (float)D3DXToRadian(d3dxVec->x), (float)D3DXToRadian(d3dxVec->z));
//	(*m_boundingWorldMatrix) = mtxRotate * (*m_boundingWorldMatrix);
//}
//
//const D3DXMATRIX* CObject::_GetBoundingRotationMatrix()
//{
//	D3DXMATRIX *pmtxRotate = new D3DXMATRIX();
//	D3DXMatrixIdentity(pmtxRotate);
//
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			pmtxRotate->m[i][j] = m_boundingWorldMatrix->m[i][j];
//		}
//	}
//
//	return pmtxRotate;
//}
//
//const D3DXVECTOR3* CObject::GetBoundingPosition()
//{
//	D3DXVECTOR3 *pPosition = new D3DXVECTOR3(m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_42, m_boundingWorldMatrix->_43);
//	return pPosition;
//}
//const D3DXVECTOR3* CObject::GetBoundingLookAt()
//{
//	D3DXVECTOR3 *pd3dxvLookAt = new D3DXVECTOR3(m_boundingWorldMatrix->_31, m_boundingWorldMatrix->_32, m_boundingWorldMatrix->_33);
//	D3DXVec3Normalize(pd3dxvLookAt, pd3dxvLookAt);
//	return pd3dxvLookAt;
//}

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
	D3DXVECTOR3 *pPosition = new D3DXVECTOR3(m_pd3dxWorldMatrix->_41, m_pd3dxWorldMatrix->_42, m_pd3dxWorldMatrix->_43);
	return pPosition;
}

const D3DXVECTOR3* CObject::GetRight()
{
	D3DXVECTOR3 *pd3dxvRight = new D3DXVECTOR3(m_pd3dxWorldMatrix->_11, m_pd3dxWorldMatrix->_12, m_pd3dxWorldMatrix->_13);
	D3DXVec3Normalize(pd3dxvRight, pd3dxvRight);
	return pd3dxvRight;
}

const D3DXVECTOR3* CObject::GetUp()
{
	D3DXVECTOR3 *pd3dxvUp = new D3DXVECTOR3(m_pd3dxWorldMatrix->_21, m_pd3dxWorldMatrix->_22, m_pd3dxWorldMatrix->_23);
	D3DXVec3Normalize(pd3dxvUp, pd3dxvUp);
	return pd3dxvUp;
}

const D3DXVECTOR3* CObject::GetLookAt()
{
	D3DXVECTOR3 *pd3dxvLookAt = new D3DXVECTOR3(m_pd3dxWorldMatrix->_31, m_pd3dxWorldMatrix->_32, m_pd3dxWorldMatrix->_33);
	D3DXVec3Normalize(pd3dxvLookAt, pd3dxvLookAt);
	return pd3dxvLookAt;
}

void CObject::SetTexture(CTexture *pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

const D3DXMATRIX* CObject::_GetRotationMatrix()
{
	D3DXMATRIX *pmtxRotate = new D3DXMATRIX();
	D3DXMatrixIdentity(pmtxRotate);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			pmtxRotate->m[i][j] = m_pd3dxWorldMatrix->m[i][j];
		}
	}

	return pmtxRotate;
}

void CObject::SetPosition(D3DXVECTOR3* pos)
{
	m_pd3dxWorldMatrix->_41 = pos->x;
	m_pd3dxWorldMatrix->_42 = pos->y;
	m_pd3dxWorldMatrix->_43 = pos->z;
}

////===========================================================
////		충돌박스, 히트박스를 위한 함수들
////		1) MyHitAndEnemyBound : 내 히트박스 + 몬스터 충돌박스 = 몬스터 체력 감소
////		2) EnemyHitAndMyBound : 적 히트박스 + 내 충돌박스 = 내 체력 감소
////		3) Collison : 내 충돌박스 + 아이템일때만 충돌박스 = 아이템 효과 발동
////===========================================================
//void CObject::SetBoundingBox()
//{
//	m_MaxVer = this->m_pMesh->GetMaxVer();
//	m_MinVer = this->m_pMesh->GetMinVer();
//
//	//위치값 변환
//	D3DXVec3TransformCoord(&m_MaxVer, &m_MaxVer, GetWorldMatrix());		// 연산의 결과가 저장될 벡터의 주소값, 연산을 수행할 3차원 벡터값, 연산을 수행할 4X4행렬의 주소값
//	D3DXVec3TransformCoord(&m_MinVer, &m_MinVer, GetWorldMatrix());
//
//	if (m_MaxVer.x < m_MinVer.x)
//	{
//		float temp;
//		temp = m_MaxVer.x;
//		m_MaxVer.x = m_MinVer.x;
//		m_MinVer.x = temp;
//	}
//	if (m_MaxVer.y < m_MinVer.y)
//	{
//		float temp;
//		temp = m_MaxVer.y;
//		m_MaxVer.y = m_MinVer.y;
//		m_MinVer.y = temp;
//	}
//	if (m_MaxVer.z < m_MinVer.z)
//	{
//		float temp;
//		temp = m_MaxVer.z;
//		m_MaxVer.z = m_MinVer.z;
//		m_MinVer.z = temp;
//	}
//
//	//printf("ID : %d, max : %f, %f, %f\n", this->GetId(), m_MaxVer.x, m_MaxVer.y, m_MaxVer.z);
//	//printf("ID : %d, min : %f, %f, %f\n", this->GetId(), m_MinVer.x, m_MinVer.y, m_MinVer.z);
//
//}
//void CObject::SetBoundingBoxMatrix()
//{
//	//printf(" 설정 전 월드변환 : %f, %f\n", m_pd3dxWorldMatrix->_41, m_pd3dxWorldMatrix->_43);
//	m_boundingWorldMatrix->_11 = m_pd3dxWorldMatrix->_11;
//	m_boundingWorldMatrix->_12 = m_pd3dxWorldMatrix->_12;
//	m_boundingWorldMatrix->_13 = m_pd3dxWorldMatrix->_13;
//	m_boundingWorldMatrix->_21 = m_pd3dxWorldMatrix->_21;
//	m_boundingWorldMatrix->_22 = m_pd3dxWorldMatrix->_22;
//	m_boundingWorldMatrix->_23 = m_pd3dxWorldMatrix->_23;
//	m_boundingWorldMatrix->_31 = m_pd3dxWorldMatrix->_31;
//	m_boundingWorldMatrix->_32 = m_pd3dxWorldMatrix->_32;
//	m_boundingWorldMatrix->_33 = m_pd3dxWorldMatrix->_33;
//	m_boundingWorldMatrix->_41 = m_pd3dxWorldMatrix->_41;
//	m_boundingWorldMatrix->_42 = m_pd3dxWorldMatrix->_42;
//	m_boundingWorldMatrix->_43 = m_pd3dxWorldMatrix->_43;
//	//printf(" 설정 후 바운딩 월드변환 : %f, %f\n", m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_43);
//}
//void CObject::SetHitBox()
//{
//	/*
//	공격별로 히트박스 크기가 다 다름. 그래서 m_HitMaxVer[공격모션개수] 이런식으로 만들어줌
//	1) 내 히트박스 + 몬스터 충돌박스 = 몬스터 체력 감소
//	2) 몬스터 히트박스 + 내 충돌박스 = 내 체력 감소
//	3) 내 충돌박스 + 아이템 충돌박스 = 아이템 삭제, 효과 적용
//	*/
//	for (int i = 0; i < ATTACK_COUNT; ++i)
//	{
//		m_HitMaxVer[i] = this->m_pMesh->m_HitMaxVer[i];
//		m_HitMinVer[i] = this->m_pMesh->m_HitMinVer[i];
//		D3DXVec3TransformCoord(&m_HitMaxVer[i], &m_HitMaxVer[i], GetWorldMatrix());
//		D3DXVec3TransformCoord(&m_HitMinVer[i], &m_HitMinVer[i], GetWorldMatrix());
//
//		if (m_HitMaxVer[i].x < m_HitMinVer[i].x)
//		{
//			float temp;
//			temp = m_HitMaxVer[i].x;
//			m_HitMaxVer[i].x = m_HitMinVer[i].x;
//			m_HitMinVer[i].x = temp;
//		}
//		if (m_HitMaxVer[i].y < m_HitMinVer[i].y)
//		{
//			float temp;
//			temp = m_HitMaxVer[i].y;
//			m_HitMaxVer[i].y = m_HitMinVer[i].y;
//			m_HitMinVer[i].y = temp;
//		}
//		if (m_HitMaxVer[i].z < m_HitMinVer[i].z)
//		{
//			float temp;
//			temp = m_HitMaxVer[i].z;
//			m_HitMaxVer[i].z = m_HitMinVer[i].z;
//			m_HitMinVer[i].z = temp;
//		}
//	}
//
//	//printf(" Set() max X : %f   Y: %f   Z: %f\n", m_HitMaxVer[0].x, m_HitMaxVer[0].y, m_HitMaxVer[0].z);
//	//printf(" Set() min X : %f   Y: %f   Z: %f\n", m_HitMinVer[0].x, m_HitMinVer[0].y, m_HitMinVer[0].z);
//}
//
//bool CObject::MyHitAndEnemyBound(CObject *pObject)
//{
//	/*
//	내가 공격시에만 확인하면 됨. 공격 스킬일때만(attack, skill1, skill2, skill3)
//	PressSkillNum = 내가 누른 스킬 번호 (≠ animation_state와는 다름)
//	0부터 ATTACK1 .. etc
//	*/
//	if (m_HitMinVer[PressSkillNum].x > pObject->m_MaxVer.x || m_HitMaxVer[PressSkillNum].x < pObject->m_MinVer.x)
//	{
//		return false;
//	}
//
//	if (m_HitMinVer[PressSkillNum].z > pObject->m_MaxVer.z || m_HitMaxVer[PressSkillNum].z < pObject->m_MinVer.z)
//	{
//		return false;
//	}
//
//	//pObject->HP -= 1;
//
//	return true;
//}
//
//bool CObject::EnemyHitAndMyBound(CObject *pObject)
//{
//	/*
//		적이 날 때렸는지 확인하는 함수. 항상 작동되고 있어야함! 언제 때릴지 모르니까
//		날 때리는게!!!!! 몬스터인지!!!! 꼭 id로 확인이 필요한 부분임.. ㅠㅠㅠ 이거 정말 중요★
//		우리팀이 날 때렸는데 피가 깎이게 할 수는 없잖아?
//	*/
//	if (pObject->m_HitMinVer[PressSkillNum].x > m_MaxVer.x || pObject->m_HitMaxVer[PressSkillNum].x < m_MinVer.x)
//	{
//		return false;
//	}
//
//	if (pObject->m_HitMinVer[PressSkillNum].z > m_MaxVer.z || pObject->m_HitMaxVer[PressSkillNum].z < m_MinVer.z)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void CObject::MoveAndRotatingHitBox()
//{
//
//	D3DXMATRIX NowMatrix = *GetWorldMatrix();
//
//	for (int i = 0; i < ATTACK_COUNT; ++i)
//	{
//		m_HitMaxVer[i] = this->m_pMesh->m_HitMaxVer[i];
//		m_HitMinVer[i] = this->m_pMesh->m_HitMinVer[i];
//		D3DXVec3TransformCoord(&m_HitMaxVer[i], &m_HitMaxVer[i], &NowMatrix);
//		D3DXVec3TransformCoord(&m_HitMinVer[i], &m_HitMinVer[i], &NowMatrix);
//
//		if (m_HitMaxVer[i].x < m_HitMinVer[i].x)
//		{
//			float temp;
//			temp = m_HitMaxVer[i].x;
//			m_HitMaxVer[i].x = m_HitMinVer[i].x;
//			m_HitMinVer[i].x = temp;
//		}
//		if (m_HitMaxVer[i].y < m_HitMinVer[i].y)
//		{
//			float temp;
//			temp = m_HitMaxVer[i].y;
//			m_HitMaxVer[i].y = m_HitMinVer[i].y;
//			m_HitMinVer[i].y = temp;
//		}
//		if (m_HitMaxVer[i].z < m_HitMinVer[i].z)
//		{
//			float temp;
//			temp = m_HitMaxVer[i].z;
//			m_HitMaxVer[i].z = m_HitMinVer[i].z;
//			m_HitMinVer[i].z = temp;
//		}
//	}
//
//	//printf("  max X : %f   Y: %f   Z: %f\n", m_HitMaxVer[0].x, m_HitMaxVer[0].y, m_HitMaxVer[0].z);
//	//printf("  min X : %f   Y: %f   Z: %f\n", m_HitMinVer[0].x, m_HitMinVer[0].y, m_HitMinVer[0].z);
//}
//
//void CObject::MoveAndRotatingBoundingBox()
//{
//	D3DXMATRIX NowMatrix = *GetBoundingWorldMatrix();
//
//	m_MaxVer = this->m_pMesh->GetMaxVer();
//	m_MinVer = this->m_pMesh->GetMinVer();
//
//	//위치값 변환
//	D3DXVec3TransformCoord(&m_MaxVer, &m_MaxVer, &NowMatrix);		// 연산의 결과가 저장될 벡터의 주소값, 연산을 수행할 3차원 벡터값, 연산을 수행할 4X4행렬의 주소값
//	D3DXVec3TransformCoord(&m_MinVer, &m_MinVer, &NowMatrix);
//
//	if (m_MaxVer.x < m_MinVer.x)
//	{
//		float temp;
//		temp = m_MaxVer.x;
//		m_MaxVer.x = m_MinVer.x;
//		m_MinVer.x = temp;
//	}
//	if (m_MaxVer.y < m_MinVer.y)
//	{
//		float temp;
//		temp = m_MaxVer.y;
//		m_MaxVer.y = m_MinVer.y;
//		m_MinVer.y = temp;
//	}
//	if (m_MaxVer.z < m_MinVer.z)
//	{
//		float temp;
//		temp = m_MaxVer.z;
//		m_MaxVer.z = m_MinVer.z;
//		m_MinVer.z = temp;
//	}
//
//	//printf("ID : %d, max : %f, %f, %f\n", this->GetId(), m_MaxVer.x, m_MaxVer.y, m_MaxVer.z);
//	//printf("ID : %d, min : %f, %f, %f\n", this->GetId(), m_MinVer.x, m_MinVer.y, m_MinVer.z);
//
//}
//
//bool CObject::Collison(CObject *pObject)
//{
//
//	// 현재 메쉬 최대 < 타겟 메쉬 최소 , 현재 메쉬 최소 > 타겟 메쉬 최대면 충돌이 아님
//	if (m_MinVer.x > pObject->m_MaxVer.x || m_MaxVer.x < pObject->m_MinVer.x)  ///여기에 nowMinver.를 쓰셈
//	{
//		return false;
//	}
//
//	if (m_MinVer.z > pObject->m_MaxVer.z || m_MaxVer.z < pObject->m_MinVer.z)
//	{
//		return false;
//	}
//
//	return true;		//y축은 할 필요가 없다.
//}
//
//
//bool CObject::Collison(vector<CObject*> &vObject)
//{
//	/*
//		카테고리별로 전체 충돌체크를 해버리자!
//	*/
//	bool isColl = false;
//
//	int Cnt = vObject.size();
//
//	for (int i = 0; i < Cnt; ++i)
//	{
//		if (m_MinVer.x > vObject[i]->m_MaxVer.x || m_MaxVer.x < vObject[i]->m_MinVer.x)
//		{
//			continue;
//		}
//
//		if (m_MinVer.z > vObject[i]->m_MaxVer.z || m_MaxVer.z < vObject[i]->m_MinVer.z)
//		{
//			continue;
//		}
//		else  //continue에 안걸리고 여기까지 왔으면 충돌한거임!!
//		{
//			CollOtherID = vObject[i]->m_id;	//충돌했을때 충돌한 것의 아이디를 넣어줌.
//			isColl = true;
//		}
//	}
//
//	if (isColl)
//		return true;
//	else
//		return false;
//}



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
		m_fAniMaxTime[i] = time[i];
		cout << i << "번째 maxtime:" << m_fAniMaxTime[i] << endl;
	}
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

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDevice->CreateBuffer(&BD, NULL, &m_pd3dcbBoneMatrix);
	pd3dDeviceContext->Map(m_pd3dcbBoneMatrix, NULL, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &d3dMappedResource);
	{
		m_pcbBoneMatrix = (VS_CB_BONE_MATRIX *)d3dMappedResource.pData;
		for (int i = 0; i < 128; ++i)
			m_pcbBoneMatrix->m_XMmtxBone[i] = XMMatrixIdentity();
	}
	pd3dDeviceContext->Unmap(m_pd3dcbBoneMatrix, NULL);
}


void CObject::AnimateAndRender(ID3D11DeviceContext* pd3dDeviceContext, float time)
{
	if (eAnimationType::None != m_eAnimationType)
	{
		m_fAnimationPlaytime += time * 1000;

		/* 현재 애니메이션을 한 번 완료했을 때 */
		if ((m_fAnimationPlaytime / 10) >= static_cast<int>(m_fAniMaxTime[static_cast<int>(m_eAnimationType)] / 10))
		{
			switch (m_eAnimationType)
			{
			case CObject::eAnimationType::Idle:
			case CObject::eAnimationType::Move:
			{
				m_fAnimationPlaytime = 0.0f;
			}break;
			case CObject::eAnimationType::Dead:
			{
				m_fAnimationPlaytime -= time * 1000;
			} break;
			case CObject::eAnimationType::Attack:
			case CObject::eAnimationType::Skill1:
			case CObject::eAnimationType::Skill2:
			case CObject::eAnimationType::Skill3:
			{
				m_eAnimationType = eAnimationType::Idle;
				m_fAnimationPlaytime = 0.0f;
			}break;
			default:
				break;
			}
		}

		for (int i = 0; i < m_AnimationIndexCount; ++i)
		{
			XMMATRIX ResultMatrix = XMLoadFloat4x4(&m_pppResult[static_cast<int>(m_eAnimationType)][static_cast<int>(m_fAnimationPlaytime) / 10][i]);
			m_pcbBoneMatrix->m_XMmtxBone[i] = ResultMatrix;
		}
		if (m_pd3dcbBoneMatrix)
		{
			pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_BONE_MATRIX, 1, &m_pd3dcbBoneMatrix);
		}
	}
	
	if (m_pMesh) m_pMesh->Render(pd3dDeviceContext);
}

void CObject::PlayAnimation(eAnimationType eType)
{
	if (eType == m_eAnimationType)
		return;

	m_eAnimationType = eType;

	m_fAnimationPlaytime = 0.0f;
}










CUnit::CUnit(UINT id) : CObject(id)
{
	m_fStrikingPower = 10;
	m_fDefensivePower = 10;
	m_fMovingSpeed = 500;
	m_fHp = 100;
}
CUnit::~CUnit()
{
}


