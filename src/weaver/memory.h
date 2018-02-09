/*90:*/
#line 2382 "cweb/weaver.w"

#ifndef _memory_h_
#define _memory_h_
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
#line 2389 "cweb/weaver.w"

/*92:*/
#line 2404 "cweb/weaver.w"

#include <sys/mman.h>  
#include <pthread.h>  
#include <string.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <math.h>  
#include <stdbool.h> 
/*:92*//*96:*/
#line 2588 "cweb/weaver.w"

struct _arena_header{
size_t total,used;
struct _breakpoint*last_breakpoint;
void*empty_position,*last_element;
#if W_DEBUG_LEVEL >= 1
char file[32];
unsigned long line;
#endif
#ifdef W_MULTITHREAD
pthread_mutex_t mutex;
#endif
#if W_DEBUG_LEVEL >= 3
size_t max_used;
#endif
};
/*:96*//*97:*/
#line 2610 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 4
void _assert__arena_header(struct _arena_header*);
#endif
/*:97*//*101:*/
#line 2745 "cweb/weaver.w"

struct _breakpoint{
unsigned long type;
#if W_DEBUG_LEVEL >= 1
char file[32];
unsigned long line;
#endif
void*last_element;
struct _arena_header*arena;







struct _breakpoint*last_breakpoint;
size_t size;
};
/*:101*//*102:*/
#line 2775 "cweb/weaver.w"

#define _BREAKPOINT_T  0x11010101
/*:102*//*103:*/
#line 2789 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _assert__breakpoint(struct _breakpoint*);
#endif
/*:103*//*108:*/
#line 2882 "cweb/weaver.w"

#define _DATA_T        0x10101010
/*:108*//*109:*/
#line 2913 "cweb/weaver.w"

struct _memory_header{
unsigned long type;
#if W_DEBUG_LEVEL >= 1
char file[32];
unsigned long line;
#endif
void*last_element;
struct _arena_header*arena;

size_t real_size,requested_size;
unsigned long flags;
};
/*:109*//*110:*/
#line 2940 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _assert__memory_header(struct _memory_header*);
#endif
/*:110*//*113:*/
#line 2994 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1


void*_Wcreate_arena(size_t size,char*filename,unsigned long line);
#else
void*_Wcreate_arena(size_t size);
#endif
int Wdestroy_arena(void*);
/*:113*//*118:*/
#line 3092 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1


#define Wcreate_arena(a) W.create_arena(a, __FILE__, __LINE__)
#else
#define Wcreate_arena(a) W.create_arena(a)
#endif
/*:118*//*120:*/
#line 3125 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _assert_no_memory_leak(void*);
#endif
/*:120*//*127:*/
#line 3223 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void*_alloc(void*arena,size_t size,char*filename,unsigned long line);
#else
void*_alloc(void*arena,size_t size);
#endif
#if W_DEBUG_LEVEL >= 2 && !defined(W_MULTITHREAD)
void _free(void*mem,char*filename,unsigned long line);
#else
void _free(void*mem);
#endif
/*:127*//*131:*/
#line 3342 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
#define Walloc_arena(a, b) W.alloc_arena(a, b, __FILE__, __LINE__)
#else
#define Walloc_arena(a, b) W.alloc_arena(a, b)
#endif
/*:131*//*135:*/
#line 3454 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 2 && !defined(W_MULTITHREAD)
#define Wfree(a) W.free(a, __FILE__, __LINE__)
#else
#define Wfree(a) W.free(a)
#endif
/*:135*//*137:*/
#line 3469 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
int _new_breakpoint(void*arena,char*filename,unsigned long line);
#else
int _new_breakpoint(void*arena);
#endif
void Wtrash_arena(void*arena);
/*:137*//*141:*/
#line 3564 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
#define Wbreakpoint_arena(a) W.breakpoint_arena(a, __FILE__, __LINE__)
#else
#define Wbreakpoint_arena(a) W.breakpoint_arena(a)
#endif
/*:141*//*147:*/
#line 3647 "cweb/weaver.w"

void _initialize_memory();
void _finalize_memory();
/*:147*//*151:*/
#line 3703 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void*_Walloc(size_t size,char*filename,unsigned long line);
void*_Winternal_alloc(size_t size,char*filename,unsigned long line);
#define _iWalloc(n) _Winternal_alloc(n, __FILE__, __LINE__)
#else
void*_Walloc(size_t size);
void*_Winternal_alloc(size_t size);
#define _iWalloc(n) _Winternal_alloc(n)
#endif
/*:151*//*155:*/
#line 3758 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
#define Walloc(a) W.alloc(a, __FILE__, __LINE__)
#else
#define Walloc(a) W.alloc(a)
#endif
/*:155*//*156:*/
#line 3770 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
int _Wbreakpoint(char*filename,unsigned long line);
int _iWbreakpoint_(char*filename,unsigned long line);
#else
int _Wbreakpoint(void);
int _iWbreakpoint_(void);
#endif
void _Wtrash(void);
void _iWtrash(void);
/*:156*//*160:*/
#line 3827 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
#define Wbreakpoint() W.breakpoint(__FILE__, __LINE__)
#define _iWbreakpoint() _iWbreakpoint_(__FILE__, __LINE__)
#else
#define Wbreakpoint() W.breakpoint()
#define _iWbreakpoint() _iWbreakpoint_()
#endif
#define Wtrash() W.trash()
/*:160*/
#line 2390 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:90*/
