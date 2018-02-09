/*215:*/
#line 5145 "cweb/weaver.w"

/*66:*/
#line 1927 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 5146 "cweb/weaver.w"





extern int make_iso_compilers_happy;
#if W_TARGET == W_ELF
#include "window.h"
#ifdef W_MULTITHREAD
pthread_mutex_t _window_mutex;
#endif
/*218:*/
#line 5205 "cweb/weaver.w"

Display*_dpy;
/*:218*//*221:*/
#line 5241 "cweb/weaver.w"

int _screen;
/*:221*//*224:*/
#line 5264 "cweb/weaver.w"

static int depth;
/*:224*//*226:*/
#line 5283 "cweb/weaver.w"

Window _window;
/*:226*//*229:*/
#line 5356 "cweb/weaver.w"

static XSetWindowAttributes at;
/*:229*//*234:*/
#line 5462 "cweb/weaver.w"

GLXContext _context;
/*:234*//*236:*/
#line 5475 "cweb/weaver.w"

static GLXFBConfig*fbConfigs;
/*:236*/
#line 5157 "cweb/weaver.w"

void _initialize_window(void){
/*220:*/
#line 5222 "cweb/weaver.w"

_dpy= XOpenDisplay(NULL);
if(_dpy==NULL){
fprintf(stderr,
"ERROR: Couldn't connect with the X Server. Are you running a "
"graphical interface?\n");
exit(1);
}
/*:220*//*223:*/
#line 5255 "cweb/weaver.w"

_screen= DefaultScreen(_dpy);
/*:223*//*225:*/
#line 5273 "cweb/weaver.w"

depth= DisplayPlanes(_dpy,_screen);
#if W_DEBUG_LEVEL>=3
printf("WARNING (3): Color depth: %d\n",depth);
#endif
/*:225*//*228:*/
#line 5296 "cweb/weaver.w"


W.resolution_x= DisplayWidth(_dpy,_screen);
W.resolution_y= DisplayHeight(_dpy,_screen);
#if W_WIDTH >  0
W.width= W_WIDTH;
#else
W.width= W.resolution_x;
#endif
#if W_HEIGHT >  0 
W.height= W_HEIGHT;
#else
W.height= W.resolution_y;
#endif

W.x= W.width/2;
W.y= W.resolution_y-W.height/2;
{
XRRScreenConfiguration*conf= XRRGetScreenInfo(_dpy,RootWindow(_dpy,0));
W.framerate= XRRConfigCurrentRate(conf);
XRRFreeScreenConfigInfo(conf);
}
_window= XCreateSimpleWindow(_dpy,
DefaultRootWindow(_dpy),

W.x-W.width/2,
W.resolution_y-W.y-W.height/2,
W.width,
W.height,
0,0,
0);
/*:228*//*230:*/
#line 5360 "cweb/weaver.w"

{
#if W_WIDTH == 0 && W_HEIGHT == 0
at.override_redirect= True;
#endif



at.event_mask= ButtonPressMask|ButtonReleaseMask|KeyPressMask|
KeyReleaseMask|PointerMotionMask|ExposureMask|StructureNotifyMask|
FocusChangeMask;
XChangeWindowAttributes(_dpy,_window,CWOverrideRedirect,&at);
XSelectInput(_dpy,_window,StructureNotifyMask|KeyPressMask|
KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|
PointerMotionMask|ExposureMask|StructureNotifyMask|
FocusChangeMask);
}
/*:230*//*231:*/
#line 5390 "cweb/weaver.w"

XMapWindow(_dpy,_window);
{
XEvent e;
XNextEvent(_dpy,&e);
while(e.type!=MapNotify){
XNextEvent(_dpy,&e);
}
}
XSetInputFocus(_dpy,_window,RevertToParent,CurrentTime);
{

int x_return,y_return;
unsigned int width_return,height_return,dummy_border,dummy_depth;
Window dummy_window;
XGetGeometry(_dpy,_window,&dummy_window,&x_return,&y_return,&width_return,
&height_return,&dummy_border,&dummy_depth);
W.width= width_return;
W.height= height_return;
W.x= x_return+W.width/2;
W.y= W.resolution_y-y_return-W.height/2;
}
#ifdef W_PROGRAM_NAME
XStoreName(_dpy,_window,W_PROGRAM_NAME);
#else
XStoreName(_dpy,_window,W_PROG);
#endif
#ifdef W_MULTITHREAD
XInitThreads();
#endif
/*:231*//*232:*/
#line 5427 "cweb/weaver.w"

{
XSizeHints*hints= XAllocSizeHints();
hints->flags= PMinSize|PMaxSize;
hints->min_width= hints->max_width= W.width;
hints->min_height= hints->max_height= W.height;
XSetWMNormalHints(_dpy,_window,hints);
XFree(hints);
}
/*:232*//*233:*/
#line 5444 "cweb/weaver.w"

{
int glx_major,glx_minor;
Bool ret;
ret= glXQueryVersion(_dpy,&glx_major,&glx_minor);
if(!ret||((glx_major==1)&&(glx_minor<3))||glx_major<1){
fprintf(stderr,
"ERROR: GLX is version %d.%d, but should be at least 1.3.\n",
glx_major,glx_minor);
exit(1);
}
}
/*:233*//*237:*/
#line 5482 "cweb/weaver.w"

{
int return_value;
int doubleBufferAttributes[]= {
GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
GLX_RENDER_TYPE,GLX_RGBA_BIT,
GLX_DOUBLEBUFFER,True,
GLX_RED_SIZE,1,
GLX_GREEN_SIZE,1,
GLX_BLUE_SIZE,1,
GLX_ALPHA_SIZE,1,
GLX_DEPTH_SIZE,1,
None
};
fbConfigs= glXChooseFBConfig(_dpy,_screen,doubleBufferAttributes,
&return_value);
if(fbConfigs==NULL){
fprintf(stderr,
"ERROR: Not possible to choose our minimal OpenGL configuration.\n");
exit(1);
}
}
/*:237*//*239:*/
#line 5523 "cweb/weaver.w"

{
int context_attribs[]= 
{
GLX_CONTEXT_MAJOR_VERSION_ARB,3,
GLX_CONTEXT_MINOR_VERSION_ARB,3,
None
};
glXCreateContextAttribsARBProc glXCreateContextAttribsARB= 0;
{

const char*glxExts= glXQueryExtensionsString(_dpy,_screen);
if(strstr(glxExts,"GLX_ARB_create_context")==NULL){
fprintf(stderr,"ERROR: Can't create an OpenGL 3.0 context.\n");
exit(1);
}
}


glXCreateContextAttribsARB= (glXCreateContextAttribsARBProc)
glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");
_context= glXCreateContextAttribsARB(_dpy,*fbConfigs,NULL,GL_TRUE,
context_attribs);
if(_context==NULL){
fprintf(stderr,"ERROR: Couldn't create an OpenGL 3.0 context.\n");
exit(1);
}





glXMakeCurrent(_dpy,_window,_context);
}
/*:239*/
#line 5159 "cweb/weaver.w"

}
void _finalize_window(void){
/*240:*/
#line 5566 "cweb/weaver.w"

glXMakeCurrent(_dpy,None,NULL);
glXDestroyContext(_dpy,_context);
XDestroyWindow(_dpy,_window);
XCloseDisplay(_dpy);
/*:240*/
#line 5162 "cweb/weaver.w"

}
/*251:*/
#line 5777 "cweb/weaver.w"

void _Wresize_window(int width,int height){
int old_width,old_height;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_window_mutex);
#endif
XResizeWindow(_dpy,_window,width,height);
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
#line 5789 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_window_mutex);
#endif
}
/*:251*//*257:*/
#line 5846 "cweb/weaver.w"

void _Wmove_window(int x,int y){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_window_mutex);
#endif
XMoveWindow(_dpy,_window,x-W.width/2,W.resolution_y-y-W.height/2);
W.x= x;
W.y= y;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_window_mutex);
#endif
}
/*:257*/
#line 5164 "cweb/weaver.w"

#endif
/*:215*/
