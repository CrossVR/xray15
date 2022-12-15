#include "stdafx.h"
#include "dx10StateCache.h"

dx10StateCache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>		RSManager;
dx10StateCache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>	DSSManager;
dx10StateCache<ID3D11BlendState, D3D11_BLEND_DESC>					BSManager;

template <class IDeviceState, class StateDecs>
dx10StateCache<IDeviceState, StateDecs>
::dx10StateCache()
{
	static const int iMasRSStates = 10;
	m_StateArray.reserve(iMasRSStates);
}

template <class IDeviceState, class StateDecs>
dx10StateCache<IDeviceState, StateDecs>
::~dx10StateCache()
{
	ClearStateArray();
//	VERIFY(m_StateArray.empty());
}

/*
template <class IDeviceState, class StateDecs>
void 
dx10StateCache<IDeviceState, StateDecs>
::FlushStates()
{
	ClearStateArray();
}
*/

template <class IDeviceState, class StateDecs>
void 
dx10StateCache<IDeviceState, StateDecs>
::ClearStateArray()
{
	for (u32 i=0; i<m_StateArray.size(); ++i)
	{
		_RELEASE(m_StateArray[i].m_pState);
	}

	m_StateArray.clear_not_free();
}

template <>
void 
dx10StateCache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>
::CreateState( D3D11_RASTERIZER_DESC desc, ID3D11RasterizerState** ppIState )
{
	CHK_DX(HW.pDevice11->CreateRasterizerState( &desc, ppIState));

	//	TODO: DX10: Remove this.
#ifdef	DEBUG
	Msg("ID3D11RasterizerState #%d created.", m_StateArray.size());
#endif	//	DEBUG
}

template <>
void 
dx10StateCache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>
::CreateState( D3D11_DEPTH_STENCIL_DESC desc, ID3D11DepthStencilState** ppIState )
{
	CHK_DX(HW.pDevice11->CreateDepthStencilState( &desc, ppIState));

	//	TODO: DX10: Remove this.
#ifdef	DEBUG
	Msg("ID3D11DepthStencilState #%d created.", m_StateArray.size());
#endif	//	DEBUG
}

template <>
void 
dx10StateCache<ID3D11BlendState, D3D11_BLEND_DESC>
::CreateState( D3D11_BLEND_DESC desc, ID3D11BlendState** ppIState )
{
	CHK_DX(HW.pDevice11->CreateBlendState( &desc, ppIState));

	//	TODO: DX10: Remove this.
#ifdef	DEBUG
	Msg("ID3D11BlendState #%d created.", m_StateArray.size());
#endif	//	DEBUG
}

/*
template <>
void 
dx10StateCache<ID3D11RasterizerState, D3D11_RASTERIZER_DESC>
::ResetDescription( D3D11_RASTERIZER_DESC &desc )
{
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = FALSE;
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthClipEnable = TRUE;
	desc.ScissorEnable = FALSE;
	desc.MultisampleEnable = FALSE;
	desc.AntialiasedLineEnable = FALSE;
}

template <>
void 
dx10StateCache<ID3D11DepthStencilState, D3D11_DEPTH_STENCIL_DESC>
::ResetDescription( D3D11_DEPTH_STENCIL_DESC &desc )
{
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = TRUE;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.StencilEnable = TRUE;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;

	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
}

template <>
void 
dx10StateCache< ID3D11BlendState , D3D11_BLEND_DESC >
::ResetDescription( D3D11_BLEND_DESC &desc )
{
	ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable = FALSE;
	desc.SrcBlend = D3D11_BLEND_ONE;
	desc.DestBlend = D3D11_BLEND_ZERO;
	desc.BlendOp = D3D11_BLEND_OP_ADD;
	desc.SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

	for ( int i=0; i<8; ++i)
	{
		desc.BlendEnable[i] = FALSE;
		desc.RenderTargetWriteMask[i] = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
}
*/