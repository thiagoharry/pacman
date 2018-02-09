/*332:*/
#line 7381 "cweb/weaver.w"

#include "plugins.h"
#if W_TARGET == W_ELF
#include <error.h> 
#endif
/*:332*//*335:*/
#line 7454 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _initialize_plugin(struct _plugin_data*data,char*path){
struct stat attr;
char*p,buffer[256];
int i;
#ifdef W_PREVENT_SELF_ENABLING_PLUGINS
data->finished_initialization= false;
#endif
#if W_DEBUG_LEVEL >= 1
if(strlen(path)>=128){
fprintf(stderr,"ERROR: Plugin path bigger than 255 characters: %s\n",
path);
return;
}
#endif
strncpy(data->library,path,255);





data->handle= dlopen(data->library,RTLD_NOW);
if(!(data->handle)){
fprintf(stderr,"%s\n",dlerror());
return;
}
dlerror();
if(stat(data->library,&attr)==-1){
perror("_initialize_plugin:");
return;
}
data->id= attr.st_ino;
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&(data->mutex),NULL)!=0){
perror("_initialize_plugin:");
return;
}
#endif
p= basename(data->library);
for(i= 0;*p!='.';i++){
if(i> 127){
fprintf(stderr,"ERROR: Plugin name bigger than 127 characters: %s\n",
path);
return;
}
data->plugin_name[i]= *p;
p++;
}
data->plugin_name[i]= '\0';

buffer[0]= '\0';
strcat(buffer,"_init_plugin_");
strcat(buffer,data->plugin_name);
data->_init_plugin= dlsym(data->handle,buffer);
if(data->_init_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

buffer[0]= '\0';
strcat(buffer,"_fini_plugin_");
strcat(buffer,data->plugin_name);
data->_fini_plugin= dlsym(data->handle,buffer);
if(data->_fini_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

buffer[0]= '\0';
strcat(buffer,"_run_plugin_");
strcat(buffer,data->plugin_name);
data->_run_plugin= dlsym(data->handle,buffer);
if(data->_run_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

buffer[0]= '\0';
strcat(buffer,"_enable_plugin_");
strcat(buffer,data->plugin_name);
data->_enable_plugin= dlsym(data->handle,buffer);
if(data->_enable_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

buffer[0]= '\0';
strcat(buffer,"_disable_plugin_");
strcat(buffer,data->plugin_name);
data->_disable_plugin= dlsym(data->handle,buffer);
if(data->_disable_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);





data->plugin_data= NULL;
data->enabled= false;
data->defined= true;
if(data->_init_plugin!=NULL)
data->_init_plugin(&W);
#ifdef W_PREVENT_SELF_ENABLING_PLUGINS
data->finished_initialization= true;
#endif
#if W_DEBUG_LEVEL >= 3
fprintf(stderr,"WARNING (3): New plugin loaded: %s.\n",data->plugin_name);
#endif
}
#endif
/*:335*//*337:*/
#line 7592 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _finalize_plugin(struct _plugin_data*data){

data->defined= false;

if(data->_fini_plugin!=NULL)
data->_fini_plugin(&W);

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&(data->mutex))!=0)
error(0,1,"Finalizing plugin %s",data->plugin_name);
#endif

if(dlclose(data->handle)!=0)
fprintf(stderr,"Error unlinking plugin %s: %s\n",data->plugin_name,
dlerror());
#if W_DEBUG_LEVEL >= 3
fprintf(stderr,"WARNING (3): Plugin finalized: %s.\n",data->plugin_name);
#endif
}
#endif
/*:337*//*339:*/
#line 7632 "cweb/weaver.w"

#if W_TARGET == W_ELF
bool _reload_plugin(int plugin_id){
char buffer[256];
struct stat attr;
struct _plugin_data*data= &(_plugins[plugin_id]);
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(data->mutex));
#endif



if(stat(_plugins[plugin_id].library,&attr)==-1){
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(data->mutex));
#endif
return false;
}
if(data->id==attr.st_ino){

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(data->mutex));
#endif
return true;
}

data->id= attr.st_ino;

if(dlclose(data->handle)!=0){
fprintf(stderr,"Error unlinking plugin %s: %s\n",data->plugin_name,
dlerror());
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(data->mutex));
#endif
return false;
}

data->handle= dlopen(data->library,RTLD_NOW);
if(!(data->handle)){
fprintf(stderr,"%s\n",dlerror());
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(data->mutex));
#endif
return false;
}
dlerror();


buffer[0]= '\0';
strcat(buffer,"_init_plugin_");
strcat(buffer,data->plugin_name);
data->_init_plugin= dlsym(data->handle,buffer);
if(data->_init_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _init_plugin_%s.\n",
data->plugin_name,data->plugin_name);

buffer[0]= '\0';
strcat(buffer,"_fini_plugin_");
strcat(buffer,data->plugin_name);
data->_fini_plugin= dlsym(data->handle,buffer);
if(data->_fini_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _fini_plugin_%s.\n",
data->plugin_name,data->plugin_name);

buffer[0]= '\0';
strcat(buffer,"_run_plugin_");
strcat(buffer,data->plugin_name);
data->_run_plugin= dlsym(data->handle,buffer);
if(data->_run_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _run_plugin_%s.\n",
data->plugin_name,data->plugin_name);

buffer[0]= '\0';
strcat(buffer,"_enable_plugin_");
strcat(buffer,data->plugin_name);
data->_enable_plugin= dlsym(data->handle,buffer);
if(data->_enable_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _enable_plugin_%s.\n",
data->plugin_name,data->plugin_name);

