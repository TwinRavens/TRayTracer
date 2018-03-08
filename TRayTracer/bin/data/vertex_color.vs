#version 400

in vec3 vertex_position;
in vec3 vertex_colour;
out vec3 v_colour;

void main()
{
	gl_Position = vec4(vertex_position, 1.0);
	v_colour = vertex_colour;
}