// copy from https://github.com/SFML/SFML/wiki/Source:-HeatHazeShader

static const char* heat_haze_frag = STRINGIFY(

varying vec2 v_texcoord;  

uniform sampler2D u_current_tex;			// Our render texture
uniform sampler2D u_distortion_map_tex;		// Our heat distortion map texture

uniform float u_time;						// Time used to scroll the distortion map
uniform float u_distortion_factor;			// Factor used to control severity of the effect
uniform float u_rise_factor;				// Factor used to control how fast air rises

void main()
{
    vec2 distortion_map_coord = v_texcoord;

    // We use the u_time value to scroll our distortion texture upwards
    // Since we enabled texture repeating, OpenGL takes care of
    // coordinates that lie outside of [0, 1] by discarding
    // the integer part and keeping the fractional part
    // Basically performing a "floating point modulo 1"
    // 1.1 = 0.1, 2.4 = 0.4, 10.3 = 0.3 etc.
	distortion_map_coord.t = fract(distortion_map_coord.t - u_time * u_rise_factor);
	
    vec4 distortion_map_value = texture2D(u_distortion_map_tex, distortion_map_coord);

    // The values are normalized by OpenGL to lie in the range [0, 1]
    // We want negative offsets too, so we subtract 0.5 and multiply by 2
    // We end up with values in the range [-1, 1]
    vec2 distortion_position_offset = distortion_map_value.xy;
    distortion_position_offset -= vec2(0.5, 0.5);
    distortion_position_offset *= 2.0;

    // The factor scales the offset and thus controls the severity
    distortion_position_offset *= u_distortion_factor;

    // The latter 2 channels of the texture are unused... be creative
    vec2 distortion_unused = distortion_map_value.zw;

    // Since we all know that hot air rises and cools,
    // the effect loses its severity the higher up we get
    // We use the t (a.k.a. y) texture coordinate of the original texture
    // to tell us how "high up" we are and damp accordingly
    // Remember, OpenGL 0 is at the bottom
    distortion_position_offset *= (1.0 - v_texcoord.t);

    vec2 distorted_tex_coord = v_texcoord + distortion_position_offset;

    gl_FragColor = texture2D(u_current_tex, distorted_tex_coord);
}

);
