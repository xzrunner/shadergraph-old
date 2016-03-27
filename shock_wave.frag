static const char* shock_wave_frag = STRINGIFY(

varying vec2 v_texcoord;

uniform sampler2D texture0;

uniform float u_time;
uniform vec2 u_center;
uniform vec3 u_params;

void main()  
{
	vec2 texcoord = v_texcoord;
	float dis = distance(v_texcoord, u_center);
	if (dis <= (u_time + u_params.z) && dis >= (u_time - u_params.z))
	{
		float diff = dis - u_time;
		float pow_diff = 1.0 - pow(abs(diff * u_params.x), u_params.y);
		float diff_time = diff * pow_diff;
		vec2 diff_uv = normalize(v_texcoord - u_center);
		texcoord = v_texcoord + diff_uv * diff_time;
	}	
	gl_FragColor = texture2D(texture0, texcoord);
}

);
