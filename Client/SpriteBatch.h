#ifndef SPRITEBATCH_H_
#define SPRITEBATCH_H_

#include "Object.h"
#include <DirectXMath.h>
#include <DirectXColors.h>



class CSpriteBatch
{
public:
	CSpriteBatch();
	CSpriteBatch(CSpriteBatch&& ref) {}
	~CSpriteBatch();

	enum SpriteSortMode
	{
		SpriteSortMode_Deferred,
		SpriteSortMode_Immediate,
		SpriteSortMode_Texture,
		SpriteSortMode_BackToFront,
		SpriteSortMode_FrontToBack,
	};
	enum SpriteEffects
	{
		SpriteEffects_None = 0,
		SpriteEffects_FlipHorizontally = 1,
		SpriteEffects_FlipVertically = 2,
		SpriteEffects_FlipBoth = SpriteEffects_FlipHorizontally | SpriteEffects_FlipVertically,
	};

	void Initialize(ID3D11DeviceContext *pd3dDeviceContext);
	void XM_CALLCONV Begin(SpriteSortMode sortMode = SpriteSortMode_Deferred, _In_opt_ ID3D11BlendState* blendState = nullptr, _In_opt_ ID3D11SamplerState* samplerState = nullptr, _In_opt_ ID3D11DepthStencilState* depthStencilState = nullptr, _In_opt_ ID3D11RasterizerState* rasterizerState = nullptr,
		_In_opt_ std::function<void __cdecl()> setCustomShaders = nullptr, DirectX::FXMMATRIX transformMatrix = DirectX::XMMatrixIdentity());

	/* The Draw method has many overloads with parameters controlling:

	1) Specify screen position as XMFLOAT2, XMVECTOR or RECT
	2) Optional source rectangle for drawing just part of a sprite sheet
	3) Tint color
	4) Rotation (in radians)
	5) Origin point (position, scaling and rotation are relative to this)
	6) Scale
	7) SpriteEffects enum (for horizontal or vertical mirroring)
	8) Layer depth (for sorting)
	*/
	// Draw overloads specifying position, origin and scale as XMFLOAT2.
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, XMFLOAT2 const& position, FXMVECTOR color = DirectX::Colors::White);
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, XMFLOAT2 const& position, _In_opt_ RECT const* sourceRectangle, FXMVECTOR color = DirectX::Colors::White, float rotation = 0, XMFLOAT2 const& origin = XMFLOAT2(0, 0), float scale = 1, SpriteEffects effects = SpriteEffects_None, float layerDepth = 0);
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, XMFLOAT2 const& position, _In_opt_ RECT const* sourceRectangle, FXMVECTOR color, float rotation, XMFLOAT2 const& origin, XMFLOAT2 const& scale, SpriteEffects effects = SpriteEffects_None, float layerDepth = 0);
	// Draw overloads specifying position, origin and scale via the first two components of an XMVECTOR.
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, FXMVECTOR position, FXMVECTOR color = DirectX::Colors::White);
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, FXMVECTOR position, _In_opt_ RECT const* sourceRectangle, FXMVECTOR color = DirectX::Colors::White, float rotation = 0, FXMVECTOR origin = g_XMZero, float scale = 1, SpriteEffects effects = SpriteEffects_None, float layerDepth = 0);
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, FXMVECTOR position, _In_opt_ RECT const* sourceRectangle, FXMVECTOR color, float rotation, FXMVECTOR origin, DirectX::GXMVECTOR scale, SpriteEffects effects = SpriteEffects_None, float layerDepth = 0);
	// Draw overloads specifying position as a RECT.
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, RECT const& destinationRectangle, FXMVECTOR color = DirectX::Colors::White);
	void XM_CALLCONV Draw(_In_ ID3D11ShaderResourceView* texture, RECT const& destinationRectangle, _In_opt_ RECT const* sourceRectangle, FXMVECTOR color = DirectX::Colors::White, float rotation = 0, XMFLOAT2 const& origin = XMFLOAT2(0, 0), SpriteEffects effects = SpriteEffects_None, float layerDepth = 0);

	void End();

protected:
private:
	void XM_CALLCONV _Begin(SpriteSortMode sortMode, _In_opt_ ID3D11BlendState* blendState,	_In_opt_ ID3D11SamplerState* samplerState,
		_In_opt_ ID3D11DepthStencilState* depthStencilState, _In_opt_ ID3D11RasterizerState* rasterizerState,
		_In_opt_ std::function<void()> setCustomShaders, DirectX::FXMMATRIX transformMatrix);

	void _Draw(_In_ ID3D11ShaderResourceView* texture, FXMVECTOR destination, _In_opt_ RECT const* sourceRectangle,
		FXMVECTOR color, FXMVECTOR originRotationDepth, int flags);
};

//std::unique_ptr<CSpriteBatch> spriteBatch = std::make_unique<CSpriteBatch>();
//spriteBatch->Initialize(devicecontext);


#endif