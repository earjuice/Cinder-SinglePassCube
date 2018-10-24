// passthrough vertex shader
// no vertex transformation

#version 440 core

in vec4			ciPosition;
in vec3			ciColor;
in vec2			ciTexCoord0;
in vec3			ciNormal;

uniform mat4		ciModelView;
uniform mat4	ciViewMatrixInverse;
uniform mat4	ciModelMatrix;
//out vec3		vColor;

out VertexData {
	vec2 texCoord;
	vec3 baseColor;
	vec3 normal;
#ifdef TEXTUREDCUBE
	vec4 vPos;
#endif //TEXTUREDCUBE
} outData;


void main() {
	outData.texCoord = ciTexCoord0;
	outData.baseColor		= ciColor;
	outData.normal = ciNormal;
	vec4 persp = ciModelView * ciPosition;
#ifdef TEXTUREDCUBE
	outData.vPos =   ciPosition;
#endif //TEXTUREDCUBE
	gl_Position	= persp;
} 