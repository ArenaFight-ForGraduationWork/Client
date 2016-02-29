#ifndef CAMERA_H_
#define CAMERA_H_



//카메라의 종류(모드: Mode)를 나타내는 상수를 다음과 같이 선언한다.
#define THIRD_PERSON_CAMERA	0x01



struct VS_CB_VIEWPROJECTION_MATRIX
{
	D3DXMATRIX m_d3dxmtxView;
	D3DXMATRIX m_d3dxmtxProjection;
};





class CCamera
{
public:
	CCamera();
	~CCamera();

	DWORD GetMode() { return m_nMode; }

	void GenerateViewMatrix();
	void GenerateViewMatrix(const D3DXVECTOR3 *pd3dxvEyePosition, const D3DXVECTOR3 *pd3dxvLookAt, const D3DXVECTOR3 *pd3dxvUp);
	void RegenerateViewMatrix();

	void GenerateProjectionMatrix(const float fNearPlaneDistance, const float fFarPlaneDistance, const float fAspectRatio, const float fFOVAngle);

	void CreateShaderVariables(ID3D11Device *pd3dDevice);
	void UpdateShaderVariables(ID3D11DeviceContext *pd3dDeviceContext);

	void SetViewport(ID3D11DeviceContext *pd3dDeviceContext, DWORD xStart, DWORD yStart, DWORD nWidth, DWORD nHeight, float fMinZ = 0.0f, float fMaxZ = 1.0f);
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

	//카메라를 d3dxvShift 만큼 이동하는 가상함수이다.
	virtual void Move(const D3DXVECTOR3* d3dxvShift) { *m_pd3dxvPosition += *d3dxvShift; }
	//카메라를 x-축, y-축, z-축으로 회전하는 가상함수이다.
	virtual void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f) {}

	virtual void RotatebyYaw(const float fYaw = 0.0f) = 0;
	//카메라의 이동, 회전에 따라 카메라의 정보를 갱신하는 가상함수이다.
	virtual void Update(const D3DXVECTOR3 *pd3dxvPosition) = 0;
	/*3인칭 카메라에서 카메라가 바라보는 지점을 설정하는 가상함수이다. 일반적으로 플레이어를 바라보도록 설정한다.*/
	virtual void SetLookAt(const D3DXVECTOR3 *vLookAt) = 0;


protected:
	D3DXVECTOR3 *m_pd3dxvPosition;

	D3DXVECTOR3 *m_pd3dxvRight;
	D3DXVECTOR3 *m_pd3dxvUp;
	D3DXVECTOR3 *m_pd3dxvLook;

	float m_fTheta;
	float m_fDistanceFromObject;

	DWORD m_nMode;

	/* 월드좌표계에서 카메라가 바라보는 점 */
	D3DXVECTOR3 *m_pd3dxvLookAtWorld;

	/* 플레이어의 이동에서 카메라의 이동까지의 시간 */
	float m_fTimeLag;

	/* 카메라 변환행렬 */
	D3DXMATRIX *m_pd3dxmtxView;
	/* 투영 변환행렬 */
	D3DXMATRIX *m_pd3dxmtxProjection;

	//뷰-포트를 나타내는 멤버 변수를 선언한다.
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


#endif