buffer[0]= '\0';
strcat(buffer,"_disable_plugin_");
strcat(buffer,data->plugin_name);
data->_disable_plugin= dlsym(data->handle,buffer);
if(data->_disable_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _disable_plugin_%s.\n",
data->plugin_name,data->plugin_name);
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(data->mutex));
#endif
#if W_DEBUG_LEVEL >= 3
fprintf(stderr,"WARNING (3): New plugin reloaded: %s.\n",
data->plugin_name);
#endif
return true;
}
#endif
/*:339*//*342:*/
#line 7745 "cweb/weaver.w"

#if W_TARGET == W_WEB
bool _reload_plugin(int plugin_id){
return(bool)(plugin_id+1);
}
#endif
/*:342*//*350:*/
#line 7949 "cweb/weaver.w"

int _Wget_plugin(char*plugin_name){
int i;
for(i= 0;i<_max_number_of_plugins;i++)
if(!strcmp(plugin_name,_plugins[i].plugin_name))
return i;
return-1;
}
/*:350*//*354:*/
#line 7983 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _reload_all_plugins(void){
if(strcmp(W_PLUGIN_PATH,"")){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(_plugin_mutex));
#endif
char*begin= W_PLUGIN_PATH,*end= W_PLUGIN_PATH;
char dir[256];
DIR*directory;
struct dirent*dp;
while(*end!='\0'){
end++;
while(*end!=':'&&*end!='\0')end++;

if(end-begin> 255){

begin= end+1;
continue;
}
strncpy(dir,begin,(size_t)(end-begin));
dir[(end-begin)]= '\0';

directory= opendir(dir);
if(directory==NULL){



begin= end+1;
continue;
}

while((dp= readdir(directory))!=NULL){
if(dp->d_name[0]!='.'&&dp->d_type==DT_REG){
char buffer[128];
int id,i;
strncpy(buffer,dp->d_name,128);
buffer[127]= '\0';
for(i= 0;buffer[i]!='.'&&buffer[i]!='\0';i++);
buffer[i]= '\0';
id= W.get_plugin(buffer);
if(id!=-1){
if(!W.reload_plugin(id)){
_plugins[id].defined= false;

}
}
else{

if(strlen(dir)+1+strlen(dp->d_name)> 255){
fprintf(stderr,"Ignoring plugin with too long path: %s/%s.\n",
dir,dp->d_name);
continue;
}
strcat(dir,"/");
strcat(dir,dp->d_name);
for(i= 0;i<_max_number_of_plugins;i++){
if(_plugins[i].defined==false){
_initialize_plugin(&(_plugins[i]),dir);
break;
}
}
if(i==_max_number_of_plugins){
fprintf(stderr,"WARNING (0): Maximum number of plugins achieved."
" Couldn't load %s.\n",buffer);
}
}
}
}

begin= end+1;
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(_plugin_mutex));
#endif
}
}
#endif
/*:354*//*358:*/
#line 8092 "cweb/weaver.w"

#if W_TARGET == W_WEB
void _reload_all_plugins(void){
return;
}
#endif
/*:358*//*381:*/
#line 8489 "cweb/weaver.w"

bool _Wenable_plugin(int plugin_id){
#ifdef W_PREVENT_SELF_ENABLING_PLUGINS
if(_plugins[plugin_id].finished_initialization==false)
return false;
#endif
if(plugin_id>=_max_number_of_plugins||
!(_plugins[plugin_id].defined))
return false;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(_plugins[plugin_id].mutex));
#endif
if(!_plugins[plugin_id].enabled){
_plugins[plugin_id].enabled= true;
if(_plugins[plugin_id]._enable_plugin!=NULL)
_plugins[plugin_id]._enable_plugin(&W);
#if W_DEBUG_LEVEL >=3
fprintf(stderr,"WARNING (3): Plugin enabled: %s.\n",
_plugins[plugin_id].plugin_name);
#endif
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(_plugins[plugin_id].mutex));
#endif
return true;
}
bool _Wdisable_plugin(int plugin_id){
if(plugin_id>=_max_number_of_plugins||
!(_plugins[plugin_id].defined))
return false;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(_plugins[plugin_id].mutex));
#endif
if(_plugins[plugin_id].enabled){
if(_plugins[plugin_id]._disable_plugin!=NULL)
_plugins[plugin_id]._disable_plugin(&W);
_plugins[plugin_id].enabled= false;
#if W_DEBUG_LEVEL >=3
fprintf(stderr,"WARNING (3): Plugin disabled: %s.\n",
_plugins[plugin_id].plugin_name);
#endif
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(_plugins[plugin_id].mutex));
#endif
return true;
}
bool _Wis_enabled(int plugin_id){
if(plugin_id>=_max_number_of_plugins||
!(_plugins[plugin_id].defined))
return false;
return _plugins[plugin_id].enabled;
}
/*:381*//*385:*/
#line 8579 "cweb/weaver.w"

#if W_TARGET == W_ELF
void*_Wget_plugin_data(int plugin_id){
if(plugin_id>=_max_number_of_plugins||
!(_plugins[plugin_id].defined))
return NULL;
return _plugins[plugin_id].plugin_data;
}
bool _Wset_plugin_data(int plugin_id,void*data){
if(plugin_id>=_max_number_of_plugins||
!(_plugins[plugin_id].defined))
return false;
_plugins[plugin_id].plugin_data= data;
return true;
}
#endif
/*:385*/
