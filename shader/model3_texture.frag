static const char* model3_texture_frag = STRINGIFY(

varying vec2 v_texcoord; 

uniform sampler2D texture0; 

void main(void)
{
	gl_FragColor = texture2D(texture0, v_texcoord);
}

);