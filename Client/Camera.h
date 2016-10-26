#ifndef CAMERA_H_
#define CAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;



enum class eCameraType : BYTE {
	START = 0,
	THIRD_PERSON_CAMERA = 0,
	END
};



class CCamera
{
public:
	CCamera();
	~CCamera();

	eCameraType GetMode() { return m_eMode; }

	void GenerateViewMatrix();
	void GenerateViewMatrix(const D3DXVECTOR3 *pd3dxvEyePosition, const D3DXVECTOR3 *pd3dxvLookAt, const D3DXVECTOR3 *pd3dxvUp);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(const float fNearPlaneDistance, const float fFarPlaneDistance, const float fAspectRatio, const float fFOVAngle);

	void CreateShaderVariables();
	void UpdateShaderVariables();

	void SetViewport(DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	const D3D11_VIEWPORT* GetViewport() { return m_pd3dViewport; }

	CXMMATRIX GetViewMatrix() { return  XMLoadFloat4x4(m_f4x4View); }
	CXMMATRIX GetProjectionMatrix() { return  XMLoadFloat4x4(m_f4x4Projection); }
	ID3D11Buffer* GetCameraConstantBuffer() { return m_pd3dcbCamera; }

	void SetPosition(D3DXVECTOR3 d3dxvPosition) {
		m_f3Position->x = d3dxvPosition.x;
		m_f3Position->y = d3dxvPosition.y;
		m_f3Position->z = d3dxvPosition.z;
	}
	CXMVECTOR GetPosition() { return  XMLoadFloat3(m_f3Position); }

	void SetLookAtPosition(D3DXVECTOR3 d3dxvLookAtWorld) {
		m_f3LookAtWorld->x = d3dxvLookAtWorld.x;
		m_f3LookAtWorld->y = d3dxvLookAtWorld.y;
		m_f3LookAtWorld->z = d3dxvLookAtWorld.z;
	}
	CXMVECTOR GetLookAtPosition() { return  XMLoadFloat3(m_f3LookAtWorld); }

	CXMVECTOR GetRightVector() { return  XMLoadFloat3(m_f3Right); }
	CXMVECTOR GetUpVector() { return  XMLoadFloat3(m_f3Up); }
	CXMVECTOR GetLookVector() { return  XMLoadFloat3(m_f3Look); }

	const float GetYaw();

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return(m_fTimeLag); }

	virtual void RotatebyYaw(const float fYaw = 0.0f) = 0;
	//카메라의 이동, 회전에 따라 카메라의 정보를 갱신하는 가상함수이다.
	virtual void Update(const D3DXVECTOR3 *pd3dxvPosition) = 0;
	/*3인칭 카메라에서 카메라가 바라보는 지점을 설정하는 가상함수이다. 일반적으로 플레이어를 바라보도록 설정한다.*/
	virtual void SetLookAt(const D3DXVECTOR3 *vLookAt) = 0;

	void Zoom(const float fZoom);

protected:
	XMFLOAT3 *m_f3Position;

	XMFLOAT3 *m_f3Right;
	XMFLOAT3 *m_f3Up;
	XMFLOAT3 *m_f3Look;

	float m_fTheta;
	float m_fDistanceFromObject;
	float m_fHeight;

	eCameraType m_eMode;

	/* 월드좌표계에서 카메라가 바라보는 점 */
	XMFLOAT3 *m_f3LookAtWorld;

	/* 플레이어의 이동에서 카메라의 이동까지의 시간 */
	float m_fTimeLag;

	/* 카메라 변환행렬 */
	XMFLOAT4X4 *m_f4x4View;
	/* 투영 변환행렬 */
	XMFLOAT4X4 *m_f4x4Projection;

	// 뷰-포트를 나타내는 멤버 변수를 선언한다.
	D3D11_VIEWPORT *m_pd3dViewport;

	/* 카메라 변환행렬을 위한 상수버퍼 인터페이스 포인터 */
	ID3D11Buffer *m_pd3dcbViewProjection;
	/* 투영 변환행렬을 위한 상수버퍼 인터페이스 포인터 */
	ID3D11Buffer *m_pd3dcbCamera;
};


class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera();

	virtual void RotatebyYaw(const float fYaw = 0.0f);
	virtual void Update(const D3DXVECTOR3 *pd3dxvPosition);
	virtual void SetLookAt(const D3DXVECTOR3 *vLookAt);
};





class CCameraManager
{
public:
	static CCameraManager* GetSingleton();
	~CCameraManager();

	void Initialize();
	void Destroy();

	CCamera* GetNowCamera();
	void ChangeCamera(eCameraType eType);

private:
	std::map<eCameraType, CCamera*> m_mCameras;

	eCameraType m_eNow;

	CCameraManager();
};


#endif