// code from http://www.geeks3d.com/20100909/shader-library-gaussian-blur-post-processing-filter-in-glsl/

static const char* gaussian_blur_frag = STRINGIFY(

varying vec2 v_texcoord;

uniform sampler2D texture0;

float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main()  
{
	vec4 tmp = texture2D(texture0, v_texcoord);
	vec2 tex_size = textureSize(texture0, 0);	
	vec3 tc = tmp.rgb * weight[0];
	for (int i = 1; i < 3; i++) 
	{
		tc += texture2D(texture0, v_texcoord + vec2(0.0, offset[i]) / tex_size.y).rgb * weight[i];
		tc += texture2D(texture0, v_texcoord - vec2(0.0, offset[i]) / tex_size.y).rgb * weight[i];
	}
	gl_FragColor = vec4(tc, tmp.a);
}

);
