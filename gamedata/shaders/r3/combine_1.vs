#include "common.h"

struct 	_in
{
	float4	P	: POSITIONT;	// xy=pos, zw=tc0
	float2	tcJ	: TEXCOORD0;	// jitter coords
};

struct 	v2p
{
#ifdef USE_VTF
	float4	tc0	: TEXCOORD0;	// tc.xy, tc.w = tonemap scale
#else
	float2	tc0	: TEXCOORD0;	// tc.xy
#endif
	float2	tcJ	: TEXCOORD1;	// jitter coords
	float4	hpos: SV_Position;
};
//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p main ( _in I )
{
	v2p 		O;
	O.hpos 		= float4	(I.P.x, -I.P.y, 0, 1);
#ifdef USE_VTF
	float  	scale 	= s_tonemap.Load(int3(0,0,0)).x;
	O.tc0		= float4	(I.P.zw, scale, scale);
#else
	O.tc0		= I.P.zw	;
#endif
	O.tcJ		= I.tcJ;
	return	O;
}

FXVS;
