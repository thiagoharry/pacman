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
  float x = texture_coordinate.x * object_size.x;
  x = x - 101.0 * floor(x / 101.0);
  x = x / 101.0;
  float y = texture_coordinate.y * object_size.y;
  y = y - 10.0 * floor(y / 10.0);
  y = y / 10.0;
  vec4 texture = texture2D(texture1, vec2(x, y));
  gl_FragData[0] = texture;
}
