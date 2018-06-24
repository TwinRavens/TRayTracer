#version 430

//Cor final do fragmento
layout(location=0) out vec4 frag_color;

in vec2 v_coords;

layout(location=0) uniform sampler2D screen;
layout(location=1) uniform sampler2D other;

float threshold = 0.5f;
float scaleNoise = 2;
void main()
{
	frag_color = texture(screen, vec2(v_coords.x, v_coords.y)) * step(texture(other, vec2(v_coords.x *scaleNoise, v_coords.y*scaleNoise)).x, threshold);
}