#version 450 core

varying vec2 texCoordVarying;


void main(){

texCoordVarying = gl_MultiTexCoord0.xy;
gl_Position = ftransform();

}