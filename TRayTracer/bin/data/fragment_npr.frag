#version 430

//Cor final do fragmento
layout(location=0) out vec4 frag_color;

in vec2 v_coords;

layout(location=0) uniform sampler2D screen;

float threshold = 0.5f;
void main()
{
	frag_color = texture(screen, vec2(v_coords.x, v_coords.y));
}