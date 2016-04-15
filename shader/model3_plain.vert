static const char* model3_plain_vert = STRINGIFY(

attribute vec4 position;

uniform mat4 u_projection;
uniform mat4 u_modelview;
	
void main(void)
{
	gl_Position = u_projection * u_modelview * position;
}

);
