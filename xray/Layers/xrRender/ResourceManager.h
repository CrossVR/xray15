// TextureManager.h: interface for the CTextureManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ResourceManagerH
#define ResourceManagerH
#pragma once

#include	"shader.h"
#include	"tss_def.h"
#include	"TextureDescrManager.h"
// refs
struct		lua_State;

class dx10ConstantBuffer;

// defs
class ECORE_API CResourceManager
{
private:
	struct str_pred {
		IC bool operator()(LPCSTR x, LPCSTR y) const
		{	return xr_strcmp(x,y)<0;	}
	};
	struct texture_detail	{
		const char*			T;
		R_constant_setup*	cs;
	};
public:
	DEFINE_MAP_PRED(const char*,IBlender*,		map_Blender,	map_BlenderIt,		str_pred);
	DEFINE_MAP_PRED(const char*,CTexture*,		map_Texture,	map_TextureIt,		str_pred);
	DEFINE_MAP_PRED(const char*,CMatrix*,		map_Matrix,		map_MatrixIt,		str_pred);
	DEFINE_MAP_PRED(const char*,CConstant*,		map_Constant,	map_ConstantIt,		str_pred);
	DEFINE_MAP_PRED(const char*,CRT*,			map_RT,			map_RTIt,			str_pred);
	//	DX10 cut DEFINE_MAP_PRED(const char*,CRTC*,			map_RTC,		map_RTCIt,			str_pred);
	DEFINE_MAP_PRED(const char*,SVS*,			map_VS,			map_VSIt,			str_pred);
#ifdef	USE_DX10
	DEFINE_MAP_PRED(const char*,SGS*,			map_GS,			map_GSIt,			str_pred);
#endif	//	USE_DX10
	DEFINE_MAP_PRED(const char*,SPS*,			map_PS,			map_PSIt,			str_pred);
	DEFINE_MAP_PRED(const char*,texture_detail,	map_TD,			map_TDIt,			str_pred);
private:
	// data
	map_Blender											m_blenders;
	map_Texture											m_textures;
	map_Matrix											m_matrices;
	map_Constant										m_constants;
	map_RT												m_rtargets;
	//	DX10 cut map_RTC												m_rtargets_c;
	map_VS												m_vs;
	map_PS												m_ps;
#ifdef	USE_DX10
	map_GS												m_gs;
#endif	//	USE_DX10
	map_TD												m_td;

	xr_vector<SState*>									v_states;
	xr_vector<SDeclaration*>							v_declarations;
	xr_vector<SGeometry*>								v_geoms;
	xr_vector<R_constant_table*>						v_constant_tables;

#ifdef	USE_DX10
	xr_vector<dx10ConstantBuffer*>						v_constant_buffer;
	xr_vector<SInputSignature*>							v_input_signature;
#endif	//	USE_DX10

	// lists
	xr_vector<STextureList*>							lst_textures;
	xr_vector<SMatrixList*>								lst_matrices;
	xr_vector<SConstantList*>							lst_constants;

	// main shader-array
	xr_vector<SPass*>									v_passes;
	xr_vector<ShaderElement*>							v_elements;
	xr_vector<Shader*>									v_shaders;
	
	xr_vector<ref_texture>								m_necessary;
	// misc
public:
	CTextureDescrMngr									m_textures_description;
//.	CInifile*											m_textures_description;
	xr_vector<std::pair<shared_str,R_constant_setup*> >	v_constant_setup;
	lua_State*											LSVM;
	BOOL												bDeferredLoad;
private:
	void							LS_Load				();
	void							LS_Unload			();
public:
	// Miscelaneous
	void							_ParseList			(sh_list& dest, LPCSTR names);
	IBlender*						_GetBlender			(LPCSTR Name);
	IBlender* 						_FindBlender		(LPCSTR Name);
	void							_GetMemoryUsage		(u32& m_base, u32& c_base, u32& m_lmaps, u32& c_lmaps);
	void							_DumpMemoryUsage	();
//.	BOOL							_GetDetailTexture	(LPCSTR Name, LPCSTR& T, R_constant_setup* &M);

	map_Blender&					_GetBlenders		()		{	return m_blenders;	}
	const xr_vector<Shader*>&		_GetShaders() { return v_shaders; }

	// Debug
	void							DBG_VerifyGeoms		();
	void							DBG_VerifyTextures	();

	// Editor cooperation
	void							ED_UpdateBlender	(LPCSTR Name, IBlender*		data);
	void							ED_UpdateMatrix		(LPCSTR Name, CMatrix*		data);
	void							ED_UpdateConstant	(LPCSTR Name, CConstant*	data);
#ifdef _EDITOR
	void							ED_UpdateTextures	(AStringVec* names);
#endif

	// Low level resource creation
	CTexture*						_CreateTexture		(LPCSTR Name);
	void							_DeleteTexture		(const CTexture* T);

	CMatrix*						_CreateMatrix		(LPCSTR Name);
	void							_DeleteMatrix		(const CMatrix*  M);

	CConstant*						_CreateConstant		(LPCSTR Name);
	void							_DeleteConstant		(const CConstant* C);

