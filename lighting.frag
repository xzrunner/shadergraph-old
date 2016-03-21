static const char* lighting_frag = STRINGIFY(

\n#version 100\n
precision highp float;

varying vec4 v_dest_color;

void main(void)
{
	gl_FragColor = v_dest_color;
}

);