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
	D3DXVECTOR3 CalculateTriAngleNormal(BYTE *pVertices, USHORT nIndex0, USHORT nIndex1, USHORT nIndex2);	/* 삼각형의 법선 벡터 계산. 삼각형의 세 정점을 사용 */
	void SetTriAngleListVertexNormal(BYTE *pVertices);	/* 정점의 법선 벡터 계산. 인덱스 버퍼를 쓰지 않는 삼각형 리스트용 */
	void SetAverageVertexNormal(BYTE *pVertices, WORD *pIndices, int nPrimitives, int nOffset, bool bStrip);	/* 정점의 법선벡터의 평균 계산. 인덱스 버퍼를 사용할 경우 */
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

	//수정시작합니다. 한줄
	CMyAni(ID3D11Device *pd3dDevice, int CharNum, int StateCnt);

	virtual ~CMyAni();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

private:
	vector<CAnimationVertex*> pVertices;
	CAnimationVertex* ppVertices;

	//수정시작합니다. 한줄
	FILE *fp[2];	//일단 달리기, 어택 두개기 때문에 2를 써둔다.

};

#endif