#version 400

out vec4 frag_colour;

in vec2 v_coords;

uniform sampler2D tex_2d;

void main()
{
	frag_colour = texture(tex_2d, v_coords);
}