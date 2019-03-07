/*269:*/
#line 6103 "./cweb/weaver.w"

#ifndef _canvas_H_
#define _canvas_h_
#ifdef __cplusplus
extern"C"{
#endif
/*66:*/
#line 1950 "./cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 6109 "./cweb/weaver.w"

#include "weaver.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <SDL/SDL.h>  

void _initialize_canvas(void);
void _finalize_canvas(void);
/*278:*/
#line 6314 "./cweb/weaver.w"

void _Wresize_window(int width,int height);
/*:278*//*284:*/
#line 6377 "./cweb/weaver.w"

void _Wmove_window(int x,int y);
/*:284*/
#line 6117 "./cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:269*/
