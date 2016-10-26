#include "stdafx.h"
#include "Object.h"

#include <DDSTextureLoader.h>





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

void CTexture::SetTexture(int nIndex, WCHAR *textureAddress)
{
	ID3D11SamplerState *pd3dSamplerState = nullptr;
	ID3D11ShaderResourceView *pd3dsrvTexture = nullptr;
	D3D11_SAMPLER_DESC d3dSamplerDesc;

	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	gpCommonState->m_pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	ID3D11Resource *pTexResource = nullptr;
	CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, textureAddress, &pTexResource, &pd3dsrvTexture);

	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->Release();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->Release();
	m_ppd3dsrvTextures[nIndex] = pd3dsrvTexture;
	m_ppd3dSamplerStates[nIndex] = pd3dSamplerState;
	if (m_ppd3dsrvTextures[nIndex]) m_ppd3dsrvTextures[nIndex]->AddRef();
	if (m_ppd3dSamplerStates[nIndex]) m_ppd3dSamplerStates[nIndex]->AddRef();

	pd3dSamplerState = nullptr;
	pd3dsrvTexture = nullptr;
}








CObject::CObject(UINT id)
{
	XMStoreFloat4x4(&m_f4x4WorldMatrix, XMMatrixIdentity());

	m_pMesh = nullptr;
	m_pMaterial = nullptr;
	m_pTexture = nullptr;

	m_id = id;

	m_f3Direction = XMFLOAT3(0, 0, 0);

	m_pd3dcbBoneMatrix = nullptr;
	m_pcbBoneMatrix = nullptr;

	m_fAnimationPlaytime = 0.0f;

	for (int i = 0; i < ANIMATION_COUNT; ++i)
		m_iAniMaxTime[i] = 0;

	m_eAnimationType = eAnimationType::Idle;

	m_pMaxVer = XMFLOAT3(0, 0, 0);
	m_pMinVer = XMFLOAT3(0, 0, 0);
	m_fRadius = 0.0;

	m_pUnitComponent = nullptr;
}

