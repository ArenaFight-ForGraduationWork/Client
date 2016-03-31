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

	int size = 0;	// text �о�ö� ���� ����.
	int tempcnt = 0;	// weight���� for�� ���� �� ����Ϸ��� ����.


	FILE *fp;
	FILE *fMonA[3];		//�ȿ� ���ڴ� �ٲ� �� ����.
	FILE *fMonB[3];
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

	void Fbx_ReadTextFile_Mesh(char* fileName, vector<CTexturedNormalVertex*> &v);		// object ����
	void Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex* &v);	// Animation����
	void Fbx_ReadTextFile_Info(int CharNum);		//������ �� ������� ����.
	void Fbx_ReadTextFile_Weight(int CharNum, CAnimationVertex*);	
	void Fbx_ReadTextFile_Ani(int CharNum, int StateCnt);

	int GetSize(){ return size; }
	XMFLOAT4X4** GetResult(int i){ return m_ppResult[i]; }
	XMFLOAT4X4*** GetResult() { return m_ppResult; }
	long long GetAnimationMaxTime(){ return m_llAnimationMaxTime; }
	int GetAnimationIndexCount(){ return m_uiAnimationNodeIndexCount; }
	long long GetAniTime(int i)	{ return m_AniTime[i]; }

	long long* GetTime(int i ){ return &m_pAniTime[i]; }
	unsigned int* GetAniIndexCnt()	{ return m_pAniIndexCount; }
};



#endif