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
  float multiplier = 1.0 + (texture_coordinate.y);
  vec4 light = vec4(0.1, 0.1, 0.1, 0.1) * multiplier * 1.1;
  float x = texture_coordinate.x * object_size.x * multiplier;
  x = x - (object_size.x / 2.0) * (multiplier - 1.0);
  x = x - 101.0 * floor(x / 101.0);
  x = x / 101.0;
  if(x < 0.0)
    x = 1.0 - x;
  float y = texture_coordinate.y * object_size.y * multiplier;
  y = y - 81.0 * floor(y / 81.0);
  y = y / 81.0;
  vec4 texture = texture2D(texture1, vec2(x, y));
  gl_FragData[0] = texture + light;
}
