/**
 *  @remarks
 *    v_texcoord			varying vec2
 *    u_texture0			uniform sampler2D
 *    u_texture1			uniform sampler2D 
 *    u_height_map_tex		uniform sampler2D  
 *    u_border_gradient_tex	uniform sampler2D  
 *	  u_lifetime			float  
 *	  u_time				float
 *    _DST_COL_				vec4
 */
static const char* burning_map_body = STRINGIFY(

	vec4 _bm_lower_ = texture2D(u_texture0, v_texcoord);
	vec4 _bm_upper_ = texture2D(u_texture1, v_texcoord);
	vec4 _bm_height_ = texture2D(u_height_map_tex, v_texcoord);
	
	float _bm_blend_ = _bm_height_.x;

	float _bm_edge_ = 1.0 - mod(u_time, u_lifetime) / u_lifetime;
	vec4 _DST_COL_;
	if (_bm_blend_ < _bm_edge_) {
		_DST_COL_ = _bm_lower_;
	} else {
		_DST_COL_ = _bm_upper_;
	}
	
	if (_bm_blend_ - _bm_edge_ < 0.5 && _bm_blend_ > _bm_edge_) {
		vec4 _bm_border_ = texture2D(u_border_gradient_tex, vec2(1.0 - (_bm_blend_ - _bm_edge_) * 2.0, 0.5));
		_DST_COL_ = _DST_COL_ * _bm_border_;
	}
	
);