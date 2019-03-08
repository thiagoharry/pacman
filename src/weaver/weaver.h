/*70:*/
#line 1999 "./cweb/weaver.w"

#ifndef _weaver_h_
#define _weaver_h_
#ifdef __cplusplus
extern"C"{
#endif
/*66:*/
#line 1950 "./cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 2005 "./cweb/weaver.w"

#include <stdlib.h> 
#include <stdbool.h> 
#if W_TARGET == W_WEB
#include <emscripten.h> 
#endif
/*71:*/
#line 2041 "./cweb/weaver.w"

void _awake_the_weaver(void);
void _may_the_weaver_sleep(void)__attribute__((noreturn));
void _update(void);
void _render(void);
#define Winit() _awake_the_weaver()
#define Wexit() _may_the_weaver_sleep()
/*:71*//*79:*/
#line 2167 "./cweb/weaver.w"

struct timeval _last_time;
/*:79*//*81:*/
#line 2184 "./cweb/weaver.w"

unsigned long _update_time(void);
/*:81*//*84:*/
#line 2230 "./cweb/weaver.w"

extern struct _game_struct _game;
/*:84*//*89:*/
#line 2392 "./cweb/weaver.w"

#include "memory.h"
/*:89*//*163:*/
#line 3873 "./cweb/weaver.w"

#include <stdio.h> 
#include <sys/time.h> 

#define W_TIMER_BEGIN() { struct timeval _begin, _end; \
gettimeofday(&_begin, NULL);
#define W_TIMER_END() gettimeofday(&_end, NULL); \
printf("%ld us\n", (1000000 * (_end.tv_sec - _begin.tv_sec) + \
_end.tv_usec - _begin.tv_usec)); \
}

/*:163*//*169:*/
#line 4189 "./cweb/weaver.w"

typedef void MAIN_LOOP;







bool _loop_begin,_running_loop;
/*:169*//*170:*/
#line 4215 "./cweb/weaver.w"

void _Wloop(MAIN_LOOP(*f)(void))__attribute__((noreturn));
void Wsubloop(MAIN_LOOP(*f)(void))__attribute__((noreturn));
#define Wloop(a) ((W.pending_files)?(false):(_Wloop(a)))
/*:170*//*171:*/
#line 4251 "./cweb/weaver.w"

bool _first_loop;

int _number_of_loops;
MAIN_LOOP(*_loop_stack[W_MAX_SUBLOOP])(void);
/*:171*//*174:*/
#line 4304 "./cweb/weaver.w"

void _exit_loop(void)__attribute__((noreturn));
/*:174*//*176:*/
#line 4344 "./cweb/weaver.w"

#define Wexit_loop() (_running_loop =  false)
/*:176*//*179:*/
#line 4424 "./cweb/weaver.w"

long _lag;
/*:179*//*182:*/
#line 4443 "./cweb/weaver.w"

#define LOOP_INIT if(!_running_loop && !W.pending_files) _exit_loop();\
  if(!_running_loop)                                                    \
   goto _LOOP_FINALIZATION; if(!_loop_begin) goto _END_LOOP_INITIALIZATION;\
   _BEGIN_LOOP_INITIALIZATION
