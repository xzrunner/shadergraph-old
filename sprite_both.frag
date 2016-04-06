static const char* sprite_both_frag = STRINGIFY(

varying vec2 v_texcoord;
varying vec4 v_color;
varying vec4 v_additive;
varying vec4 v_rmap;
varying vec4 v_gmap;
varying vec4 v_bmap;

uniform sampler2D texture0;

\n#define neql(a, b) (step(abs((a) - (b)), 0.0001))\n

void main() {
    vec4 tmp = texture2D(texture0, v_texcoord);

    float s = 1.2;
    
	float k = tmp.r + tmp.g + tmp.b;
	
	float r_valid = step(neql(v_rmap.r, 1.0) + neql(v_rmap.g, 0.0) + neql(v_rmap.b, 0.0), 0.5);
	float cmp_gr = step(tmp.g * s, tmp.r); 
	float cmp_br = step(tmp.b * s, tmp.r); 
	vec3 dr = (v_rmap * k - tmp.rgb) * r_valid * cmp_gr * cmp_br; 

	float g_valid = step(neql(v_gmap.r, 0.0) + neql(v_gmap.g, 1.0) + neql(v_gmap.b, 0.0), 0.5);	
	float cmp_rg = step(tmp.r * s, tmp.g); 
	float cmp_bg = step(tmp.b * s, tmp.g); 
	vec3 dg = (v_gmap * k - tmp.rgb) * g_valid * cmp_rg * cmp_bg; 

	float b_valid = step(neql(v_bmap.r, 0.0) + neql(v_bmap.g, 0.0) + neql(v_bmap.b, 1.0), 0.5);	
	float cmp_rb = step(tmp.r * s, tmp.b); 
	float cmp_gb = step(tmp.g * s, tmp.b); 
	vec3 db = (v_bmap * k - tmp.rgb) * b_valid * cmp_rb * cmp_gb; 

    tmp.rgb = dr + dg + db + tmp.rgb;

    gl_FragColor.xyz = tmp.xyz * v_color.xyz;
    gl_FragColor.w = tmp.w;
    gl_FragColor *= v_color.w;
    gl_FragColor.xyz += v_additive.xyz * tmp.w * v_color.w;
}

);
