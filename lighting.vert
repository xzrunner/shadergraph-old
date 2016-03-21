static const char* lighting_vert = STRINGIFY(

#ifdef GL_ES
\n#version 100\n
precision highp float;
#endif

attribute vec4 position;
attribute vec3 normal;

uniform mat4 u_projection;
uniform mat4 u_modelview;

uniform vec3 u_diffuse_material;
uniform vec3 u_ambient_material;
uniform vec3 u_specular_material;
uniform float u_shininess;

uniform mat3 u_normal_matrix;
uniform vec3 u_light_position;

varying vec4 v_dest_color;
	
void main(void)
{
	vec3 N = u_normal_matrix * normal;
	vec3 L = normalize(u_light_position);
	vec3 E = vec3(0, 0, 1);
	vec3 H = normalize(L + E);

	float df = max(0.0, dot(N, L));
	float sf = max(0.0, dot(N, H));
	sf = pow(sf, u_shininess);

	vec3 color = u_ambient_material + df * u_diffuse_material + sf * u_specular_material;
	v_dest_color = vec4(color, 1);
	gl_Position = u_projection * u_modelview * position;
}

);
