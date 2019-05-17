#shader vertex
#version 400 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 v_TexCoord;

uniform mat4 u_World;
uniform mat4 u_MVP; // Model View Projection Matrix
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
	//vec4 viewSpaceVertex = u_View * u_World;
	gl_Position = u_MVP * vec4(inPosition,1.0f);
	v_TexCoord = inTexCoord;
};

#shader pixel
#version 400 core

layout(location = 0) out vec4 Albedo;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 textColor = texture(u_Texture, v_TexCoord) * u_Color;
	//vec4 textColor = texture(u_Texture, v_TexCoord) * u_Color;
	//Albedo = textColor;
	//Albedo = u_Color;
	//Albedo = vec4(1.0, 0.0, 0.0, 1.0);
	Albedo = textColor;
};