#version 400

//Cor final do fragmento
layout(location=0) out vec4 frag_color;

in vec2 v_coords;

uniform sampler2D tex_2d;

float threshold = 1;
void main()
{
	vec4 top         = texture(tex_2d, vec2(v_coords.x, v_coords.y + 1.0 / 200.0));
	vec4 bottom      = texture(tex_2d, vec2(v_coords.x, v_coords.y - 1.0 / 200.0));
	vec4 left        = texture(tex_2d, vec2(v_coords.x - 1.0 / 300.0, v_coords.y));
	vec4 right       = texture(tex_2d, vec2(v_coords.x + 1.0 / 300.0, v_coords.y));
	vec4 topLeft     = texture(tex_2d, vec2(v_coords.x - 1.0 / 300.0, v_coords.y + 1.0 / 200.0));
	vec4 topRight    = texture(tex_2d, vec2(v_coords.x + 1.0 / 300.0, v_coords.y + 1.0 / 200.0));
	vec4 bottomLeft  = texture(tex_2d, vec2(v_coords.x - 1.0 / 300.0, v_coords.y - 1.0 / 200.0));
	vec4 bottomRight = texture(tex_2d, vec2(v_coords.x + 1.0 / 300.0, v_coords.y - 1.0 / 200.0));
	vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
	vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
	vec4 sobel = sqrt(sx * sx + sy * sy);

	frag_color = texture(tex_2d, vec2(v_coords.x, v_coords.y)) * ((step(sobel.x, threshold) * step(sobel.y, threshold) * step(sobel.z, threshold)));

	//frag_color = texture(tex_2d, vec2(v_coords.x, v_coords.y)) * (1 - sobel);
}