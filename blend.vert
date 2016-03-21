static const char* blend_vert = STRINGIFY(

#ifdef GL_ES
\n#version 100\n
precision highp float;
#endif

attribute vec4 position;
attribute vec2 texcoord;
attribute vec4 color;
attribute vec4 additive;
attribute vec2 texcoord_base;

varying vec2 v_texcoord;
varying vec2 v_texcoord_base;
varying vec4 v_color;
varying vec4 v_additive;

uniform mat4 u_projection;
uniform mat4 u_modelview;

void main()
{
	gl_Position = u_projection * u_modelview * position;
	v_color = color;
	v_additive = additive;
	v_texcoord = texcoord;
	v_texcoord_base = texcoord_base;
}

);