	R_constant_table*				_CreateConstantTable(R_constant_table& C);
	void							_DeleteConstantTable(const R_constant_table* C);

#ifdef	USE_DX10
	dx10ConstantBuffer*				_CreateConstantBuffer(ID3D11ShaderReflectionConstantBuffer* pTable);
	void							_DeleteConstantBuffer(const dx10ConstantBuffer* pBuffer);

	SInputSignature*				_CreateInputSignature(ID3DBlob* pBlob);
	void							_DeleteInputSignature(const SInputSignature* pSignature);
#endif	//	USE_DX10

	CRT*							_CreateRT			(LPCSTR Name, u32 w, u32 h,	D3DFORMAT f, u32 SampleCount = 1 );
	void							_DeleteRT			(const CRT*	RT	);

	//	DX10 cut CRTC*							_CreateRTC			(LPCSTR Name, u32 size,	D3DFORMAT f);
	//	DX10 cut void							_DeleteRTC			(const CRTC*	RT	);
#ifdef	USE_DX10
	SGS*							_CreateGS			(LPCSTR Name);
	void							_DeleteGS			(const SGS*	GS	);
#endif	//	USE_DX10

	SPS*							_CreatePS			(LPCSTR Name);
	void							_DeletePS			(const SPS*	PS	);

	SVS*							_CreateVS			(LPCSTR Name);
	void							_DeleteVS			(const SVS*	VS	);

#ifdef	USE_DX10
	SPass*							_CreatePass			(ref_state& _state, ref_ps& _ps, ref_vs& _vs, ref_gs& _gs, ref_ctable& _ctable, ref_texture_list& _T, ref_matrix_list& _M, ref_constant_list& _C);
#else	//	USE_DX10
	SPass*							_CreatePass			(ref_state& _state, ref_ps& _ps, ref_vs& _vs, ref_ctable& _ctable, ref_texture_list& _T, ref_matrix_list& _M, ref_constant_list& _C);
#endif	//	USE_DX10
	void							_DeletePass			(const SPass* P	);

	// Shader compiling / optimizing
	SState*							_CreateState		(SimulatorStates& Code);
	void							_DeleteState		(const SState* SB);

	SDeclaration*					_CreateDecl			(D3DVERTEXELEMENT9* dcl);
	void							_DeleteDecl			(const SDeclaration* dcl);

	STextureList*					_CreateTextureList	(STextureList& L);
	void							_DeleteTextureList	(const STextureList* L);

	SMatrixList*					_CreateMatrixList	(SMatrixList& L);
	void							_DeleteMatrixList	(const SMatrixList* L);

	SConstantList*					_CreateConstantList	(SConstantList& L);
	void							_DeleteConstantList	(const SConstantList* L);

	ShaderElement*					_CreateElement		(ShaderElement& L);
	void							_DeleteElement		(const ShaderElement* L);

	Shader*							_cpp_Create			(LPCSTR		s_shader,	LPCSTR s_textures=0,	LPCSTR s_constants=0,	LPCSTR s_matrices=0);
	Shader*							_cpp_Create			(IBlender*	B,			LPCSTR s_shader=0,		LPCSTR s_textures=0,	LPCSTR s_constants=0, LPCSTR s_matrices=0);
	Shader*							_lua_Create			(LPCSTR		s_shader,	LPCSTR s_textures);
	BOOL							_lua_HasShader		(LPCSTR		s_shader);

	CResourceManager						()	: bDeferredLoad(TRUE){	}
	~CResourceManager						()	;

	void			OnDeviceCreate			(IReader* F);
	void			OnDeviceCreate			(LPCSTR name);
	void			OnDeviceDestroy			(BOOL   bKeepTextures);

	void			reset_begin				();
	void			reset_end				();

	// Creation/Destroying
	Shader*			Create					(LPCSTR s_shader=0, LPCSTR s_textures=0,	LPCSTR s_constants=0,	LPCSTR s_matrices=0);
	Shader*			Create					(IBlender*	B,		LPCSTR s_shader=0,		LPCSTR s_textures=0,	LPCSTR s_constants=0, LPCSTR s_matrices=0);
	void			Delete					(const Shader*		S	);
	void			RegisterConstantSetup	(LPCSTR name,		R_constant_setup* s)	{	v_constant_setup.push_back(mk_pair(shared_str(name),s));	}

	SGeometry*		CreateGeom				(D3DVERTEXELEMENT9* decl, ID3DVertexBuffer* vb, ID3DIndexBuffer* ib);
	SGeometry*		CreateGeom				(u32 FVF				, ID3DVertexBuffer* vb, ID3DIndexBuffer* ib);
	void			DeleteGeom				(const SGeometry* VS		);
	void			DeferredLoad			(BOOL E)					{ bDeferredLoad=E;	}
	void			DeferredUpload			();
//.	void			DeferredUnload			();
	void			Evict					();
	void			StoreNecessaryTextures	();
	void			DestroyNecessaryTextures();
	void			Dump					(bool bBrief);
};

#endif //ResourceManagerH
