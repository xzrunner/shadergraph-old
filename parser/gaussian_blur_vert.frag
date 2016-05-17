// code from http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/

static const char* gaussian_blur_vert_header = STRINGIFY(

	float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
	float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );
	
);

static const char* gaussian_blur_vert_body = STRINGIFY(

	vec4 tmp = texture2D(u_texture0, v_texcoord);
	vec2 tex_size = textureSize(u_texture0, 0);	
	vec3 tc = tmp.rgb * weight[0];
	for (int i = 1; i < 3; i++) 
	{
		tc += texture2D(u_texture0, v_texcoord + vec2(0.0, offset[i]) / tex_size.y).rgb * weight[i];
		tc += texture2D(u_texture0, v_texcoord - vec2(0.0, offset[i]) / tex_size.y).rgb * weight[i];
	}
	vec4 _DST_COL_ = vec4(tc, tmp.a);

);
