#include "stdafx.h"
#include "Fbx.h"


CFbx::CFbx()
{
	m_fAnimationPlayTime = 0.0f;
	m_llAnimationMaxTime = 0;

	for (int i = 0; i < ANIMATION_COUNT; ++i)
	{
		m_ppResult[i] = nullptr;
		m_AniTime[i] = 0;
	}

	m_pAniIndexCount = nullptr;
	m_pAniTime = nullptr;
}


CFbx::~CFbx()
{
	/*for (int i = 0; i < 5; ++i)
		m_ppResult[i] = nullptr;*/

}

CFbx* CFbx::m_instance;

void CFbx::Fbx_ReadTextFile_Mesh(char* fileName, vector<CTexturedNormalVertex*> &v, D3DXVECTOR3 scale)
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

		m_pos.x *= scale.x;
		m_pos.y *= scale.y;
		m_pos.z *= scale.z;

		CTexturedNormalVertex* m_pVer = new CTexturedNormalVertex(D3DXVECTOR3(m_pos.x, m_pos.z, -m_pos.y), D3DXVECTOR3(outNormal.x, outNormal.z, -outNormal.y), D3DXVECTOR2(outUV.x, outUV.y));
		v.push_back(m_pVer);
	}

	fscanf_s(fp, "%f %f %f\n", &m_MaxVer.x, &m_MaxVer.y, &m_MaxVer.z);
	fscanf_s(fp, "%f %f %f\n", &m_MinVer.x, &m_MinVer.y, &m_MinVer.z);

	m_MaxVer *= *scale;
	m_MinVer *= *scale;

	//printf(" Object max x : %f, max y : %f, ,max z : %f\n", m_MaxVer.x, m_MaxVer.y, m_MaxVer.z);
	//printf(" Object min x : %f, min y : %f, ,min z : %f\n", m_MinVer.x, m_MinVer.y, m_MinVer.z);

	fclose(fp);
}

void CFbx::Fbx_ReadTextFile_Info(int CharNum)
{
	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fp, "Data\\C_info.txt", "rt");
		break;
	case 1:   //슬라임
		fopen_s(&fp, "Data\\M_info.txt", "rt");
		break;
	}

	int Cnt = 0;		//size값을 가져올 임시 변수
	fscanf_s(fp, "%d\n", &Cnt);
	size = Cnt;
	fclose(fp);
}

void CFbx::Fbx_ReadTextFile_Weight(int CharNum, CAnimationVertex* cAniVer)
{
	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fp, "Data\\C_weight.txt", "rt");
		break;
	case 1:  //슬라임
		fopen_s(&fp, "Data\\M_weight.txt", "rt");
		break;
	}

	int iter = 0;
	int index = 0;
	float weight = 0.0f;

	for (int i = 0; i <tempcnt; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			fscanf_s(fp, "%d %d %f\n", &iter, &index, &weight);
			cAniVer[iter].AddBone(index, weight);
		}
	}

	fclose(fp);
}

void CFbx::Fbx_ReadTextFile_Mesh(int CharNum, CAnimationVertex* &v)
{
	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fp, "Data\\C_info.txt", "rt");
		break;
	case 1:   //슬라임
		fopen_s(&fp, "Data\\M_info.txt", "rt");
		break;
	}

	int Cnt = 0;
	fscanf_s(fp, "%d\n", &Cnt);
	tempcnt = Cnt;	//tempCnt는 weight에서 포문돌릴때 사용함.

	for (int i = 0; i < Cnt; ++i)
	{
		fscanf_s(fp, "%f %f %f\n", &v[i].m_d3dxvPosition.x, &v[i].m_d3dxvPosition.y, &v[i].m_d3dxvPosition.z);
		fscanf_s(fp, "%f %f %f\n", &v[i].m_d3dxvNormal.x, &v[i].m_d3dxvNormal.y, &v[i].m_d3dxvNormal.z);
		fscanf_s(fp, "%f %f\n", &v[i].m_d3dxvTexCoord.x, &v[i].m_d3dxvTexCoord.y);
	}

	fscanf_s(fp, "%f %f %f\n", &m_MaxVer.x, &m_MaxVer.y, &m_MaxVer.z);
	fscanf_s(fp, "%f %f %f\n", &m_MinVer.x, &m_MinVer.y, &m_MinVer.z);

	fclose(fp);
}

