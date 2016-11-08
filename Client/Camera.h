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
	void GenerateViewMatrix(CXMVECTOR vEyePosition, CXMVECTOR vLookAt, CXMVECTOR vUp);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(const float fNearPlaneDistance, const float fFarPlaneDistance, const float fAspectRatio, const float fFOVAngle);

	void CreateShaderVariables();
	void UpdateShaderVariables();

	void SetViewport(DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	const D3D11_VIEWPORT* GetViewport() { return m_pd3dViewport; }

	XMFLOAT4X4* GetViewMatrix() { return m_f4x4View; }
	XMFLOAT4X4* GetProjectionMatrix() { return m_f4x4Projection; }
	ID3D11Buffer* GetCameraConstantBuffer() { return m_pd3dcbCamera; }

	void SetPosition(CXMVECTOR vPosition) { XMStoreFloat3(m_f3Position, vPosition); }
	XMFLOAT3* GetPosition() { return m_f3Position; }

	void SetLookAtPosition(CXMVECTOR vLookAtWorld) { XMStoreFloat3(m_f3LookAtWorld, vLookAtWorld); }
	CXMVECTOR GetLookAtPosition() { return  XMLoadFloat3(m_f3LookAtWorld); }

	CXMVECTOR GetRightVector() { return  XMLoadFloat3(m_f3Right); }
	CXMVECTOR GetUpVector() { return  XMLoadFloat3(m_f3Up); }
	CXMVECTOR GetLookVector() { return  XMLoadFloat3(m_f3Look); }

	const float GetYaw();

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return(m_fTimeLag); }

	virtual void RotatebyYaw(const float fYaw = 0.0f) = 0;
	//카메라의 이동, 회전에 따라 카메라의 정보를 갱신하는 가상함수이다.
	virtual void Update(CXMVECTOR vPosition) = 0;
	/*3인칭 카메라에서 카메라가 바라보는 지점을 설정하는 가상함수이다. 일반적으로 플레이어를 바라보도록 설정한다.*/
	virtual void SetLookAt(CXMVECTOR vLookAt) = 0;

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
	virtual void Update(CXMVECTOR vPosition);
	virtual void SetLookAt(CXMVECTOR vLookAt);
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
	map<eCameraType, CCamera*> m_mCameras;

	eCameraType m_eNow;

	CCameraManager();
};


#endif