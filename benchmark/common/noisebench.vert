#version 120

uniform float time;

/*
 * Both 2D, 3D and 4D texture coordinates are defined, for testing purposes.
 */
varying vec2 v_texCoord2D;
varying vec3 v_texCoord3D;
varying vec4 v_texCoord4D;

void main( void )
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	v_texCoord2D = gl_MultiTexCoord0.xy * 16.0 + vec2(0.0, time);
    v_texCoord3D = vec3(gl_MultiTexCoord0.xy * 16.0, time);
    v_texCoord4D = vec4(gl_MultiTexCoord0.xy * 16.0, 0.0, time);
}
