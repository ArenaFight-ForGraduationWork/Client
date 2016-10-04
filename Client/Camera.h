#ifndef CAMERA_H_
#define CAMERA_H_



enum class eCameraType : BYTE{
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
	void GenerateOrthoMatrix(const float fNearPlaneDistance, const float fFarPlaneDistance);
	D3DXMATRIX* GetOrthoMatrix() { return m_pd3dxmtxOrtho; }

	void CreateShaderVariables();
	void UpdateShaderVariables();

	void SetViewport(DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
	const D3D11_VIEWPORT* GetViewport() { return m_pd3dViewport; }

	const D3DXMATRIX* GetViewMatrix() { return m_pd3dxmtxView; }
	const D3DXMATRIX* GetProjectionMatrix() { return m_pd3dxmtxProjection; }
	ID3D11Buffer* GetCameraConstantBuffer() { return(m_pd3dcbCamera); }

	void SetPosition(D3DXVECTOR3 d3dxvPosition) { *m_pd3dxvPosition = d3dxvPosition; }
	const D3DXVECTOR3* GetPosition() { return m_pd3dxvPosition; }

	void SetLookAtPosition(D3DXVECTOR3 d3dxvLookAtWorld) { *m_pd3dxvLookAtWorld = d3dxvLookAtWorld; }
	const D3DXVECTOR3* GetLookAtPosition() { return m_pd3dxvLookAtWorld; }

	const D3DXVECTOR3* GetRightVector() { return m_pd3dxvRight; }
	const D3DXVECTOR3* GetUpVector() { return m_pd3dxvUp; }
	const D3DXVECTOR3* GetLookVector() { return m_pd3dxvLook; }

	const float GetYaw();

	void SetTimeLag(float fTimeLag) { m_fTimeLag = fTimeLag; }
	float GetTimeLag() { return(m_fTimeLag); }

	virtual void RotatebyYaw(const float fYaw = 0.0f) = 0;
	//ī�޶��� �̵�, ȸ���� ���� ī�޶��� ������ �����ϴ� �����Լ��̴�.
	virtual void Update(const D3DXVECTOR3 *pd3dxvPosition) = 0;
	/*3��Ī ī�޶󿡼� ī�޶� �ٶ󺸴� ������ �����ϴ� �����Լ��̴�. �Ϲ������� �÷��̾ �ٶ󺸵��� �����Ѵ�.*/
	virtual void SetLookAt(const D3DXVECTOR3 *vLookAt) = 0;

	void Zoom(const float fZoom);

	// private�� �̵�
	float m_fNearPlaneDistance;
	float m_fFarPlaneDistance;
	float m_fAspectRatio;
	float m_fFOVAngle;

protected:
	D3DXVECTOR3 *m_pd3dxvPosition;

	D3DXVECTOR3 *m_pd3dxvRight;
	D3DXVECTOR3 *m_pd3dxvUp;
	D3DXVECTOR3 *m_pd3dxvLook;

	float m_fTheta;
	float m_fDistanceFromObject;
	float m_fHeight;

	eCameraType m_eMode;

	/* ������ǥ�迡�� ī�޶� �ٶ󺸴� �� */
	D3DXVECTOR3 *m_pd3dxvLookAtWorld;

	/* �÷��̾��� �̵����� ī�޶��� �̵������� �ð� */
	float m_fTimeLag;

	/* ī�޶� ��ȯ��� */
	D3DXMATRIX *m_pd3dxmtxView;
	/* ���� ��ȯ��� */
	D3DXMATRIX *m_pd3dxmtxProjection;
	/* ���翵 ��ȯ��� */
	D3DXMATRIX *m_pd3dxmtxOrtho;
	/* ī�޶� ���庯ȯ ��� */
	D3DXMATRIX *m_pd3dxmtxWorld;

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