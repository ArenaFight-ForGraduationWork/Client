#pragma once

#include "Vertex.h"

#include <DirectXMath.h>
using namespace DirectX;


/* ���ڿ� ��ġ�� strcat(a, b) : a�� b�� ��ħ. */

class CFbx
{
public:
	CFbx();
	~CFbx();

	void ReadTextFile_Mesh(char* fileName, CTexturedNormalVertex* &v);		// object ����
	void ReadTextFile_Mesh(int CharNum, CAnimationVertex* &v);									// Animation����
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

	int m_iAnimationMaxTime;					// �ִϸ��̼� �ִ� ����
	unsigned int m_uiAnimationNodeIndexCount;	//�ִϸ��̼� ��� ����
	float m_fAnimationPlayTime;					//�ִϸ��̼� ��� �ð�

	XMFLOAT3 m_pMaxVer;	// �ִ밪
	XMFLOAT3 m_pMinVer;	// �ּҰ�

	unsigned int* m_pAniIndexCount;
};