#define LOOP_BODY _loop_begin =   false; if(_loop_begin)\
   goto _BEGIN_LOOP_INITIALIZATION; _END_LOOP_INITIALIZATION:\
   _lag +=   _update_time(); while(_lag >= 40000){ _update(); _LABEL_0
#define LOOP_END _lag -=   40000; W.t +=   40000; } \
   _render(); if(_running_loop || W.pending_files) return; \
   if(W.t == 0) goto _LABEL_0;\
   _LOOP_FINALIZATION
/*:182*//*189:*/
#line 4605 "./cweb/weaver.w"

#include "numeric.h"
/*:189*//*215:*/
#line 5102 "./cweb/weaver.w"

#include "trie.h"
/*:215*//*233:*/
#line 5510 "./cweb/weaver.w"

#include <GL/glew.h> 
/*:233*//*237:*/
#line 5591 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
#include <pthread.h> 
extern pthread_mutex_t _window_mutex;
#endif
/*:237*//*239:*/
#line 5609 "./cweb/weaver.w"

#if W_TARGET == W_ELF
#include "window.h"
#endif
/*:239*//*245:*/
#line 5718 "./cweb/weaver.w"

#if W_TARGET == W_ELF
#include <X11/Xlib.h> 
extern Display*_dpy;
#endif
/*:245*//*248:*/
#line 5754 "./cweb/weaver.w"

#if W_TARGET == W_ELF
extern int _screen;
#endif
/*:248*//*253:*/
#line 5796 "./cweb/weaver.w"

#if W_TARGET == W_ELF
#include <X11/Xlib.h> 
extern Window _window;
#endif
/*:253*//*261:*/
#line 5980 "./cweb/weaver.w"

#if W_TARGET == W_ELF
#include <GL/glx.h> 
extern GLXContext _context;
#endif
/*:261*//*268:*/
#line 6095 "./cweb/weaver.w"

#if W_TARGET == W_WEB
#include "canvas.h"
#endif
/*:268*//*298:*/
#line 6609 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_t _input_mutex;
#endif

/*:298*//*301:*/
#line 6670 "./cweb/weaver.w"

#define W_SHIFT  2 
#define W_CTRL   3 
#define W_ALT    4 
#define W_ANY    6 
/*:301*//*303:*/
#line 6707 "./cweb/weaver.w"

int _Wkey_translate(unsigned old_value,unsigned new_value);
void _Werase_key_translations(void);
/*:303*//*311:*/
#line 6931 "./cweb/weaver.w"

#if W_TARGET == W_ELF
#define W_UP          XK_Up
#define W_RIGHT       XK_Right
#define W_DOWN        XK_Down
#define W_LEFT        XK_Left
#define W_PLUS        XK_KP_Add
#define W_MINUS       XK_KP_Subtract
#define W_ESC         XK_Escape
#define W_A           XK_a
#define W_S           XK_s
#define W_D           XK_d
#define W_W           XK_w
#define W_ENTER       XK_Return
#define W_SPACEBAR    XK_space
#define W_LEFT_CTRL   XK_Control_L
#define W_RIGHT_CTRL  XK_Control_R
#define W_F1          XK_F1
#define W_F2          XK_F2
#define W_F3          XK_F3
#define W_F4          XK_F4
#define W_F5          XK_F5
#define W_F6          XK_F6
#define W_F7          XK_F7
#define W_F8          XK_F8
#define W_F9          XK_F9
#define W_F10         XK_F10
#define W_F11         XK_F11
#define W_F12         XK_F12
#define W_BACKSPACE   XK_BackSpace
#define W_DELETE      XK_Delete
#define W_TAB         XK_Tab
#define W_PAUSE       XK_Pause
#define W_SCROLL_LOCK XK_Scroll_Lock
#define W_HOME        XK_Home
#define W_PAGE_UP     XK_Page_Up
#define W_PAGE_DOWN   XK_Page_Down
#define W_END         XK_End
#define W_INSERT      XK_Insert
#define W_NUM_LOCK    XK_Num_Lock
#define W_0           XK_0
#define W_1           XK_1
#define W_2           XK_2
#define W_3           XK_3
#define W_4           XK_4
#define W_5           XK_5
#define W_6           XK_6
#define W_7           XK_7
#define W_8           XK_8
#define W_9           XK_9
#define W_LEFT_SHIFT  XK_Shift_L
#define W_RIGHT_SHIFT XK_Shift_R
#define W_CAPS_LOCK   XK_Caps_Lock
#define W_LEFT_ALT    XK_Alt_L
#define W_RIGHT_ALT   XK_Alt_R
#define W_Q           XK_q
#define W_E           XK_e
#define W_R           XK_r
#define W_T           XK_t
#define W_Y           XK_y
#define W_U           XK_u
#define W_I           XK_i
#define W_O           XK_o
#define W_P           XK_p
#define W_F           XK_f
#define W_G           XK_g
#define W_H           XK_h
#define W_J           XK_j
#define W_K           XK_k
#define W_L           XK_l
#define W_Z           XK_z
#define W_X           XK_x
#define W_C           XK_c
#define W_V           XK_v
#define W_B           XK_b
#define W_N           XK_n
#define W_M           XK_m
#endif
/*:311*//*312:*/
#line 7023 "./cweb/weaver.w"

void _Wflush_input(void);
/*:312*//*317:*/
#line 7076 "./cweb/weaver.w"

#if W_TARGET == W_WEB
#include <SDL/SDL.h> 
#endif
/*:317*//*320:*/
#line 7144 "./cweb/weaver.w"

#if W_TARGET == W_WEB
#define W_UP          SDLK_UP
#define W_RIGHT       SDLK_RIGHT
#define W_DOWN        SDLK_DOWN
#define W_LEFT        SDLK_LEFT
#define W_PLUS        SDLK_PLUS
#define W_MINUS       SDLK_MINUS
#define W_ESC         SDLK_ESCAPE
#define W_A           SDLK_a
#define W_S           SDLK_s
#define W_D           SDLK_d
#define W_W           SDLK_w
#define W_ENTER       SDLK_RETURN
#define W_SPACEBAR    SDLK_SPACE
#define W_LEFT_CTRL   SDLK_LCTRL
#define W_RIGHT_CTRL  SDLK_RCTRL
#define W_F1          SDLK_F1
#define W_F2          SDLK_F2
#define W_F3          SDLK_F3
#define W_F4          SDLK_F4
#define W_F5          SDLK_F5
#define W_F6          SDLK_F6
#define W_F7          SDLK_F7
#define W_F8          SDLK_F8
#define W_F9          SDLK_F9
#define W_F10         SDLK_F10
#define W_F11         SDLK_F11
#define W_F12         SDLK_F12
#define W_BACKSPACE   SDLK_BACKSPACE
#define W_TAB         SDLK_TAB
#define W_PAUSE       SDLK_PAUSE
#define W_DELETE      SDLK_DELETE
#define W_SCROLL_LOCK SDLK_SCROLLOCK
#define W_HOME        SDLK_HOME
#define W_PAGE_UP     SDLK_PAGEUP
#define W_PAGE_DOWN   SDLK_PAGEDOWN
#define W_END         SDLK_END
#define W_INSERT      SDLK_INSERT
#define W_NUM_LOCK    SDLK_NUMLOCK
#define W_0           SDLK_0
#define W_1           SDLK_1
#define W_2           SDLK_2
#define W_3           SDLK_3
#define W_4           SDLK_4
#define W_5           SDLK_5
#define W_6           SDLK_6
#define W_7           SDLK_7
#define W_8           SDLK_8
#define W_9           SDLK_9
#define W_LEFT_SHIFT  SDLK_LSHIFT
#define W_RIGHT_SHIFT SDLK_RSHIFT
#define W_CAPS_LOCK   SDLK_CAPSLOCK
#define W_LEFT_ALT    SDLK_LALT
#define W_RIGHT_ALT   SDLK_RALT
#define W_Q           SDLK_q
#define W_E           SDLK_e
#define W_R           SDLK_r
#define W_T           SDLK_t
#define W_Y           SDLK_y
#define W_U           SDLK_u
#define W_I           SDLK_i
#define W_O           SDLK_o
#define W_P           SDLK_p
#define W_F           SDLK_f
#define W_G           SDLK_g
#define W_H           SDLK_h
#define W_J           SDLK_j
#define W_K           SDLK_k
#define W_L           SDLK_l
#define W_Z           SDLK_z
#define W_X           SDLK_x
#define W_C           SDLK_c
#define W_V           SDLK_v
#define W_B           SDLK_b
#define W_N           SDLK_n
#define W_M           SDLK_m
#endif
/*:320*//*324:*/
#line 7260 "./cweb/weaver.w"

struct _mouse{


long buttons[7];
int x,y,dx,dy,ddx,ddy;
};
/*:324*//*326:*/
#line 7278 "./cweb/weaver.w"

#if W_TARGET == W_ELF
#define W_MOUSE_LEFT   Button1
#define W_MOUSE_MIDDLE Button2
#define W_MOUSE_RIGHT  Button3
#define W_MOUSE_B1     Button4
#define W_MOUSE_B2     Button5
#endif
#if W_TARGET == W_WEB
#define W_MOUSE_LEFT   SDL_BUTTON_LEFT
#define W_MOUSE_MIDDLE SDL_BUTTON_MIDDLE
#define W_MOUSE_RIGHT  SDL_BUTTON_RIGHT
#define W_MOUSE_B1     4
#define W_MOUSE_B2     5
#endif
/*:326*//*344:*/
#line 7600 "./cweb/weaver.w"

bool _using_original_cursor;
void _Whide_cursor(void);
/*:344*//*392:*/
#line 8716 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_t _scheduler_mutex;
#endif
struct{
bool periodic;
unsigned long last_execution;
unsigned long period;
void(*f)(void);
}_scheduled_functions[W_MAX_SUBLOOP][W_MAX_SCHEDULING];
/*:392*//*398:*/
#line 8838 "./cweb/weaver.w"

void _run_periodically(void(*f)(void),float t);
void _run_futurelly(void(*f)(void),float t);
float _cancel(void(*f)(void));
float _period(void(*f)(void));
/*:398*//*419:*/
#line 9250 "./cweb/weaver.w"

#include "interface.h"
/*:419*//*467:*/
#line 10199 "./cweb/weaver.w"

#include <ctype.h>  
#include "shaders.h"
/*:467*//*507:*/
#line 11353 "./cweb/weaver.w"

#define W_NONE            0
#define W_DEFAULT_SHADER -1
/*:507*//*508:*/
#line 11358 "./cweb/weaver.w"










int _final_shader[W_MAX_SUBLOOP];


bool _changed_resolution;


GLuint _framebuffer;
/*:508*//*511:*/
#line 11404 "./cweb/weaver.w"



GLuint _texture;
/*:511*//*513:*/
#line 11443 "./cweb/weaver.w"


GLuint _depth_stencil;
/*:513*//*537:*/
#line 11846 "./cweb/weaver.w"

int _final_shader_integer[W_MAX_SUBLOOP];
/*:537*//*545:*/
#line 11951 "./cweb/weaver.w"

#include "sound.h"
/*:545*//*610:*/
#line 13060 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_t _finalizing_mutex;
#endif
struct _finalize_element{
void*data;
void(*finalize)(void*);
struct _finalize_element*prev,*next;
};
struct _finalize_element*_finalize_list[W_MAX_SUBLOOP];
/*:610*//*613:*/
#line 13104 "./cweb/weaver.w"

void _finalize_after(void*,void(*f)(void*));
/*:613*//*615:*/
#line 13146 "./cweb/weaver.w"

void _finalize_all(void);
/*:615*//*619:*/
#line 13184 "./cweb/weaver.w"

void _finalize_this(void*,bool);
/*:619*//*625:*/
#line 13323 "./cweb/weaver.w"

#include "gif.h"
/*:625*//*668:*/
#line 14956 "./cweb/weaver.w"

GLuint _empty_texture;
char _empty_image[4];
/*:668*//*716:*/
#line 16057 "./cweb/weaver.w"

#include "database.h"
/*:716*//*819:*/
#line 18139 "./cweb/weaver.w"

#include "metafont.h"
/*:819*/
#line 2011 "./cweb/weaver.w"


/*75:*/
#line 2111 "./cweb/weaver.w"



extern struct _weaver_struct{
/*78:*/
#line 2161 "./cweb/weaver.w"


unsigned long long t;
unsigned long dt;
/*:78*//*85:*/
#line 2234 "./cweb/weaver.w"


struct _game_struct*game;
/*:85*//*234:*/
#line 5519 "./cweb/weaver.w"


int resolution_x,resolution_y,framerate;
/*:234*//*236:*/
#line 5582 "./cweb/weaver.w"


int width,height,x,y;
/*:236*//*296:*/
#line 6589 "./cweb/weaver.w"


long keyboard[0x10000];
/*:296*//*325:*/
#line 7269 "./cweb/weaver.w"


struct _mouse mouse;
/*:325*//*532:*/
#line 11798 "./cweb/weaver.w"


int final_shader_integer;
/*:532*//*555:*/
#line 12032 "./cweb/weaver.w"


int number_of_sound_devices;
char**sound_device_name;
/*:555*//*575:*/
#line 12344 "./cweb/weaver.w"

unsigned pending_files;
#ifdef W_MULTITHREAD
pthread_mutex_t _pending_files_mutex;
#endif
/*:575*/
#line 2115 "./cweb/weaver.w"

/*116:*/
#line 3098 "./cweb/weaver.w"


#if W_DEBUG_LEVEL >= 1
void*(*create_arena)(size_t,char*,unsigned long);
#else
void*(*create_arena)(size_t);
#endif
/*:116*//*124:*/
#line 3224 "./cweb/weaver.w"


int(*destroy_arena)(void*);
/*:124*//*129:*/
#line 3349 "./cweb/weaver.w"


#if W_DEBUG_LEVEL >= 1
void*(*alloc_arena)(void*,size_t,char*,unsigned long);
#else
void*(*alloc_arena)(void*,size_t);
#endif
/*:129*//*133:*/
#line 3461 "./cweb/weaver.w"


#if W_DEBUG_LEVEL >= 2 && !defined(W_MULTITHREAD)
void(*free)(void*,char*,unsigned long);
#else
void(*free)(void*);
#endif
/*:133*//*139:*/
#line 3571 "./cweb/weaver.w"


#if W_DEBUG_LEVEL >= 1
int(*breakpoint_arena)(void*,char*,unsigned long);
#else
int(*breakpoint_arena)(void*);
#endif
/*:139*//*143:*/
#line 3637 "./cweb/weaver.w"


void(*trash_arena)(void*);
/*:143*//*154:*/
#line 3770 "./cweb/weaver.w"


#if W_DEBUG_LEVEL >= 1
void*(*alloc)(size_t,char*,unsigned long);
#else
void*(*alloc)(size_t);
#endif
/*:154*//*159:*/
#line 3838 "./cweb/weaver.w"


#if W_DEBUG_LEVEL >= 1
int(*breakpoint)(char*,unsigned long);
#else
int(*breakpoint)(void);
#endif
void(*trash)(void);
/*:159*//*205:*/
#line 4953 "./cweb/weaver.w"


unsigned long(*random)(void);
/*:205*//*280:*/
#line 6343 "./cweb/weaver.w"

void(*resize_window)(int,int);
/*:280*//*286:*/
#line 6389 "./cweb/weaver.w"

void(*move_window)(int,int);
/*:286*//*305:*/
#line 6763 "./cweb/weaver.w"

int(*key_translate)(unsigned,unsigned);
void(*erase_key_translations)(void);
/*:305*//*314:*/
#line 7053 "./cweb/weaver.w"

void(*flush_input)(void);
/*:314*//*348:*/
#line 7667 "./cweb/weaver.w"

void(*hide_cursor)(void);
/*:348*//*366:*/
#line 8244 "./cweb/weaver.w"

bool(*reload_plugin)(int);
/*:366*//*377:*/
#line 8474 "./cweb/weaver.w"

int(*get_plugin)(char*);
/*:377*//*381:*/
#line 8586 "./cweb/weaver.w"

void(*reload_all_plugins)(void);
/*:381*//*403:*/
#line 8984 "./cweb/weaver.w"


void(*run_periodically)(void(*f)(void),float);
void(*run_futurelly)(void(*f)(void),float);
float(*cancel)(void(*f)(void));
float(*period)(void(*f)(void));
/*:403*//*408:*/
#line 9073 "./cweb/weaver.w"

bool(*enable_plugin)(int);
bool(*disable_plugin)(int);
bool(*is_plugin_enabled)(int);
/*:408*//*412:*/
#line 9123 "./cweb/weaver.w"

void*(*get_plugin_data)(int);
bool(*set_plugin_data)(int,void*);
/*:412*//*434:*/
#line 9567 "./cweb/weaver.w"

struct interface*(*new_interface)(int,int,int,int,int,...);
/*:434*//*438:*/
#line 9615 "./cweb/weaver.w"

bool(*destroy_interface)(struct interface*);
/*:438*//*442:*/
#line 9681 "./cweb/weaver.w"

struct interface*(*copy_interface)(struct interface*);
/*:442*//*447:*/
#line 9779 "./cweb/weaver.w"

void(*move_interface)(struct interface*,float,float);
/*:447*//*451:*/
#line 9858 "./cweb/weaver.w"

void(*resize_interface)(struct interface*,float,float);
/*:451*//*455:*/
#line 9938 "./cweb/weaver.w"

void(*rotate_interface)(struct interface*,float);
/*:455*//*525:*/
#line 11754 "./cweb/weaver.w"

void(*change_resolution)(int,int);
/*:525*//*530:*/
#line 11787 "./cweb/weaver.w"

void(*change_final_shader)(int);
/*:530*//*562:*/
#line 12141 "./cweb/weaver.w"

bool(*select_sound_device)(int);
/*:562*//*566:*/
#line 12174 "./cweb/weaver.w"

int(*current_sound_device)(void);
/*:566*//*599:*/
#line 12954 "./cweb/weaver.w"

struct sound*(*new_sound)(char*);
/*:599*//*603:*/
#line 12986 "./cweb/weaver.w"

void(*play_sound)(struct sound*);
/*:603*//*607:*/
#line 13035 "./cweb/weaver.w"

void(*destroy_sound)(struct sound*);
/*:607*//*727:*/
#line 16253 "./cweb/weaver.w"

void(*write_integer)(char*,int);
/*:727*//*732:*/
#line 16325 "./cweb/weaver.w"

void(*write_float)(char*,float);
/*:732*//*737:*/
#line 16393 "./cweb/weaver.w"

void(*write_string)(char*,char*);
/*:737*//*743:*/
#line 16465 "./cweb/weaver.w"

bool(*read_integer)(char*,int*);
bool(*read_float)(char*,float*);
bool(*read_string)(char*,char*,int);
/*:743*//*753:*/
#line 16714 "./cweb/weaver.w"

void(*delete_integer)(char*);
void(*delete_float)(char*);
void(*delete_string)(char*);
void(*delete_all)(void);
/*:753*//*776:*/
#line 17242 "./cweb/weaver.w"

bool(*play_music)(char*,bool);
/*:776*//*780:*/
#line 17297 "./cweb/weaver.w"

bool(*pause_music)(char*);
/*:780*//*786:*/
#line 17401 "./cweb/weaver.w"

bool(*stop_music)(char*);
/*:786*//*792:*/
#line 17469 "./cweb/weaver.w"

float(*get_volume)(char*);
/*:792*//*796:*/
#line 17529 "./cweb/weaver.w"

float(*increase_volume)(char*,float);
/*:796*/
#line 2116 "./cweb/weaver.w"

}W;
/*:75*/
#line 2013 "./cweb/weaver.w"

/*356:*/
#line 7864 "./cweb/weaver.w"

#include "plugins.h"
/*:356*//*389:*/
#line 8676 "./cweb/weaver.w"

#if W_TARGET == W_WEB
#include "../../.hidden_code/header.h"
#endif
/*:389*/
#line 2014 "./cweb/weaver.w"

/*353:*/
#line 7808 "./cweb/weaver.w"


#define W_PLUGIN struct _weaver_struct *_W
#ifdef W_PLUGIN_CODE
#define W (*_W)
#endif
/*:353*/
#line 2015 "./cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:70*/
