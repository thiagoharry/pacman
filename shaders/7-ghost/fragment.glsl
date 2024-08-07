#version 100

#define LEFT            0
#define RIGHT           1
#define UP              2
#define DOWN            3
#define STUCK           4

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
  vec4 texture = texture2D(texture1, texture_coordinate);
  vec4 color = texture;
  int eye = 0;
  if(integer == DOWN || (integer == STUCK && sin(3.0 * time) > 0.0)){
      if(distance(texture_coordinate, vec2(0.37, 0.6)) < 0.1 ||
         distance(texture_coordinate, vec2(0.63, 0.6)) < 0.1){
          if(distance(texture_coordinate, vec2(0.37, 0.55)) < 0.06 ||
             distance(texture_coordinate, vec2(0.63, 0.55)) < 0.06){
              color = vec4(0.0, 0.0, 0.0, 1.0);
          }
          else{
              color = vec4(1.0, 1.0, 1.0, 1.0);
          }
          eye = 1;
      }
  }
  else if(integer == LEFT){
      if(distance(texture_coordinate, vec2(0.30, 0.6)) < 0.1 ||
         distance(texture_coordinate, vec2(0.56, 0.6)) < 0.1){
          if(distance(texture_coordinate, vec2(0.26, 0.6)) < 0.06 ||
             distance(texture_coordinate, vec2(0.52, 0.6)) < 0.06){
              color = vec4(0.0, 0.0, 0.0, 1.0);
          }
          else{
              color = vec4(1.0, 1.0, 1.0, 1.0);
          }
          eye = 1;
      }
  }
  else if(integer == RIGHT){
      if(distance(texture_coordinate, vec2(0.44, 0.6)) < 0.1 ||
         distance(texture_coordinate, vec2(0.70, 0.6)) < 0.1){
          if(distance(texture_coordinate, vec2(0.48, 0.6)) < 0.06 ||
             distance(texture_coordinate, vec2(0.74, 0.6)) < 0.06){
              color = vec4(0.0, 0.0, 0.0, 1.0);
          }
          else{
              color = vec4(1.0, 1.0, 1.0, 1.0);
          }
          eye = 1;
      }
  }
  else{
      if(distance(texture_coordinate, vec2(0.37, 0.8)) < 0.1 ||
         distance(texture_coordinate, vec2(0.63, 0.8)) < 0.1){
          if(distance(texture_coordinate, vec2(0.37, 0.84)) < 0.06 ||
             distance(texture_coordinate, vec2(0.63, 0.84)) < 0.06){
              color = vec4(0.0, 0.0, 0.0, 1.0);
          }
          else{
              color = vec4(1.0, 1.0, 1.0, 1.0);
          }
          eye = 1;
      }
  }
  if(eye == 0 && object_color.r <= 0.2)
      gl_FragData[0] = vec4(0.0, 0.0, 0.0, 0.0);
  else if(eye == 0 && object_color.g >= 0.9 && object_color.b >= 0.9){
      if(sin(60.0 * time) > 0.0)
          gl_FragData[0] = vec4(0.0, 0.0, 1.0, color.a);
      else
          gl_FragData[0] = color;
  }
  else if(eye == 0 && object_color.b == 1.0 && texture_coordinate.y > 0.09)
      gl_FragData[0] = vec4(0.0, 0.0, 1.0, color.a);
  else
      gl_FragData[0] = color;
}
