static const char* shape_vert = STRINGIFY(

#ifdef GL_ES
\n#version 100\n
precision highp float;
#endif

attribute vec4 position;
attribute vec4 color;   

varying vec4 v_color;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	v_color = color;
}

);