void CObject::SetResourceType(int type)
{
	m_iSourceType = type;
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

void CObject::SetPositionRelative(float& fx, float& fy, float& fz)
{
	XMVECTOR position;
	position = GetPosition();

	position += fx * GetRight();
	position += fy * GetUp();
	position += fz * GetLookAt();

	SetPositionAbsolute(position);
}
void CObject::SetPositionRelative(CXMVECTOR vec)
{
	XMVECTOR position;
	position = GetPosition();

	position += XMVectorGetX(vec) * GetRight();
	position += XMVectorGetY(vec) * GetUp();
	position += XMVectorGetZ(vec) * GetLookAt();

	SetPositionAbsolute(position);
}
void CObject::SetPositionAbsolute(float& fx, float& fy, float& fz)
{
	m_f4x4WorldMatrix._41 = fx;
	m_f4x4WorldMatrix._42 = fy;
	m_f4x4WorldMatrix._43 = fz;
}
void CObject::SetPositionAbsolute(CXMVECTOR vec)
{
	m_f4x4WorldMatrix._41 = XMVectorGetX(vec);
	m_f4x4WorldMatrix._42 = XMVectorGetY(vec);
	m_f4x4WorldMatrix._43 = XMVectorGetZ(vec);
}
void CObject::MoveForward(float& fVar)
{
	XMVECTOR position = GetPosition();
	if (m_pUnitComponent)	// 속력이 존재하면 속력 * 시간(fVar)
		position += m_pUnitComponent->GetMovingSpeed() * fVar * GetLookAt();
	else // 속력이 없으면 거리(fVar)
		position += fVar * GetLookAt();

	SetPositionAbsolute(position);
}

void CObject::SetDirectionRelative(float& fPitch, float& fYaw, float& fRoll)
{
	m_f3Direction.x += fPitch;
	m_f3Direction.y += fYaw;
	m_f3Direction.z += fRoll;

	XMMATRIX mRotate;
	mRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));

	XMMATRIX mWorld;
	mWorld = XMLoadFloat4x4(&m_f4x4WorldMatrix);

	mWorld = XMMatrixMultiply(mRotate, mWorld);

	XMStoreFloat4x4(&m_f4x4WorldMatrix, mWorld);
}
void CObject::SetDirectionRelative(CXMVECTOR vec)
{
	m_f3Direction.x += XMVectorGetX(vec);
	m_f3Direction.y += XMVectorGetY(vec);
	m_f3Direction.z += XMVectorGetZ(vec);

	XMMATRIX mRotate;
	mRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(XMVectorGetX(vec)), XMConvertToRadians(XMVectorGetY(vec)), XMConvertToRadians(XMVectorGetZ(vec)));

	XMMATRIX mWorld;
	mWorld = XMLoadFloat4x4(&m_f4x4WorldMatrix);

	mWorld = XMMatrixMultiply(mRotate, mWorld);

	XMStoreFloat4x4(&m_f4x4WorldMatrix, mWorld);
}
void CObject::SetDirectionAbsolute(float& fPitch, float& fYaw, float& fRoll)
{
	m_f3Direction = XMFLOAT3(0, 0, 0);

	//   1-1) 회전각을 0,0,0으로 되돌리기 = 현재 회전행렬 얻어오기 > 행렬을 역행렬로 바꾸기 > 역행렬을 현재 월드변환행렬에 곱해주기
	// v 1-2) 회전각을 0,0,0으로 되돌리기 = 3x3부분을 단위행렬로 바꿈
	m_f4x4WorldMatrix._11 = 1;	m_f4x4WorldMatrix._12 = 0;	m_f4x4WorldMatrix._13 = 0;
	m_f4x4WorldMatrix._21 = 0;	m_f4x4WorldMatrix._22 = 1;	m_f4x4WorldMatrix._23 = 0;
	m_f4x4WorldMatrix._31 = 0;	m_f4x4WorldMatrix._32 = 0;	m_f4x4WorldMatrix._33 = 1;

	// 2) fPitch, fYaw, fRoll로 회전하기
	SetDirectionRelative(fPitch, fYaw, fRoll);
}
void CObject::SetDirectionAbsolute(CXMVECTOR vec)
{
	m_f3Direction = XMFLOAT3(0, 0, 0);

	m_f4x4WorldMatrix._11 = 1;	m_f4x4WorldMatrix._12 = 0;	m_f4x4WorldMatrix._13 = 0;
	m_f4x4WorldMatrix._21 = 0;	m_f4x4WorldMatrix._22 = 1;	m_f4x4WorldMatrix._23 = 0;
	m_f4x4WorldMatrix._31 = 0;	m_f4x4WorldMatrix._32 = 0;	m_f4x4WorldMatrix._33 = 1;

	SetDirectionRelative(vec);
}

CXMVECTOR CObject::GetPosition()
{
	XMFLOAT3 position;
	position = XMFLOAT3(m_f4x4WorldMatrix._41, m_f4x4WorldMatrix._42, m_f4x4WorldMatrix._43);

	return XMLoadFloat3(&position);
}
CXMVECTOR CObject::GetDirection()
{
	return XMLoadFloat3(&m_f3Direction);
}

CXMVECTOR CObject::GetRight()
{
	XMFLOAT3 f3Right;
	f3Right = XMFLOAT3(m_f4x4WorldMatrix._11, m_f4x4WorldMatrix._12, m_f4x4WorldMatrix._13);

	XMVECTOR vRight;
	vRight = XMLoadFloat3(&f3Right);
	vRight = XMVector3Normalize(vRight);

	return vRight;
}

CXMVECTOR CObject::GetUp()
{
	XMFLOAT3 f3Up;
	f3Up = XMFLOAT3(m_f4x4WorldMatrix._21, m_f4x4WorldMatrix._22, m_f4x4WorldMatrix._23);

	XMVECTOR vUp;
	vUp = XMLoadFloat3(&f3Up);
	vUp = XMVector3Normalize(vUp);

	return vUp;
}

CXMVECTOR CObject::GetLookAt()
{
	XMFLOAT3 f3Look;
	f3Look = XMFLOAT3(m_f4x4WorldMatrix._31, m_f4x4WorldMatrix._32, m_f4x4WorldMatrix._33);

	XMVECTOR vLook;
	vLook = XMLoadFloat3(&f3Look);
	vLook = XMVector3Normalize(vLook);

	return vLook;
}





