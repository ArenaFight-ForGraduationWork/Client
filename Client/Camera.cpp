#include "stdafx.h"
#include "Camera.h"

#include "ConstantBuffers.h"



CCamera::CCamera()
{
	m_pd3dxvPosition = new  XMFLOAT3(0.0f, 1.0f, 0.0f);

	m_pd3dxvRight = new  XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pd3dxvUp = new  XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_pd3dxvLook = new  XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_fTheta = 200.0f;
	m_fDistanceFromObject = 800.0f;
	m_fHeight = 400.0f;

	m_fTimeLag = 0.0f;

	m_pd3dxvLookAtWorld = new  XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_eMode = eCameraType::THIRD_PERSON_CAMERA;

	m_pd3dxmtxView = new  XMFLOAT4X4();
	XMStoreFloat4x4(m_pd3dxmtxView, XMMatrixIdentity());
	m_pd3dxmtxProjection = new  XMFLOAT4X4();
	XMStoreFloat4x4(m_pd3dxmtxProjection, XMMatrixIdentity());

	m_pd3dcbCamera = nullptr;
}
CCamera::~CCamera()
{
	if (m_pd3dcbCamera) m_pd3dcbCamera->Release();
}

void CCamera::SetViewport(DWORD xTopLeft, DWORD yTopLeft, DWORD nWidth, DWORD nHeight, float fMinZ, float fMaxZ)
{
	m_pd3dViewport = new D3D11_VIEWPORT();
	m_pd3dViewport->TopLeftX = float(xTopLeft);
	m_pd3dViewport->TopLeftY = float(yTopLeft);
	m_pd3dViewport->Width = float(nWidth);
	m_pd3dViewport->Height = float(nHeight);
	m_pd3dViewport->MinDepth = fMinZ;
	m_pd3dViewport->MaxDepth = fMaxZ;
	gpCommonState->m_pd3dDeviceContext->RSSetViewports(1, m_pd3dViewport);
}

void CCamera::GenerateViewMatrix()
{
	// convert XMFLOAT3 to XMVECTOR
	XMVECTOR P, L, U;
	P = XMLoadFloat3(m_pd3dxvPosition);
	L = XMLoadFloat3(m_pd3dxvLookAtWorld);
	U = XMLoadFloat3(m_pd3dxvUp);

	// generate camera view matrix
	XMStoreFloat4x4(m_pd3dxmtxView, XMMatrixLookAtLH(P, L, U));
}
void CCamera::GenerateViewMatrix(const D3DXVECTOR3 *pd3dxvEyePosition, const D3DXVECTOR3 *pd3dxvLookAt, const D3DXVECTOR3 *pd3dxvUp)
{
	// convert D3DXVECTOR3 to XMFLOAT3
	XMFLOAT3 pos, lookAt, up;
	pos.x = pd3dxvEyePosition->x;	pos.y = pd3dxvEyePosition->y;	pos.z = pd3dxvEyePosition->z;
	lookAt.x = pd3dxvLookAt->x;	lookAt.y = pd3dxvLookAt->y;	lookAt.z = pd3dxvLookAt->z;
	up.x = pd3dxvUp->x;	up.y = pd3dxvUp->y;	up.z = pd3dxvUp->z;

	// convert XMFLOAT3 to XMVECTOR
	XMVECTOR P, L, U;
	P = XMLoadFloat3(&pos);
	L = XMLoadFloat3(&lookAt);
	U = XMLoadFloat3(&up);

	// generate camera view matrix
	XMStoreFloat4x4(m_pd3dxmtxView, XMMatrixLookAtLH(P, L, U));
}
void CCamera::RegenerateViewMatrix()
{
	/* 변환하다보면 Right, Up, Look축이 서로 직교하지 않을 수 있으므로,	직교하도록 다시 계산한다 */

	// convert XMFLOAT3 to XMVECTOR
	XMVECTOR R, U, L, pos;
	R = XMLoadFloat3(m_pd3dxvRight);
	U = XMLoadFloat3(m_pd3dxvUp);
	L = XMLoadFloat3(m_pd3dxvLook);
	pos = XMLoadFloat3(m_pd3dxvPosition);

	// re-caculate x, y, z-axis
	XMVector3Normalize(L);
	R = XMVector3Cross(U, L);
	XMVector3Normalize(R);
	U = XMVector3Cross(L, R);
	XMVector3Normalize(U);

	// convert XMVECTOR to XMFLOAT3( to store vectors again )
	XMStoreFloat3(m_pd3dxvRight, R);
	XMStoreFloat3(m_pd3dxvUp, U);
	XMStoreFloat3(m_pd3dxvLook, L);

	// re-generate matrix using vectors( Right, Up, Look, position )
	m_pd3dxmtxView->_11 = m_pd3dxvRight->x;
	m_pd3dxmtxView->_12 = m_pd3dxvUp->x;
	m_pd3dxmtxView->_13 = m_pd3dxvLook->x;
	m_pd3dxmtxView->_21 = m_pd3dxvRight->y;
	m_pd3dxmtxView->_22 = m_pd3dxvUp->y;
	m_pd3dxmtxView->_23 = m_pd3dxvLook->y;
	m_pd3dxmtxView->_31 = m_pd3dxvRight->z;
	m_pd3dxmtxView->_32 = m_pd3dxvUp->z;
	m_pd3dxmtxView->_33 = m_pd3dxvLook->z;
	m_pd3dxmtxView->_41 = -XMVectorGetX(XMVector3Dot(pos, R));
	m_pd3dxmtxView->_42 = -XMVectorGetX(XMVector3Dot(pos, U));
	m_pd3dxmtxView->_43 = -XMVectorGetX(XMVector3Dot(pos, L));
}

