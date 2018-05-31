/*517:*/
#line 11362 "cweb/weaver.w"

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
#line 11371 "cweb/weaver.w"

/*522:*/
#line 11422 "cweb/weaver.w"

void _initialize_sound(void);
/*:522*//*524:*/
#line 11441 "cweb/weaver.w"

void _finalize_sound(void);
/*:524*//*534:*/
#line 11568 "cweb/weaver.w"

bool _select_sound_device(int position);
/*:534*//*538:*/
#line 11601 "cweb/weaver.w"

int _current_sound_device(void);
/*:538*//*566:*/
#line 12196 "cweb/weaver.w"

struct sound{
unsigned long size;
int channels,freq,bitrate;
ALuint _data;
bool loaded;
};
/*:566*//*567:*/
#line 12213 "cweb/weaver.w"

struct sound*_new_sound(char*filename);
/*:567*//*575:*/
#line 12411 "cweb/weaver.w"

void _play_sound(struct sound*snd);
/*:575*//*579:*/
#line 12446 "cweb/weaver.w"

void _destroy_sound(struct sound*snd);
/*:579*//*738:*/
#line 16426 "cweb/weaver.w"

#if W_TARGET == W_ELF
#include <semaphore.h> 
#ifndef W_DISABLE_MP3
#include <mpg123.h> 
#endif
#endif
/*:738*//*740:*/
#line 16454 "cweb/weaver.w"

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
#line 16478 "cweb/weaver.w"

extern struct _music_data _music[W_MAX_MUSIC];
#ifdef W_MULTITHREAD


extern pthread_mutex_t _music_mutex;
#endif
/*:741*//*743:*/
#line 16498 "cweb/weaver.w"

#define _NOT_LOADED 0
#define _PLAYING    1
#define _PAUSED     2
/*:743*//*748:*/
#line 16610 "cweb/weaver.w"

bool _play_music(char*,bool);
/*:748*//*752:*/
#line 16689 "cweb/weaver.w"

bool _pause_music(char*);
/*:752*//*756:*/
#line 16751 "cweb/weaver.w"

bool _resume_music(char*);
/*:756*//*758:*/
#line 16796 "cweb/weaver.w"

bool _stop_music(char*);
/*:758*//*762:*/
#line 16855 "cweb/weaver.w"

float _get_volume(char*);
/*:762*//*766:*/
#line 16888 "cweb/weaver.w"

float _increase_volume(char*,float);
/*:766*//*776:*/
#line 17126 "cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
void*_music_thread(void*);
#endif
/*:776*/
#line 11372 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:517*/
