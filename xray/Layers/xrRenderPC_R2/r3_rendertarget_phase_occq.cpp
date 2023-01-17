#include "stdafx.h"

void	CRenderTarget::phase_occq	()
{
	u_setrt						(NULL,NULL,NULL,rt_Depth);
	RCache.set_Shader			( s_occq	);
	RCache.set_CullMode			( CULL_CCW	);
	RCache.set_Stencil			(TRUE,D3DCMP_LESSEQUAL,0x01,0xff,0x00);
	RCache.set_ColorWriteEnable	(FALSE		);
}
