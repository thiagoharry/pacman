#version 100

#define LEFT            0
#define RIGHT           1
#define UP_FROM_LEFT    2
#define UP_FROM_RIGHT   3
#define DOWN_FROM_LEFT  4
#define DOWN_FROM_RIGHT 5

#if GL_FRAGMENT_PRECISION_HIGH == 1
  precision highp float;
  precision highp int;
#else
  precision mediump float;
  precision mediump int;
#endif
  precision lowp sampler2D;
  precision lowp samplerCube;

uniform vec4 object_color;

uniform float time;
uniform vec2 object_size;
uniform int integer;

uniform sampler2D texture1;
varying mediump vec2 texture_coordinate;

void main(){
    vec4 texture;
    texture = texture2D(texture1, texture_coordinate);
    gl_FragData[0] = texture;
}
