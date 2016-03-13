#ifndef VERTEX_H_
#define VERTEX_H_

class CVertex
{
private:
	D3DXVECTOR3 m_d3dxvPosition;

public:
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
	D3DXVECTOR3 GetPosition()
	{
		return m_d3dxvPosition;
	}
};

class CDiffusedVertex
{
public:
	CDiffusedVertex();
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse);
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse);
	~CDiffusedVertex();

private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXCOLOR m_d3dxcDiffuse;
};

class CNormalVertex
{
public:
	CNormalVertex(float x, float y, float z, float nx, float ny, float nz) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); }
	CNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; }
	CNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	~CNormalVertex() { }

	void SetNormal(D3DXVECTOR3 d3dxvNormal) { m_d3dxvNormal = d3dxvNormal; }

private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
};

class CTexturedVertex
{
public:
	CTexturedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f); }
	CTexturedVertex(float x, float y, float z, float u, float v) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvTexCoord = D3DXVECTOR2(u, v); }
	CTexturedVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR2 d3dxvTexture) { m_d3dxvPosition = d3dxvPosition; m_d3dxvTexCoord = d3dxvTexture; }
	~CTexturedVertex() { }

private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR2 m_d3dxvTexCoord;
};

class CTexturedNormalVertex
{
private:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	D3DXVECTOR2 m_d3dxvTexCoord;

public:
	CTexturedNormalVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxvNormal = D3DXVECTOR3(nx, ny, nz); m_d3dxvTexCoord = D3DXVECTOR2(u, v); }
	CTexturedNormalVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal, D3DXVECTOR2 d3dxvTexCoord) { m_d3dxvPosition = d3dxvPosition; m_d3dxvNormal = d3dxvNormal; m_d3dxvTexCoord = d3dxvTexCoord; }
	CTexturedNormalVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxvTexCoord = D3DXVECTOR2(0.0f, 0.0f); }
	~CTexturedNormalVertex() { }

	D3DXVECTOR3 GetPosition()
	{
		return m_d3dxvPosition;
	}
};

class CAnimationVertex
{
public:
	D3DXVECTOR3 m_d3dxvPosition;
	D3DXVECTOR3 m_d3dxvNormal;
	D3DXVECTOR2 m_d3dxvTexCoord;

	int BoneIndexArr[8];
	float BoneWeightArr[8];

	CAnimationVertex()
	{
		m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_d3dxvNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < 8; ++i)
		{
			BoneIndexArr[i] = 0;
			BoneWeightArr[i] = -1.0f;
		}
	}

	CAnimationVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxvNormal = d3dxvNormal;
	}

	CAnimationVertex(D3DXVECTOR3 d3dxvPosition, D3DXVECTOR3 d3dxvNormal, D3DXVECTOR2 d3dxvUV)
	{
		m_d3dxvPosition = d3dxvPosition;
		m_d3dxvNormal = d3dxvNormal;
		m_d3dxvTexCoord = d3dxvUV;
	}
	~CAnimationVertex(){}
	void AddBone(int index, float Weight);
};


#endif