/*519:*/
#line 11513 "./cweb/weaver.w"

#version 100

attribute mediump vec3 vertex_position;

uniform mat4 model_view_matrix;
uniform vec4 object_color;
uniform vec2 object_size;
uniform float time;
uniform sampler2D texture1;
uniform int integer;

varying mediump vec2 coordinate;

void main(){


gl_Position= model_view_matrix*vec4(vertex_position,1.0);

coordinate= vec2(((vertex_position[0]+0.5)),
((vertex_position[1]+0.5)));
}
/*:519*/
