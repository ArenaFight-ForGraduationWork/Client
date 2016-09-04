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
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);	/* 삼각형의 법선 벡터 계산. 삼각형의 세 정점을 사용 */
	void SetTriAngleListVertexNormal(BYTE *pVertices);	/* 정점의 법선 벡터 계산. 인덱스 버퍼를 쓰지 않는 삼각형 리스트용 */
	void SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip);	/* 정점의 법선벡터의 평균 계산. 인덱스 버퍼를 사용할 경우 */
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