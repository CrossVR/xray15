struct	v_blur
{
	float4	P		: POSITIONT;
	float2	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
};

struct	v2p_blur
{
	float2 	Tex0	: TEXCOORD0;
	float2	Tex1	: TEXCOORD1;
	float4 	HPos	: POSITION;
};

//////////////////////////////////////////////////////////////////////////////////////////
// Vertex
v2p_blur main ( v_blur I )
{
	v2p_blur O;

	O.HPos = I.P;
	O.Tex0 = I.Tex0;
	O.Tex1 = I.Tex1;

 	return O;
}