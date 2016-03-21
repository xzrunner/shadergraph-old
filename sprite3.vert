static const char* sprite3_vert = STRINGIFY(

#ifdef GL_ES
\n#version 100\n
precision highp float;
#endif

attribute vec4 position;
attribute vec2 texcoord;

varying vec2 v_texcoord;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	v_texcoord = texcoord;
}

);