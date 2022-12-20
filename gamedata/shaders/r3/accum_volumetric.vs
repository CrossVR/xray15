#include "common.h"

cbuffer VolumetricLights
{
	float3	vMinBounds;
	float3	vMaxBounds;
	float4	FrustumClipPlane[6];
}

struct v2p
{
	float3 	lightToPos	: TEXCOORD0;		// light center to plane vector
	float3 	vPos		: TEXCOORD1;		// position in camera space
	float 	fDensity	: TEXCOORD2;		// plane density alon Z axis
//	float2	tNoise 		: TEXCOORD3;		// projective noise
	float4 	hpos		: SV_Position;
};

[clipplanes(FrustumClipPlane[0],
			FrustumClipPlane[1],
			FrustumClipPlane[2],
			FrustumClipPlane[3],
			FrustumClipPlane[4],
			FrustumClipPlane[5])]
v2p main ( float3 P : POSITION )
{
	v2p 		o;
	float4	vPos;
	vPos.xyz 	= lerp( vMinBounds, vMaxBounds, P);	//	Position in camera space
	vPos.w 		= 1;
	o.hpos 		= mul			(m_P, vPos);		// xform, input in camera coordinates

	o.lightToPos = vPos.xyz - Ldynamic_pos.xyz;
	o.vPos = vPos;

//	o.fDensity = (vMaxBounds.z-vMinBounds.z)/2000.0h;
//	o.fDensity = (vMaxBounds.z-vMinBounds.z)/2000.0h*2;
	o.fDensity = 1.0h/40.0h;
//	o.fDensity = 1.0h/20.0h;

	return o;
}