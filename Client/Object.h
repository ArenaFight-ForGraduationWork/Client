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

	void MoveRelative(const float fx, const float fy, const float fz);
	void MoveRelative(const D3DXVECTOR3 *d3dxVec);
	void MoveAbsolute(const float fx, const float fy, const float fz);
	void MoveAbsolute(const D3DXVECTOR3 *d3dxVec);
	/* 로컬 Z축 방향으로 이동한다 */
	virtual void MoveForward(float fDistance = 1.0f);

	void RotateRelative(const float fPitch, const float fYaw, const float fRoll);
	void RotateRelative(const D3DXVECTOR3 *d3dxVec);
	void RotateRelative(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle);
	void RotateAbsolute(const float fPitch, const float fYaw, const float fRoll);
	void RotateAbsolute(const D3DXVECTOR3 *d3dxVec);
	void RotateAbsolute(const D3DXVECTOR3 *pd3dxvAxis, const float fAngle);

	void SetPosition(D3DXVECTOR3* pos);
	//객체의 위치, 로컬 x-축, y-축, z-축 방향 벡터를 반환한다.
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

	virtual void Animate(int State, ID3D11DeviceContext*pd3dDeviceContext, float fTimeElapsed);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	//애니메이션 + 렌더
	virtual void AnimateObjectAndRender(ID3D11DeviceContext*, float time);

	//=================================================================
	/* 애니메이션을 위한 변수 및 함수 */
	ID3D11Buffer *g_pd3dcbBoneMatrix = nullptr;
	D3D11_MAPPED_SUBRESOURCE g_d3dMappedResource;
	VS_CB_BONE_MATRIX *g_pcbBoneMatrix = nullptr;
	XMFLOAT4X4 ***m_pppResult;
	float m_fAnimationPlaytime = 0.0f;
	long long  NowTime;
	int PreState;	//이전 상태
	long long m_AniMaxTime[5];
	int m_AnimationIndexCount;
	int Animation_state = 0;		// IDLE, RUN, ATTACK...
	
	void SetConstantBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext *pd3dDeviceContext);
	void PlayAnimation(int StateNum, ID3D11DeviceContext* pd3dDeviceContext);
	
	void SetAniIndexCount(int);
	void SetResult(XMFLOAT4X4***);
	void SetTime(long long*);

	void SetPlayAnimationState(int animation_state);	
	//=================================================================

	//=================================================================
	/* 충돌체크를 위한 변수 및 함수 */
	int PressSkillNum;		// 변수이름 바꿀거임, 지금 누른 스킬이 몇번째 스킬인가. SkillNumber; 이런걸로..
	D3DXVECTOR3 m_MaxVer;			//충돌체크를 위한 최대값
	D3DXVECTOR3 m_MinVer;			//충돌체크를 위한 최소값

	D3DXVECTOR3 m_HitMaxVer[ATTACK_COUNT];		//히트박스를 위한 최대값
	D3DXVECTOR3 m_HitMinVer[ATTACK_COUNT];		//히트박스를 위한 최소값

	bool Collison(CObject *pObject);		//충돌체크 확인하는 함수
	bool MyHitAndEnemyBound(CObject* pObject);		// 내 히트박스 + 몬스터 충돌박스 = 몬스터 체력 감소
	bool EnemyHitAndMyBound(CObject* pObject);		// 몬스터 히트박스 + 내 충돌박스 = 내 체력 감소
	
	bool Collison(vector<CObject*> &vObject);	//카테고리별로 충돌체크
	int CollOtherID;	//나랑 부딪힌 것의 아이디

	virtual void SetBoundingBox();	// 모든 오브젝트에 다 있음
	virtual void SetHitBox();			// 공격모션이 있는 것만 히트박스가 있음

	virtual void MoveAndRotatingHitBox();				//애니메이션 있는것만 사용
	virtual void MoveAndRotatingBoundingBox();		//애니메이션 있는것만 사용

	D3DXVECTOR3 GetMaxVer(){ return m_MaxVer; }
	D3DXVECTOR3 GetMinVer() { return m_MinVer; }

	void SetPressSkill(int num) { PressSkillNum = num; }		//어느 스킬을 눌렀는가~~
	//=================================================================
	void SetHP() { HP = HP - 1; }
	int GetHP()	{ return HP; }
	void SetIsAnimation()	{ isAnimating = true; }		//애니메이션이 있으니 재생부분을 통과할수 있게끔 해주는 변수
private:
	D3DXMATRIX *m_pd3dxWorldMatrix;

	CMesh *m_pMesh;
	CMaterial *m_pMaterial;
	CTexture *m_pTexture;
	UINT m_id;

	int HP = 100;

	bool isAnimating = false;
	const D3DXMATRIX* _GetRotationMatrix();
};





#endif