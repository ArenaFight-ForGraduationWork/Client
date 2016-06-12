#ifndef FBX_H_
#define FBX_H_

#include "Vertex.h"


/* ���ڿ� ��ġ�� strcat(a, b) : a�� b�� ��ħ. */

class CFbx
{
public:
	CFbx();
	~CFbx();

	void Fbx_ReadTextFile_Info(int CharNum);		//������ �� ������� ����.
	void Fbx_ReadTextFile_Mesh(char* fileName, CTexturedNormalVertex *v, D3DXVECTOR3 scale);		// object ����
	void Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex *v);									// Animation����
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
	int tempcnt;	// weight���� for�� ���� �� ����Ϸ��� ����

	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];
	long long m_AniTime[ANIMATION_COUNT];

	long long m_llAnimationMaxTime;				//�ִϸ��̼� �ִ� ����
	unsigned int m_uiAnimationNodeIndexCount;	//�ִϸ��̼� ��� ����
	float m_fAnimationPlayTime;					//�ִϸ��̼� ��� �ð�

	D3DXVECTOR3 *m_pMaxVer;	//�о�� �ִ밪
	D3DXVECTOR3 *m_pMinVer;	//�о�� �ּҰ�

	unsigned int* m_pAniIndexCount;
};



#endif