#include "stdafx.h"
#include "Fbx.h"


CFbx::CFbx()
{
	m_fAnimationPlayTime = 0.0f;
	m_llAnimationMaxTime = 0;
}


CFbx::~CFbx()
{
}

CFbx* CFbx::m_instance;

void CFbx::Fbx_ReadTextFile_Mesh(char* fileName, vector<CTexturedNormalVertex*> &v)
{
	fopen_s(&fp, fileName, "rt");

	int Cnt = 0;
	fscanf_s(fp, "%d\n", &Cnt);
	
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 outNormal;
	D3DXVECTOR3 outUV;

	for (int i = 0; i < Cnt; ++i)
	{
		//정점데이터 얻어오기
		fscanf_s(fp, "%f %f %f\n", &m_pos.x, &m_pos.y, &m_pos.z);
		fscanf_s(fp, "%f %f %f\n", &outNormal.x, &outNormal.y, &outNormal.z);
		fscanf_s(fp, "%f %f\n", &outUV.x, &outUV.y);

		CTexturedNormalVertex* m_pVer = new CTexturedNormalVertex(D3DXVECTOR3(m_pos.x, m_pos.z, -m_pos.y), D3DXVECTOR3(outNormal.x, outNormal.z, -outNormal.y), D3DXVECTOR2(outUV.x, outUV.y));
		v.push_back(m_pVer);
	}
	fclose(fp);

}