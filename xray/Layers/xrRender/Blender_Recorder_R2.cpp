#include "stdafx.h"


#include "ResourceManager.h"
#include "blenders\Blender_Recorder.h"
#include "blenders\Blender.h"

#include "dxRenderDeviceRender.h"

void fix_texture_name(LPSTR fn);

void	CBlender_Compile::r_Pass		(LPCSTR _vs, LPCSTR _ps, bool bFog, BOOL bZtest, BOOL bZwrite,	BOOL bABlend, D3DBLEND abSRC, D3DBLEND abDST, BOOL aTest, u32 aRef)
{
	RS.Invalidate			();
	ctable.clear			();
	passTextures.clear		();
	passMatrices.clear		();
	passConstants.clear		();
	dwStage					= 0;

	// Setup FF-units (Z-buffer, blender)
	PassSET_ZB				(bZtest,bZwrite);
	PassSET_Blend			(bABlend,abSRC,abDST,aTest,aRef);
	PassSET_LightFog		(FALSE,bFog);

	// Create shaders
	SPS* ps					= DEV->_CreatePS			(_ps);
	SVS* vs					= DEV->_CreateVS			(_vs);
	dest.ps					= ps;
	dest.vs					= vs;
#ifdef	USE_DX10
	SGS* gs					= DEV->_CreateGS			("null");
	dest.gs					= gs;
#endif	//	USE_DX10
	ctable.merge			(&ps->constants);
	ctable.merge			(&vs->constants);
	SetMapping				();

	// Last Stage - disable
	if (0==stricmp(_ps,"null"))	{
		RS.SetTSS				(0,D3DTSS_COLOROP,D3DTOP_DISABLE);
		RS.SetTSS				(0,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
	}
}

void	CBlender_Compile::r_Constant	(LPCSTR name, R_constant_setup* s)
{
	R_ASSERT				(s);
	ref_constant C			= ctable.get(name);
	if (C)					C->handler	= s;
}

void CBlender_Compile::r_ColorWriteEnable( bool cR, bool cG, bool cB, bool cA)
{
	BYTE	Mask = 0;
	Mask |= cR ? D3DCOLORWRITEENABLE_RED : 0;
	Mask |= cG ? D3DCOLORWRITEENABLE_GREEN : 0;
	Mask |= cB ? D3DCOLORWRITEENABLE_BLUE : 0;
	Mask |= cA ? D3DCOLORWRITEENABLE_ALPHA : 0;

	RS.SetRS( D3DRS_COLORWRITEENABLE, Mask);
	RS.SetRS( D3DRS_COLORWRITEENABLE1, Mask);
	RS.SetRS( D3DRS_COLORWRITEENABLE2, Mask);
	RS.SetRS( D3DRS_COLORWRITEENABLE3, Mask);
}

#if	RENDER != R_R3
ref_constant CBlender_Compile::i_Sampler(LPCSTR _name)
{
	//
	string256				name;
	strcpy_s					(name,_name);
//. andy	if (strext(name)) *strext(name)=0;
	fix_texture_name		(name);

	// Find index
	ref_constant C			= ctable.get(name);
	R_ASSERT				(!C || C->type == RC_sampler);

	// Create texture
	// while (stage>=passTextures.size())	passTextures.push_back		(NULL);
	return					C;
}
void	CBlender_Compile::i_Texture		(u32 s, LPCSTR name)
{
	VERIFY(s != u16(-1));
	if (name && s!=u16(-1))	passTextures.push_back	(mk_pair(s, ref_texture(DEV->_CreateTexture(name))));
}
void	CBlender_Compile::i_Projective	(u32 s, bool b)
{
	if	(b)	RS.SetTSS	(s,D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE | D3DTTFF_PROJECTED);
	else	RS.SetTSS	(s,D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
}
void	CBlender_Compile::i_Address		(u32 s, u32	address)
{
	RS.SetSAMP			(s,D3DSAMP_ADDRESSU,	address);
	RS.SetSAMP			(s,D3DSAMP_ADDRESSV,	address);
	RS.SetSAMP			(s,D3DSAMP_ADDRESSW,	address);
}
void	CBlender_Compile::i_Filter_Min		(u32 s, u32	f)
{
	RS.SetSAMP			(s,D3DSAMP_MINFILTER,	f);
}
void	CBlender_Compile::i_Filter_Mip		(u32 s, u32	f)
{
	RS.SetSAMP			(s,D3DSAMP_MIPFILTER,	f);
}
void	CBlender_Compile::i_Filter_Mag		(u32 s, u32	f)
{
	RS.SetSAMP			(s,D3DSAMP_MAGFILTER,	f);
}
void	CBlender_Compile::i_Filter			(u32 s, u32 _min, u32 _mip, u32 _mag)
{
	i_Filter_Min	(s,_min);
	i_Filter_Mip	(s,_mip);
	i_Filter_Mag	(s,_mag);
}
ref_constant		CBlender_Compile::r_Sampler		(LPCSTR _name, LPCSTR texture, bool b_ps1x_ProjectiveDivide, u32 address, u32 fmin, u32 fmip, u32 fmag)
{
	ref_constant sampler = i_Sampler(_name);
	dwStage = sampler ? sampler->samp.index : u32(-1);
	if (u32(-1)!=dwStage)
	{
#ifdef USE_DX10
		i_Texture				(sampler->tex.index,texture);
#else // USE_DX10
		i_Texture				(sampler->samp.index,texture);
#endif // USE_DX10

		// force ANISO-TF for "s_base"
#ifdef USE_DX10
		if ((0==xr_strcmp(_name,"s_base")) && (fmin==D3DTEXF_LINEAR))	{ i_dx10FilterAnizo(dwStage,TRUE); }
#else // USE_DX10
		if ((0==xr_strcmp(_name,"s_base")) && (fmin==D3DTEXF_LINEAR))	{ fmin = D3DTEXF_ANISOTROPIC; fmag=D3DTEXF_ANISOTROPIC; }
#endif // USE_DX10
		
		if ( 0==xr_strcmp(_name,"s_base_hud") )
		{
			fmin	= D3DTEXF_GAUSSIANQUAD; //D3DTEXF_PYRAMIDALQUAD; //D3DTEXF_ANISOTROPIC; //D3DTEXF_LINEAR; //D3DTEXF_POINT; //D3DTEXF_NONE
			fmag	= D3DTEXF_GAUSSIANQUAD; //D3DTEXF_PYRAMIDALQUAD; //D3DTEXF_ANISOTROPIC; //D3DTEXF_LINEAR; //D3DTEXF_POINT; //D3DTEXF_NONE; 
		}

#ifdef USE_DX10
		if ((0==xr_strcmp(_name,"s_detail")) && (fmin==D3DTEXF_LINEAR))	{ i_dx10FilterAnizo(dwStage,TRUE); }
#else // USE_DX10
		if ((0==xr_strcmp(_name,"s_detail")) && (fmin==D3DTEXF_LINEAR))	{ fmin = D3DTEXF_ANISOTROPIC; fmag=D3DTEXF_ANISOTROPIC; }
#endif // USE_DX10

		// Sampler states
		i_Address				(dwStage,address);
		i_Filter				(dwStage,fmin,fmip,fmag);
		//.i_Filter				(dwStage,D3DTEXF_POINT,D3DTEXF_POINT,D3DTEXF_POINT); // show pixels
		if (dwStage<4)			i_Projective		(dwStage,b_ps1x_ProjectiveDivide);
	}
	return	sampler;
}
	
void	CBlender_Compile::r_Sampler_rtf	(LPCSTR name, LPCSTR texture, bool b_ps1x_ProjectiveDivide)
{
	r_Sampler	(name,texture,b_ps1x_ProjectiveDivide,D3DTADDRESS_CLAMP,D3DTEXF_POINT,D3DTEXF_NONE,D3DTEXF_POINT);
}
void	CBlender_Compile::r_Sampler_clf	(LPCSTR name, LPCSTR texture, bool b_ps1x_ProjectiveDivide)
{
	r_Sampler	(name,texture,b_ps1x_ProjectiveDivide,D3DTADDRESS_CLAMP,D3DTEXF_LINEAR,D3DTEXF_NONE,D3DTEXF_LINEAR);
}
void	CBlender_Compile::r_Sampler_clw	(LPCSTR name, LPCSTR texture, bool b_ps1x_ProjectiveDivide)
{
	ref_constant s	= r_Sampler	(name,texture,b_ps1x_ProjectiveDivide,D3DTADDRESS_CLAMP,D3DTEXF_LINEAR,D3DTEXF_NONE,D3DTEXF_LINEAR);
	if (s)			RS.SetSAMP	(s->samp.index,D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
}

#ifndef USE_DX10
void	CBlender_Compile::r_End			()
{
	dest.constants			= DEV->_CreateConstantTable(ctable);
	dest.state				= DEV->_CreateState		(RS.GetContainer());
	dest.T					= DEV->_CreateTextureList	(passTextures);
	dest.C					= 0;
#ifdef _EDITOR
	dest.M					= 0;
	SH->passes.push_back	(DEV->_CreatePass(dest.state,dest.ps,dest.vs,dest.constants,dest.T,dest.M,dest.C));
#else
	ref_matrix_list			temp(0);
	SH->passes.push_back	(DEV->_CreatePass(dest.state,dest.ps,dest.vs,dest.constants,dest.T,temp,dest.C));
#endif
}
#endif

#endif	//	RENDER != R_R3