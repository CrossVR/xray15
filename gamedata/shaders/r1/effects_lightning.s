function normal		(shader, t_base, t_second, t_detail)
	shader	: begin	("simple_color","simple_color")
			: zb	(true,false)
			: blend	(true,blend.srcalpha,blend.one)
			: aref 		(true,2)
	shader:sampler        ("s_base")       :texture  (t_base)
end