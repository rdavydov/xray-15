#ifndef	glR_Backend_Runtime_included
#define	glR_Backend_Runtime_included
#pragma once

IC void		CBackend::set_xform(u32 ID, const Fmatrix& M)
{
	stat.xforms++;
	//	TODO: OGL: Implement CBackend::set_xform
	//VERIFY(!"Implement CBackend::set_xform");
}

IC void CBackend::set_RT(GLuint RT, u32 ID)
{
	if (RT != pRT[ID])
	{
		PGO(Msg("PGO:setRT"));
		stat.target_rt++;
		pRT[ID] = RT;
		// TODO: Set render target
	}
}

IC void	CBackend::set_ZB(GLuint ZB)
{
	if (ZB != pZB)
	{
		PGO(Msg("PGO:setZB"));
		stat.target_zb++;
		pZB = ZB;
		// TODO: Set Z buffer
	}
}

ICF void CBackend::set_Format(SDeclaration* _decl)
{
	if (decl != _decl)
	{
		PGO(Msg("PGO:v_format:%x", _decl));
#ifdef DEBUG
		stat.decl++;
#endif
		decl = _decl;
		// TODO: Set vertex format
	}
}

ICF void CBackend::set_PS(GLuint _ps, LPCSTR _n)
{
	if (ps != _ps)
	{
		PGO(Msg("PGO:Pshader:%x", _ps));
		stat.ps++;
		ps = _ps;
		// TODO: Set pixel shader
#ifdef DEBUG
		ps_name = _n;
#endif
	}
}

ICF void CBackend::set_VS(GLuint _vs, LPCSTR _n)
{
	if (vs != _vs)
	{
		PGO(Msg("PGO:Vshader:%x", _vs));
		stat.vs++;
		vs = _vs;
		// TODO: Set vertex shader
#ifdef DEBUG
		vs_name = _n;
#endif
	}
}

ICF void CBackend::set_Vertices(GLuint _vb, u32 _vb_stride)
{
	if ((vb != _vb) || (vb_stride != _vb_stride))
	{
		PGO(Msg("PGO:VB:%x,%d", _vb, _vb_stride));
#ifdef DEBUG
		stat.vb++;
#endif
		vb = _vb;
		vb_stride = _vb_stride;
		// TODO: Set vertex buffer
	}
}

ICF void CBackend::set_Indices(GLuint _ib)
{
	if (ib != _ib)
	{
		PGO(Msg("PGO:IB:%x", _ib));
#ifdef DEBUG
		stat.ib++;
#endif
		ib = _ib;
		// TODO: Set index buffer
	}
}

ICF void CBackend::Render(u32 T, u32 baseV, u32 startV, u32 countV, u32 startI, u32 PC)
{
	//Fix D3D ERROR
	if (PC == 0)
		return;

	stat.calls++;
	stat.verts += countV;
	stat.polys += PC;
	constants.flush();
	// TODO: Draw vertices
	PGO(Msg("PGO:DIP:%dv/%df", countV, PC));
}

ICF void CBackend::Render(u32 T, u32 startV, u32 PC)
{
	//Fix D3D ERROR
	if (PC == 0)
		return;

	stat.calls++;
	stat.verts += 3 * PC;
	stat.polys += PC;
	constants.flush();
	// TODO: Draw vertices
	PGO(Msg("PGO:DIP:%dv/%df", 3 * PC, PC));
}

IC void CBackend::set_Geometry(SGeometry* _geom)
{
	set_Format(&*_geom->dcl);
	set_Vertices(_geom->vb, _geom->vb_stride);
	set_Indices(_geom->ib);
}

IC void	CBackend::set_Scissor(Irect*	R)
{
	// TODO: Implement this
	VERIFY(!"CBackend::set_Scissor not implemented.");
}

IC void CBackend::set_Stencil(u32 _enable, u32 _func, u32 _ref, u32 _mask, u32 _writemask, u32 _fail, u32 _pass, u32 _zfail)
{
	// TODO: Implement this
	VERIFY(!"CBackend::set_Stencil not implemented.");
}

IC  void CBackend::set_Z(u32 _enable)
{
	if (z_enable != _enable)
	{
		z_enable = _enable;
		// TODO: Set depth state
		VERIFY(!"CBackend::set_Z not implemented.");
	}
}

IC  void CBackend::set_ZFunc(u32 _func)
{
	if (z_func != _func)
	{
		z_func = _func;
		// TODO: Set depth function
		VERIFY(!"CBackend::set_ZFunc not implemented.");
	}
}

IC  void CBackend::set_AlphaRef(u32 _value)
{
	if (alpha_ref != _value)
	{
		alpha_ref = _value;
		// TODO: Set alpha reference
		VERIFY(!"CBackend::set_AlphaRef not implemented.");
	}
}

IC void	CBackend::set_ColorWriteEnable(u32 _mask)
{
	if (colorwrite_mask != _mask)		{
		colorwrite_mask = _mask;
		// TODO: Set color mask
		VERIFY(!"CBackend::set_ColorWriteEnable not implemented.");
	}
}

ICF void	CBackend::set_CullMode(u32 _mode)
{
	if (cull_mode != _mode)		{
		cull_mode = _mode;
		// TODO: Set cull mode
		VERIFY(!"CBackend::set_CullMode not implemented.");
	}
}

ICF void CBackend::set_VS(ref_vs& _vs)
{
	set_VS(_vs->vs, _vs->cName.c_str());
}

IC void CBackend::set_Constants(R_constant_table* C)
{
	// caching
	if (ctable == C)	return;
	ctable = C;
	xforms.unmap();
	hemi.unmap();
	tree.unmap();
	if (0 == C)		return;

	PGO(Msg("PGO:c-table"));

	// process constant-loaders
	R_constant_table::c_table::iterator	it = C->table.begin();
	R_constant_table::c_table::iterator	end = C->table.end();
	for (; it != end; it++)	{
		R_constant*		Cs = &**it;
		if (Cs->handler)	Cs->handler->setup(Cs);
	}
}

#endif	//	glR_Backend_Runtime_included