#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTextureCoordinates;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec2 vTextureCoordinates;

void main()
{
	vTextureCoordinates = aTextureCoordinates;
	gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);	
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 vTextureCoordinates;

uniform vec4 uColor;
uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, vTextureCoordinates) * uColor;
}