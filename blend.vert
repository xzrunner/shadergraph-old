static const char* blend_vert = STRINGIFY(

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
	v_color = color / 255.0;
	v_additive = additive / 255.0;
	v_texcoord = texcoord;
	v_texcoord_base = texcoord_base;
}

);