#version 460

in vec3 Position;
in vec3 Normal;
noperspective in vec2 TexCoord;
noperspective in vec3 Colour;
in float FogDensity;

layout( location = 0 ) out vec4 FragColor;

uniform vec4 FogColour = vec4(0.4f, 0.4f, 0.4f, 1.0f);

uniform sampler2D Texture;

void main()
{       
	vec4 textureFragment = texture(Texture, TexCoord);
	vec4 shadedFragment = textureFragment * vec4(Colour, 1.0);
	vec4 fogFragment = mix(FogColour, shadedFragment, FogDensity);
	FragColor = fogFragment;
}