void CFbx::Fbx_ReadTextFile_Ani(int CharNum, int StateCnt)
{

	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fMonA[0], "Data\\C_matrix_idle.txt", "rt");
		fopen_s(&fMonA[1], "Data\\C_matrix_run.txt", "rt");
		fopen_s(&fMonA[2], "Data\\C_matrix_dead.txt", "rt");
		fopen_s(&fMonA[3], "Data\\C_matrix_attack.txt", "rt");
		fopen_s(&fMonA[4], "Data\\C_matrix_skill1.txt", "rt");
		fopen_s(&fMonA[5], "Data\\C_matrix_skill2.txt", "rt");
		fopen_s(&fMonA[6], "Data\\C_matrix_skill3.txt", "rt");
		break;
	case 1:  //슬라임
		fopen_s(&fMonA[0], "Data\\M_matrix_idle.txt", "rt");
		fopen_s(&fMonA[1], "Data\\M_matrix_move.txt", "rt");
		fopen_s(&fMonA[2], "Data\\M_matrix_dead.txt", "rt");		//아직 dead가 없음
		fopen_s(&fMonA[3], "Data\\M_matrix_attack.txt", "rt");		//평타 : 물기
		fopen_s(&fMonA[4], "Data\\M_matrix_dash.txt", "rt");			//스킬 1 : 돌진
		fopen_s(&fMonA[5], "Data\\M_matrix_jump.txt", "rt");			//스킬 2 : 충격파
		fopen_s(&fMonA[6], "Data\\M_matrix_jumpjump.txt", "rt");	//스킬 3:  쿵쿵쿵
		break;
	}

	fscanf_s(fMonA[StateCnt], "%lld %d\n", &m_llAnimationMaxTime, &m_uiAnimationNodeIndexCount);

	m_ppResult[StateCnt] = new XMFLOAT4X4*[m_llAnimationMaxTime / 10];

	for (long long i = 0; i < m_llAnimationMaxTime / 10; ++i)
	{
		m_ppResult[StateCnt][i] = new XMFLOAT4X4[m_uiAnimationNodeIndexCount];
	}

	for (long long i = 0; i < m_llAnimationMaxTime / 10; ++i)
	{
		for (unsigned int j = 0; j < m_uiAnimationNodeIndexCount; ++j)
		{
			for (int m = 0; m < 4; ++m)
			{
				for (int n = 0; n < 4; ++n)
				{
					fscanf_s(fMonA[StateCnt], "%f\n", &m_ppResult[StateCnt][i][j](m, n));

				}
			}
		}
	}

	for (int i = 0; i < StateCnt; ++i)
	{
		fclose(fMonA[i]);
	}

}

void CFbx::ReadTextFile_HitBox(int CharNum, D3DXVECTOR3* &max, D3DXVECTOR3* &min)
{
	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fp, "Data\\C_hitbox.txt", "rt");
		break;
	case 1:	// 슬라임
		fopen_s(&fp, "Data\\M_hitbox.txt", "rt");
		break;
	}

	int Cnt = 0;
	fscanf_s(fp, "%d\n", &Cnt);
	tempsize = Cnt;

	for (int i = 0; i < Cnt; ++i)
	{
		fscanf_s(fp, "%f %f %f %f %f %f\n", &max[i].x, &max[i].y, &max[i].z, &min[i].x, &min[i].y, &min[i].z);
		//printf("%f %f %f %f %f %f\n", m_HitMaxVer[i].x, m_HitMaxVer[i].y, m_HitMaxVer[i].z, m_HitMinVer[i].x, m_HitMinVer[i].y, m_HitMinVer[i].z);
	}

	fclose(fp);
}
