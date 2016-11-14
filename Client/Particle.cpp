#include "stdafx.h"
#include "Particle.h"
#include "MathHelper.h"


CParticle::CParticle()
	: mInitVB(0), mDrawVB(0), mStreamOutVB(0), mTexArraySRV(0), mRandomTexSRV(0)
{
	mFirstRun = true;
	mGameTime = 0.0f;
	mTimeStep = 0.0f;
	mAge = 0.0f;

	mEyePosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEmitPosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mEmitDirW = XMFLOAT3(0.0f, 1.0f, 0.0f);

	vector<wstring> vFlare;

	m_pShader = new CParticleShader("Fire.fxo");
	m_pShader->CreateShader();
	mMaxParticles = 10000;
	vFlare.push_back(L"flare0.dds");

	//
	// Load the texture elements individually from file.  These textures
	// won't be used by the GPU (0 bind flags), they are just used to 
	// load the image data from file.  We use the STAGING usage so the
	// CPU can read the resource.
	//
	UINT size = vFlare.size();

	vector<ID3D11Resource*> srcTex(size);
	for (UINT i = 0; i < size; ++i)
	{
		ID3D11ShaderResourceView* shaderResourceView;
		HR(CreateDDSTextureFromFile(gpCommonState->m_pd3dDevice, vFlare[i].c_str(), &srcTex[i], &shaderResourceView));
		shaderResourceView->Release();
	}

	//
	// Create the texture array.  Each element in the texture 
	// array has the same format/dimensions.
	//
	D3D11_TEXTURE2D_DESC texElementDesc;
	((ID3D11Texture2D*)srcTex[0])->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = size;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	ID3D11Texture2D* texArray = 0;
	HR(gpCommonState->m_pd3dDevice->CreateTexture2D(&texArrayDesc, 0, &texArray));

	//
	// Copy individual texture elements into texture array.
	//
	for (UINT texElement = 0; texElement < size; ++texElement)	// for each texture element...	
	{
		for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)	// for each mipmap level...
		{
			const uint32_t subResourceIndex = mipLevel;
			const uint32_t destinationSubresource = D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels);
			gpCommonState->m_pd3dDeviceContext->CopySubresourceRegion(texArray, static_cast<uint32_t> (destinationSubresource),
				0, 0, 0, srcTex[texElement], subResourceIndex, nullptr);
		}
	}

	//
	// Create a resource view to the texture array.
	//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = size;

	ID3D11ShaderResourceView* texArraySRV = 0;
	HR(gpCommonState->m_pd3dDevice->CreateShaderResourceView(texArray, &viewDesc, &texArraySRV));

	//
	// Cleanup--we only need the resource view.
	//
	texArray->Release();

	for (UINT i = 0; i < size; ++i)
	{
		srcTex[i]->Release();
	}

	mTexArraySRV = texArraySRV;

	// 
	// Create the random data for Random Texture
	//
	XMFLOAT4 randomValues[1024];
	for (int i = 0; i < 1024; ++i)
	{
		randomValues[i].x = MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].y = MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].z = MathHelper::RandF(-1.0f, 1.0f);
		randomValues[i].w = MathHelper::RandF(-1.0f, 1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues;
	initData.SysMemPitch = 1024 * sizeof(XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	//
	// Create the texture.
	//
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex = 0;
	HR(gpCommonState->m_pd3dDevice->CreateTexture1D(&texDesc, &initData, &randomTex));

	//
	// Create the resource view.
	//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc1;
	viewDesc1.Format = texDesc.Format;
	viewDesc1.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc1.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc1.Texture1D.MostDetailedMip = 0;

	ID3D11ShaderResourceView* randomTexSRV = 0;
	HR(gpCommonState->m_pd3dDevice->CreateShaderResourceView(randomTex, &viewDesc1, &randomTexSRV));
	randomTex->Release();

	mRandomTexSRV = randomTexSRV;

	BuildVB();
}
CParticle::~CParticle()
{
	if (mInitVB) mInitVB->Release();
	if (mDrawVB) mDrawVB->Release();
	if (mStreamOutVB) mStreamOutVB->Release();
}

float CParticle::GetAge()const
{
	return mAge;
}

void CParticle::SetEyePos(const CXMVECTOR eyePosW)
{
	XMStoreFloat3(&mEyePosW, eyePosW);
}
void CParticle::SetEmitPos(const CXMVECTOR emitPosW)
{
	XMStoreFloat3(&mEmitPosW, emitPosW);
}
void CParticle::SetEmitDir(const CXMVECTOR emitDirW)
{
	XMStoreFloat3(&mEmitDirW, emitDirW);
}

void CParticle::Reset()
{
	mFirstRun = true;
	mAge = 0.0f;
}

void CParticle::Update(float dt, float gameTime)
{
	mGameTime = gameTime;
	mTimeStep = dt;

	mAge += dt;
}

void CParticle::Draw(CXMMATRIX viewMatrix, CXMMATRIX projectionMatrix)
{
	//
	// Set constants.
	//
	XMMATRIX VP;
	VP = XMMatrixMultiply(viewMatrix, projectionMatrix);
	XMVECTOR eyePosW;
	eyePosW = XMLoadFloat3(&mEyePosW);
	XMVECTOR emitPosW;
	emitPosW = XMLoadFloat3(&mEmitPosW);
	XMVECTOR emitDirW;
	emitDirW = XMLoadFloat3(&mEmitDirW);

	m_pShader->m_particle->SetViewProj(VP);
	m_pShader->m_particle->SetGameTime(mGameTime);
	m_pShader->m_particle->SetTimeStep(mTimeStep);
	m_pShader->m_particle->SetEyePosW(eyePosW);
	m_pShader->m_particle->SetEmitPosW(emitPosW);
	m_pShader->m_particle->SetEmitDirW(emitDirW);
	m_pShader->m_particle->SetTexArray(mTexArraySRV);
	m_pShader->m_particle->SetRandomTex(mRandomTexSRV);

	//
	// Set IA stage.
	//
	m_pShader->SetInputLayout();
	gpCommonState->m_pd3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(VS_CB_PARTICLE);
	UINT offset = 0;

	// On the first pass, use the initialization VB.  Otherwise, use
	// the VB that contains the current particle list.
	if (mFirstRun)
		gpCommonState->m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &mInitVB, &stride, &offset);
	else
		gpCommonState->m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	//
	// Draw the current particle list using stream-out only to update them.  
	// The updated vertices are streamed-out to the target VB. 
	//
	gpCommonState->m_pd3dDeviceContext->SOSetTargets(1, &mStreamOutVB, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pShader->m_particle->StreamOutTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		//// herehere
		HR(m_pShader->m_particle->StreamOutTech->GetPassByIndex(p)->Apply(0, gpCommonState->m_pd3dDeviceContext));

		if (mFirstRun)
		{
			gpCommonState->m_pd3dDeviceContext->Draw(1, 0);
			mFirstRun = false;
		}
		else
		{
			gpCommonState->m_pd3dDeviceContext->DrawAuto();
		}
	}

	// done streaming-out--unbind the vertex buffer
	ID3D11Buffer* bufferArray[1] = { 0 };
	gpCommonState->m_pd3dDeviceContext->SOSetTargets(1, bufferArray, &offset);

	// ping-pong the vertex buffers
	swap(mDrawVB, mStreamOutVB);

	//
	// Draw the updated particle system we just streamed-out. 
	//
	gpCommonState->m_pd3dDeviceContext->IASetVertexBuffers(0, 1, &mDrawVB, &stride, &offset);

	m_pShader->m_particle->DrawTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		//// herehere
		HR(m_pShader->m_particle->DrawTech->GetPassByIndex(p)->Apply(0, gpCommonState->m_pd3dDeviceContext));
		gpCommonState->m_pd3dDeviceContext->DrawAuto();
	}

	//
	// restore default states.
	//
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	gpCommonState->m_pd3dDeviceContext->RSSetState(0);
	gpCommonState->m_pd3dDeviceContext->OMSetDepthStencilState(0, 0);
	gpCommonState->m_pd3dDeviceContext->OMSetBlendState(0, blendFactor, 0xffffffff);
}

void CParticle::BuildVB()
{
	//
	// Create the buffer to kick-off the particle system.
	//
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(VS_CB_PARTICLE) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	VS_CB_PARTICLE p;
	ZeroMemory(&p, sizeof(VS_CB_PARTICLE));
	p.m_fAge = 0.0f;
	p.m_uiType = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;

	HR(gpCommonState->m_pd3dDevice->CreateBuffer(&vbd, &vinitData, &mInitVB));

	//
	// Create the ping-pong buffers for stream-out and drawing.
	//
	vbd.ByteWidth = sizeof(VS_CB_PARTICLE)* mMaxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	HR(gpCommonState->m_pd3dDevice->CreateBuffer(&vbd, 0, &mDrawVB));
	HR(gpCommonState->m_pd3dDevice->CreateBuffer(&vbd, 0, &mStreamOutVB));
}
