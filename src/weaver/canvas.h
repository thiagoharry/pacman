/*243:*/
#line 5594 "cweb/weaver.w"

#ifndef _canvas_H_
#define _canvas_h_
#ifdef __cplusplus
extern"C"{
#endif
/*66:*/
#line 1927 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 5600 "cweb/weaver.w"

#include "weaver.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <SDL/SDL.h>  

void _initialize_canvas(void);
void _finalize_canvas(void);
/*252:*/
#line 5805 "cweb/weaver.w"

void _Wresize_window(int width,int height);
/*:252*//*258:*/
#line 5868 "cweb/weaver.w"

void _Wmove_window(int x,int y);
/*:258*/
#line 5608 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:243*/
