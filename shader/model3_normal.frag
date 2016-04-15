static const char* model3_normal_frag = STRINGIFY(

varying vec4 v_dest_color;

void main(void)
{
	gl_FragColor = v_dest_color;
}

);