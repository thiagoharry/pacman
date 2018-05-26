#version 100

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
    float x = (float(integer) / 10.0) + texture_coordinate.x * 0.1;
    vec4 texture = texture2D(texture1, vec2(x, texture_coordinate.y));
    gl_FragData[0] = texture;
}
