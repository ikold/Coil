#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTextureCoordinates;
layout(location = 3) in float aTextureIndex;

uniform mat4 uViewProjection;

out vec4 vColor;
out vec2 vTextureCoordinates;
out float vTextureIndex;

void main()
{
	vColor = aColor;
	vTextureCoordinates = aTextureCoordinates;
	vTextureIndex = aTextureIndex;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);	
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 vColor;
in vec2 vTextureCoordinates;
in float vTextureIndex;

uniform sampler2D uTextures[32];

void main()
{
	vec4 textureColor = vColor;

	switch(int(vTextureIndex))
	{
		case 0: textureColor *= texture(uTextures[0], vTextureCoordinates); break;
		case 1: textureColor *= texture(uTextures[1], vTextureCoordinates); break;
		case 2: textureColor *= texture(uTextures[2], vTextureCoordinates); break;
		case 3: textureColor *= texture(uTextures[3], vTextureCoordinates); break;
		case 4: textureColor *= texture(uTextures[4], vTextureCoordinates); break;
		case 5: textureColor *= texture(uTextures[5], vTextureCoordinates); break;
		case 6: textureColor *= texture(uTextures[6], vTextureCoordinates); break;
		case 7: textureColor *= texture(uTextures[7], vTextureCoordinates); break;
		case 8: textureColor *= texture(uTextures[8], vTextureCoordinates); break;
		case 9: textureColor *= texture(uTextures[9], vTextureCoordinates); break;
		case 10: textureColor *= texture(uTextures[10], vTextureCoordinates); break;
		case 11: textureColor *= texture(uTextures[11], vTextureCoordinates); break;
		case 12: textureColor *= texture(uTextures[12], vTextureCoordinates); break;
		case 13: textureColor *= texture(uTextures[13], vTextureCoordinates); break;
		case 14: textureColor *= texture(uTextures[14], vTextureCoordinates); break;
		case 15: textureColor *= texture(uTextures[15], vTextureCoordinates); break;
		case 16: textureColor *= texture(uTextures[16], vTextureCoordinates); break;
		case 17: textureColor *= texture(uTextures[17], vTextureCoordinates); break;
		case 18: textureColor *= texture(uTextures[18], vTextureCoordinates); break;
		case 19: textureColor *= texture(uTextures[19], vTextureCoordinates); break;
		case 20: textureColor *= texture(uTextures[20], vTextureCoordinates); break;
		case 21: textureColor *= texture(uTextures[21], vTextureCoordinates); break;
		case 22: textureColor *= texture(uTextures[22], vTextureCoordinates); break;
		case 23: textureColor *= texture(uTextures[23], vTextureCoordinates); break;
		case 24: textureColor *= texture(uTextures[24], vTextureCoordinates); break;
		case 25: textureColor *= texture(uTextures[25], vTextureCoordinates); break;
		case 26: textureColor *= texture(uTextures[26], vTextureCoordinates); break;
		case 27: textureColor *= texture(uTextures[27], vTextureCoordinates); break;
		case 28: textureColor *= texture(uTextures[28], vTextureCoordinates); break;
		case 29: textureColor *= texture(uTextures[29], vTextureCoordinates); break;
		case 30: textureColor *= texture(uTextures[30], vTextureCoordinates); break;
		case 31: textureColor *= texture(uTextures[31], vTextureCoordinates); break;
	}

	color = textureColor;
}