void CObject::SetBoundingBox()
{
	XMStoreFloat3(&m_pMaxVer, m_pMesh->GetMaxVer());
	XMStoreFloat3(&m_pMinVer, m_pMesh->GetMinVer());

	if (m_pMaxVer.x < m_pMinVer.x)
	{
		float temp;
		temp = m_pMaxVer.x;
		m_pMaxVer.x = m_pMinVer.x;
		m_pMinVer.x = temp;
	}
	if (m_pMaxVer.y < m_pMinVer.y)
	{
		float temp;
		temp = m_pMaxVer.y;
		m_pMaxVer.y = m_pMinVer.y;
		m_pMinVer.y = temp;
	}
	if (m_pMaxVer.z < m_pMinVer.z)
	{
		float temp;
		temp = m_pMaxVer.z;
		m_pMaxVer.z = m_pMinVer.z;
		m_pMinVer.z = temp;
	}

	m_fRadius = static_cast<float>(sqrt((m_pMinVer.x * m_pMinVer.x) + (m_pMinVer.z * m_pMinVer.z)));
}

CXMVECTOR CObject::GetMaxVer()
{
	XMVECTOR vVertex;
	vVertex = XMVector3TransformCoord(XMLoadFloat3(&m_pMaxVer), XMLoadFloat4x4(&m_f4x4WorldMatrix));

	return vVertex;
}
CXMVECTOR CObject::GetMinVer()
{
	XMVECTOR vVertex;
	vVertex = XMVector3TransformCoord(XMLoadFloat3(&m_pMinVer), XMLoadFloat4x4(&m_f4x4WorldMatrix));

	return vVertex;
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
	m_iAnimationIndexCount = count;
}

void CObject::SetTime(int *time)
{
	for (int i = 0; i < ANIMATION_COUNT; ++i)
	{
		m_iAniMaxTime[i] = time[i];
	}
}

void CObject::SetConstantBuffer()
{
	//애니메이션을 위한 본 매트릭스 상수버퍼 생성
	D3D11_BUFFER_DESC BD;
	::ZeroMemory(&BD, sizeof(D3D11_BUFFER_DESC));
	BD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	BD.ByteWidth = sizeof(VS_CB_BONE_MATRIX);
	BD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	gpCommonState->m_pd3dDevice->CreateBuffer(&BD, NULL, &m_pd3dcbBoneMatrix);
	gpCommonState->m_pd3dDeviceContext->Map(m_pd3dcbBoneMatrix, NULL, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, NULL, &d3dMappedResource);
	{
		m_pcbBoneMatrix = (VS_CB_BONE_MATRIX *)d3dMappedResource.pData;
		for (int i = 0; i < 128; ++i)
			m_pcbBoneMatrix->m_mtxBone[i] = XMMatrixIdentity();
	}
	gpCommonState->m_pd3dDeviceContext->Unmap(m_pd3dcbBoneMatrix, NULL);
}


void CObject::AnimateAndRender(float& time)
{
	if (eAnimationType::None != m_eAnimationType)
	{
		m_fAnimationPlaytime += time * 1000;

		/* 현재 애니메이션을 한 번 완료했을 때 */
		if ((m_fAnimationPlaytime / 10) >= m_iAniMaxTime[static_cast<int>(m_eAnimationType)] / 10)
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

		for (int i = 0; i < m_iAnimationIndexCount; ++i)
		{
			XMMATRIX ResultMatrix = XMLoadFloat4x4(&m_pppResult[static_cast<int>(m_eAnimationType)][static_cast<int>(m_fAnimationPlaytime) / 10][i]);
			m_pcbBoneMatrix->m_mtxBone[i] = ResultMatrix;
		}
		if (m_pd3dcbBoneMatrix)
		{
			gpCommonState->m_pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_BONE_MATRIX, 1, &m_pd3dcbBoneMatrix);
		}
	}

	if (m_pMesh) m_pMesh->Render();
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
	case CObject::eAnimationType::Dead:		// 죽었으면 애니메이션이 안 바뀜
	case CObject::eAnimationType::Attack:	// 공격중엔 바꾸지 마라
	case CObject::eAnimationType::Skill1:
	case CObject::eAnimationType::Skill2:
	case CObject::eAnimationType::Skill3:
	case CObject::eAnimationType::None:
		return;
	default:break;
	}
}

void CObject::SetComponent()
{
	m_pUnitComponent = new CUnitComponent();
}






