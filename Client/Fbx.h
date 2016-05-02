#ifndef FBX_H_
#define FBX_H_

#include "Vertex.h"	//원래는 "Mesh.h"인데, CVertex가 필요해서 썼던걸로 기억함.


class CFbx
{
private:
	static CFbx* m_instance;

	/*애니메이션을 위한 변수들*/
	XMFLOAT4X4** m_ppResult[5];
	long long m_AniTime[ANIMATION_COUNT];

	long long m_llAnimationMaxTime;			//애니메이션 최대길이
	unsigned int m_uiAnimationNodeIndexCount;	//애니메이션 노드 갯수
	float m_fAnimationPlayTime;				//애니메이션 재생 시간

	int *tsize;
	int size = 0;	// text 읽어올때 쓰는 전용.
	int tempsize = 0;
	int tempcnt = 0;	// weight에서 for문 돌릴 때 사용하려고 만듦.

	D3DXVECTOR3 m_MaxVer;	//읽어온 최대값
	D3DXVECTOR3 m_MinVer;	//읽어온 최소값

	D3DXVECTOR3 m_HitMaxVer[2];	// 안에 숫자는 바뀔 수 있음. 일단 공격모션이 2개라는 가정하에, 각각 다른 히트박스일 경우
	D3DXVECTOR3 m_HitMinVer[2];

	FILE *fp;
	FILE *fMonA[3];		//안에 숫자는 바뀔 수 있음.

	long long* m_pAniTime;
	unsigned int* m_pAniIndexCount;

public:
	CFbx();
	~CFbx();

	static CFbx* GetInstance()
	{
		if (m_instance == NULL)
		{
			m_instance = new CFbx();
		}
		return m_instance;
	}

	void Fbx_ReadTextFile_Mesh(char* fileName, vector<CTexturedNormalVertex*> &v, D3DXVECTOR3 );		// object 전용
	void Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex* &v);	// Animation전용
	void Fbx_ReadTextFile_Info(int CharNum);		//사이즈 좀 얻을라고 만듬.
	void Fbx_ReadTextFile_Weight(int CharNum, CAnimationVertex*);	
	void Fbx_ReadTextFile_Ani(int CharNum, int StateCnt);
	void ReadTextFile_HitBox(int CharNum);
	void ReadTextFile_HitBox(int CharNum, D3DXVECTOR3* &max, D3DXVECTOR3* &min);

	//Get()
	int GetSize(){ return size; }
	int GetTempSize() { return tempsize; }
	XMFLOAT4X4** GetResult(int i){ return m_ppResult[i]; }
	XMFLOAT4X4*** GetResult() { return m_ppResult; }
	long long GetAnimationMaxTime(){ return m_llAnimationMaxTime; }
	int GetAnimationIndexCount(){ return m_uiAnimationNodeIndexCount; }
	long long GetAniTime(int i)	{ return m_AniTime[i]; }
	long long* GetTime(int i ){ return &m_pAniTime[i]; }
	unsigned int* GetAniIndexCnt()	{ return m_pAniIndexCount; }
	D3DXVECTOR3& GetMaxVer()	{ return m_MaxVer; }
	D3DXVECTOR3& GetMinVer()		{ return m_MinVer; }

	D3DXVECTOR3 GetHitMaxVer(int i )	{ return m_HitMaxVer[i]; }
	
};



#endif