#include "common.h"

uniform float4		screen_res;		// Screen resolution (x-Width,y-Height, zw - 1/resolution)

struct	v_TL0uv_positiont
{
	float4	P		: POSITIONT;
	float4	Color	: COLOR; 
};

struct	v2p_TL0uv
{
	float4	Color	: COLOR;
	float4 	HPos	: SV_Position;	// Clip-space position 	(for rasterization)
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_TL0uv main ( v_TL0uv_positiont I )
{
	v2p_TL0uv O;

	{
		I.P.xy += 0.5f;
//		O.HPos.x = I.P.x/1024 * 2 - 1;
//		O.HPos.y = (I.P.y/768 * 2 - 1)*-1;
		O.HPos.x = I.P.x * screen_res.z * 2 - 1;
		O.HPos.y = (I.P.y * screen_res.w * 2 - 1)*-1;
		O.HPos.zw = I.P.zw;
	}

	O.Color = I.Color;

 	return O;
}