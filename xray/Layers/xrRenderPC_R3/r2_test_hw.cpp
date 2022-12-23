#include "stdafx.h"

BOOL	xrRender_test_hw		()
{
	CHW							_HW;
	_HW.CreateD3D				()		;
	_HW.DestroyD3D				()		;
	return	_HW.m_FeatureLevel >= D3D_FEATURE_LEVEL_9_3;
}
