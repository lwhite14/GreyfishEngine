#version 460

layout (location = 0) in vec3 VertexPosition;
//layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(VertexPosition, 1.0f);
	TexCoord = vec2(VertexTexCoord.x, VertexTexCoord.y);
}