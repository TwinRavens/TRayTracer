#version 400

//Cor final do fragmento
layout(location=0) out vec4 frag_color;

in vec2 v_coords;

uniform sampler2D tex_2d;

vec4 getGrayTexture(vec4 frag_color)
{
	float avg = 0.2126 * frag_color.r + 0.7152 * frag_color.g + 0.0722 * frag_color.b;
	return vec4(avg, avg, avg, 1.0);
}

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

	top = getGrayTexture(top);
	bottom = getGrayTexture(bottom);
	left = getGrayTexture(left);
	right = getGrayTexture(right);
	topLeft = getGrayTexture(topLeft);
	topRight = getGrayTexture(topRight);
	bottomLeft = getGrayTexture(bottomLeft);
	bottomRight = getGrayTexture(bottomRight);

	vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
	vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
	vec4 sobel = sqrt(sx * sx + sy * sy);
	frag_color = sobel;
}