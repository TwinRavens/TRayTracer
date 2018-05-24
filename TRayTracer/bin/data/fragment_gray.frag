#version 400

//Cor final do fragmento
layout(location=0) out vec4 frag_color;

in vec2 v_coords;

uniform sampler2D tex_2d;

void main()
{
	frag_color = texture(tex_2d, v_coords);
	float avg = 0.2126 * frag_color.r + 0.7152 * frag_color.g + 0.0722 * frag_color.b;
	frag_color = vec4(avg, avg, avg, 1.0);
}