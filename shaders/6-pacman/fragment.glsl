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
    vec2 coordinate;
    if(integer == RIGHT)
        coordinate = texture_coordinate;
    else if(integer == LEFT)
        coordinate = vec2(1.0 - texture_coordinate.x, texture_coordinate.y);
    else if(integer == UP_FROM_LEFT)
        coordinate = vec2(texture_coordinate.y, texture_coordinate.x);
    else if(integer == UP_FROM_RIGHT)
        coordinate = vec2(texture_coordinate.y, 1.0 - texture_coordinate.x);
    else if(integer == DOWN_FROM_RIGHT)
        coordinate = vec2(1.0 - texture_coordinate.y, texture_coordinate.x);
    else
        coordinate = vec2(1.0 - texture_coordinate.y, 1.0 - texture_coordinate.x);
    texture = texture2D(texture1, coordinate);
    gl_FragData[0] = vec4(texture.rgb, min(object_color.a, texture.a));
}
