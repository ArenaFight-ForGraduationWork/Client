#ifndef OBJECT_H_
#define OBJECT_H_



#include "Mesh.h"

struct VS_CB_BONE_MATRIX
{
	XMMATRIX m_XMmtxBone[128];
};

/* ���� ���� ������ ǥ�� */
struct MATERIAL
{
	D3DXCOLOR m_d3dxcAmbient;
	D3DXCOLOR m_d3dxcDiffuse;
	D3DXCOLOR m_d3dxcSpecular; //(r,g,b,a=power)
	D3DXCOLOR m_d3dxcEmissive;
};

class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

	void AddRef();
	void Release();

	MATERIAL* GetMaterial() { return m_pMaterial; }

private:
	int m_nReferences;
	MATERIAL *m_pMaterial;
};





class CTexture
{
public:
	CTexture(int nTextures);
	virtual ~CTexture();

	void AddRef();
	void Release();
	
	/* nIndex��° �ε��� �ڸ��� �ؽ��ĸ� �����Ѵ� */
	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture, ID3D11SamplerState *pd3dSamplerState);

	int& GetNumOfTextures() { return m_nTextures; }

	ID3D11ShaderResourceView** GetShaderResourceViewTextures() { return m_ppd3dsrvTextures; }
	ID3D11SamplerState** GetSamplerState() { return m_ppd3dSamplerStates; }

private:
	int m_nReferences;

	ID3D11ShaderResourceView **m_ppd3dsrvTextures;
	ID3D11SamplerState **m_ppd3dSamplerStates;

	int m_nTextures;
};


class CAnimationData
{
public:
	void SetAnimationTime();
	void SetResultMatrix();
	void SetAnimationIndexCount();

	XMFLOAT4X4** GetResult(int i)
	{
		return m_ppResult[i];
	}
	
	XMFLOAT4X4*** GetResult()
	{
		return m_ppResult;
	}

	long long* GetTime()
	{
		return m_AniMaxTime;
	}

	int GetAnimationIndexCount() { return m_AnimationIndexCount; }
	long long GetTime(int i)
	{
		return m_AniMaxTime[i];
	}
	
private:
	long long m_AniMaxTime[ANIMATION_COUNT];				// �� ANIMATION_COUNT ��ŭ�� ��� �ð�		, 5��
	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];			// �� ANIMATION_COUNT ��ŭ�� ��� ��Ʈ���� , 5��
	int m_AnimationIndexCount;										// indexCount�� � �ִϸ��̼��̴��� ������� 1���� ����
};



class CObject
{
public:
	CObject(UINT id);
	virtual ~CObject();

	void MoveRelative(const float fx, const float fy, const float fz);
	void MoveRelative(const D3DXVECTOR3 *d3dxVec);
	void MoveAbsolute(const float fx, const float fy, const float fz);
	void MoveAbsolute(const D3DXVECTOR3 *d3dxVec);
	/* ���� Z�� �������� �̵��Ѵ� */
	virtual void MoveForward(float fDistance = 1.0f);

	void RotateRelative(const float fPitch, const float fYaw, const float fRoll);
	void RotateRelative(const D3DXVECTOR3 *d3dxVec);
	void RotateRelative(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle);
	void RotateAbsolute(const float fPitch, const float fYaw, const float fRoll);
	void RotateAbsolute(const D3DXVECTOR3 *d3dxVec);
	void RotateAbsolute(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle);

	//��ü�� ��ġ, ���� x-��, y-��, z-�� ���� ���͸� ��ȯ�Ѵ�.
	const D3DXVECTOR3* GetPosition();
	const D3DXVECTOR3* GetRight();
	const D3DXVECTOR3* GetUp();
	const D3DXVECTOR3* GetLookAt();

	D3DXMATRIX* GetWorldMatrix() { return m_pd3dxWorldMatrix; }

	UINT GetId() { return m_id; }

	virtual void SetMesh(CMesh *pMesh);
	CMesh* GetMesh() { return m_pMesh; }
	void SetMaterial(CMaterial *pMaterial);
	CMaterial* GetMaterial() { return m_pMaterial; }
	void SetTexture(CTexture *pTexture);
	CTexture* GetTexture() { return m_pTexture; }

	virtual void Animate(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);	//����
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	//�ִϸ��̼� + ����
	virtual void AnimateObjectAndRender(ID3D11DeviceContext*, float time);

	//=================================================================
	/* �ִϸ��̼��� ���� ���� �� �Լ� */
	ID3D11Buffer *g_pd3dcbBoneMatrix = nullptr;
	D3D11_MAPPED_SUBRESOURCE g_d3dMappedResource;
	VS_CB_BONE_MATRIX *g_pcbBoneMatrix = nullptr;
	XMFLOAT4X4 ***m_pppResult;
	float m_fAnimationPlaytime = 0.0f;
	long long  NowTime;
	int PreState;	//���� ����
	long long m_AniMaxTime[5];
	int m_AnimationIndexCount;
	int Animation_state = 0;		// IDLE, RUN, ATTACK...
	void SetConstantBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext);
	void PlayAnimation(int StateNum, ID3D11DeviceContext* pd3dDeviceContext);
	
	void SetAniIndexCount(int);
	void SetResult(XMFLOAT4X4***);
	void SetTime(long long*);

	void SetAnimationIndexCount();
	void SetResult();
	void SetTime();
	void ReadTextFile(int charNum, int State);

	void SetPlayAnimationState(int stateNum);
	//=================================================================

	//=================================================================
	/* �浹üũ�� ���� ���� �� �Լ� */
	D3DXVECTOR3 m_MaxVer;			//�浹üũ�� ���� �ִ밪
	D3DXVECTOR3 m_MinVer;			//�浹üũ�� ���� �ּҰ�

	D3DXVECTOR3 m_tempMaxVer;	// �ٿ���ڽ��� �����̱� ���� �ӽ� ����
	D3DXVECTOR3 m_tempMinVer;

	bool Collison(CObject *pObject);		//�浹üũ Ȯ���ϴ� �Լ�
	virtual void SetBoundingBox();	//������Ʈ �浹üũ�� ���� �����(?)
	virtual void MoveBoundingBox(const D3DXVECTOR3* d3dxvec);
	
	CBoundingBoxMesh* GetBoundingBox() { return m_pBoundingBox; }
	D3DXVECTOR3 GetMaxVer(){ return m_MaxVer; }
	D3DXVECTOR3 GetMinVer() { return m_MinVer; }
	//=================================================================

private:
	D3DXMATRIX *m_pd3dxWorldMatrix;

	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;
	CBoundingBoxMesh *m_pBoundingBox;
	UINT m_id;

	const D3DXMATRIX* _GetRotationMatrix();
};





#endif