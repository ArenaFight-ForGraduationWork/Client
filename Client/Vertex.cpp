#include "stdafx.h"
#include "Vertex.h"
#include "Effects.h"


CDiffusedVertex::CDiffusedVertex()
{
	m_d3dxvPosition = D3DXVECTOR3(0, 0, 0);
	m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
}
CDiffusedVertex::CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse)
{
	m_d3dxvPosition = D3DXVECTOR3(x, y, z);
	m_d3dxcDiffuse = d3dxcDiffuse;
}
CDiffusedVertex::CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse)
{
	m_d3dxvPosition = d3dxvPosition;
	m_d3dxcDiffuse = d3dxcDiffuse;
}
CDiffusedVertex::~CDiffusedVertex()
{
}

void CAnimationVertex::AddBone(int index, float weight)
{
	for (int i = 0; i < 8; ++i)
	{
		if (BoneWeightArr[i] <= 0.0f)
		{
			BoneIndexArr[i] = index;
			BoneWeightArr[i] = weight;
			return;
		}
	}

	//만약 배열을 벗어나면 첫번째 인덱스에 다 더한다.
	BoneWeightArr[0] += weight;
}



#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Pos[1] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Terrain[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Particle[5] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

#pragma endregion

#pragma region InputLayouts

ID3D11InputLayout* InputLayouts::Pos = 0;
ID3D11InputLayout* InputLayouts::Basic32 = 0;
ID3D11InputLayout* InputLayouts::Terrain = 0;
ID3D11InputLayout* InputLayouts::Particle = 0;

void InputLayouts::InitAll(ID3D11Device* device)
{
	D3DX11_PASS_DESC passDesc;

	//
	// Pos
	//

	Effects::SkyFX->SkyTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Pos, 1, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Pos));

	//
	// Basic32
	//

	Effects::BasicFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Basic32));

	//
	// Terrain
	//

	Effects::TerrainFX->Light1Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Terrain, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Terrain));

	//
	// Particle
	//

	Effects::FireFX->StreamOutTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::Particle, 5, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &Particle));
}

void InputLayouts::DestroyAll()
{
	ReleaseCOM(Pos);
	ReleaseCOM(Basic32);
	ReleaseCOM(Terrain);
	ReleaseCOM(Particle);
}

#pragma endregion
