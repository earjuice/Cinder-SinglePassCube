// passthrough fragment shader
#version 440 core

#ifdef TEXTUREDCUBE
uniform samplerCube uCubeMapTex;
#endif //TEXTUREDCUBE

in VertexData {
	vec2 texCoord;
	vec3 baseColor;
	vec3 normal;
#ifdef TEXTUREDCUBE
	vec4 vPos;
#endif //TEXTUREDCUBE
};
out vec4	oColor;

void main() {
	oColor	= vec4( baseColor, 1.0 );
#ifdef TEXTUREDCUBE
	oColor =vec4( texture( uCubeMapTex, vPos.xyz ));
#endif //TEXTUREDCUBE

}