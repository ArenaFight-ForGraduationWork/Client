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

	//m_pMaxVer = nullptr;
	//m_pMinVer = nullptr;
	m_fRadius = 0.0;
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
void CObject::SetTexture(CTexture *pTexture)
{
	if (m_pTexture) m_pTexture->Release();
	m_pTexture = pTexture;
	if (m_pTexture) m_pTexture->AddRef();
}

void CObject::SetPositionRelative(const float fx, const float fy, const float fz)
{
	D3DXVECTOR3 d3dxvPosition = *GetPosition();

	D3DXVECTOR3 d3dxvRight = *GetRight();
	D3DXVECTOR3 d3dxvUp = *GetUp();
	D3DXVECTOR3 d3dxvLookAt = *GetLookAt();

	d3dxvPosition += fx * d3dxvRight;
	d3dxvPosition += fy * d3dxvUp;
	d3dxvPosition += fz * d3dxvLookAt;

	SetPositionAbsolute(&d3dxvPosition);
}
void CObject::SetPositionRelative(const D3DXVECTOR3 *d3dxVec)
{
	D3DXVECTOR3 d3dxvPosition = *GetPosition();

	D3DXVECTOR3 d3dxvRight = *GetRight();
	D3DXVECTOR3 d3dxvUp = *GetUp();
	D3DXVECTOR3 d3dxvLookAt = *GetLookAt();

	d3dxvPosition += d3dxVec->x * d3dxvRight;
	d3dxvPosition += d3dxVec->y * d3dxvUp;
	d3dxvPosition += d3dxVec->z * d3dxvLookAt;

	SetPositionAbsolute(&d3dxvPosition);
}
void CObject::SetPositionAbsolute(const float fx, const float fy, const float fz)
{
	m_pd3dxWorldMatrix->_41 = fx;
	m_pd3dxWorldMatrix->_42 = fy;
	m_pd3dxWorldMatrix->_43 = fz;
}
void CObject::SetPositionAbsolute(const D3DXVECTOR3 *d3dxVec)
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
	SetPositionAbsolute(&d3dxvPosition);
}



//void CObject::BoundingMoveForward(const float fDistance)
//{
//	D3DXVECTOR3 d3dxvTempPosition = *GetPosition();
//	D3DXVECTOR3 d3dxvTempLookAt = *GetLookAt();
//	d3dxvTempPosition += fDistance * d3dxvTempLookAt;
//	//cout << d3dxvTempPosition.x << ", " << d3dxvTempPosition.z << endl;
//	BoundingSetPositionAbsolute(&d3dxvTempPosition);
//}
//
//void CObject::BoundingSetPositionAbsolute(const D3DXVECTOR3 *d3dxVec)
//{
//	m_boundingWorldMatrix->_41 = d3dxVec->x;
//	m_boundingWorldMatrix->_42 = d3dxVec->y;
//	m_boundingWorldMatrix->_43 = d3dxVec->z;
//	//printf(" �ٿ�� : %f, %f\n", m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_43);
//}
//
//void CObject::BoundingSetDirectionAbsolute(const D3DXVECTOR3 *d3dxVec)
//{
//	// 1) ȸ������ 0,0,0���� �ǵ����� = ���� ȸ����� ������ > ����� ����ķ� �ٲٱ� > ������� ���� ���庯ȯ��Ŀ� �����ֱ�
//	D3DXMATRIX mtxPresentRotation = (*_GetBoundingRotationMatrix());
//	D3DXMatrixInverse(&mtxPresentRotation, 0, &mtxPresentRotation);
//	(*m_boundingWorldMatrix) = mtxPresentRotation * (*m_boundingWorldMatrix);
//
//	// 2) d3dxVec��ŭ ȸ���ϱ�
//	BoundingSetDirectionRelative(d3dxVec);
//}
//void CObject::BoundingSetDirectionRelative(const D3DXVECTOR3 *d3dxVec)
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

