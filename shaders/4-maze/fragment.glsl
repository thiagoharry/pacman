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
  float x = texture_coordinate.x * object_size.x * multiplier;
  x = x - (object_size.x / 2.0) * (multiplier - 1.0);
  x = x / 1024.0;
  float y = texture_coordinate.y * object_size.y * multiplier;
  y = y / 1184.0;
  if(x < 0.0 || y < 0.0 || x > 1.0 || y > 1.0){
    gl_FragData[0] = vec4(0.0, 0.0, 0.0, 0.0);
  }
  else{
    vec4 texture = texture2D(texture1, vec2(x, y));
    // 0: 0.046875 0.09594594
    // 0.109375 0.8890625
    // 0, 0
    if(distance(vec2(x, y), vec2(0.046875, 0.09594594)) < 0.0046)
      gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
    else if(distance(vec2(x, y), vec2(0.046875, 0.905405405)) < 0.0046)
      gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
    else if(distance(vec2(x, y), vec2(0.9515625, 0.905405405)) < 0.0046)
      gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
    else if(distance(vec2(x, y), vec2(0.9515625, 0.09594594)) < 0.0046)
      gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
    else
      gl_FragData[0] = texture;
  }
}
