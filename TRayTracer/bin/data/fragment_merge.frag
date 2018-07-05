#version 430

out vec4 frag_colour;

in vec2 v_coords;

layout(location=0) uniform sampler2D screen;
layout(location=1) uniform sampler2D other;

void main()
{
	frag_colour = texture(screen, v_coords) + texture(other, v_coords);
}