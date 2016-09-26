/**
 *  @remarks
 *    v_texcoord    varying vec2
 *    u_texture0    uniform sampler2D
 *    _DST_COL_     vec4
 */
static const char* color_grading_body = STRINGIFY(
	vec4 src_col = texture2D(u_texture0, v_texcoord);
	vec3 color = src_col.xyz;
	
	color.r /= 16;
	color.b *= 16;
	
	color.r = 1.0 / 288 + (1 - 2.0 / 18) * color.r;
	color.g = 1.0 / 18 + (1 - 2.0 / 18) * color.g;
		     
	float shift = floor(color.b);
	float start = shift / 16.0;
	color.rgb = mix(texture2D(u_col_lut, color.rg + vec2(start, 0)).rgb, texture2D(u_col_lut, color.rg + vec2(start + 0.0625, 0)).rgb, color.b - shift);
	
	vec4 _DST_COL_ = vec4(color, src_col.a);		
);
