#include "stdafx.h"
#include "Object.h"
#include "Player.h"
#include "Camera.h"

CCamera::CCamera()
{
	m_pd3dxvPosition = new D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_pd3dxvRight = new D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_pd3dxvUp = new D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_pd3dxvLook = new D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_fTheta = 270.0f;
	m_fDistanceFromObject = 50.0f;

	m_fTimeLag = 0.0f;

	m_pd3dxvLookAtWorld = new D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nMode = 0x00;

	m_pd3dxmtxView = new D3DXMATRIX();
	D3DXMatrixIdentity(m_pd3dxmtxView);
	m_pd3dxmtxProjection = new D3DXMATRIX();
	D3DXMatrixIdentity(m_pd3dxmtxProjection);

	m_pd3dcbCamera = nullptr;
}
CCamera::~CCamera()
{
	if (m_pd3dcbCamera) m_pd3dcbCamera->Release();
}

void CCamera::SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_pd3dViewport = new D3D11_VIEWPORT();
	m_pd3dViewport->TopLeftX = float(xTopLeft);
	m_pd3dViewport->TopLeftY = float(yTopLeft);
	m_pd3dViewport->Width = float(nWidth);
	m_pd3dViewport->Height = float(nHeight);
	m_pd3dViewport->MinDepth = fMinZ;
	m_pd3dViewport->MaxDepth = fMaxZ;
	pd3dDeviceContext->RSSetViewports(1, m_pd3dViewport);
}

void CCamera::GenerateViewMatrix()
{
	D3DXMatrixLookAtLH(m_pd3dxmtxView, m_pd3dxvPosition, m_pd3dxvLookAtWorld, m_pd3dxvUp);
}
void CCamera::GenerateViewMatrix(const D3DXVECTOR3 *pd3dxvEyePosition, const D3DXVECTOR3 *pd3dxvLookAt, const D3DXVECTOR3 *pd3dxvUp)
{
	D3DXMatrixLookAtLH(m_pd3dxmtxView, pd3dxvEyePosition, pd3dxvLookAt, pd3dxvUp);
}
void CCamera::RegenerateViewMatrix()
{
	/* 변환하다보면 Right, Up, Look축이 서로 직교하지 않을 수 있으므로,	직교하도록 다시 계산한다 */

	/* 1) 카메라의 Z축 벡터를 정규화 */
	D3DXVec3Normalize(m_pd3dxvLook, m_pd3dxvLook);
	/* 2) 카메라의 Z축과 Y축에 수직인 벡터를 X축으로 설정 */
	D3DXVec3Cross(m_pd3dxvRight, m_pd3dxvUp, m_pd3dxvLook);
	/* 3) 카메라의 X축 벡터를 정규화 */
	D3DXVec3Normalize(m_pd3dxvRight, m_pd3dxvRight);
	/* 4) 카메라의 Z축과 X축에 수직인 벡터를 Y축으로 설정 */
	D3DXVec3Cross(m_pd3dxvUp, m_pd3dxvLook, m_pd3dxvRight);
	/* 5) 카메라의 Y축 벡터를 정규화 */
	D3DXVec3Normalize(m_pd3dxvUp, m_pd3dxvUp);

	/* 6) Right, Up, Look벡터와 Position벡터로 변환행렬 생성 */
	m_pd3dxmtxView->_11 = m_pd3dxvRight->x;
	m_pd3dxmtxView->_12 = m_pd3dxvUp->x;
	m_pd3dxmtxView->_13 = m_pd3dxvLook->x;
	m_pd3dxmtxView->_21 = m_pd3dxvRight->y;
	m_pd3dxmtxView->_22 = m_pd3dxvUp->y;
	m_pd3dxmtxView->_23 = m_pd3dxvLook->y;
	m_pd3dxmtxView->_31 = m_pd3dxvRight->z;
	m_pd3dxmtxView->_32 = m_pd3dxvUp->z;
	m_pd3dxmtxView->_33 = m_pd3dxvLook->z;
	m_pd3dxmtxView->_41 = -D3DXVec3Dot(m_pd3dxvPosition, m_pd3dxvRight);
	m_pd3dxmtxView->_42 = -D3DXVec3Dot(m_pd3dxvPosition, m_pd3dxvUp);
	m_pd3dxmtxView->_43 = -D3DXVec3Dot(m_pd3dxvPosition, m_pd3dxvLook);
}

void CCamera::GenerateProjectionMatrix(const float fNearPlaneDistance, const float fFarPlaneDistance, const float fAspectRatio, const float fFOVAngle)
{
	D3DXMatrixPerspectiveFovLH(m_pd3dxmtxProjection, (float)D3DXToRadian(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
}

void CCamera::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
}

void CCamera::UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_VIEWPROJECTION_MATRIX *pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX *)d3dMappedResource.pData;
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxView, m_pd3dxmtxView);
	D3DXMatrixTranspose(&pcbViewProjection->m_d3dxmtxProjection, m_pd3dxmtxProjection);
	pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

	pd3dDeviceContext->VSSetConstantBuffers(NULL, 1, &m_pd3dcbCamera);
}

const float CCamera::GetYaw()
{
	float fYaw = 270 - m_fTheta;
	if (fYaw < 0) fYaw += 360;
	else if (fYaw > 359) fYaw -= 360;

	return fYaw;
}




CThirdPersonCamera::CThirdPersonCamera() : CCamera()
{
	m_nMode = THIRD_PERSON_CAMERA;
}

void CThirdPersonCamera::Update(const D3DXVECTOR3 *pd3dxvPosition)
{
	double theta = D3DXToRadian(m_fTheta);
	m_pd3dxvPosition->x = pd3dxvPosition->x + (m_fDistanceFromObject * cos(theta));
	m_pd3dxvPosition->y = pd3dxvPosition->y + 20;
	m_pd3dxvPosition->z = pd3dxvPosition->z + (m_fDistanceFromObject * sin(theta));

	SetLookAtPosition(*pd3dxvPosition);
	SetLookAt(GetLookAtPosition());

	RegenerateViewMatrix();
}

void CThirdPersonCamera::SetLookAt(const D3DXVECTOR3 *pd3dxvLookAt)
{
	D3DXMATRIX mtxLookAt;
	D3DXMatrixLookAtLH(&mtxLookAt, m_pd3dxvPosition, pd3dxvLookAt, &D3DXVECTOR3(0, 1, 0));
	m_pd3dxvRight = new D3DXVECTOR3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
	m_pd3dxvUp = new D3DXVECTOR3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
	m_pd3dxvLook = new D3DXVECTOR3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}

void CThirdPersonCamera::RotatebyYaw(const float fYaw)
{
	m_fTheta += fYaw;

	if (m_fTheta < 0)
		m_fTheta += 360;
	else if (m_fTheta > 359)
		m_fTheta -= 360;
}



