#shader vertex
#version 400 core
//#define ver_light
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec3 inNormal;

uniform mat4 u_World;
uniform mat4 u_MVP; // Model View Projection Matrix
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform vec4 u_Color;
uniform vec4 u_LightDir;

uniform vec4 u_ViewPosition;               // Camera position (World Space)
uniform vec4 u_SpecularPower;                // Specular pow factor
uniform vec4 u_SpecularColor;               // Specular color (sRGBA)
uniform vec4 u_DifuseColor;                 // Difuse color (sRGBA)

out vec2 v_TexCoord;
//out vec3 v_Color;
out vec4 v_Color;
out vec4 v_wsPos;
out vec4 v_wsNorms;


void main()
{
	//vec4 viewSpaceVertex = u_View * u_World;
	gl_Position = u_MVP * vec4(inPosition, 1.0f);
	v_TexCoord = inTexCoord;

	vec4 wsPos = vec4(inPosition,1);

	vec4 normalWS = vec4(inNormal, 1);
#ifdef ver_light
	// Lambert light calc
	//float3 wsLightDir = -normalize(LightDir.xyz);
	vec4 lightDirWS = normalize(-u_LightDir);
	float Ndl = dot(lightDirWS, normalWS);
	vec4 NDLR = vec4(Ndl, Ndl, Ndl, Ndl);
	//float wsNdL = max(0.0f, dot(wsLightDir.xyz, wsNormal.xyz));

	// Blinn specular
	vec3 wsViewDir = -normalize(wsPos.xyz - u_ViewPosition.xyz);
	vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, normalWS.xyz));
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
	float SpecularFactor = pow(wsVdR, u_SpecularPower.x) * NDLR.x;

	v_Color = (NDLR * u_DifuseColor) + (SpecularFactor * u_SpecularColor);
#else
	v_wsPos.xyz = wsPos.xyz;
	v_wsNorms.xyz = normalWS.xyz;
#endif

	//v_Color = NDLR;
};

#shader pixel
#version 400 core

layout(location = 0) out vec3 Albedo;
//layout(location = 1) out vec3 Lambert;

in vec2 v_TexCoord;
in vec4 v_Color;
in vec4 v_wsPos;
in vec4 v_wsNorms;

uniform vec4 u_Color;
uniform vec4 u_LightDir;

uniform vec4 u_ViewPosition;
uniform vec4 u_SpecularPower;
uniform vec4 u_SpecularColor;
uniform vec4 u_DifuseColor;

uniform sampler2D u_Texture;

void main()
{
	//Albedo = texture(u_Texture, v_TexCoord).rgb;
	//vec4 textColor = texture(u_Texture, v_TexCoord) * u_Color;
	//vec4 textColor = texture(u_Texture, v_TexCoord) * u_Color;
	//Albedo = textColor;
	//Albedo = u_Color;
#ifdef ver_light
	Albedo =v_Color.xyz;
#else
	vec4 lightDirWS = normalize(-u_LightDir);
	float Ndl = dot(lightDirWS, v_wsNorms);
	vec4 NDLR = vec4(Ndl, Ndl, Ndl, Ndl);
	//float wsNdL = max(0.0f, dot(wsLightDir.xyz, wsNormal.xyz));

	// Blinn specular
	vec3 wsViewDir = -normalize(v_wsPos.xyz - u_ViewPosition.xyz);
	vec3 wsReflect = normalize(reflect(-lightDirWS.xyz, v_wsNorms.xyz));
	float wsVdR = max(0.0f, dot(wsViewDir.xyz, wsReflect.xyz));
	float SpecularFactor = pow(wsVdR, u_SpecularPower.x) * NDLR.x;

	Albedo=vec4( (NDLR * u_DifuseColor) + (SpecularFactor * u_SpecularColor)).xyz;
#endif
	//Albedo = Albedo * u_Color.rgb;
};