void CObject::SetDirectionRelative(const float fPitch, const float fYaw, const float fRoll)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	(*m_pd3dxWorldMatrix) = mtxRotate * (*m_pd3dxWorldMatrix);
}
void CObject::SetDirectionRelative(const D3DXVECTOR3 *d3dxVec)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(d3dxVec->y), (float)D3DXToRadian(d3dxVec->x), (float)D3DXToRadian(d3dxVec->z));
	(*m_pd3dxWorldMatrix) = mtxRotate * (*m_pd3dxWorldMatrix);
}
void CObject::SetDirectionRelative(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle)
{
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	(*m_pd3dxWorldMatrix) = mtxRotate * (*m_pd3dxWorldMatrix);
}
void CObject::SetDirectionAbsolute(const float fPitch, const float fYaw, const float fRoll)
{
	//   1-1) ȸ������ 0,0,0���� �ǵ����� = ���� ȸ����� ������ > ����� ����ķ� �ٲٱ� > ������� ���� ���庯ȯ��Ŀ� �����ֱ�
	// v 1-2) ȸ������ 0,0,0���� �ǵ����� = 3x3�κ��� ������ķ� �ٲ�
	m_pd3dxWorldMatrix->_11 = 1;	m_pd3dxWorldMatrix->_12 = 0;	m_pd3dxWorldMatrix->_13 = 0;
	m_pd3dxWorldMatrix->_21 = 0;	m_pd3dxWorldMatrix->_22 = 1;	m_pd3dxWorldMatrix->_23 = 0;
	m_pd3dxWorldMatrix->_31 = 0;	m_pd3dxWorldMatrix->_32 = 0;	m_pd3dxWorldMatrix->_33 = 1;

	// 2) fPitch, fYaw, fRoll�� ȸ���ϱ�
	SetDirectionRelative(fPitch, fYaw, fRoll);
}
void CObject::SetDirectionAbsolute(const D3DXVECTOR3 *d3dxVec)
{
	m_pd3dxWorldMatrix->_11 = 1;	m_pd3dxWorldMatrix->_12 = 0;	m_pd3dxWorldMatrix->_13 = 0;
	m_pd3dxWorldMatrix->_21 = 0;	m_pd3dxWorldMatrix->_22 = 1;	m_pd3dxWorldMatrix->_23 = 0;
	m_pd3dxWorldMatrix->_31 = 0;	m_pd3dxWorldMatrix->_32 = 0;	m_pd3dxWorldMatrix->_33 = 1;

	SetDirectionRelative(d3dxVec);
}
void CObject::SetDirectionAbsolute(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle)
{
	m_pd3dxWorldMatrix->_11 = 1;	m_pd3dxWorldMatrix->_12 = 0;	m_pd3dxWorldMatrix->_13 = 0;
	m_pd3dxWorldMatrix->_21 = 0;	m_pd3dxWorldMatrix->_22 = 1;	m_pd3dxWorldMatrix->_23 = 0;
	m_pd3dxWorldMatrix->_31 = 0;	m_pd3dxWorldMatrix->_32 = 0;	m_pd3dxWorldMatrix->_33 = 1;

	SetDirectionRelative(pd3dxvAxis, fAngle);
}

