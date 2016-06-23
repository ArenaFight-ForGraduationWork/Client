#ifndef FBX_H_
#define FBX_H_

#include "Vertex.h"


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

	D3DXVECTOR3& GetMaxVer() { return *m_pMaxVer; }
	D3DXVECTOR3& GetMinVer() { return *m_pMinVer; }

private:
	int m_iSize;

	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];

	int m_iAnimationMaxTime;					// �ִϸ��̼� �ִ� ����
	unsigned int m_uiAnimationNodeIndexCount;	//�ִϸ��̼� ��� ����
	float m_fAnimationPlayTime;					//�ִϸ��̼� ��� �ð�

	D3DXVECTOR3 *m_pMaxVer;	//�о�� �ִ밪
	D3DXVECTOR3 *m_pMinVer;	//�о�� �ּҰ�

	unsigned int* m_pAniIndexCount;
};



#endif