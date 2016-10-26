#pragma once



#include "Mesh.h"
#include "ConstantBuffers.h"

#include <DirectXMath.h>
using namespace DirectX;


/* 재질 관련 정보를 표현 */
struct MATERIAL
{
	// R, G, B, A
	float m_fAmbient[4];
	float m_fDiffuse[4];
	float m_fSpecular[4]; // a = power
	float m_fEmissive[4];
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
	void SetTexture(int nIndex, WCHAR *textureAddress);

	int& GetNumOfTextures() { return m_nTextures; }

	ID3D11ShaderResourceView** GetShaderResourceViewTextures() { return m_ppd3dsrvTextures; }
	ID3D11SamplerState** GetSamplerState() { return m_ppd3dSamplerStates; }

private:
	int m_nReferences;

	ID3D11ShaderResourceView **m_ppd3dsrvTextures;
	ID3D11SamplerState **m_ppd3dSamplerStates;

	int m_nTextures;
};






class CUnitComponent;
class CObject
{
public:
	CObject(UINT id);
	virtual ~CObject();

	void SetPositionRelative(float& fx, float& fy, float& fz);
	void SetPositionRelative(CXMVECTOR vec);
	void SetPositionAbsolute(float& fx, float& fy, float& fz);
	void SetPositionAbsolute(CXMVECTOR vec);
	/* 로컬 Z축 방향으로 이동한다. 속력이 있으면 속력*시간(fVar), 없으면 거리(fVar)만큼 움직인다  */
	virtual void MoveForward(float& fVar);

	void SetDirectionRelative(float& fPitch, float& fYaw, float& fRoll);
	void SetDirectionRelative(CXMVECTOR vec);
	void SetDirectionAbsolute(float& fPitch, float& fYaw, float& fRoll);
	void SetDirectionAbsolute(CXMVECTOR vec);

	//객체의 위치, 로컬 x-축, y-축, z-축 방향 벡터를 반환
	CXMVECTOR GetPosition();
	CXMVECTOR GetDirection();
	CXMVECTOR GetRight();
	CXMVECTOR GetUp();
	CXMVECTOR GetLookAt();

	CXMMATRIX GetWorldMatrix() { return XMLoadFloat4x4(&m_f4x4WorldMatrix); }

	const UINT& GetId() { return m_id; }

	virtual void SetMesh(CMesh *pMesh);
	CMesh* GetMesh() { return m_pMesh; }
	void SetMaterial(CMaterial *pMaterial);
	CMaterial* GetMaterial() { return m_pMaterial; }
	void SetTexture(CTexture *pTexture);
	CTexture* GetTexture() { return m_pTexture; }

	void SetResourceType(int eType);
	int& GetResourceType() { return m_iSourceType; }

	void AnimateAndRender(float& time);

	void SetComponent();
	CUnitComponent* GetComponent() { return m_pUnitComponent; }

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

	void SetConstantBuffer();
	void SetAniIndexCount(int);
	void SetResult(XMFLOAT4X4***);
	void SetTime(int*);

	void PlayAnimation(eAnimationType eType);
	eAnimationType GetNowAnimation() { return m_eAnimationType; }

	//==============================================================================================
	//==============================================================================================
	/* Collision Detection */
private:
	XMFLOAT3 m_pMaxVer;
	XMFLOAT3 m_pMinVer;
	float m_fRadius;

public:
	void SetBoundingBox();

	CXMVECTOR GetMaxVer();
	CXMVECTOR GetMinVer();
	const float& GetRadius() { return m_fRadius; }
	//==============================================================================================
	//==============================================================================================

private:
	UINT m_id;

	XMFLOAT3 m_f3Direction;

	int m_iSourceType;
	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;

	XMFLOAT4X4 m_f4x4WorldMatrix;

	ID3D11Buffer *m_pd3dcbBoneMatrix;
	VS_CB_BONE_MATRIX *m_pcbBoneMatrix;

	XMFLOAT4X4 ***m_pppResult;
	float m_fAnimationPlaytime;
	int m_iAniMaxTime[ANIMATION_COUNT];
	int m_iAnimationIndexCount;
	eAnimationType m_eAnimationType;

	CUnitComponent *m_pUnitComponent;
};



class CUnitComponent
{
public:
	CUnitComponent() : m_fStrikingPower(10), m_fDefensivePower(10), m_fMovingSpeed(300), m_fHp(100) {}
	~CUnitComponent() {}

	void SetStrikingPower(float& fSp) { m_fStrikingPower = fSp; }
	void SetDefensivePower(float& fDp) { m_fDefensivePower = fDp; }
	void SetMovingSpeed(float& fMs) { m_fMovingSpeed = fMs; }
	void SetHealthPoint(float& fHp) { m_fHp = fHp; }

	float GetStrikingPower() { return m_fStrikingPower; }
	float GetDefensivePower() { return m_fDefensivePower; }
	float GetMovingSpeed() { return m_fMovingSpeed; }
	float GetHealthPoint() { return m_fHp; }

protected:
private:
	float m_fStrikingPower;		/* 공격력 */
	float m_fDefensivePower;	/* 방어력 */
	float m_fMovingSpeed;		/* 이동속도. m/s */
	float m_fHp;				/* 체력 */
};



