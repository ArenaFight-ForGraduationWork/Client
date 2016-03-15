#pragma once

#include "Vertex.h"	//원래는 "Mesh.h"인데, CVertex가 필요해서 썼던걸로 기억함.


class CFbx
{
private:
	static CFbx* m_instance;

	/*애니메이션을 위한 변수들*/
	long long m_llAnimationMaxTime;			//애니메이션 최대길이
	unsigned int m_uiAnimationNodeIndexCount;	//애니메이션 노드 갯수
	float m_fAnimationPlayTime;				//애니메이션 재생 시간


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

	void Fbx_ReadTextFile_Mesh(char* fileName, vector<CTexturedNormalVertex*> &v);		// object 전용

};

