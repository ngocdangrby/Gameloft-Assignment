precision mediump float;

uniform sampler2D u_texture;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_blendmap;

varying vec2 v_uv;

void main()
{
	vec4 blendpoint = texture2D(u_blendmap, v_uv);
	vec4 tex = texture2D(u_texture, v_uv);
	vec4 tex1 = texture2D(u_texture1, v_uv);
	vec4 tex2 = texture2D(u_texture2, v_uv);
	gl_FragColor = (blendpoint.r * tex + blendpoint.g * tex1 + blendpoint.b * tex2) / (blendpoint.r+blendpoint.g+blendpoint.b);
}
