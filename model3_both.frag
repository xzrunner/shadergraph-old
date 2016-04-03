static const char* model3_both_frag = STRINGIFY(

varying vec2 v_texcoord;
varying vec4 v_dest_color;
varying vec3 v_specular;

uniform sampler2D texture0; 

void main(void)
{
	vec4 tmp = v_dest_color * texture2D(texture0, v_texcoord);
	tmp.rgb += v_specular;
	gl_FragColor = tmp;
}

);