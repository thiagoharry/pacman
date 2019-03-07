/*682:*/
#line 15262 "./cweb/weaver.w"


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

/*471:*/
#line 10262 "./cweb/weaver.w"

attribute vec3 vertex_position;
/*:471*/
#line 15276 "./cweb/weaver.w"



/*472:*/
#line 10269 "./cweb/weaver.w"

uniform vec4 object_color;
uniform mat4 model_view_matrix;
uniform vec2 object_size;
uniform float time;
uniform sampler2D texture1;
uniform int integer;
/*:472*/
#line 15279 "./cweb/weaver.w"


varying mediump vec2 coordinate;

void main(){
gl_Position= model_view_matrix*vec4(vertex_position,1.0);

coordinate= vec2(vertex_position[0]+0.5,vertex_position[1]+0.5);
}
/*:682*/
