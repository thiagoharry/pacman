/*597:*/
#line 12711 "cweb/weaver.w"

#ifndef _gif_h_
#define _gif_h_
#ifdef __cplusplus
extern"C"{
#endif
#include "weaver.h"
/*66:*/
#line 1927 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 12718 "cweb/weaver.w"

/*601:*/
#line 12739 "cweb/weaver.w"

GLuint*_extract_gif(char*,unsigned*,unsigned**,int*,bool*);
/*:601*//*618:*/
#line 13250 "cweb/weaver.w"

struct _image_list{
unsigned char*rgba_image;
unsigned delay_time;
unsigned x_offset,y_offset,width,height;
int disposal_method;

struct _image_list*next,*prev;
};
/*:618*/
#line 12719 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:597*/
