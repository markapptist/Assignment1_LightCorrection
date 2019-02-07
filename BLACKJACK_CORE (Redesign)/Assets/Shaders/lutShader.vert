#version 450 core

uniform sampler2D tex;
varying vec2 texCoordVarying;

uniform float lutSize;
uniform float AMOUNT;

void main() {

vec3 rawColor = texture2D(tex, texCoordVarying).rgb;

//Compute the 3D LUT lookup scale/offset factor
vec3 scale = vec3(lutSize - 1.0) / lutSize);
vec3 offset = vec3(1.0 / (2.0 * lutSize));

//Apply 3D LUT color transform

vec3 applyLUT = texture3D(lutTexture, scale * rawColor + offset).rgb;

vec3 finalColor = lerp(rawColor.applyLut, AMOUNT));

gl_FragColor = vec4(finalColor, 1.0);
}