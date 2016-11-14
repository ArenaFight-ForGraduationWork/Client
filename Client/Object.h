#pragma once

#include "Mesh.h"
#include "ConstantBuffers.h"
#include <DirectXMath.h>
using namespace DirectX;





//
//	Material
//
class CMaterial
{
public:
	CMaterial();
	virtual ~CMaterial();

	void AddRef();
	void Release();

	PS_CB_MATERIAL* GetMaterial() { return m_pMaterial; }

private:
	int m_nReferences;
	PS_CB_MATERIAL *m_pMaterial;
};





//
//	Texture
//
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
	int m_nTextures;

	ID3D11ShaderResourceView **m_ppd3dsrvTextures;
	ID3D11SamplerState **m_ppd3dSamplerStates;
};





//
//	Object
//
class CUnitComponent;
class CObject
{
public:
	CObject();
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
	XMFLOAT3* GetPosition();
	XMFLOAT3* GetDirection();
	XMFLOAT3* GetRight();
	XMFLOAT3* GetUp();
	XMFLOAT3* GetLookAt();

	XMFLOAT4X4* GetWorldMatrix() { return m_pf4x4WorldMatrix; }

	virtual void SetMesh(CMesh *pMesh);
	CMesh* GetMesh() { return m_pMesh; }
	void SetMaterial(CMaterial *pMaterial);
	CMaterial* GetMaterial() { return m_pMaterial; }
	void SetTexture(CTexture *pTexture);
	CTexture* GetTexture() { return m_pTexture; }

	void SetResourceType(int eType);
	int& GetResourceType() { return m_iSourceType; }

	void Animate();
	void Render();

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
	XMFLOAT3 *m_pf3MaxVer;
	XMFLOAT3 *m_pf3MinVer;
	float m_fRadius;

public:
	void SetBoundingBox();

	XMFLOAT3* GetMaxVer();
	XMFLOAT3* GetMinVer();
	const float& GetRadius() { return m_fRadius; }
	//==============================================================================================
	//==============================================================================================

private:
	XMFLOAT3 *m_pf3Direction;

	int m_iSourceType;
	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;

	XMFLOAT4X4 *m_pf4x4WorldMatrix;

	ID3D11Buffer *m_pd3dcbBoneMatrix;
	VS_CB_BONE_MATRIX *m_pcbBoneMatrix;

	XMFLOAT4X4 ***m_pppResult;
	float m_fAnimationPlaytime;
	int m_iAniMaxTime[ANIMATION_COUNT];
	int m_iAnimationIndexCount;
	eAnimationType m_eAnimationType;

	CUnitComponent *m_pUnitComponent;
};





//
//	Unit Component
//
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



