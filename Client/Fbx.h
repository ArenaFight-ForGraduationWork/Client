#ifndef FBX_H_
#define FBX_H_

#include "Vertex.h"


class CFbx
{
private:
	/*애니메이션을 위한 변수들*/
	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];
	long long m_AniTime[ANIMATION_COUNT];

	long long m_llAnimationMaxTime;				//애니메이션 최대길이
	unsigned int m_uiAnimationNodeIndexCount;	//애니메이션 노드 갯수
	float m_fAnimationPlayTime;					//애니메이션 재생 시간

	int size = 0;	// text 읽어올때 쓰는 전용.
	int tempcnt = 0;	// weight에서 for문 돌릴 때 사용하려고 만듦.

	D3DXVECTOR3 m_MaxVer;	//읽어온 최대값
	D3DXVECTOR3 m_MinVer;	//읽어온 최소값

	unsigned int* m_pAniIndexCount;

public:
	CFbx();
	~CFbx();
	static CFbx* GetSingleton()
	{
		static CFbx instance;
		return &instance;
	}

	void Fbx_ReadTextFile_Mesh(char* fileName, CTexturedNormalVertex *v, D3DXVECTOR3 scale);		// object 전용
	void Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex *v);	// Animation전용
	void Fbx_ReadTextFile_Info(int CharNum);		//사이즈 좀 얻을라고 만듬.
	void Fbx_ReadTextFile_Weight(int CharNum, CAnimationVertex*);
	void Fbx_ReadTextFile_Ani(int CharNum, int StateCnt);
	void ReadTextFile_HitBox(int CharNum, D3DXVECTOR3* &max, D3DXVECTOR3* &min);

	int GetSize() { return size; }
	XMFLOAT4X4** GetResult(int i) { return m_ppResult[i]; }

	long long GetAnimationMaxTime() { return m_llAnimationMaxTime; }
	int GetAnimationIndexCount() { return m_uiAnimationNodeIndexCount; }

	D3DXVECTOR3& GetMaxVer() { return m_MaxVer; }
	D3DXVECTOR3& GetMinVer() { return m_MinVer; }
};



#endif