void CCamera::GenerateProjectionMatrix(const float fNearPlaneDistance, const float fFarPlaneDistance, const float fAspectRatio, const float fFOVAngle)
{
	XMMATRIX projection;

	m_fNearPlaneDistance = fNearPlaneDistance;
	m_fFarPlaneDistance = fFarPlaneDistance;
	m_fAspectRatio = fAspectRatio;
	m_fFOVAngle = static_cast<float>(D3DXToRadian(fFOVAngle));

	// generate camera projection matrix
	projection = XMMatrixPerspectiveFovLH(static_cast<float>(D3DXToRadian(fFOVAngle)), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);

	// convert XMMATRIX to XMFLOAT4X4
	XMStoreFloat4x4(m_pd3dxmtxProjection, projection);
}

void CCamera::CreateShaderVariables()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gpCommonState->m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
}

void CCamera::UpdateShaderVariables()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	gpCommonState->m_pd3dDeviceContext->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_VIEWPROJECTION_MATRIX *pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX *)d3dMappedResource.pData;
	pcbViewProjection->m_d3dxmtxView = XMMatrixTranspose(XMLoadFloat4x4(m_pd3dxmtxView));
	pcbViewProjection->m_d3dxmtxProjection = XMMatrixTranspose(XMLoadFloat4x4(m_pd3dxmtxProjection));
	gpCommonState->m_pd3dDeviceContext->Unmap(m_pd3dcbCamera, 0);

	gpCommonState->m_pd3dDeviceContext->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbCamera);
}

const float CCamera::GetYaw()
{
	float fYaw = 270 - m_fTheta;
	if (fYaw < 0) fYaw += 360;
	else if (fYaw > 359) fYaw -= 360;

	return fYaw;
}

void CCamera::Zoom(const float fZoom)
{
	if (m_fDistanceFromObject + fZoom < 10.0f)
		return;
	else if (m_fDistanceFromObject + fZoom > 2000)
		return;

	float fGradient = m_fDistanceFromObject / m_fHeight;	// 기울기

	m_fHeight = (m_fHeight * (m_fDistanceFromObject + fZoom)) / m_fDistanceFromObject;
	m_fDistanceFromObject += fZoom;
}




CThirdPersonCamera::CThirdPersonCamera() : CCamera()
{
	m_eMode = eCameraType::THIRD_PERSON_CAMERA;
}

