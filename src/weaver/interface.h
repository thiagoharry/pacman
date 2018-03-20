/*391:*/
#line 8700 "cweb/weaver.w"

#ifndef _interface_h_
#define _interface_h_
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
#line 8707 "cweb/weaver.w"

/*394:*/
#line 8738 "cweb/weaver.w"

struct interface{
int type;
int integer;

float x,y;
float rotation;
float r,g,b,a;
float height,width;
bool visible;

GLfloat _transform_matrix[16];

GLenum _mode;
/*641:*/
#line 14273 "cweb/weaver.w"


GLuint*_texture;
bool _loaded_texture;
bool animate;
unsigned number_of_frames;
unsigned current_frame;
unsigned*frame_duration;
unsigned long _t;
int max_repetition;
/*:641*/
#line 8752 "cweb/weaver.w"


#ifdef W_MULTITHREAD
pthread_mutex_t _mutex;
#endif
}_interfaces[W_MAX_SUBLOOP][W_MAX_INTERFACES];
#ifdef W_MULTITHREAD


pthread_mutex_t _interface_mutex;
#endif
/*:394*//*395:*/
#line 8771 "cweb/weaver.w"

#define W_NONE                 0
#define W_INTERFACE_SQUARE    -1
#define W_INTERFACE_PERIMETER -2
/*:395*//*398:*/
#line 8815 "cweb/weaver.w"

void _flush_interfaces(void);
/*:398*//*403:*/
#line 8887 "cweb/weaver.w"

GLfloat _interface_vertices[12];

GLuint _interface_VBO;

GLuint _interface_VAO;
/*:403*//*406:*/
#line 8944 "cweb/weaver.w"

struct interface*_new_interface(int type,int x,int y,int width,
int height,...);
/*:406*//*410:*/
#line 9042 "cweb/weaver.w"

bool _destroy_interface(struct interface*inter);
/*:410*//*414:*/
#line 9096 "cweb/weaver.w"

struct interface*_copy_interface(struct interface*inter);
/*:414*//*419:*/
#line 9159 "cweb/weaver.w"

void _move_interface(struct interface*,float x,float y);
/*:419*//*423:*/
#line 9251 "cweb/weaver.w"

void _resize_interface(struct interface*inter,float size_x,float size_y);
/*:423*//*427:*/
#line 9330 "cweb/weaver.w"

void _rotate_interface(struct interface*inter,float rotation);
/*:427*//*434:*/
#line 9552 "cweb/weaver.w"

void _update_interface_screen_size(void);
/*:434*//*464:*/
#line 10383 "cweb/weaver.w"

struct interface*_interface_queue[W_MAX_SUBLOOP][W_MAX_INTERFACES];
/*:464*//*466:*/
#line 10410 "cweb/weaver.w"

void _insert_interface_queue(struct interface*inter);
/*:466*//*468:*/
#line 10458 "cweb/weaver.w"

void _remove_interface_queue(struct interface*inter);
/*:468*//*470:*/
#line 10527 "cweb/weaver.w"

void _clean_interface_queue(void);
/*:470*//*646:*/
#line 14342 "cweb/weaver.w"

#define W_INTERFACE_IMAGE -3
/*:646*//*652:*/
#line 14564 "cweb/weaver.w"

void _finalize_interface_texture(void*);
/*:652*//*668:*/
#line 14939 "cweb/weaver.w"

#if !defined(W_DISABLE_PNG) && (W_TARGET == W_ELF)
GLuint*_extract_png(char*,unsigned*,unsigned**,int*,bool*);
#endif
/*:668*/
#line 8708 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:391*/
