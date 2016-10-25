#pragma once

#include "Vertex.h"

#include <DirectXMath.h>
using namespace DirectX;


/* 문자열 합치기 strcat(a, b) : a와 b를 합침. */

class CFbx
{
public:
	CFbx();
	~CFbx();

	void ReadTextFile_Mesh(char* fileName, CTexturedNormalVertex* &v);		// object 전용
	void ReadTextFile_Mesh(int CharNum, CAnimationVertex* &v);									// Animation전용
	void ReadTextFile_Ani(int CharNum, int StateCnt);
	void ReadTextFile_Weight(int CharNum, CAnimationVertex*);

	int& GetSize() { return m_iSize; }
	XMFLOAT4X4** GetResult(int i) { return m_ppResult[i]; }

	int& GetAnimationMaxTime() { return m_iAnimationMaxTime; }
	int GetAnimationIndexCount() { return m_uiAnimationNodeIndexCount; }

	CXMVECTOR GetMaxVer() { return XMLoadFloat3(&m_pMaxVer); }
	CXMVECTOR GetMinVer() { return XMLoadFloat3(&m_pMinVer); }

private:
	int m_iSize;

	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];

	int m_iAnimationMaxTime;					// 애니메이션 최대 길이
	unsigned int m_uiAnimationNodeIndexCount;	//애니메이션 노드 갯수
	float m_fAnimationPlayTime;					//애니메이션 재생 시간

	XMFLOAT3 m_pMaxVer;	// 최대값
	XMFLOAT3 m_pMinVer;	// 최소값

	unsigned int* m_pAniIndexCount;
};



