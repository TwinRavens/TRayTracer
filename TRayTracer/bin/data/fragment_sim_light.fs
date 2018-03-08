#version 400

out vec4 frag_colour;

in vec2 v_coords;

uniform vec2 mousePos;
uniform vec2 screenSizeDiv;
uniform sampler2D previewTexture;

void main()
{
	vec3 mouse3Pos = vec3(mousePos.x, mousePos.y, 1.0);
	vec2 fragPos = vec2(gl_FragCoord.x*screenSizeDiv.x, gl_FragCoord.y*screenSizeDiv.y);
	fragPos = fragPos * 2.0f - vec2(1.0f, 1.0f);
	vec3 centerToFrag = normalize(vec3(fragPos, 1) - vec3(0, 0, 0));
	vec3 reflection = 2.0f*mouse3Pos*dot(centerToFrag, mouse3Pos)-centerToFrag;

	frag_colour = vec4(abs(reflection.x), 0, abs(reflection.y), 0);
}