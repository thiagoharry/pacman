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
    float x = (texture_coordinate.x * 5.0) - float(int(texture_coordinate.x * 5.0));
    vec4 texture = texture2D(texture1, vec2(x, texture_coordinate.y));
    if(texture_coordinate.x < float(integer - 1) / 5.0)
        gl_FragData[0] = texture;
    else
        gl_FragData[0] = vec4(0.0, 0.0, 0.0, 0.0);
}
