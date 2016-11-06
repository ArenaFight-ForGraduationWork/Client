#pragma once

#include "Shader.h"

#include <DDSTextureLoader.h>
#include <DirectXMath.h>
using namespace DirectX;


struct VS_CB_PARTICLE
{
	float m_fInitialPosition[3];
	float m_fInitialVelocity[3];
	float m_fSize[2];
	float m_fAge;
	unsigned int m_uiType;
};

class CParticle
{
public:
	CParticle();
	~CParticle();

	float GetAge() const;

	void SetEyePos(const CXMVECTOR eyePosW);
	void SetEmitPos(const CXMVECTOR emitPosW);
	void SetEmitDir(const CXMVECTOR emitDirW);

	void Reset();
	void Update(float dt, float gameTime);
	void Draw(CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix);

	void BuildVB();

private:
	UINT mMaxParticles;
	bool mFirstRun;

	float mGameTime;
	float mTimeStep;
	float mAge;

	XMFLOAT3 mEyePosW;
	XMFLOAT3 mEmitPosW;
	XMFLOAT3 mEmitDirW;

	CParticleShader *m_pShader;

	ID3D11Buffer* mInitVB;
	ID3D11Buffer* mDrawVB;
	ID3D11Buffer* mStreamOutVB;

	ID3D11ShaderResourceView* mTexArraySRV;
	ID3D11ShaderResourceView* mRandomTexSRV;
};

