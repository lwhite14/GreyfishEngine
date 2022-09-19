#version 460

out vec4 FragColor;

in vec3 Position;
in vec2 TexCoord;

uniform sampler2D Texture;

void main()
{
	FragColor = texture(Texture, TexCoord);
}