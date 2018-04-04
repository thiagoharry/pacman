/*214:*/
#line 5109 "cweb/weaver.w"

#ifndef _window_h_
#define _window_h_
#ifdef __cplusplus
extern"C"{
#endif
/*66:*/
#line 1928 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 5115 "cweb/weaver.w"

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
/*238:*/
#line 5520 "cweb/weaver.w"

typedef GLXContext
(*glXCreateContextAttribsARBProc)(Display*,GLXFBConfig,GLXContext,Bool,
const int*);
/*:238*//*250:*/
#line 5779 "cweb/weaver.w"

void _Wresize_window(int width,int height);
/*:250*//*256:*/
#line 5848 "cweb/weaver.w"

void _Wmove_window(int x,int y);
/*:256*/
#line 5136 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:214*/
