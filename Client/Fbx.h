#ifndef FBX_H_
#define FBX_H_

#include "Vertex.h"


/* 문자열 합치기 strcat(a, b) : a와 b를 합침. */

class CFbx
{
public:
	CFbx();
	~CFbx();

	void Fbx_ReadTextFile_Info(int CharNum);		//사이즈 좀 얻을라고 만듦.
	void Fbx_ReadTextFile_Mesh(char* fileName, CTexturedNormalVertex *v, D3DXVECTOR3 scale);		// object 전용
	void Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex *v);									// Animation전용
	void Fbx_ReadTextFile_Ani(int CharNum, int StateCnt);
	void Fbx_ReadTextFile_Weight(int CharNum, CAnimationVertex*);

	int& GetSize() { return m_iSize; }
	XMFLOAT4X4** GetResult(int i) { return m_ppResult[i]; }

	long long GetAnimationMaxTime() { return m_llAnimationMaxTime; }
	int GetAnimationIndexCount() { return m_uiAnimationNodeIndexCount; }

	D3DXVECTOR3& GetMaxVer() { return *m_pMaxVer; }
	D3DXVECTOR3& GetMinVer() { return *m_pMinVer; }

private:
	int m_iSize;
	int tempcnt;	// weight에서 for문 돌릴 때 사용하려고 만듦

	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];
	long long m_AniTime[ANIMATION_COUNT];

	long long m_llAnimationMaxTime;				//애니메이션 최대 길이
	unsigned int m_uiAnimationNodeIndexCount;	//애니메이션 노드 갯수
	float m_fAnimationPlayTime;					//애니메이션 재생 시간

	D3DXVECTOR3 *m_pMaxVer;	//읽어온 최대값
	D3DXVECTOR3 *m_pMinVer;	//읽어온 최소값

	unsigned int* m_pAniIndexCount;
};



#endif