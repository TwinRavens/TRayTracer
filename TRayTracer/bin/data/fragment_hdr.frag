#version 400

//Cor final do fragmento
layout(location=0) out vec4 frag_color;

in vec2 v_coords;

uniform float exposure = 1.0;
uniform sampler2D tex_2d;

// ACES Film Academy tone mapping
vec3 acesFilmMapping( vec3 color )
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp(((color*(a*color+b))/(color*(c*color+d)+e)), 0.0, 1.0);
}

// Reinhard tone mapping
vec3 reinhardMapping(vec3 color)
{
    vec3 mapped = color / (color + vec3(1.0));

	return mapped;
}

// Exposure tone mapping
vec3 exposureMapping(vec3 color, float expos)
{
    vec3 mapped = vec3(1.0) - exp(-color * expos);

	return mapped;
}

void main()
{
	
	vec3 hdr_color = texture(tex_2d, v_coords).rgb;

	//tone mapping
	vec3 mapped = acesFilmMapping(exposureMapping(hdr_color, exposure));

	frag_color = vec4(mapped, 1.0);
}