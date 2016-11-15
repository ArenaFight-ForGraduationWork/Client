#include "stdafx.h"
#include "Camera.h"
#include "ConstantBuffers.h"





//
//	Camera
//
CCamera::CCamera()
{
	m_f3Position = new XMFLOAT3(0.0f, 1.0f, 0.0f);

	m_f3Right = new  XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_f3Up = new  XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_f3Look = new  XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_fTheta = 200.0f;
	m_fDistanceFromObject = 800.0f;
	m_fHeight = 400.0f;

	m_fTimeLag = 0.0f;

	m_f3LookAtWorld = new  XMFLOAT3(0.0f, 0.0f, 1.0f);

	m_eMode = eCameraType::THIRD_PERSON_CAMERA;

	m_f4x4View = new  XMFLOAT4X4();
	XMStoreFloat4x4(m_f4x4View, XMMatrixIdentity());
	m_f4x4Projection = new  XMFLOAT4X4();
	XMStoreFloat4x4(m_f4x4Projection, XMMatrixIdentity());

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
	gpCommonState->GetDeviceContext()->RSSetViewports(1, m_pd3dViewport);
}

void CCamera::GenerateViewMatrix()
{
	// convert XMFLOAT3 to XMVECTOR
	XMVECTOR P, L, U;
	P = XMLoadFloat3(m_f3Position);
	L = XMLoadFloat3(m_f3LookAtWorld);
	U = XMLoadFloat3(m_f3Up);

	// generate camera view matrix
	XMStoreFloat4x4(m_f4x4View, XMMatrixLookAtLH(P, L, U));
}
void CCamera::GenerateViewMatrix(CXMVECTOR vEyePosition, CXMVECTOR vLookAt, CXMVECTOR vUp)
{
	// generate camera view matrix
	XMStoreFloat4x4(m_f4x4View, XMMatrixLookAtLH(vEyePosition, vLookAt, vUp));
}
void CCamera::RegenerateViewMatrix()
{
	/* 변환하다보면 Right, Up, Look축이 서로 직교하지 않을 수 있으므로,	직교하도록 다시 계산한다 */

	// convert XMFLOAT3 to XMVECTOR
	XMVECTOR R, U, L, pos;
	R = XMLoadFloat3(m_f3Right);
	U = XMLoadFloat3(m_f3Up);
	L = XMLoadFloat3(m_f3Look);
	pos = XMLoadFloat3(m_f3Position);

	// re-caculate x, y, z-axis
	XMVector3Normalize(L);
	R = XMVector3Cross(U, L);
	XMVector3Normalize(R);
	U = XMVector3Cross(L, R);
	XMVector3Normalize(U);

	// convert XMVECTOR to XMFLOAT3( to store vectors again )
	XMStoreFloat3(m_f3Right, R);
	XMStoreFloat3(m_f3Up, U);
	XMStoreFloat3(m_f3Look, L);

	// re-generate matrix using vectors( Right, Up, Look, position )
	m_f4x4View->_11 = m_f3Right->x;
	m_f4x4View->_21 = m_f3Right->y;
	m_f4x4View->_31 = m_f3Right->z;
	m_f4x4View->_12 = m_f3Up->x;
	m_f4x4View->_22 = m_f3Up->y;
	m_f4x4View->_32 = m_f3Up->z;
	m_f4x4View->_13 = m_f3Look->x;
	m_f4x4View->_23 = m_f3Look->y;
	m_f4x4View->_33 = m_f3Look->z;
	m_f4x4View->_41 = -XMVectorGetX(XMVector3Dot(pos, R));
	m_f4x4View->_42 = -XMVectorGetX(XMVector3Dot(pos, U));
	m_f4x4View->_43 = -XMVectorGetX(XMVector3Dot(pos, L));
}

void CCamera::GenerateProjectionMatrix(const float fNearPlaneDistance, const float fFarPlaneDistance, const float fAspectRatio, const float fFOVAngle)
{
	XMMATRIX projection;

	// generate camera projection matrix
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);

	// convert XMMATRIX to XMFLOAT4X4
	XMStoreFloat4x4(m_f4x4Projection, projection);
}

