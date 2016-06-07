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

	///* �ٿ���ڽ� */
	//void BoundingMoveForward(float fDistance = 1.0f);
	//void BoundingMoveAbsolute(const D3DXVECTOR3 *d3dxVec);
	//void BoundingRotateAbsolute(const D3DXVECTOR3 *d3dxVec);
	//void BoundingRotateRelative(const D3DXVECTOR3 *d3dxVec);
	//void SetBoundingBoxMatrix();
	//D3DXMATRIX* GetBoundingWorldMatrix() { return m_boundingWorldMatrix; }
	//const D3DXVECTOR3* GetBoundingPosition();
	//const D3DXVECTOR3* GetBoundingLookAt();

	void RotateRelative(const float fPitch, const float fYaw, const float fRoll);
	void RotateRelative(const D3DXVECTOR3 *d3dxVec);
	void RotateRelative(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle);
	void RotateAbsolute(const float fPitch, const float fYaw, const float fRoll);
	void RotateAbsolute(const D3DXVECTOR3 *d3dxVec);
	void RotateAbsolute(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle);

	void SetPosition(D3DXVECTOR3* pos);
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
	void SetTime(long long*);

	void PlayAnimation(eAnimationType eType);
	//==============================================================================================
	//==============================================================================================

	//==============================================================================================
	//==============================================================================================
	/* Collision Detection */
	///* �浹üũ�� ���� ���� �� �Լ� */
	//int PressSkillNum;						// ���� ���� ��ų�� ���° ���ݽ�ų�ΰ�. 1. attack, 2. skill1, 3. skill2, 4.skill3
	//D3DXVECTOR3 m_MaxVer;			//�浹üũ�� ���� �ִ밪
	//D3DXVECTOR3 m_MinVer;			//�浹üũ�� ���� �ּҰ�

	//D3DXVECTOR3 m_HitMaxVer[ATTACK_COUNT];		//��Ʈ�ڽ��� ���� �ִ밪
	//D3DXVECTOR3 m_HitMinVer[ATTACK_COUNT];		//��Ʈ�ڽ��� ���� �ּҰ�

	//bool MyHitAndEnemyBound(CObject* pObject);		// �� ��Ʈ�ڽ� + ���� �浹�ڽ� = ���� ü�� ����
	//bool EnemyHitAndMyBound(CObject* pObject);		// ���� ��Ʈ�ڽ� + �� �浹�ڽ� = �� ü�� ����
	//bool Collison(CObject *pObject);							//�浹üũ
	//bool Collison(vector<CObject*> &vObject);			//ī�װ����� �浹üũ
	//int CollOtherID;												//���� �ε��� ���� ���̵�
	//bool isColl = false;											//�浹�ߴ°� ���ߴ°� Ȯ�ο뵵

	//virtual void SetBoundingBox();							// ��� ������Ʈ�� �� ����
	//virtual void SetHitBox();									// ���ݸ���� �ִ� �͸� ��Ʈ�ڽ��� ����

	//virtual void MoveAndRotatingHitBox();					//�ִϸ��̼� �ִ°͸� ���
	//virtual void MoveAndRotatingBoundingBox();			//�ִϸ��̼� �ִ°͸� ���

	//D3DXVECTOR3 GetMaxVer() { return m_MaxVer; }
	//D3DXVECTOR3 GetMinVer() { return m_MinVer; }

	//void SetPressSkill(int num) { PressSkillNum = num; }		//��� ���ݽ�ų�� �����°�
	//==============================================================================================
	//==============================================================================================

private:
	UINT m_id;

	int  eSourceType;
	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;

	D3DXMATRIX *m_pd3dxWorldMatrix;
	D3DXMATRIX *m_boundingWorldMatrix;

	ID3D11Buffer *m_pd3dcbBoneMatrix;
	VS_CB_BONE_MATRIX *m_pcbBoneMatrix;
	XMFLOAT4X4 ***m_pppResult;

	float m_fAnimationPlaytime;
	float m_fAniMaxTime[ANIMATION_COUNT];
	int m_AnimationIndexCount;
	eAnimationType m_eAnimationType;

	const D3DXMATRIX* _GetRotationMatrix();
	//const D3DXMATRIX* _GetBoundingRotationMatrix();
};




class CUnit : public CObject{
public:
	CUnit(UINT id);
	~CUnit();

	void SetStrikingPower(float fSp) { m_fStrikingPower = fSp; }
	void SetDefensivePower(float fDp) { m_fDefensivePower = fDp; }
	void SetMovingSpeed(float fMs) { m_fMovingSpeed = fMs; }
	void SetHealthPoint(float fHp) { m_fHp = fHp; }

	float GetStrikingPower() { return m_fStrikingPower; }
	float GetDefensivePower() { return m_fDefensivePower; }
	float GetMovingSpeed() { return m_fMovingSpeed; }
	float GetHealthPoint() { return m_fHp; }

protected:
private:
	float m_fStrikingPower;		/* ���ݷ� */
	float m_fDefensivePower;	/* ���� */
	float m_fMovingSpeed;		/* �̵��ӵ�. m/s */
	float m_fHp;				/* ü�� */
};





#endif