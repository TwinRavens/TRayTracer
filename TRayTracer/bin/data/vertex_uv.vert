#version 400

in vec3 vertex_position;
in vec2 vertex_coords;
out vec2 v_coords;

void main()
{
	gl_Position = vec4(vertex_position, 1.0);
	v_coords = vertex_coords;
}