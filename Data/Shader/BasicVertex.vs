#version 120

attribute vec3 vert; // location 0
attribute vec2 texCoord; // location 1

varying vec2 texCoord0;
varying vec4 position;

uniform mat4 camera;
uniform mat4 transform;

void main()
{
	vec4 p = transform * vec4(vert, 1.0);
	position = camera * p;
    gl_Position = position;
	texCoord0 = texCoord;

}