const D3DXVECTOR3* CObject::GetPosition()
{
	return  new D3DXVECTOR3(m_pd3dxWorldMatrix->_41, m_pd3dxWorldMatrix->_42, m_pd3dxWorldMatrix->_43);
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





////===========================================================
////		�浹�ڽ�, ��Ʈ�ڽ��� ���� �Լ���
////		1) MyHitAndEnemyBound : �� ��Ʈ�ڽ� + ���� �浹�ڽ� = ���� ü�� ����
////		2) EnemyHitAndMyBound : �� ��Ʈ�ڽ� + �� �浹�ڽ� = �� ü�� ����
////		3) Collison : �� �浹�ڽ� + �������϶��� �浹�ڽ� = ������ ȿ�� �ߵ�
////===========================================================
void CObject::SetBoundingBox()
{
	//if (m_pMaxVer)	delete m_pMaxVer;
	//if (m_pMinVer)	delete m_pMinVer;

	//m_pMaxVer =	&(m_pMesh->GetMaxVer());
	//m_pMinVer = &(m_pMesh->GetMinVer());
	m_MaxVer =	(m_pMesh->GetMaxVer());
	m_MinVer = (m_pMesh->GetMinVer());

	//if (m_pMaxVer->x < m_pMinVer->x)
	//{
	//	float temp;
	//	temp = m_pMaxVer->x;
	//	m_pMaxVer->x = m_pMinVer->x;
	//	m_pMinVer->x = temp;
	//}
	//if (m_pMaxVer->y < m_pMinVer->y)
	//{
	//	float temp;
	//	temp = m_pMaxVer->y;
	//	m_pMaxVer->y = m_pMinVer->y;
	//	m_pMinVer->y = temp;
	//}
	//if (m_pMaxVer->z < m_pMinVer->z)
	//{
	//	float temp;
	//	temp = m_pMaxVer->z;
	//	m_pMaxVer->z = m_pMinVer->z;
	//	m_pMinVer->z = temp;
	//}
	if (m_MaxVer.x < m_MinVer.x)
	{
		float temp;
		temp = m_MinVer.x;
		m_MinVer.x = m_MinVer.x;
		m_MinVer.x = temp;
	}
	if (m_MinVer.y < m_MinVer.y)
	{
		float temp;
		temp = m_MinVer.y;
		m_MinVer.y = m_MinVer.y;
		m_MinVer.y = temp;
	}
	if (m_MinVer.z < m_MinVer.z)
	{
		float temp;
		temp = m_MinVer.z;
		m_MinVer.z = m_MinVer.z;
		m_MinVer.z = temp;
	}

	//cout << "Max" << m_pMaxVer->x << ", "
	//	<< m_pMaxVer->y << ", "
	//	<< m_pMaxVer->z << endl;
	//cout << "Min" << m_pMinVer->x << ", "
	//	<< m_pMinVer->y << ", "
	//	<< m_pMinVer->z << endl;
	//cout << "Max" << m_MaxVer.x << ", "
	//	<< m_MaxVer.y << ", "
	//	<< m_MaxVer.z << endl;
	//cout << "Min" << m_MinVer.x << ", "
	//	<< m_MinVer.y << ", "
	//	<< m_MinVer.z << endl;

	m_fRadius = static_cast<float>(sqrt((m_MinVer.x * m_MinVer.x) + (m_MinVer.z * m_MinVer.z)));
}
//void CObject::SetBoundingBoxMatrix()
//{
//	//printf(" ���� �� ���庯ȯ : %f, %f\n", m_pd3dxWorldMatrix->_41, m_pd3dxWorldMatrix->_43);
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
//	//printf(" ���� �� �ٿ�� ���庯ȯ : %f, %f\n", m_boundingWorldMatrix->_41, m_boundingWorldMatrix->_43);
//}
//void CObject::SetHitBox()
//{
//	/*
//	���ݺ��� ��Ʈ�ڽ� ũ�Ⱑ �� �ٸ�. �׷��� m_HitMaxVer[���ݸ�ǰ���] �̷������� �������
//	1) �� ��Ʈ�ڽ� + ���� �浹�ڽ� = ���� ü�� ����
//	2) ���� ��Ʈ�ڽ� + �� �浹�ڽ� = �� ü�� ����
//	3) �� �浹�ڽ� + ������ �浹�ڽ� = ������ ����, ȿ�� ����
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
//	���� ���ݽÿ��� Ȯ���ϸ� ��. ���� ��ų�϶���(attack, skill1, skill2, skill3)
//	PressSkillNum = ���� ���� ��ų ��ȣ (�� animation_state�ʹ� �ٸ�)
//	0���� ATTACK1 .. etc
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
//		���� �� ���ȴ��� Ȯ���ϴ� �Լ�. �׻� �۵��ǰ� �־����! ���� ������ �𸣴ϱ�
//		�� �����°�!!!!! ��������!!!! �� id�� Ȯ���� �ʿ��� �κ���.. �ФФ� �̰� ���� �߿��
//		�츮���� �� ���ȴµ� �ǰ� ���̰� �� ���� ���ݾ�?
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
//	//��ġ�� ��ȯ
//	D3DXVec3TransformCoord(&m_MaxVer, &m_MaxVer, &NowMatrix);		// ������ ����� ����� ������ �ּҰ�, ������ ������ 3���� ���Ͱ�, ������ ������ 4X4����� �ּҰ�
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
//	// ���� �޽� �ִ� < Ÿ�� �޽� �ּ� , ���� �޽� �ּ� > Ÿ�� �޽� �ִ�� �浹�� �ƴ�
//	if (m_MinVer.x > pObject->m_MaxVer.x || m_MaxVer.x < pObject->m_MinVer.x)  ///���⿡ nowMinver.�� ����
//	{
//		return false;
//	}
//
//	if (m_MinVer.z > pObject->m_MaxVer.z || m_MaxVer.z < pObject->m_MinVer.z)
//	{
//		return false;
//	}
//
//	return true;		//y���� �� �ʿ䰡 ����.
//}
//
//
//bool CObject::Collison(vector<CObject*> &vObject)
//{
//	/*
//		ī�װ����� ��ü �浹üũ�� �ع�����!
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
//		else  //continue�� �Ȱɸ��� ������� ������ �浹�Ѱ���!!
//		{
//			CollOtherID = vObject[i]->m_id;	//�浹������ �浹�� ���� ���̵� �־���.
//			isColl = true;
//		}
//	}
//
//	if (isColl)
//		return true;
//	else
//		return false;
//}
const D3DXVECTOR3* CObject::GetMaxVer()
{
	//cout << "Max" << m_pMaxVer->x << ", "
	//	<< m_pMaxVer->y << ", "
	//	<< m_pMaxVer->z << endl;
	//cout << "Min" << m_pMinVer->x << ", "
	//	<< m_pMinVer->y << ", "
	//	<< m_pMinVer->z << endl;
	//cout << "Max" << m_MaxVer.x << ", "
	//	<< m_MaxVer.y << ", "
	//	<< m_MaxVer.z << endl;
	//cout << "Min" << m_MinVer.x << ", "
	//	<< m_MinVer.y << ", "
	//	<< m_MinVer.z << endl;

	//D3DXVECTOR3 *pVertex = new D3DXVECTOR3();
	//D3DXVec3TransformCoord(pVertex, m_pMaxVer, m_pd3dxWorldMatrix);
	//return pVertex;
	D3DXVECTOR3 *pVertex = new D3DXVECTOR3();
	D3DXVec3TransformCoord(pVertex, &m_MaxVer, m_pd3dxWorldMatrix);
	return pVertex;
}
const D3DXVECTOR3* CObject::GetMinVer()
{
	//D3DXVECTOR3 *pVertex = new D3DXVECTOR3();
	//D3DXVec3TransformCoord(pVertex, m_pMinVer, m_pd3dxWorldMatrix);
	//return pVertex;
	D3DXVECTOR3 *pVertex = new D3DXVECTOR3();
	D3DXVec3TransformCoord(pVertex, &m_MinVer, m_pd3dxWorldMatrix);
	return pVertex;
}




//===========================================================
//
//		�ִϸ��̼� ����� �ʿ��� �Լ���
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
		m_fAniMaxTime[i] = static_cast<float>(time[i]);
	}
}

void CObject::SetConstantBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext)
{
	//�ִϸ��̼��� ���� �� ��Ʈ���� ������� ����
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

		/* ���� �ִϸ��̼��� �� �� �Ϸ����� �� */
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

	switch (m_eAnimationType)
	{
	case CObject::eAnimationType::Idle:
	case CObject::eAnimationType::Move:
	{
		m_eAnimationType = eType;
		m_fAnimationPlaytime = 0.0f;
	}break;
	case CObject::eAnimationType::Dead:		// �׾����� �ִϸ��̼��� �� �ٲ�
	case CObject::eAnimationType::Attack:	// �����߿� �ٲ��� ����
	case CObject::eAnimationType::Skill1:
	case CObject::eAnimationType::Skill2:
	case CObject::eAnimationType::Skill3:
	case CObject::eAnimationType::None:
		return;
	default:break;
	}
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


