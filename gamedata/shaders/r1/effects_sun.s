function normal(shader, t_base, t_second, t_detail)
	shader:begin("effects_sun","vert")
--        : sorting        (2, false)
--        : blend                (true,blend.srcalpha,blend.invsrcalpha)
--        : blend                (true,blend.one,blend.one)
        : blend                (true,blend.srcalpha,blend.one)
		: zb                (true,false)
	shader:sampler        ("s_base")       :texture  (t_base)
end