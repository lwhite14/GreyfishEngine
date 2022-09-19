#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
noperspective out vec2 TexCoord;
noperspective out vec3 Colour;
out float FogDensity;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform vec2 TargetResolution = vec2(320, 240);

uniform float FogDepthMax = 6.0;
uniform float FogDepthMin = 0.0;

uniform struct LightInfo 
{
	vec4 Position;	
	vec3 La;
	vec3 Ld;
	vec3 Ls;
} Light;

uniform struct MaterialInfo 
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
} Material;

vec3 Lighting( vec3 position, vec3 normal ) 
{
	// Ambient
	vec3 ambient = Material.Ka * Light.La;

	// Diffuse
	vec3 s = normalize(vec3(Light.Position - vec4(position, 1.0f)));
	float sDotN = max( dot(s,normal), 0.0 );
	vec3 diffuse = Material.Kd * Light.Ld * sDotN;

	// Specular
	vec3 spec = vec3(0.0);
	if( sDotN > 0.0 )
	{
		vec3 v = normalize(-position.xyz);
		vec3 h = normalize( v + s ); 
		spec = Material.Ks * pow( max( dot(h,normal), 0.0 ), Material.Shininess );
	}
	return ambient + diffuse + spec;
}

void main() 
{ 
	Normal = normalize( NormalMatrix * VertexNormal);
	Position = (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;
	TexCoord = VertexTexCoord;
	Colour = Lighting(Position, Normal);

	vec4 depthVert = ModelViewMatrix * vec4(VertexPosition, 1.0);
	float depth = abs(depthVert.z / depthVert.w);
	FogDensity = 1.0 - clamp((FogDepthMax - depth) / (FogDepthMin - FogDepthMax), 0.0, 1.0);

	vec4 VertInClipSpace = MVP * vec4(VertexPosition,1.0); 
	vec2 grid = TargetResolution.xy * 0.5;
	vec4 snapped = VertInClipSpace;
	snapped.xyz = VertInClipSpace.xyz / VertInClipSpace.w;
	snapped.xy = floor(grid * snapped.xy) / grid;
	snapped.xyz *= VertInClipSpace.w;
	gl_Position = snapped;
}