/*358:*/
#line 7895 "./cweb/weaver.w"

#include "plugins.h"
/*:358*//*361:*/
#line 7965 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _initialize_plugin(struct _plugin_data*data,char*path){
struct stat attr;
char*p,buffer[256];
size_t plugin_name_length= strlen(data->plugin_name);
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


memcpy(buffer,"_init_plugin_",14);
memcpy(&buffer[13],data->plugin_name,plugin_name_length+1);
data->_init_plugin= dlsym(data->handle,buffer);
if(data->_init_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

memcpy(buffer,"_fini_plugin_",14);
memcpy(&buffer[13],data->plugin_name,plugin_name_length+1);
data->_fini_plugin= dlsym(data->handle,buffer);
if(data->_fini_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

memcpy(buffer,"_run_plugin_",13);
memcpy(&buffer[12],data->plugin_name,plugin_name_length+1);
data->_run_plugin= dlsym(data->handle,buffer);
if(data->_run_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

memcpy(buffer,"_enable_plugin_",16);
memcpy(&buffer[15],data->plugin_name,plugin_name_length+1);
data->_enable_plugin= dlsym(data->handle,buffer);
if(data->_enable_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define %s.\n",
data->plugin_name,buffer);

memcpy(buffer,"_disable_plugin_",17);
memcpy(&buffer[16],data->plugin_name,plugin_name_length+1);
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
/*:361*//*363:*/
#line 8100 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _finalize_plugin(struct _plugin_data*data){

data->defined= false;

if(data->_fini_plugin!=NULL)
data->_fini_plugin(&W);

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&(data->mutex))!=0)
fprintf(stderr,"ERROR: Finalizing plugin %s: couldn't destroy mutex.",
data->plugin_name);
#endif

if(dlclose(data->handle)!=0)
fprintf(stderr,"Error unlinking plugin %s: %s\n",data->plugin_name,
dlerror());
#if W_DEBUG_LEVEL >= 3
fprintf(stderr,"WARNING (3): Plugin finalized: %s.\n",data->plugin_name);
#endif
}
#endif
/*:363*//*365:*/
#line 8141 "./cweb/weaver.w"

#if W_TARGET == W_ELF
bool _reload_plugin(int plugin_id){
char buffer[256];
struct stat attr;
struct _plugin_data*data= &(_plugins[plugin_id]);
size_t string_length,plugin_name_length= strlen(data->plugin_name);
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


string_length= 13;
memcpy(buffer,"_init_plugin_",string_length+1);
memcpy(&buffer[13],data->plugin_name,plugin_name_length+1);
data->_init_plugin= dlsym(data->handle,buffer);
if(data->_init_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _init_plugin_%s.\n",
data->plugin_name,data->plugin_name);

string_length= 13;
memcpy(buffer,"_fini_plugin_",string_length+1);
memcpy(&buffer[13],data->plugin_name,plugin_name_length+1);
data->_fini_plugin= dlsym(data->handle,buffer);
if(data->_fini_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _fini_plugin_%s.\n",
data->plugin_name,data->plugin_name);

string_length= 12;
memcpy(buffer,"_run_plugin_",string_length+1);
memcpy(&buffer[12],data->plugin_name,plugin_name_length+1);
data->_run_plugin= dlsym(data->handle,buffer);
if(data->_run_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _run_plugin_%s.\n",
data->plugin_name,data->plugin_name);

string_length= 15;
memcpy(buffer,"_enable_plugin_",string_length+1);
memcpy(&buffer[15],data->plugin_name,plugin_name_length+1);
data->_enable_plugin= dlsym(data->handle,buffer);
if(data->_enable_plugin==NULL)
fprintf(stderr,"ERROR: Plugin %s doesn't define _enable_plugin_%s.\n",
data->plugin_name,data->plugin_name);

string_length= 16;
memcpy(buffer,"_disable_plugin_",string_length+1);
memcpy(&buffer[16],data->plugin_name,plugin_name_length+1);
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
/*:365*//*368:*/
#line 8255 "./cweb/weaver.w"

#if W_TARGET == W_WEB
bool _reload_plugin(int plugin_id){
return(bool)(plugin_id+1);
}
#endif
/*:368*//*376:*/
#line 8462 "./cweb/weaver.w"

int _Wget_plugin(char*plugin_name){
int i;
for(i= 0;i<_max_number_of_plugins;i++)
if(!strcmp(plugin_name,_plugins[i].plugin_name))
return i;
return-1;
}
/*:376*//*380:*/
#line 8496 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _reload_all_plugins(void){
if(strcmp(W_PLUGIN_PATH,"")){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(_plugin_mutex));
#endif
char*begin= W_PLUGIN_PATH,*end= W_PLUGIN_PATH;
char dir[256];
DIR*directory;
size_t dir_length,d_name_length;
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
dir_length= (end-begin);

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

d_name_length= strlen(dp->d_name);
if(dir_length+1+d_name_length> 255){
fprintf(stderr,"Ignoring plugin with too long path: %s/%s.\n",
dir,dp->d_name);
continue;
}
memcpy(&dir[dir_length],"/",2);
memcpy(&dir[dir_length+1],dp->d_name,d_name_length);
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
/*:380*//*384:*/
#line 8608 "./cweb/weaver.w"

#if W_TARGET == W_WEB
void _reload_all_plugins(void){
return;
}
#endif
/*:384*//*407:*/
#line 9013 "./cweb/weaver.w"

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
/*:407*//*411:*/
#line 9103 "./cweb/weaver.w"

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
/*:411*/
