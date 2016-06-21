#ifndef OBJECT_H_
#define OBJECT_H_



#include "Mesh.h"

struct VS_CB_BONE_MATRIX
{
	XMMATRIX m_XMmtxBone[128];
};

/* 재질 관련 정보를 표현 */
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
	
	/* nIndex번째 인덱스 자리에 텍스쳐를 설정한다 */
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







class CObject
{
public:
	CObject(UINT id);
	virtual ~CObject();

	void SetPositionRelative(float& fx, float& fy, float& fz);
	void SetPositionRelative(D3DXVECTOR3 *d3dxVec);
	void SetPositionAbsolute(float& fx, float& fy, float& fz);
	void SetPositionAbsolute(D3DXVECTOR3 *d3dxVec);
	/* 로컬 Z축 방향으로 이동한다 */
	virtual void MoveForward(float& fDistance);

	void SetDirectionRelative(float& fPitch, float& fYaw, float& fRoll);
	void SetDirectionRelative(D3DXVECTOR3 *d3dxVec);
	void SetDirectionAbsolute(float& fPitch, float& fYaw, float& fRoll);
	void SetDirectionAbsolute(D3DXVECTOR3 *d3dxVec);

	//객체의 위치, 로컬 x-축, y-축, z-축 방향 벡터를 반환
	const D3DXVECTOR3* GetPosition();
	const D3DXVECTOR3* GetDirection();
	const D3DXVECTOR3* GetRight();
	const D3DXVECTOR3* GetUp();
	const D3DXVECTOR3* GetLookAt();

	const D3DXMATRIX* GetWorldMatrix() { return m_pd3dxWorldMatrix; }

	const UINT& GetId() { return m_id; }

	virtual void SetMesh(CMesh *pMesh);
	CMesh* GetMesh() { return m_pMesh; }
	void SetMaterial(CMaterial *pMaterial);
	CMaterial* GetMaterial() { return m_pMaterial; }
	void SetTexture(CTexture *pTexture);
	CTexture* GetTexture() { return m_pTexture; }

	virtual void SetResourceType(int eType);
	int GetResourceType() { return eSourceType; }

	virtual void AnimateAndRender(ID3D11DeviceContext*, float time);

	//==============================================================================================
	//==============================================================================================
	/* Animation */
	enum class eAnimationType : BYTE {
		START = 0,
		Idle = 0,
		Move,
		Dead,
		Attack,
		Skill1,
		Skill2,
		Skill3,
		None,
		END
	};

	void SetConstantBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext);
	void SetAniIndexCount(int);
	void SetResult(XMFLOAT4X4***);
	void SetTime(int*);

	void PlayAnimation(eAnimationType eType);

	//==============================================================================================
	//==============================================================================================
	/* Collision Detection */
private:
	D3DXVECTOR3 *m_pMaxVer;
	D3DXVECTOR3 *m_pMinVer;
	float m_fRadius;

public:
	void SetBoundingBox();

	const D3DXVECTOR3* GetMaxVer();
	const D3DXVECTOR3* GetMinVer();
	const float& GetRadius() { return m_fRadius; }

private:
	UINT m_id;

	D3DXVECTOR3 *m_pd3dxvDirection;

	int  eSourceType;
	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;

	D3DXMATRIX *m_pd3dxWorldMatrix;

	ID3D11Buffer *m_pd3dcbBoneMatrix;
	VS_CB_BONE_MATRIX *m_pcbBoneMatrix;
	XMFLOAT4X4 ***m_pppResult;

	float m_fAnimationPlaytime;
	int m_iAniMaxTime[ANIMATION_COUNT];
	int m_AnimationIndexCount;
	eAnimationType m_eAnimationType;
};




class CUnit : public CObject{
public:
	CUnit(UINT id);
	~CUnit();

	void SetStrikingPower(float fSp) { m_fStrikingPower = fSp; }
	void SetDefensivePower(float fDp) { m_fDefensivePower = fDp; }
	void SetMovingSpeed(float fMs) { m_fMovingSpeed = fMs; }
	void SetHealthPoint(float fHp) { m_fHp = fHp; }

	float& GetStrikingPower() { return m_fStrikingPower; }
	float& GetDefensivePower() { return m_fDefensivePower; }
	float& GetMovingSpeed() { return m_fMovingSpeed; }
	float& GetHealthPoint() { return m_fHp; }

protected:
private:
	float m_fStrikingPower;		/* 공격력 */
	float m_fDefensivePower;	/* 방어력 */
	float m_fMovingSpeed;		/* 이동속도. m/s */
	float m_fHp;				/* 체력 */
};





#endif