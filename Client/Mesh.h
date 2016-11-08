#pragma once

#include "Vertex.h"

#include <DirectXMath.h>
using namespace DirectX;





//
//	Mesh ( base class )
//
class CMesh
{
public:
	CMesh();
	virtual ~CMesh();

	void AddRef();
	void Release();

	virtual void CreateRasterizerState() {}
	virtual void Render();

	XMFLOAT4X4*** GetResultMatrix() { return m_ppResult; }
	int* GetAniMaxTime() { return m_iAnimationMaxTime; }
	unsigned int GetAnimationIndexCnt() { return m_uiAnimationIndexCnt; }

	void SetMaxVer(CXMVECTOR max) { XMStoreFloat3(m_pf3MaxVer, max); }
	void SetMinVer(CXMVECTOR min) { XMStoreFloat3(m_pf3MinVer, min); }
	XMFLOAT3* GetMaxVer() { return m_pf3MaxVer; }
	XMFLOAT3* GetMinVer() { return m_pf3MinVer; }

protected:
	ID3D11Buffer *m_pd3dVertexBuffer;	/* 정점 버퍼 인터페이스 포인터. 정점 데이터 저장용 */
	UINT m_nVertices;	/* 정점 버퍼의 정점 개수 */
	UINT m_nStride;	/* 정점의 바이트 크기 */
	UINT m_nOffset;	/* 정점 데이터가 정점 버퍼의 어디서부터 시작하는가 */
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;	/* 정점 데이터가 어떤 프리미티브를 표현하고 있는가 */

	ID3D11Buffer *m_pd3dIndexBuffer;	/* 인덱스 버퍼 인터페이스 포인터. 인덱스 버퍼 저장용 */
	UINT m_nIndices;	/* 인덱스 버퍼의 인덱스 개수 */
	UINT m_nStartIndex;	/* 인덱스 버퍼에서 사용할 시작 인덱스 */
	int m_nBaseVertex;	/* 각 인덱스에 더해질 인덱스 */

	ID3D11RasterizerState *m_pd3dRasterizerState;

	XMFLOAT3 *m_pf3MaxVer;
	XMFLOAT3 *m_pf3MinVer;

	XMFLOAT4X4** m_ppResult[ANIMATION_COUNT];
	int m_iAnimationMaxTime[ANIMATION_COUNT];
	unsigned int m_uiAnimationIndexCnt;

private:
	int m_nReferences;
};





//
//	Cube Mesh
//
class CCubeMesh : public CMesh
{
public:
	CCubeMesh(float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMesh();

	virtual void SetRasterizerState();
	virtual void Render();

private:
	void _CalculateVertexNormal(BYTE *pVertices, WORD *pIndices);

	// for _CalculateVertexNormal()
	XMVECTOR _CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);	/* 삼각형의 법선 벡터 계산. 삼각형의 세 정점을 사용 */
	void _SetTriAngleListVertexNormal(BYTE *pVertices);	/* 정점의 법선 벡터 계산. 인덱스 버퍼를 쓰지 않는 삼각형 리스트용 */
	void _SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip);	/* 정점의 법선벡터의 평균 계산. 인덱스 버퍼를 사용할 경우 */
};





//
//	Imported Mesh
//
class CImportedMesh : public CMesh
{
public:
	CImportedMesh(char* pTxtName);
	virtual ~CImportedMesh();

	virtual void CreateRasterizerState();

private:
	CTexturedNormalVertex* ppVertices;
};





//
//	Imported Animating Mesh
//
class CImportedAnimatingMesh : public CMesh
{
public:
	CImportedAnimatingMesh(int CharNum, int StateCnt);
	virtual ~CImportedAnimatingMesh();

	virtual void CreateRasterizerState();

private:
	CAnimationVertex* ppVertices;
};


