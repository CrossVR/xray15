#include "stdafx.h"


#include "../xrRender/r_constants_cache.h"

dx10ConstantBuffer& R_constants::GetCBuffer(R_constant* C, BufferType BType)
{
	if (BType==BT_PixelBuffer)
	{
		//	Decode index
		int iBufferIndex = (C->destination&RC_dest_pixel_cb_index_mask)>>RC_dest_pixel_cb_index_shift;

		VERIFY(iBufferIndex<CBackend::MaxCBuffers);
		VERIFY(RCache.m_aPixelConstants[iBufferIndex]);
		return *RCache.m_aPixelConstants[iBufferIndex];
	}
	else if (BType==BT_VertexBuffer)
	{
		//	Decode index
		int iBufferIndex = (C->destination&RC_dest_vertex_cb_index_mask)>>RC_dest_vertex_cb_index_shift;

		VERIFY(iBufferIndex<CBackend::MaxCBuffers);
		VERIFY(RCache.m_aVertexConstants[iBufferIndex]);
		return *RCache.m_aVertexConstants[iBufferIndex];
	}
	else //if (BType==BT_GeometryBuffer)
	{
		//	Decode index
		int iBufferIndex = (C->destination&RC_dest_geometry_cb_index_mask)>>RC_dest_geometry_cb_index_shift;

		VERIFY(iBufferIndex<CBackend::MaxCBuffers);
		VERIFY(RCache.m_aGeometryConstants[iBufferIndex]);
		return *RCache.m_aGeometryConstants[iBufferIndex];
	}
}

void R_constants::flush_cache()
{
	for (int i=0; i < CBackend::MaxCBuffers; ++i)
	{
		if (RCache.m_aVertexConstants[i])
			RCache.m_aVertexConstants[i]->Flush();

		if (RCache.m_aPixelConstants[i])
			RCache.m_aPixelConstants[i]->Flush();

		if (RCache.m_aGeometryConstants[i])
			RCache.m_aGeometryConstants[i]->Flush();
	}
}

/*
void R_constants::flush_cache()
{
	if (a_pixel.b_dirty)
	{
		// fp
		R_constant_array::t_f&	F	= a_pixel.c_f;
		{
			//if (F.r_lo() <= 32) //. hack
			{		
				void	*pBuffer;
				const int iVectorElements = 4;
				const int iVectorNumber = 256;
				RCache.m_pPixelConstants->Map(D3D11_MAP_WRITE_DISCARD, 0, &pBuffer);
				CopyMemory(pBuffer, F.access(0), iVectorNumber*iVectorElements*sizeof(float));
				RCache.m_pPixelConstants->Unmap();
			}
		}
		a_pixel.b_dirty		= false;
	}
	if (a_vertex.b_dirty)
	{
		// fp
		R_constant_array::t_f&	F	= a_vertex.c_f;
		{
			u32		count		= F.r_hi()-F.r_lo();
			if (count)			{
#ifdef DEBUG
				if (F.r_hi() > HW.Caps.geometry.dwRegisters)
				{
					Debug.fatal(DEBUG_INFO,"Internal error setting VS-constants: overflow\nregs[%d],hi[%d]",
						HW.Caps.geometry.dwRegisters,F.r_hi()
						);
				}
				PGO		(Msg("PGO:V_CONST:%d",count));
#endif
				{		
					void	*pBuffer;
					const int iVectorElements = 4;
					const int iVectorNumber = 256;
					RCache.m_pVertexConstants->Map(D3D11_MAP_WRITE_DISCARD, 0, &pBuffer);
					CopyMemory(pBuffer, F.access(0), iVectorNumber*iVectorElements*sizeof(float));
					RCache.m_pVertexConstants->Unmap();
				}
				F.flush	();
			}
		}
		a_vertex.b_dirty	= false;
	}
}
*/