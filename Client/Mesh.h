#ifndef MESH_H_
#define MESH_H_

#include "Vertex.h"

#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)





class CMesh
{
public:

	XMFLOAT4X4** m_ppResult[5];
	long long m_AniMaxTime[5];
	unsigned int m_AnimationIndexCnt;
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

	void AddRef();
	void Release();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

	void SetAnimationMaxTime(long long time) { m_AnimationMaxTime = time; }
	void SetAnimationIndexCnt(int cnt) { m_AnimationIndexCount = cnt; }

	void SetMaxVer(D3DXVECTOR3 max)	{ m_MaxVer = max; }
	void SetMinVer(D3DXVECTOR3 min)	{ m_MinVer = min; }
	D3DXVECTOR3 GetMaxVer()	{ return m_MaxVer; }
	D3DXVECTOR3 GetMinVer()		{ return m_MinVer; }
protected:
	ID3D11Buffer *m_pd3dVertexBuffer;	/* ���� ���� �������̽� ������. ���� ������ ����� */
	UINT m_nVertices;	/* ���� ������ ���� ���� */
	UINT m_nStride;	/* ������ ����Ʈ ũ�� */
	UINT m_nOffset;	/* ���� �����Ͱ� ���� ������ ��𼭺��� �����ϴ°� */
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;	/* ���� �����Ͱ� � ������Ƽ�긦 ǥ���ϰ� �ִ°� */

	ID3D11Buffer *m_pd3dIndexBuffer;	/* �ε��� ���� �������̽� ������. �ε��� ���� ����� */
	UINT m_nIndices;	/* �ε��� ������ �ε��� ���� */
	UINT m_nStartIndex;	/* �ε��� ���ۿ��� ����� ���� �ε��� */
	int m_nBaseVertex;	/* �� �ε����� ������ �ε��� */

	ID3D11RasterizerState *m_pd3dRasterizerState;

private:
	int m_nReferences;
	long long m_AnimationMaxTime = 0;
	int m_AnimationIndexCount = 0;

	D3DXVECTOR3 m_MaxVer;
	D3DXVECTOR3 m_MinVer;

	
};






class CCubeMeshIlluminatedTextured : public CMesh
{
public:
	CCubeMeshIlluminatedTextured(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshIlluminatedTextured();

	virtual void SetRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

private:
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);	/* �ﰢ���� ���� ���� ���. �ﰢ���� �� ������ ��� */
	void SetTriAngleListVertexNormal(BYTE *pVertices);	/* ������ ���� ���� ���. �ε��� ���۸� ���� �ʴ� �ﰢ�� ����Ʈ�� */
	void SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip);	/* ������ ���������� ��� ���. �ε��� ���۸� ����� ��� */
	void CalculateVertexNormal(BYTE *pVertices, WORD *pIndices);
};




class CMyModel : public CMesh
{
public:
	CMyModel(ID3D11Device *pd3dDevice, char* pTxtName, D3DXVECTOR3 d3dxvScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	virtual ~CMyModel();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

private:
	std::vector<CTexturedNormalVertex*> pVertices;
	CTexturedNormalVertex* ppVertices;
};


class CMyAni : public CMesh
{
public:
	CMyAni(ID3D11Device *pd3dDevice, char* pFbxName, char* pTxtName);

	//���������մϴ�. ����
	CMyAni(ID3D11Device *pd3dDevice, int CharNum, int StateCnt);

	virtual ~CMyAni();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

private:
	vector<CAnimationVertex*> pVertices;
	CAnimationVertex* ppVertices;

	//���������մϴ�. ����
	FILE *fp[2];	//�ϴ� �޸���, ���� �ΰ��� ������ 2�� ��д�.

};

#endif