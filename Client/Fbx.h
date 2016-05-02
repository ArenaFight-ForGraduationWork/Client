#ifndef FBX_H_
#define FBX_H_

#include "Vertex.h"	//������ "Mesh.h"�ε�, CVertex�� �ʿ��ؼ� ����ɷ� �����.


class CFbx
{
private:
	static CFbx* m_instance;

	/*�ִϸ��̼��� ���� ������*/
	XMFLOAT4X4** m_ppResult[5];
	long long m_AniTime[ANIMATION_COUNT];

	long long m_llAnimationMaxTime;			//�ִϸ��̼� �ִ����
	unsigned int m_uiAnimationNodeIndexCount;	//�ִϸ��̼� ��� ����
	float m_fAnimationPlayTime;				//�ִϸ��̼� ��� �ð�

	int *tsize;
	int size = 0;	// text �о�ö� ���� ����.
	int tempsize = 0;
	int tempcnt = 0;	// weight���� for�� ���� �� ����Ϸ��� ����.

	D3DXVECTOR3 m_MaxVer;	//�о�� �ִ밪
	D3DXVECTOR3 m_MinVer;	//�о�� �ּҰ�

	D3DXVECTOR3 m_HitMaxVer[2];	// �ȿ� ���ڴ� �ٲ� �� ����. �ϴ� ���ݸ���� 2����� �����Ͽ�, ���� �ٸ� ��Ʈ�ڽ��� ���
	D3DXVECTOR3 m_HitMinVer[2];

	FILE *fp;
	FILE *fMonA[3];		//�ȿ� ���ڴ� �ٲ� �� ����.

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

	void Fbx_ReadTextFile_Mesh(char* fileName, vector<CTexturedNormalVertex*> &v, D3DXVECTOR3 );		// object ����
	void Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex* &v);	// Animation����
	void Fbx_ReadTextFile_Info(int CharNum);		//������ �� ������� ����.
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