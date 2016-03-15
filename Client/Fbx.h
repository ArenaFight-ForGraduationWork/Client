#pragma once

#include "Vertex.h"	//������ "Mesh.h"�ε�, CVertex�� �ʿ��ؼ� ����ɷ� �����.


class CFbx
{
private:
	static CFbx* m_instance;

	/*�ִϸ��̼��� ���� ������*/
	long long m_llAnimationMaxTime;			//�ִϸ��̼� �ִ����
	unsigned int m_uiAnimationNodeIndexCount;	//�ִϸ��̼� ��� ����
	float m_fAnimationPlayTime;				//�ִϸ��̼� ��� �ð�


	FILE *fp;

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

};

