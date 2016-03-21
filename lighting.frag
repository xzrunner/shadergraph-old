static const char* lighting_frag = STRINGIFY(

#ifdef GL_ES
\n#version 100\n
precision highp float;
#endif

varying vec4 v_dest_color;

void main(void)
{
	gl_FragColor = v_dest_color;
}

);