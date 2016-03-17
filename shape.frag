static const char* shape_frag = STRINGIFY(

\n#version 100\n
precision highp float;

varying vec4 v_color;

void main()
{  
	gl_FragColor = v_color;
}

);
