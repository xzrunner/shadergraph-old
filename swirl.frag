// code from http://www.geeks3d.com/20110428/shader-library-swirl-post-processing-filter-in-glsl/

static const char* swirl_frag = STRINGIFY(

varying vec2 v_texcoord;

uniform sampler2D texture0;

//uniform float u_time;

uniform float u_radius;
uniform float u_angle;
uniform vec2 u_center;

void main()  
{
	vec2 tex_size = textureSize(texture0, 0);
	vec2 tc = v_texcoord * tex_size;
	tc -= u_center;
	float dist = length(tc);
	if (dist < u_radius)
	{
		float percent = (u_radius - dist) / u_radius;
		float theta = percent * percent * u_angle * 8.0;
		float s = sin(theta);
		float c = cos(theta);
		tc = vec2(dot(tc, vec2(c, -s)), dot(tc, vec2(s, c)));		
	}
	tc += u_center;
	gl_FragColor = texture2D(texture0, tc / tex_size);
}

);
