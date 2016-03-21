static const char* shape_frag = STRINGIFY(

#ifdef GL_ES
\n#version 100\n
precision highp float;
#endif

varying vec4 v_color;

void main()
{  
	gl_FragColor = v_color;
}

);
