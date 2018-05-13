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

#define STUCK           4

attribute vec3 vertex_position;

uniform vec4 object_color;
uniform mat4 model_view_matrix;
uniform float time;
uniform vec2 object_size;
uniform int integer;

varying mediump vec2 texture_coordinate;

void main(){
    vec4 offset = vec4(0.0, 0.0, 0.0, 0.0);
    if(integer == STUCK)
        offset.y = 0.02 * cos(3.0 * time);
    gl_Position = model_view_matrix * vec4(vertex_position, 1.0);
    gl_Position += offset;
    texture_coordinate = vec2(vertex_position[0] + 0.5, vertex_position[1] + 0.5);
}
