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
	//ī�޶��� �̵�, ȸ���� ���� ī�޶��� ������ �����ϴ� �����Լ��̴�.
	virtual void Update(CXMVECTOR vPosition) = 0;
	/*3��Ī ī�޶󿡼� ī�޶� �ٶ󺸴� ������ �����ϴ� �����Լ��̴�. �Ϲ������� �÷��̾ �ٶ󺸵��� �����Ѵ�.*/
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

	/* ������ǥ�迡�� ī�޶� �ٶ󺸴� �� */
	XMFLOAT3 *m_f3LookAtWorld;

	/* �÷��̾��� �̵����� ī�޶��� �̵������� �ð� */
	float m_fTimeLag;

	/* ī�޶� ��ȯ��� */
	XMFLOAT4X4 *m_f4x4View;
	/* ���� ��ȯ��� */
	XMFLOAT4X4 *m_f4x4Projection;

	// ��-��Ʈ�� ��Ÿ���� ��� ������ �����Ѵ�.
	D3D11_VIEWPORT *m_pd3dViewport;

	/* ī�޶� ��ȯ����� ���� ������� �������̽� ������ */
	ID3D11Buffer *m_pd3dcbViewProjection;
	/* ���� ��ȯ����� ���� ������� �������̽� ������ */
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