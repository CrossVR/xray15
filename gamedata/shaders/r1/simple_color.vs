#include "common.h"

struct v2p
{
 	float2 tc0	: TEXCOORD0	;
	float4 C 	: COLOR		;
	float4 hpos	: POSITION	;
};

struct	v_simple_color
{
	float4 P	: POSITION	;
	float2 tc0	: TEXCOORD0	;
	float4 C	: COLOR		; 
};

v2p main (v_simple_color I)
{
	v2p 		o;

	o.hpos 		= mul			(m_WVP, I.P);			// xform, input in world coords
	o.tc0		= I.tc0;
	o.C 		= I.C;

	return o;
}
