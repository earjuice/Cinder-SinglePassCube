// instanced geometry shader
// invocated once for each of the 4 viewports
// takes care of setting the viewport and transforming the vertices/primitives

#version 440 core
#extension GL_EXT_geometry_shader4 : require
#extension GL_NV_viewport_array2 : require
#extension GL_NV_geometry_shader_passthrough : require

#ifdef TRIANGLES
layout(triangles) in;
#endif
#ifdef LINES
layout(lines) in;
#endif
#ifdef POINTS
layout(points) in;
#endif

// Redeclare gl_PerVertex to pass through "gl_Position".
layout(passthrough) in gl_PerVertex {
	vec4 gl_Position;
};

// Declare "Inputs" with "passthrough" to copy members attributes
layout(passthrough) in VertexData {
	vec2 texCoord;
	vec3 baseColor;
	vec3 normal;
#ifdef TEXTUREDCUBE
	vec4 vPos;
#endif //TEXTUREDCUBE
} passthroughData[];

out 	vec4 vPosition;

// No output primitive layout qualifiers required.
layout(viewport_relative) out highp int gl_Layer;

void main() {
	gl_ViewportMask[0] = 0x3F; // Replicate primitive 6 times
	gl_Layer = 0;
}