void CThirdPersonCamera::Update(const D3DXVECTOR3 *pd3dxvPosition)
{
	double theta = D3DXToRadian(m_fTheta);
	m_pd3dxvPosition->x = pd3dxvPosition->x + (m_fDistanceFromObject * static_cast<float>(cos(theta)));
	m_pd3dxvPosition->y = pd3dxvPosition->y + m_fHeight;
	m_pd3dxvPosition->z = pd3dxvPosition->z + (m_fDistanceFromObject * static_cast<float>(sin(theta)));

	D3DXVECTOR3 d3dxvLookAtPosition = D3DXVECTOR3(pd3dxvPosition->x, pd3dxvPosition->y + 170, pd3dxvPosition->z);
	SetLookAtPosition(d3dxvLookAtPosition);

	//D3DXVECTOR3 *pTemp = new D3DXVECTOR3(XMVectorGetX(GetLookAtPosition()), XMVectorGetY(GetLookAtPosition()), XMVectorGetZ(GetLookAtPosition()));
	//SetLookAt(pTemp);
	SetLookAt(&d3dxvLookAtPosition);

	RegenerateViewMatrix();
}

void CThirdPersonCamera::SetLookAt(const D3DXVECTOR3 *pd3dxvLookAt)
{
	XMFLOAT4X4 mtx;
	XMVECTOR P, L, U;
	XMFLOAT3 lookAt, up, temp;

	// convert XMFLOAT3 to XMVECTOR
	P = XMLoadFloat3(m_pd3dxvPosition);
	lookAt.x = pd3dxvLookAt->x;	lookAt.y = pd3dxvLookAt->y;	lookAt.z = pd3dxvLookAt->z;
	L = XMLoadFloat3(&lookAt);
	up = XMFLOAT3(0, 1, 0);
	U = XMLoadFloat3(&up);

	// caculate lookAt matrix
	XMStoreFloat4x4(&mtx, XMMatrixLookAtLH(P, L, U));

	temp = XMFLOAT3(mtx._11, mtx._21, mtx._31);
	if (!XMVector3Equal(XMLoadFloat3(&XMFLOAT3(0, 0, 0)), XMLoadFloat3(&temp)))
	{
		*m_pd3dxvRight = XMFLOAT3(mtx._11, mtx._21, mtx._31);
	}

	temp = XMFLOAT3(mtx._12, mtx._22, mtx._32);
	if (!XMVector3Equal(XMLoadFloat3(&XMFLOAT3(0, 0, 0)), XMLoadFloat3(&temp)))
	{
		*m_pd3dxvUp = XMFLOAT3(mtx._12, mtx._22, mtx._32);
	}

	temp = XMFLOAT3(mtx._13, mtx._23, mtx._33);
	if (!XMVector3Equal(XMLoadFloat3(&XMFLOAT3(0, 0, 0)), XMLoadFloat3(&temp)))
	{
		*m_pd3dxvLook = XMFLOAT3(mtx._13, mtx._23, mtx._33);
	}
}

void CThirdPersonCamera::RotatebyYaw(const float fYaw)
{
	m_fTheta += fYaw;

	if (m_fTheta < 0)
		m_fTheta += 360;
	else if (m_fTheta > 359)
		m_fTheta -= 360;
}









CCameraManager::CCameraManager()
{
}
CCameraManager::~CCameraManager()
{
}
CCameraManager* CCameraManager::GetSingleton()
{
	static CCameraManager instance;
	return &instance;
}

void CCameraManager::Initialize()
{
	CCamera *pCamera;

	pCamera = new CThirdPersonCamera();
	pCamera->CreateShaderVariables();
	pCamera->SetLookAt(new D3DXVECTOR3(0, 0, 0));
	gpCommonState->m_pd3dDevice->GetImmediateContext(&gpCommonState->m_pd3dDeviceContext);
	pCamera->SetViewport(0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT, 0.0f, 1.0f);
	pCamera->GenerateProjectionMatrix(1.01f, 10000.0f, ASPECT_RATIO, 60.0f);
	pCamera->GenerateViewMatrix();
	m_mCameras[eCameraType::THIRD_PERSON_CAMERA] = pCamera;

	m_eNow = eCameraType::THIRD_PERSON_CAMERA;
}
void CCameraManager::Destroy()
{
}

CCamera* CCameraManager::GetNowCamera()
{
	return m_mCameras[m_eNow];
}
void CCameraManager::ChangeCamera(eCameraType eType)
{
	m_eNow = eType;
}

