static const char* model3_normal_vert = STRINGIFY(

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
	vec3 eye_normal = u_normal_matrix * normal;
	
	vec4 pos4 = u_modelview * position;
	vec3 pos3 = pos4.xyz / pos4.w;
	vec3 light_dir = normalize(u_light_position - pos3);
	
	float diff = max(0.0, dot(eye_normal, light_dir));
	
	v_dest_color = vec4(diff * u_diffuse_material, 1);

	v_dest_color.rgb += u_ambient_material;
	
	vec3 reflection = normalize(reflect(-light_dir, eye_normal));
	float spec = max(0.0, dot(eye_normal, reflection));
	spec = pow(spec, u_shininess);
	v_dest_color.rgb += spec * u_specular_material;
		
	gl_Position = u_projection * u_modelview * position;
}

);
