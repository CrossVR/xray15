// xrRender_R2.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "../xrRender/dxRenderFactory.h"
#include "../xrRender/dxUIRender.h"
#include "../xrRender/dxDebugRender.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH	:
		//	Can't call CreateDXGIFactory from DllMain
		//if (!xrRender_test_hw())	return FALSE;
		::Render					= &RImplementation;
		::RenderFactory				= &RenderFactoryImpl;
		::DU						= &DUImpl;
		//::vid_mode_token			= inited by HW;
		UIRender					= &UIRenderImpl;
#ifdef DEBUG
		DRender						= &DebugRenderImpl;
#endif	//	DEBUG
		xrRender_initconsole		();
		break	;
	case DLL_THREAD_ATTACH	:
	case DLL_THREAD_DETACH	:
	case DLL_PROCESS_DETACH	:
		break;
	}
	return TRUE;
}


extern "C"
{
	bool _declspec(dllexport) SupportsLevel93Rendering();
	bool _declspec(dllexport) SupportsAdvancedRendering();
	bool _declspec(dllexport) SupportsDX10Rendering();
};

bool _declspec(dllexport) SupportsLevel93Rendering()
{
	return xrRender_test_hw() ? true : false;
}

bool _declspec(dllexport) SupportsAdvancedRendering()
{
	D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT d3d9ShadowSupportResults;
	ZeroMemory(&d3d9ShadowSupportResults, sizeof(D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT));

	CHW							_HW;
	_HW.CreateD3D();
	_HW.pDevice11->CheckFeatureSupport(
		D3D11_FEATURE_D3D9_SHADOW_SUPPORT,
		&d3d9ShadowSupportResults,
		sizeof(D3D11_FEATURE_DATA_D3D9_SHADOW_SUPPORT)
	);
	_HW.DestroyD3D();

	return HW.m_FeatureLevel >= D3D10_FEATURE_LEVEL_10_0 || d3d9ShadowSupportResults.SupportsDepthAsTextureWithLessEqualComparisonFilter;
}

bool _declspec(dllexport) SupportsDX10Rendering()
{
	CHW							_HW;
	_HW.CreateD3D();
	_HW.DestroyD3D();
	return	_HW.m_FeatureLevel >= D3D_FEATURE_LEVEL_10_0;
}