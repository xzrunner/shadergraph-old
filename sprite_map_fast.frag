static const char* sprite_map_frag = STRINGIFY(

varying vec2 v_texcoord;
varying vec4 v_rmap;
varying vec4 v_gmap;
varying vec4 v_bmap;

uniform sampler2D texture0;

void main() {
    vec4 tmp = texture2D(texture0, v_texcoord);

	float s = 1.5;

	float cmp_r = step(tmp.r * s, tmp.g); 
	float cmp_b = step(tmp.b * s, tmp.g); 
	vec3 trans = (v_gmap - vec3(1, 1, 1)) * cmp_r * cmp_b + vec3(1, 1, 1); 
	tmp.xyz = vec3(tmp.r * trans.r, tmp.g * trans.g, tmp.b * trans.b); 

    gl_FragColor = tmp;
}

);
