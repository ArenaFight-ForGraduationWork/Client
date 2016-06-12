#ifndef FBX_H_
#define FBX_H_

#include "Vertex.h"


class CFbx
{
private:
	/*�ִϸ��̼��� ���� ������*/
	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];
	long long m_AniTime[ANIMATION_COUNT];

	long long m_llAnimationMaxTime;				//�ִϸ��̼� �ִ����
	unsigned int m_uiAnimationNodeIndexCount;	//�ִϸ��̼� ��� ����
	float m_fAnimationPlayTime;					//�ִϸ��̼� ��� �ð�

	int size = 0;	// text �о�ö� ���� ����.
	int tempcnt = 0;	// weight���� for�� ���� �� ����Ϸ��� ����.

	D3DXVECTOR3 m_MaxVer;	//�о�� �ִ밪
	D3DXVECTOR3 m_MinVer;	//�о�� �ּҰ�

	unsigned int* m_pAniIndexCount;

public:
	CFbx();
	~CFbx();
	static CFbx* GetSingleton()
	{
		static CFbx instance;
		return &instance;
	}

	void Fbx_ReadTextFile_Mesh(char* fileName, CTexturedNormalVertex *v, D3DXVECTOR3 scale);		// object ����
	void Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex *v);	// Animation����
	void Fbx_ReadTextFile_Info(int CharNum);		//������ �� ������� ����.
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