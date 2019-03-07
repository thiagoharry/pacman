/*240:*/
#line 5617 "./cweb/weaver.w"

#ifndef _window_h_
#define _window_h_
#ifdef __cplusplus
extern"C"{
#endif
/*66:*/
#line 1950 "./cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 5623 "./cweb/weaver.w"

#include "weaver.h"
#include "memory.h"
#include <signal.h> 
#include <stdio.h>  
#include <stdlib.h>  
#include <X11/Xlib.h>  




#include <GL/gl.h> 
#include <GL/glx.h>  
#include <X11/extensions/Xrandr.h>  




#include <X11/XKBlib.h>  
void _initialize_window(void);
void _finalize_window(void);
/*264:*/
#line 6028 "./cweb/weaver.w"

typedef GLXContext
(*glXCreateContextAttribsARBProc)(Display*,GLXFBConfig,GLXContext,Bool,
const int*);
/*:264*//*276:*/
#line 6287 "./cweb/weaver.w"

void _Wresize_window(int width,int height);
/*:276*//*282:*/
#line 6356 "./cweb/weaver.w"

void _Wmove_window(int x,int y);
/*:282*/
#line 5644 "./cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:240*/
