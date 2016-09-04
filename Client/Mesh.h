#ifndef MESH_H_
#define MESH_H_

#include "Vertex.h"





class CMesh
{
public:
	CMesh();
	virtual ~CMesh();

	void AddRef();
	void Release();

	virtual void CreateRasterizerState();
	virtual void Render();

	DirectX::XMFLOAT4X4*** GetResultMatrix() { return m_ppResult; }
	int* GetAniMaxTime() { return m_iAnimationMaxTime; }
	unsigned int GetAnimationIndexCnt() { return m_uiAnimationIndexCnt; }

	void SetMaxVer(D3DXVECTOR3 *max) { m_pMaxVer = max; }
	void SetMinVer(D3DXVECTOR3 *min) { m_pMinVer = min; }
	D3DXVECTOR3& GetMaxVer() { return *m_pMaxVer; }
	D3DXVECTOR3& GetMinVer() { return *m_pMinVer; }

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

	D3DXVECTOR3 *m_pMaxVer;
	D3DXVECTOR3 *m_pMinVer;

	DirectX::XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];
	int m_iAnimationMaxTime[ANIMATION_COUNT];
	unsigned int m_uiAnimationIndexCnt;

private:
	int m_nReferences;
};



class CCubeMeshIlluminatedTextured : public CMesh
{
public:
	CCubeMeshIlluminatedTextured(float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshIlluminatedTextured();

	virtual void SetRasterizerState();
	virtual void Render();

private:
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);	/* �ﰢ���� ���� ���� ���. �ﰢ���� �� ������ ��� */
	void SetTriAngleListVertexNormal(BYTE *pVertices);	/* ������ ���� ���� ���. �ε��� ���۸� ���� �ʴ� �ﰢ�� ����Ʈ�� */
	void SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip);	/* ������ ���������� ��� ���. �ε��� ���۸� ����� ��� */
	void CalculateVertexNormal(BYTE *pVertices, WORD *pIndices);
};




class CImportedMesh : public CMesh
{
public:
	CImportedMesh(char* pTxtName);
	virtual ~CImportedMesh();

	virtual void CreateRasterizerState();
	virtual void Render();

private:
	CTexturedNormalVertex* ppVertices;
};






class CImportedAnimatingMesh : public CMesh
{
public:
	CImportedAnimatingMesh(int CharNum, int StateCnt);
	virtual ~CImportedAnimatingMesh();

	virtual void CreateRasterizerState();
	virtual void Render();

private:
	CAnimationVertex* ppVertices;
};




#endif