void CCamera::CreateShaderVariables()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VS_CB_VIEWPROJECTION_MATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gpCommonState->GetDevice()->CreateBuffer(&bd, NULL, &m_pd3dcbCamera);
}

void CCamera::UpdateShaderVariables()
{
	D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
	gpCommonState->GetDeviceContext()->Map(m_pd3dcbCamera, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
	VS_CB_VIEWPROJECTION_MATRIX *pcbViewProjection = (VS_CB_VIEWPROJECTION_MATRIX *)d3dMappedResource.pData;
	pcbViewProjection->m_mtxView = XMMatrixTranspose(XMLoadFloat4x4(m_f4x4View));
	pcbViewProjection->m_mtxProjection = XMMatrixTranspose(XMLoadFloat4x4(m_f4x4Projection));
	gpCommonState->GetDeviceContext()->Unmap(m_pd3dcbCamera, 0);

	gpCommonState->GetDeviceContext()->VSSetConstantBuffers(VS_SLOT_VIEWPROJECTION_MATRIX, 1, &m_pd3dcbCamera);
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





//
//	Third Person Camera
//
CThirdPersonCamera::CThirdPersonCamera() : CCamera()
{
	m_eMode = eCameraType::THIRD_PERSON_CAMERA;
}

void CThirdPersonCamera::Update(CXMVECTOR vPosition)
{
	float theta = XMConvertToRadians(m_fTheta);
	m_f3Position->x = XMVectorGetX(vPosition) + (m_fDistanceFromObject * cos(theta));
	m_f3Position->y = XMVectorGetY(vPosition) + m_fHeight;
	m_f3Position->z = XMVectorGetZ(vPosition) + (m_fDistanceFromObject * sin(theta));

	XMFLOAT3 f3LookAt = XMFLOAT3(XMVectorGetX(vPosition), XMVectorGetY(vPosition) + 170, XMVectorGetZ(vPosition));
	XMVECTOR vLookAt;
	vLookAt = XMLoadFloat3(&f3LookAt);
	SetLookAtPosition(vLookAt);
	SetLookAt(vLookAt);

	RegenerateViewMatrix();
}

void CThirdPersonCamera::SetLookAt(CXMVECTOR vLookAt)
{
	XMFLOAT4X4 mtx;
	XMVECTOR P, U;
	XMFLOAT3 up, temp;

	// convert XMFLOAT3 to XMVECTOR
	P = XMLoadFloat3(m_f3Position);
	up = XMFLOAT3(0, 1, 0);
	U = XMLoadFloat3(&up);

	// caculate lookAt matrix
	XMStoreFloat4x4(&mtx, XMMatrixLookAtLH(P, vLookAt, U));

	temp = XMFLOAT3(mtx._11, mtx._21, mtx._31);
	if (!XMVector3Equal(XMLoadFloat3(&XMFLOAT3(0, 0, 0)), XMLoadFloat3(&temp)))
	{
		*m_f3Right = XMFLOAT3(mtx._11, mtx._21, mtx._31);
	}

	temp = XMFLOAT3(mtx._12, mtx._22, mtx._32);
	if (!XMVector3Equal(XMLoadFloat3(&XMFLOAT3(0, 0, 0)), XMLoadFloat3(&temp)))
	{
		*m_f3Up = XMFLOAT3(mtx._12, mtx._22, mtx._32);
	}

	temp = XMFLOAT3(mtx._13, mtx._23, mtx._33);
	if (!XMVector3Equal(XMLoadFloat3(&XMFLOAT3(0, 0, 0)), XMLoadFloat3(&temp)))
	{
		*m_f3Look = XMFLOAT3(mtx._13, mtx._23, mtx._33);
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





//
//	Camera Manager
//
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
	XMFLOAT3 lookAt = XMFLOAT3(0, 0, 0);
	pCamera->SetLookAt(XMLoadFloat3(&lookAt));
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


