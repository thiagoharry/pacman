/*494:*/
#line 10990 "cweb/weaver.w"

#version 100

uniform sampler2D texture1;

varying mediump vec2 coordinate;

void main(){
gl_FragData[0]= texture2D(texture1,coordinate);
}
/*:494*/
