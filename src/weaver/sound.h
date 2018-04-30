/*517:*/
#line 11351 "cweb/weaver.w"

#ifndef _sound_h_
#define _sound_h_
#ifdef __cplusplus
extern"C"{
#endif
#include <AL/al.h> 
#include <AL/alc.h> 
#include "weaver.h"
/*66:*/
#line 1928 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 11360 "cweb/weaver.w"

/*522:*/
#line 11411 "cweb/weaver.w"

void _initialize_sound(void);
/*:522*//*524:*/
#line 11430 "cweb/weaver.w"

void _finalize_sound(void);
/*:524*//*534:*/
#line 11557 "cweb/weaver.w"

bool _select_sound_device(int position);
/*:534*//*538:*/
#line 11590 "cweb/weaver.w"

int _current_sound_device(void);
/*:538*//*566:*/
#line 12185 "cweb/weaver.w"

struct sound{
unsigned long size;
int channels,freq,bitrate;
ALuint _data;
bool loaded;
};
/*:566*//*567:*/
#line 12202 "cweb/weaver.w"

struct sound*_new_sound(char*filename);
/*:567*//*575:*/
#line 12400 "cweb/weaver.w"

void _play_sound(struct sound*snd);
/*:575*//*579:*/
#line 12435 "cweb/weaver.w"

void _destroy_sound(struct sound*snd);
/*:579*//*738:*/
#line 16414 "cweb/weaver.w"

#if W_TARGET == W_ELF
#include <semaphore.h> 
#ifndef W_DISABLE_MP3
#include <mpg123.h> 
#endif
#endif
/*:738*//*740:*/
#line 16442 "cweb/weaver.w"

struct _music_data{
char filename[W_MAX_SUBLOOP][256];
int status[W_MAX_SUBLOOP];
float volume[W_MAX_SUBLOOP];
bool loop[W_MAX_SUBLOOP];
#if W_TARGET == W_ELF
unsigned char*buffer;
size_t buffer_size;

pthread_t thread;
sem_t semaphore;
#ifndef W_DISABLE_MP3

mpg123_handle*mpg_handle;
#endif

ALuint sound_source,openal_buffer[2];
#endif
};
/*:740*//*741:*/
#line 16466 "cweb/weaver.w"

extern struct _music_data _music[W_MAX_MUSIC];
#ifdef W_MULTITHREAD


extern pthread_mutex_t _music_mutex;
#endif
/*:741*//*743:*/
#line 16486 "cweb/weaver.w"

#define _NOT_LOADED 0
#define _PLAYING    1
#define _PAUSED     2
/*:743*//*748:*/
#line 16598 "cweb/weaver.w"

bool _play_music(char*,bool);
/*:748*//*752:*/
#line 16677 "cweb/weaver.w"

bool _pause_music(char*);
/*:752*//*756:*/
#line 16738 "cweb/weaver.w"

bool _resume_music(char*);
/*:756*//*758:*/
#line 16783 "cweb/weaver.w"

bool _stop_music(char*);
/*:758*//*762:*/
#line 16842 "cweb/weaver.w"

float _get_volume(char*);
/*:762*//*766:*/
#line 16875 "cweb/weaver.w"

float _increase_volume(char*,float);
/*:766*//*776:*/
#line 17109 "cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
void*_music_thread(void*);
#endif
/*:776*/
#line 11361 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:517*/
