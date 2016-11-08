#include "stdafx.h"
#include "Fbx.h"




CFbx::CFbx()
{
	m_iSize = 0;

	for (int i = 0; i < ANIMATION_COUNT; ++i)
	{
		m_ppResult[i] = nullptr;
	}

	m_iAnimationMaxTime = 0;
	m_uiAnimationNodeIndexCount = 0;
	m_fAnimationPlayTime = 0.0f;

	m_pMaxVer = new XMFLOAT3(0, 0, 0);
	m_pMinVer = new XMFLOAT3(0, 0, 0);

	m_pAniIndexCount = nullptr;
}
CFbx::~CFbx()
{
}

void CFbx::ReadTextFile_Mesh(char *fileName, CTexturedNormalVertex* &v)
{
	FILE *fp;
	fopen_s(&fp, fileName, "rt");

	fscanf_s(fp, "%d\n", &m_iSize);

	v = new CTexturedNormalVertex[m_iSize];

	XMFLOAT3 outTemp;
	for (int i = 0; i < m_iSize; ++i)
	{
		//정점데이터 얻어오기
		fscanf_s(fp, "%f %f %f\n", &outTemp.x, &outTemp.y, &outTemp.z);
		v[i].SetPosition(outTemp.x, outTemp.z, -outTemp.y);

		fscanf_s(fp, "%f %f %f\n", &outTemp.x, &outTemp.y, &outTemp.z);
		v[i].SetNormal(outTemp.x, outTemp.z, -outTemp.y);

		fscanf_s(fp, "%f %f\n", &outTemp.x, &outTemp.y);
		v[i].SetUV(outTemp.x, outTemp.y);
	}

	fscanf_s(fp, "%f %f %f\n", &(m_pMaxVer->x), &(m_pMaxVer->y), &(m_pMaxVer->z));
	fscanf_s(fp, "%f %f %f\n", &(m_pMinVer->x), &(m_pMinVer->y), &(m_pMinVer->z));

	fclose(fp);
}
void CFbx::ReadTextFile_Mesh(int CharNum, CAnimationVertex* &v)
{
	FILE *fp;

	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fp, "Data\\Human\\C_info.txt", "rt");
		break;
	case 1:   //슬라임
	default:
		fopen_s(&fp, "Data\\Slime\\M_info.txt", "rt");
		break;
	}

	fscanf_s(fp, "%d\n", &m_iSize);
	v = new CAnimationVertex[m_iSize];

	for (int i = 0; i < m_iSize; ++i)
	{
		fscanf_s(fp, "%f %f %f\n", &(v[i].m_f3Position.x), &v[i].m_f3Position.y, &v[i].m_f3Position.z);
		fscanf_s(fp, "%f %f %f\n", &v[i].m_f3Normal.x, &v[i].m_f3Normal.y, &v[i].m_f3Normal.z);
		fscanf_s(fp, "%f %f\n", &v[i].m_f2TexCoord.x, &v[i].m_f2TexCoord.y);
	}

	fscanf_s(fp, "%f %f %f\n", &(m_pMaxVer->x), &(m_pMaxVer->y), &(m_pMaxVer->z));
	fscanf_s(fp, "%f %f %f\n", &(m_pMinVer->x), &(m_pMinVer->y), &(m_pMinVer->z));

	fclose(fp);
}

void CFbx::ReadTextFile_Ani(int CharNum, int StateCnt)
{
	FILE *fMonA[ANIMATION_COUNT];

	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fMonA[0], "Data\\Human\\C_matrix_idle.txt", "rt");
		fopen_s(&fMonA[1], "Data\\Human\\C_matrix_run.txt", "rt");
		fopen_s(&fMonA[2], "Data\\Human\\C_matrix_dead.txt", "rt");
		fopen_s(&fMonA[3], "Data\\Human\\C_matrix_attack.txt", "rt");
		fopen_s(&fMonA[4], "Data\\Human\\C_matrix_skill1.txt", "rt");
		fopen_s(&fMonA[5], "Data\\Human\\C_matrix_skill2.txt", "rt");
		fopen_s(&fMonA[6], "Data\\Human\\C_matrix_skill3.txt", "rt");
		break;
	case 1:  //슬라임
		fopen_s(&fMonA[0], "Data\\Slime\\M_matrix_idle.txt", "rt");
		fopen_s(&fMonA[1], "Data\\Slime\\M_matrix_move.txt", "rt");
		fopen_s(&fMonA[2], "Data\\Slime\\M_matrix_dead.txt", "rt");		//아직 dead가 없음
		fopen_s(&fMonA[3], "Data\\Slime\\M_matrix_attack.txt", "rt");	//평타 : 물기
		fopen_s(&fMonA[4], "Data\\Slime\\M_matrix_dash.txt", "rt");		//스킬 1 : 돌진
		fopen_s(&fMonA[5], "Data\\Slime\\M_matrix_jump.txt", "rt");		//스킬 2 : 충격파
		fopen_s(&fMonA[6], "Data\\Slime\\M_matrix_jumpjump.txt", "rt");	//스킬 3:  쿵쿵쿵
		break;
	}

	fscanf_s(fMonA[StateCnt], "%d %d\n", &m_iAnimationMaxTime, &m_uiAnimationNodeIndexCount);

	m_ppResult[StateCnt] = new  XMFLOAT4X4*[static_cast<unsigned int>(m_iAnimationMaxTime) / 10];

	for (long long i = 0; i < m_iAnimationMaxTime / 10; ++i)
	{
		m_ppResult[StateCnt][i] = new  XMFLOAT4X4[m_uiAnimationNodeIndexCount];
	}

	for (long long i = 0; i < m_iAnimationMaxTime / 10; ++i)
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

void CFbx::ReadTextFile_Weight(int CharNum, CAnimationVertex* cAniVer)
{
	FILE *fp;

	switch (CharNum)
	{
	case 0:	// 인간
		fopen_s(&fp, "Data\\Human\\C_weight.txt", "rt");
		break;
	case 1:  //슬라임
	default:
		fopen_s(&fp, "Data\\Slime\\M_weight.txt", "rt");
		break;
	}

	int iter = 0;
	int index = 0;
	float weight = 0.0f;

	for (int i = 0; i < m_iSize; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			fscanf_s(fp, "%d %d %f\n", &iter, &index, &weight);
			cAniVer[iter].AddBone(index, weight);
		}
	}

	fclose(fp);
}




