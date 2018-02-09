/*244:*/
#line 5614 "cweb/weaver.w"

/*66:*/
#line 1927 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 5615 "cweb/weaver.w"

extern int make_iso_compilers_happy;
#if W_TARGET == W_WEB
#include <emscripten.h>  
#include "canvas.h"
static SDL_Surface*window;
#ifdef W_MULTITHREAD
pthread_mutex_t _window_mutex;
#endif

void _initialize_canvas(void){
SDL_Init(SDL_INIT_VIDEO);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
W.resolution_x= emscripten_run_script_int("window.innerWidth");
W.resolution_y= emscripten_run_script_int("window.innerHeight");
if(W.resolution_x<800)
W.resolution_x= 800;
if(W.resolution_y<600)
W.resolution_y= 600;



W.framerate= 60;
W.x= W.resolution_x/2;
W.y= W.resolution_y/2;
window= SDL_SetVideoMode(
#if W_WIDTH >  0
W.width= W_WIDTH,
#else
W.width= W.resolution_x,
#endif
#if W_HEIGHT >  0
W.height= W_HEIGHT,
#else
W.height= W.resolution_y,
#endif
0,
SDL_OPENGL
#if W_WIDTH == 0 && W_HEIGHT == 0
|SDL_WINDOW_FULLSCREEN
#endif
);
if(window==NULL){
fprintf(stderr,"ERROR: Could not create window: %s\n",SDL_GetError());
exit(1);
}

EM_ASM(
var el= document.getElementById("canvas");
el.style.position= "absolute";
el.style.top= "0px";
el.style.left= "0px";
el= document.getElementById("output");
el.style.display= "none";
el= document.getElementsByTagName("BODY")[0];
el.style.overflow= "hidden";
);
}
void _finalize_canvas(void){
SDL_FreeSurface(window);
}
/*253:*/
#line 5804 "cweb/weaver.w"

void _Wresize_window(int width,int height){
int old_width,old_height;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_window_mutex);
#endif
window= SDL_SetVideoMode(width,height,
0,
SDL_OPENGL
);
old_width= W.width;
old_height= W.height;
W.width= width;
W.height= height;
glViewport(0,0,W.width,W.height);
/*436:*/
#line 9582 "cweb/weaver.w"

_update_interface_screen_size();
/*:436*//*478:*/
#line 10646 "cweb/weaver.w"

{


int i,j;


float new_width,new_height;
for(i= 0;i<W_MAX_SUBLOOP;i++)
for(j= 0;j<W_MAX_INTERFACES;j++){
if(_interfaces[i][j].type==W_NONE)continue;
W.move_interface(&_interfaces[i][j],
_interfaces[i][j].x*
((float)width)/((float)old_width),
_interfaces[i][j].y*
((float)height)/((float)old_height));
new_width= _interfaces[i][j].width*
((float)width/(float)old_width);
new_height= _interfaces[i][j].height*
((float)height/(float)old_height);
W.resize_interface(&_interfaces[i][j],new_width,new_height);
}
}
/*:478*/
#line 5819 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_window_mutex);
#endif
}
/*:253*//*259:*/
#line 5867 "cweb/weaver.w"

void _Wmove_window(int width,int height){
return;
}
/*:259*/
#line 5677 "cweb/weaver.w"

#endif
/*:244*/
