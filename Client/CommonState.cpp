#include "stdafx.h"
#include "CommonState.h"

CCommonState *gpCommonState;





//
//	Common State
//
CCommonState::CCommonState()
{
	m_pd3dDevice = nullptr;
	m_pd3dDeviceContext = nullptr;

	m_pDXGISwapChain = nullptr;

	m_pd3dDepthStencilDefault = nullptr;
	m_pd3dDepthStencilDisable = nullptr;

	m_pTimer = new CGameTimer();
}
CCommonState::~CCommonState()
{
	//Destroy();
}

void CCommonState::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	if (!_CreateDevices())
	{
		cout << "creating device & device context failed" << endl;
	}

	if (!_CreateDepthStencilState())
	{
		cout << "depth stencil state initializing failed" << endl;
	}

	if (!_CreateBlendState())
	{
		cout << "blend state initializing failed" << endl;
	}
}

//void CCommonState::Destroy()
//{
//	if (m_pd3dDepthStencilDefault) m_pd3dDepthStencilDefault->Release();
//	if (m_pd3dDepthStencilDisable) m_pd3dDepthStencilDisable->Release();
//	if (m_pd3dDeviceContext)
//	{
//		m_pd3dDeviceContext->ClearState();
//		m_pd3dDeviceContext->Release();
//	}
//	if (m_pd3dDevice) m_pd3dDevice->Release();
//}

void CCommonState::TurnZBufferOn()
{
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilDefault, 1);
	return;
}

void CCommonState::TurnZBufferOff()
{
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilDisable, 1);
	return;
}

void CCommonState::EnableAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_pd3dDeviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
}
void CCommonState::DisableAlphaBlending()
{
	float blendFactor[4];

	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_pd3dDeviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}

bool CCommonState::_CreateDevices()
{
	UINT dwCreateDeviceFlags = 0;

	// 디바이스를 생성하기 위하여 시도할 드라이버 유형의 순서
	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT nDriverTypes = sizeof(d3dDriverTypes) / sizeof(D3D10_DRIVER_TYPE);

	//디바이스를 생성하기 위하여 시도할 특성 레벨의 순서
	D3D_FEATURE_LEVEL d3dFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT nFeatureLevels = sizeof(d3dFeatureLevels) / sizeof(D3D_FEATURE_LEVEL);

	// 생성할 스왑 체인을 서술하는 구조체
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	dxgiSwapChainDesc.BufferCount = 1;
	dxgiSwapChainDesc.BufferDesc.Width = FRAME_BUFFER_WIDTH;
	dxgiSwapChainDesc.BufferDesc.Height = FRAME_BUFFER_HEIGHT;
	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dxgiSwapChainDesc.OutputWindow = m_hWnd;
	dxgiSwapChainDesc.SampleDesc.Count = 1;
	dxgiSwapChainDesc.SampleDesc.Quality = 0;
	dxgiSwapChainDesc.Windowed = TRUE;

	D3D_DRIVER_TYPE nd3dDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	// 디바이스의 드라이버 유형과 특성 레벨을 지원하는 디바이스와 스왑 체인을 생성
	// 고수준의 디바이스 생성을 시도하고 실패하면 다음 수준의 디바이스를 생성
	for (UINT i = 0; i < nDriverTypes; i++)
	{
		nd3dDriverType = d3dDriverTypes[i];
		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(NULL, nd3dDriverType, NULL,
			dwCreateDeviceFlags, d3dFeatureLevels, nFeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc,
			&m_pDXGISwapChain, &m_pd3dDevice, &nd3dFeatureLevel, &m_pd3dDeviceContext)))
			break;
	}

	if (!m_pDXGISwapChain || !m_pd3dDevice || !m_pd3dDeviceContext)
	{
		return false;
	}

	return true;
}

bool CCommonState::_CreateDepthStencilState()
{
	// Create first depth stencil state(using depth buffer)
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	if (FAILED(m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pd3dDepthStencilDefault)))
	{
		return FALSE;
	}
	m_pd3dDeviceContext->OMSetDepthStencilState(m_pd3dDepthStencilDefault, 1);

	// Create second depth stencil state(unusing depth buffer)
	depthStencilDesc.DepthEnable = false;	/// <<<	only defference from other depth stencil view
	if (FAILED(m_pd3dDevice->CreateDepthStencilState(&depthStencilDesc, &m_pd3dDepthStencilDisable)))
	{
		return FALSE;
	}

	return TRUE;
}

bool CCommonState::_CreateBlendState()
{
	// Initialize Alpha Enabled Blend State & Alpha Disabled Blend State
	D3D11_BLEND_DESC blendStateDescription;

	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	if (FAILED(m_pd3dDevice->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState)))
	{
		return FALSE;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	if (FAILED(m_pd3dDevice->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState)))
	{
		return FALSE;
	}

	return TRUE;
}
