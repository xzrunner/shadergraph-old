static const char* gray_frag = STRINGIFY(

varying vec2 v_texcoord;  
uniform sampler2D texture0;  

void main()  
{    
	vec4 tmp = texture2D(texture0, v_texcoord);
	float y = dot(tmp.rgb , vec3(0.299, 0.587, 0.114));
	gl_FragColor = vec4(y, y, y, tmp.a);
}

);
