/*331:*/
#line 7353 "cweb/weaver.w"

#ifndef _plugins_h_
#define _plugins_h_
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
#line 7360 "cweb/weaver.w"

#if W_TARGET == W_ELF
#include <dlfcn.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <string.h>  
#include <stdio.h>  
#include <libgen.h>  
#include <sys/types.h>  
#include <dirent.h>  
#include <errno.h> 
#endif
#include <stdbool.h> 
/*333:*/
#line 7391 "cweb/weaver.w"

struct _plugin_data{
#if W_TARGET == W_ELF
char library[256];
void*handle;
ino_t id;
#ifdef W_PREVENT_SELF_ENABLING_PLUGINS
bool finished_initialization;
#endif
#ifdef W_MULTITHREAD
pthread_mutex_t mutex;
#endif
#endif
char plugin_name[128];
void(*_init_plugin)(struct _weaver_struct*);
void(*_fini_plugin)(struct _weaver_struct*);
void(*_run_plugin)(struct _weaver_struct*);
void(*_enable_plugin)(struct _weaver_struct*);
void(*_disable_plugin)(struct _weaver_struct*);
void*plugin_data;
bool enabled,defined;
};
/*:333*//*334:*/
#line 7448 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _initialize_plugin(struct _plugin_data*data,char*path);
#endif
/*:334*//*336:*/
#line 7586 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _finalize_plugin(struct _plugin_data*data);
#endif
/*:336*//*338:*/
#line 7628 "cweb/weaver.w"

bool _reload_plugin(int plugin_id);
/*:338*//*344:*/
#line 7778 "cweb/weaver.w"

struct _plugin_data*_plugins;
int _max_number_of_plugins;
#ifdef W_MULTITHREAD
pthread_mutex_t _plugin_mutex;
#endif
/*:344*//*349:*/
#line 7945 "cweb/weaver.w"

int _Wget_plugin(char*plugin_name);
/*:349*//*353:*/
#line 7979 "cweb/weaver.w"

void _reload_all_plugins(void);
/*:353*//*380:*/
#line 8483 "cweb/weaver.w"

bool _Wenable_plugin(int plugin_id);
bool _Wdisable_plugin(int plugin_id);
bool _Wis_enabled(int plugin_id);
/*:380*//*384:*/
#line 8574 "cweb/weaver.w"

void*_Wget_plugin_data(int plugin_id);
bool _Wset_plugin_data(int plugin_id,void*data);
/*:384*/
#line 7375 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:331*/
