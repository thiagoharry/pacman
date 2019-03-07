/*544:*/
#line 11931 "./cweb/weaver.w"

#include <string.h>  
#include <sys/stat.h>  
#include <sys/types.h>  
#include <time.h>  
#include <pthread.h> 
#ifdef W_MULTITHREAD
#include <sched.h> 
#endif
#include "sound.h"
#include "weaver.h"

#if W_TARGET == W_WEB
extern ALenum alGetError(void);
#endif

/*547:*/
#line 11964 "./cweb/weaver.w"

static ALCdevice*default_device;
/*:547*//*568:*/
#line 12194 "./cweb/weaver.w"

static ALCcontext*default_context= NULL;
/*:568*//*570:*/
#line 12219 "./cweb/weaver.w"

static ALuint default_source[5];
/*:570*//*578:*/
#line 12378 "./cweb/weaver.w"

static ALuint extract_wave(const char*filename,unsigned long*size,int*freq,
int*channels,int*bitrate,bool*error){
void*returned_data= NULL;
ALuint returned_buffer= 0;
FILE*fp= fopen(filename,"r");
*error= false;
if(fp==NULL){
*error= false;
return 0;
}
/*579:*/
#line 12398 "./cweb/weaver.w"

{
char data[5];
size_t size_t_ret;
data[0]= '\0';
size_t_ret= fread(data,1,4,fp);
data[4]= '\0';
if(strcmp(data,"RIFF")){
fprintf(stderr,"WARNING: Not compatible audio format: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
if(size_t_ret){};
}
/*:579*//*580:*/
#line 12423 "./cweb/weaver.w"

{
int i;
unsigned long multiplier= 1;
*size= 0;
for(i= 0;i<4;i++){
unsigned long size_tmp= 0;
if(fread(&size_tmp,1,1,fp)!=1){
fprintf(stderr,"WARNING: Damaged file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
*size+= size_tmp*multiplier;
multiplier*= 256;
}
}
/*:580*//*581:*/
#line 12448 "./cweb/weaver.w"

{
char data[5];
size_t size_t_ret;
data[0]= '\0';
size_t_ret= fread(data,1,4,fp);
data[4]= '\0';
if(size_t_ret){};
if(strcmp(data,"WAVE")){
fprintf(stderr,"WARNING: Not compatible audio format: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}


*size-= 4;
}
/*:581*//*582:*/
#line 12475 "./cweb/weaver.w"

{
int c,i;
for(i= 0;i<8;i++){
c= getc(fp);
if(c==EOF){
fprintf(stderr,"WARNING: Damaged audio file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
}
*size-= 8;
}
/*:582*//*583:*/
#line 12501 "./cweb/weaver.w"

{
int i,format= 0;
unsigned long multiplier= 1;
for(i= 0;i<2;i++){
unsigned long format_tmp= 0;
if(fread(&format_tmp,1,1,fp)!=1){
fprintf(stderr,"WARNING: Damaged file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
format+= format_tmp*multiplier;
multiplier*= 256;
}
if(format!=1){
fprintf(stderr,"WARNING: Not compatible WAVE file format: %s.\n",
filename);
fclose(fp);
*error= true;
return 0;
}


*size-= 2;
}
/*:583*//*584:*/
#line 12533 "./cweb/weaver.w"

{
int i;
*channels= 0;
unsigned long multiplier= 1;
for(i= 0;i<2;i++){
unsigned long channel_tmp= 0;
if(fread(&channel_tmp,1,1,fp)!=1){
fprintf(stderr,"WARNING: Damaged file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
*channels+= channel_tmp*multiplier;
multiplier*= 256;
}


*size-= 2;
}
/*:584*//*585:*/
#line 12558 "./cweb/weaver.w"

{
int i;
*freq= 0;
unsigned long multiplier= 1;
for(i= 0;i<4;i++){
unsigned long freq_tmp= 0;
if(fread(&freq_tmp,1,1,fp)!=1){
fprintf(stderr,"WARNING: Damaged file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
*freq+= freq_tmp*multiplier;
multiplier*= 256;
}


*size-= 4;
}
/*:585*//*586:*/
#line 12585 "./cweb/weaver.w"

{
int c,i;
for(i= 0;i<6;i++){
c= getc(fp);
if(c==EOF){
fprintf(stderr,"WARNING: Damaged audio file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
}
*size-= 6;
}
/*:586*//*587:*/
#line 12605 "./cweb/weaver.w"

{
int i;
*bitrate= 0;
unsigned long multiplier= 1;
for(i= 0;i<2;i++){
unsigned long bitrate_tmp= 0;
if(fread(&bitrate_tmp,1,1,fp)!=1){
fprintf(stderr,"WARNING: Damaged file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
*bitrate+= bitrate_tmp*multiplier;
multiplier*= 256;
}


*size-= 2;
}
/*:587*//*588:*/
#line 12632 "./cweb/weaver.w"

{
int c,i;
for(i= 0;i<8;i++){
c= getc(fp);
if(c==EOF){
fprintf(stderr,"WARNING: Damaged audio file: %s\n",
filename);
fclose(fp);
*error= true;
return 0;
}
}
*size-= 8;
}
/*:588*//*589:*/
#line 12652 "./cweb/weaver.w"

{
size_t size_t_ret;
returned_data= Walloc((size_t)*size);
if(returned_data==NULL){
printf("WARNING(0): Not enough memory to read file: %s.\n",
filename);
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): You should increase the value of "
"W_INTERNAL_MEMORY at conf/conf.h.\n");
#endif
fclose(fp);
*error= true;
return 0;
}
size_t_ret= fread(returned_data,*size,1,fp);
if(size_t_ret){};
}
/*:589*//*590:*/
#line 12676 "./cweb/weaver.w"

{
ALenum status;
ALuint format= 0;

alGetError();

alGenBuffers(1,&returned_buffer);
status= alGetError();
if(status!=AL_NO_ERROR){
fprintf(stderr,"WARNING(0)): No sound buffer could be created. "
"alGenBuffers failed. ");
if(status==AL_INVALID_VALUE){
fprintf(stderr,"Internal error: buffer array isn't large enough.\n");
}
else if(status==AL_OUT_OF_MEMORY){
fprintf(stderr,"Internal error: out of memory.\n");
}
else{
fprintf(stderr,"Unknown error (%d).\n",status);
}
Wfree(returned_data);
*error= true;
fclose(fp);
return 0;
}

format= 0xfff5;
if(*bitrate==8){
if(*channels==1)format= AL_FORMAT_MONO8;
else if(*channels==2)format= AL_FORMAT_STEREO8;
}else if(*bitrate==16){
if(*channels==1)format= AL_FORMAT_MONO16;
else if(*channels==2)format= AL_FORMAT_STEREO16;
}
if(format==0xfff5){
fprintf(stderr,"WARNING(0): Combination of channel and bitrate not "
"supported (sound have %d channels and %d bitrate while "
"we support just 1 or 2 channels and 8 or 16 as bitrate).\n",
*channels,*bitrate);
Wfree(returned_data);
alDeleteBuffers(1,&returned_buffer);
*error= true;
fclose(fp);
return 0;
}

alBufferData(returned_buffer,format,returned_data,(ALsizei)*size,
*freq);
status= alGetError();
if(status!=AL_NO_ERROR){
fprintf(stderr,"WARNING(0): Can't pass audio to OpenAL. "
"alBufferData failed. Sound may not work.\n");
Wfree(returned_data);
alDeleteBuffers(1,&returned_buffer);
*error= true;
fclose(fp);
return 0;
}

Wfree(returned_data);
fclose(fp);
}
/*:590*/
#line 12389 "./cweb/weaver.w"

return returned_buffer;
}
/*:578*//*768:*/
#line 17038 "./cweb/weaver.w"

struct _music_data _music[W_MAX_MUSIC];
#ifdef W_MULTITHREAD


pthread_mutex_t _music_mutex;
#endif
/*:768*/
#line 11947 "./cweb/weaver.w"

/*595:*/
#line 12852 "./cweb/weaver.w"



static void _finalize_openal(void*data){
ALuint*p= (ALuint*)data;
alDeleteBuffers(1,p);
}
/*:595*//*596:*/
#line 12878 "./cweb/weaver.w"

#if W_TARGET == W_WEB
static void onerror_sound(unsigned undocumented,void*snd,
int error_code){
fprintf(stderr,"WARNING (0): Couldn't load a sound file. Code %d.\n",
error_code);
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(W._pending_files_mutex));
#endif
W.pending_files--;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(W._pending_files_mutex));
#endif
}
#endif
/*:596*//*597:*/
#line 12900 "./cweb/weaver.w"

#if W_TARGET == W_WEB
static void onload_sound(unsigned undocumented,void*snd,
const char*filename){
char*ext;
bool ret= true;
struct sound*my_sound= (struct sound*)snd;

ext= strrchr(filename,'.');
if(!ext){
onerror_sound(0,snd,0);
return;
}
if(!strcmp(ext,".wav")||!strcmp(ext,".WAV")){
my_sound->_data= extract_wave(filename,&(my_sound->size),
&(my_sound->freq),
&(my_sound->channels),
&(my_sound->bitrate),&ret);


_finalize_after(&(my_sound->_data),_finalize_openal);
}
if(ret){
onerror_sound(0,snd,1);
return;
}
my_sound->loaded= true;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(W._pending_files_mutex));
#endif
W.pending_files--;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(W._pending_files_mutex));
#endif
}
#endif
/*:597*//*598:*/
#line 12942 "./cweb/weaver.w"

#if W_TARGET == W_WEB
static void onprogress_sound(unsigned int undocumented,void*snd,
int percent){
return;
}
#endif
/*:598*//*765:*/
#line 16991 "./cweb/weaver.w"

#if W_TARGET == W_WEB
static char*basename(char*path){
char*p= path,*c;
for(c= path;*c!='\0';c++)
if(*c=='/'&&*(c+1)!='\0')
p= c+1;
return p;
}
#endif
/*:765*//*806:*/
#line 17799 "./cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
bool _music_thread_prepare_new_music(struct _music_data*music_data,
long*rate,int*channels,int*encoding,
int*bits,int*current_format,
size_t*size){
*current_format= 0xfff5;
if(mpg123_open(music_data->mpg_handle,
music_data->filename[_number_of_loops])!=MPG123_OK)
return false;
mpg123_getformat(music_data->mpg_handle,rate,channels,encoding);
*bits= mpg123_encsize(*encoding)*8;
if(*bits==8){
if(*channels==1)*current_format= AL_FORMAT_MONO8;
else if(*channels==2)*current_format= AL_FORMAT_STEREO8;
}else if(*bits==16){
if(*channels==1)*current_format= AL_FORMAT_MONO16;
else if(*channels==2)*current_format= AL_FORMAT_STEREO16;
}
if(*current_format==0xfff5)
return false;

mpg123_read(music_data->mpg_handle,music_data->buffer,
music_data->buffer_size,size);
alBufferData(music_data->openal_buffer[0],
*current_format,music_data->buffer,
(ALsizei)*size,*rate);
mpg123_read(music_data->mpg_handle,music_data->buffer,
music_data->buffer_size,size);
alBufferData(music_data->openal_buffer[1],
*current_format,music_data->buffer,
(ALsizei)*size,*rate);
alSourceQueueBuffers(music_data->sound_source,2,
music_data->openal_buffer);
alSourcef(music_data->sound_source,AL_GAIN,
music_data->volume[_number_of_loops]);
alSourcePlay(music_data->sound_source);
return true;
}
#endif
/*:806*//*807:*/
#line 17843 "./cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
bool _music_thread_play_music(struct _music_data*music_data,
long rate,int current_format,size_t size){
int buffers,ret;
ALuint buf;

if(music_data->status[_number_of_loops]!=_PLAYING)
return true;

alGetSourcei(music_data->sound_source,AL_BUFFERS_PROCESSED,&buffers);
if(!buffers)
return true;
alSourceUnqueueBuffers(music_data->sound_source,1,&buf);
ret= mpg123_read(music_data->mpg_handle,music_data->buffer,
music_data->buffer_size,&size);
if(ret==MPG123_OK){
alBufferData(buf,current_format,music_data->buffer,
(ALsizei)size,rate);
alSourceQueueBuffers(music_data->sound_source,1,&buf);
}
else if(ret==MPG123_DONE)
return false;
return true;
}
#endif
/*:807*//*808:*/
#line 17873 "./cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
void _music_thread_end_music(struct _music_data*music_data){
ALuint buf;
ALint stat;
int ret;

do{
alSourceUnqueueBuffers(music_data->sound_source,1,&buf);
ret= alGetError();
}while(ret==AL_INVALID_VALUE);
do{
alGetSourcei(music_data->sound_source,AL_SOURCE_STATE,&stat);
}while(stat==AL_PLAYING);

mpg123_close(music_data->mpg_handle);
}
#endif
/*:808*//*809:*/
#line 17895 "./cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
void _music_thread_interrupt_music(struct _music_data*music_data){
ALuint buf;
ALint stat;
int ret;
mpg123_close(music_data->mpg_handle);
alSourceStop(music_data->sound_source);
do{
alSourceUnqueueBuffers(music_data->sound_source,1,&buf);
ret= alGetError();
}while(ret==AL_INVALID_VALUE);
do{
alSourceUnqueueBuffers(music_data->sound_source,1,&buf);
ret= alGetError();
}while(ret==AL_INVALID_VALUE);
do{
alGetSourcei(music_data->sound_source,AL_SOURCE_STATE,&stat);
}while(stat==AL_PLAYING);
}
#endif
/*:809*//*810:*/
#line 17920 "./cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
void _music_thread_update_volume(struct _music_data*music_data){
alSourcef(music_data->sound_source,AL_GAIN,
music_data->volume[_number_of_loops]);
}
#endif
/*:810*/
#line 11948 "./cweb/weaver.w"

/*549:*/
#line 11979 "./cweb/weaver.w"

void _initialize_sound(void){
default_device= alcOpenDevice(NULL);
if(default_device==NULL)
fprintf(stderr,"WARNING (0): No sound device detected.\n");
/*556:*/
#line 12040 "./cweb/weaver.w"

W.number_of_sound_devices= 0;
W.sound_device_name= NULL;
/*:556*//*557:*/
#line 12055 "./cweb/weaver.w"

{
char*devices,*c;
int end= 0;
c= devices= (char*)alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);
while(end!=2){
if(*c=='\0'){
end++;
if(c!=devices&&*(c-1)!='\0')
W.number_of_sound_devices++;
}
else
end= 0;
c++;
}
if(W.number_of_sound_devices==0)
goto AFTER_SOUND_INITIALIZATION;
}
/*:557*//*558:*/
#line 12079 "./cweb/weaver.w"

{
char*devices,*c;
int i= 0;
W.sound_device_name= (char**)Walloc(W.number_of_sound_devices*
sizeof(char*));
if(W.sound_device_name==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the value"
" of W_INTERNAL_MEMORY at conf/conf.h and try to run the "
"program again.\n");
exit(1);
}
c= devices= (char*)alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);
W.sound_device_name[0]= devices;
for(;;c++){
if(*c=='\0'){
i++;
if(i<W.number_of_sound_devices)
W.sound_device_name[i]= c+1;
else
break;
}
}
}
/*:558*//*569:*/
#line 12198 "./cweb/weaver.w"

{
if(default_device){

default_context= alcCreateContext(default_device,NULL);
alcMakeContextCurrent(default_context);
}
alGetError();
}
/*:569*//*571:*/
#line 12225 "./cweb/weaver.w"

{
ALenum error;
if(default_device!=NULL){
alGenSources(5,default_source);
error= alGetError();
if(error!=AL_NO_ERROR){
fprintf(stderr,"WARNING(0)): No sound source could be created. "
"alGenSources failed. Sound probably won't work.\n");
}
}
}
/*:571*//*770:*/
#line 17059 "./cweb/weaver.w"

{
int i,j;
#if W_TARGET == W_ELF
#ifndef W_DISABLE_MP3
int ret;
mpg123_init();
#endif
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
_music[i].mpg_handle= mpg123_new(NULL,&ret);
if(_music[i].mpg_handle==NULL){
fprintf(stderr,"WARNING: MP3 handling failed.\n");
}
_music[i].buffer_size= mpg123_outblock(_music[i].mpg_handle);
_music[i].buffer= (unsigned char*)Walloc(_music[i].buffer_size);
#endif
for(j= 0;j<W_MAX_SUBLOOP;j++){
_music[i].volume[j]= 0.5;
_music[i].status[j]= _NOT_LOADED;
_music[i].filename[j][0]= '\0';
#if W_TARGET == W_ELF
alGenSources(1,&_music[i].sound_source);
alGenBuffers(2,_music[i].openal_buffer);
if(alGetError()!=AL_NO_ERROR){
fprintf(stderr,"WARNING: Error generating music buffer.\n");
}
#endif
}
}
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_music_mutex,NULL)!=0){
perror("Initializing music mutex:");
exit(1);
}
#endif
}
/*:770*//*802:*/
#line 17668 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i;
int ret;
for(i= 0;i<W_MAX_MUSIC;i++){

ret= sem_init(&(_music[i].semaphore),0,0);
if(ret==-1){
perror("sem_init");
}
#ifndef W_DISABLE_MP3
ret= pthread_create(&(_music[i].thread),NULL,&_music_thread,
&(_music[i]));
if(ret!=0){
fprintf(stderr,"WARNING (0): Can't create music threads. "
"Music may fail to play.");
break;
}
#endif
}
}
#endif
/*:802*/
#line 11984 "./cweb/weaver.w"

AFTER_SOUND_INITIALIZATION:
return;
}
/*:549*//*551:*/
#line 11998 "./cweb/weaver.w"

void _finalize_sound(void){
/*771:*/
#line 17102 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i;
for(i= W_MAX_MUSIC-1;i>=0;i--){
#ifndef W_DISABLE_MP3
mpg123_close(_music[i].mpg_handle);
mpg123_delete(_music[i].mpg_handle);
#endif
alDeleteSources(1,&_music[i].sound_source);
alDeleteBuffers(2,_music[i].openal_buffer);
#ifndef W_DISABLE_MP3
Wfree(_music[i].buffer);
#endif
}
#ifndef W_DISABLE_MP3
mpg123_exit();
#endif
}
#endif
#ifdef W_MULTITHREAD
pthread_mutex_destroy(&_music_mutex);
#endif
/*:771*/
#line 12000 "./cweb/weaver.w"

/*559:*/
#line 12109 "./cweb/weaver.w"

{
if(W.sound_device_name!=NULL)
Wfree(W.sound_device_name);
}

/*:559*//*572:*/
#line 12241 "./cweb/weaver.w"

{
alDeleteSources(5,default_source);
if(default_context!=NULL)
alcDestroyContext(default_context);
}
/*:572*//*803:*/
#line 17696 "./cweb/weaver.w"

#if W_TARGET == W_ELF
_close_music();
#endif
/*:803*/
#line 12001 "./cweb/weaver.w"



alcCloseDevice(default_device);
}
/*:551*//*561:*/
#line 12125 "./cweb/weaver.w"

bool _select_sound_device(int position){
if(position<0||position>=W.number_of_sound_devices)
return false;


/*573:*/
#line 12253 "./cweb/weaver.w"

{
alDeleteSources(5,default_source);
if(default_context!=NULL)
alcDestroyContext(default_context);
}
/*:573*//*772:*/
#line 17130 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
alGenSources(1,&_music[i].sound_source);
}
}
#endif
/*:772*/
#line 12131 "./cweb/weaver.w"

alcCloseDevice(default_device);
default_device= alcOpenDevice(W.sound_device_name[position]);
/*773:*/
#line 17144 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
alDeleteSources(1,&_music[i].sound_source);
}
}
#endif
/*:773*/
#line 12134 "./cweb/weaver.w"

return true;
}
/*:561*//*565:*/
#line 12158 "./cweb/weaver.w"

int _current_sound_device(void){
int i;
char*query;
if(W.sound_device_name==NULL)
return-1;
query= (char*)alcGetString(NULL,ALC_DEFAULT_DEVICE_SPECIFIER);
for(i= 0;i<W.number_of_sound_devices;i++)
if(!strcmp(query,W.sound_device_name[i]))
return i;
return-1;
}
/*:565*//*594:*/
#line 12776 "./cweb/weaver.w"

struct sound*_new_sound(char*filename){
char complete_path[256];
struct sound*snd;
#if W_TARGET == W_ELF
bool ret= true;
char*ext;
#endif
#if W_TARGET == W_WEB
char dir[]= "sound/";
#elif W_DEBUG_LEVEL >= 1
char dir[]= "./sound/";
#elif W_TARGET == W_ELF
char dir[]= W_INSTALL_DATA"/sound/";
#endif
snd= (struct sound*)Walloc(sizeof(struct sound));
if(snd==NULL){
printf("WARNING(0): Not enough memory to read file: %s.\n",
filename);
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): You should increase the value of "
"W_INTERNAL_MEMORY at conf/conf.h.\n");
#endif
return NULL;
}
snd->loaded= false;
strncpy(complete_path,dir,256);
complete_path[255]= '\0';
strncat(complete_path,filename,256-strlen(complete_path));
#if W_TARGET == W_WEB
mkdir("sound/",0777);
W.pending_files++;
emscripten_async_wget2(complete_path,complete_path,
"GET","",(void*)snd,
&onload_sound,&onerror_sound,
&onprogress_sound);
return snd;
#else 

ext= strrchr(filename,'.');
if(!ext){
fprintf(stderr,"WARNING (0): No file extension in %s.\n",
filename);
return NULL;
}
if(!strcmp(ext,".wav")||!strcmp(ext,".WAV")){
snd->_data= extract_wave(complete_path,&(snd->size),
&(snd->freq),&(snd->channels),
&(snd->bitrate),&ret);



_finalize_after(&(snd->_data),_finalize_openal);
}
/*812:*/
#line 17941 "./cweb/weaver.w"

#ifndef W_DISABLE_MP3
else if(!strcmp(ext,".mp3")||!strcmp(ext,".MP3")){
/*813:*/
#line 17959 "./cweb/weaver.w"

int current_format= 0xfff5;
size_t buffer_size;
unsigned char*buffer= NULL;
ALuint openal_buffer= 0;
ret= false;
{
int test;
size_t decoded_bytes;
mpg123_handle*mpg_handle= mpg123_new(NULL,&test);
buffer_size= mpg123_outblock(mpg_handle);
for(;;){

test= mpg123_open(mpg_handle,complete_path);
if(test!=MPG123_OK){
fprintf(stderr,"Warning: Error opening %s\n",complete_path);
buffer_size= 0;
ret= true;
break;
}

if(current_format==0xfff5){
int channels,encoding,bits;
long rate;
mpg123_getformat(mpg_handle,&rate,&channels,&encoding);
bits= mpg123_encsize(encoding)*8;
snd->freq= rate;
snd->channels= channels;
snd->bitrate= bits;
if(bits==8){
if(channels==1)current_format= AL_FORMAT_MONO8;
else if(channels==2)current_format= AL_FORMAT_STEREO8;
}else if(bits==16){
if(channels==1)current_format= AL_FORMAT_MONO16;
else if(channels==2)current_format= AL_FORMAT_STEREO16;
}
if(current_format==0xfff5){
fprintf(stderr,
"WARNING(0): Combination of channel and bitrate not "
"supported in file %s (sound have %d channels and %d bitrate"
" while "
"we support just 1 or 2 channels and 8 or 16 as "
"bitrate).\n",
complete_path,channels,bits);
}
}

buffer= (unsigned char*)Walloc(buffer_size);
if(buffer==NULL){
fprintf(stderr,"ERROR: Not enough memory to load %s. Please, "
"increase the value of W_MAX_MEMORY at conf/conf.h.\n",
complete_path);
buffer_size= 0;
ret= true;
break;
}
test= mpg123_read(mpg_handle,buffer,buffer_size,&decoded_bytes);
mpg123_close(mpg_handle);

if(decoded_bytes> buffer_size){
Wfree(buffer);
buffer= NULL;
buffer_size*= 2;
}
else break;
}

snd->size= buffer_size;
}
/*:813*//*814:*/
#line 18035 "./cweb/weaver.w"

if(buffer!=NULL){
int status;
alGenBuffers(1,&openal_buffer);
status= alGetError();
if(status!=AL_NO_ERROR){
fprintf(stderr,"WARNING(0)): No sound buffer could be created. "
"alGenBuffers failed. ");
if(status==AL_INVALID_VALUE){
fprintf(stderr,"Internal error: buffer array isn't large enough.\n");
}
else if(status==AL_OUT_OF_MEMORY){
fprintf(stderr,"Internal error: out of memory.\n");
}
else{
fprintf(stderr,"Unknown error (%d).\n",status);
}
ret= true;
}
else{
alBufferData(openal_buffer,current_format,buffer,snd->size,
snd->freq);
status= alGetError();
if(status!=AL_NO_ERROR){
fprintf(stderr,"WARNING(0): Can't pass audio to OpenAL. "
"alBufferData failed. Sound may not work.\n");
ret= true;
}
Wfree(buffer);
snd->_data= openal_buffer;
_finalize_after(&(snd->_data),_finalize_openal);
}
}
/*:814*/
#line 17944 "./cweb/weaver.w"

}
#endif
/*:812*/
#line 12830 "./cweb/weaver.w"

if(ret){



if(_running_loop)
_finalize_this(&(snd->_data),true);
Wfree(snd);
return NULL;
}
snd->loaded= true;
return snd;
#endif
}
/*:594*//*602:*/
#line 12968 "./cweb/weaver.w"

void _play_sound(struct sound*snd){
if(!snd->loaded)return;
int i= -1;
ALenum status= AL_NO_ERROR;

do{
i++;
if(i> 4)break;
alSourcei(default_source[i],AL_BUFFER,snd->_data);
status= alGetError();
}while(status!=AL_NO_ERROR);
alSourcePlay(default_source[i]);
}
/*:602*//*606:*/
#line 13003 "./cweb/weaver.w"

void _destroy_sound(struct sound*snd){




while(snd->loaded==false&&W.pending_files> 0){
#ifdef W_MULTITHREAD
sched_yield();
#elif W_TARGET == W_ELF
{
struct timespec tim;

tim.tv_sec= 0;
tim.tv_nsec= 1000000L;
nanosleep(&tim,NULL);
}
#elif W_TARGET == W_WEB
return;
#endif
}

alDeleteBuffers(1,&(snd->_data));



if(_running_loop)
_finalize_this(&(snd->_data),false);
Wfree(snd);
}
/*:606*//*775:*/
#line 17170 "./cweb/weaver.w"

bool _play_music(char*name,bool loop){
int i;
bool success= false;
size_t path_length= 0;
size_t name_length= strlen(name);



if(_resume_music(name))
return true;

#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
if(_music[i].status[_number_of_loops]==_NOT_LOADED){
#if W_TARGET == W_WEB

EM_ASM_({
document["music"+$0]= new Audio("music/"+Pointer_stringify($1));
document["music"+$0].volume= 0.5;
if($2){
document["music"+$0].loop= true;
}
document["music"+$0].play();
},i,name,loop);
#endif
_music[i].volume[_number_of_loops]= 0.5;

_music[i].filename[_number_of_loops][0]= '\0';
#if W_DEBUG_LEVEL == 0
path_length= strlen(W_INSTALL_DATA);
memcpy(_music[i].filename[_number_of_loops],W_INSTALL_DATA,
path_length+1);
memcpy(&_music[i].filename[_number_of_loops][path_length],"/",2);
path_length++;
#endif
if(path_length+name_length> 249){
fprintf(stderr,"WARNING: Path is too long: %smusic/%s",
_music[i].filename[_number_of_loops],name);
break;
}
memcpy(&_music[i].filename[_number_of_loops][path_length],"music/",7);
path_length+= 6;
memcpy(&_music[i].filename[_number_of_loops][path_length],name,
name_length);
success= true;
if(_music[i].status[_number_of_loops]!=_PLAYING){
_music[i].status[_number_of_loops]= _PLAYING;
_music[i].loop[_number_of_loops]= loop;
#if W_TARGET == W_ELF

sem_post(&(_music[i].semaphore));
#endif
}
break;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
#ifdef W_DISABLE_MP3
return success&&false;
#else
return success;
#endif
}
/*:775*//*779:*/
#line 17261 "./cweb/weaver.w"

bool _pause_music(char*name){
int i;
bool success= false;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
if(!strcmp(name,basename(_music[i].filename[_number_of_loops]))&&
_music[i].status[_number_of_loops]==_PLAYING){
#if W_TARGET == W_WEB

EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].pause();
}
},i);
#endif
_music[i].status[_number_of_loops]= _PAUSED;
success= true;
break;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
#ifdef W_DISABLE_MP3
return success&&false;
#else
return success;
#endif
}
/*:779*//*783:*/
#line 17311 "./cweb/weaver.w"

bool _resume_music(char*name){
int i;
bool success= false;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
if(!strcmp(name,basename(_music[i].filename[_number_of_loops]))&&
_music[i].status[_number_of_loops]==_PAUSED){
#if W_TARGET == W_WEB

EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].play();
}
},i);
#endif
_music[i].status[_number_of_loops]= _PLAYING;
#if W_TARGET == W_ELF

sem_post(&(_music[i].semaphore));
#endif
success= true;
break;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
#ifdef W_DISABLE_MP3
return success&&false;
#else
return success;
#endif
}
/*:783*//*785:*/
#line 17358 "./cweb/weaver.w"

bool _stop_music(char*name){
int i;
bool success= false;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
if(!strcmp(name,basename(_music[i].filename[_number_of_loops]))){
#if W_TARGET == W_WEB


EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].pause();
document["music"+$0]= undefined;
}
},i);
#endif
_music[i].filename[_number_of_loops][0]= '\0';
_music[i].status[_number_of_loops]= _NOT_LOADED;
#if W_TARGET == W_ELF

if(_music[i].status[_number_of_loops]==_PAUSED)
sem_post(&(_music[i].semaphore));
#endif
success= true;
break;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
#ifdef W_DISABLE_MP3
return success&&false;
#else
return success;
#endif
}
/*:785*//*789:*/
#line 17417 "./cweb/weaver.w"

bool _close_music(void){
int i;
bool success= false;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
_music[i].filename[_number_of_loops][0]= '\0';
_music[i].status[_number_of_loops]= _CLOSED;
#if W_TARGET == W_ELF

sem_post(&(_music[i].semaphore));
#endif
}
success= true;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
#ifdef W_DISABLE_MP3
return success&&false;
#else
return success;
#endif
}
/*:789*//*791:*/
#line 17455 "./cweb/weaver.w"

float _get_volume(char*name){
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
if(!strcmp(name,_music[i].filename[_number_of_loops])){
return _music[i].volume[_number_of_loops];
}
}
return-1.0;
}
/*:791*//*795:*/
#line 17489 "./cweb/weaver.w"

float _increase_volume(char*name,float increment){
int i;
float success= -1.0,total;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
if(!strcmp(name,basename(_music[i].filename[_number_of_loops]))){
total= _music[i].volume[_number_of_loops]+increment;
if(total> 1.0)
_music[i].volume[_number_of_loops]= 1.0;
else if(total<0.0)
_music[i].volume[_number_of_loops]= 0.0;
else _music[i].volume[_number_of_loops]= total;
#if W_TARGET == W_WEB


EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].volume= $0;
}
},_music[i].volume[_number_of_loops]);
#endif
success= _music[i].volume[_number_of_loops];
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
#ifdef W_DISABLE_MP3
return success&&false;
#else
return success;
#endif
}
/*:795*//*805:*/
#line 17715 "./cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
void*_music_thread(void*arg){

struct _music_data*music_data= (struct _music_data*)arg;

int last_loop= _number_of_loops;

float last_volume= music_data->volume[last_loop];

int current_format= 0xfff5;
size_t size;
long rate;
int channels,encoding,bits;
sem_musica_nenhuma:
while(music_data->status[_number_of_loops]==_NOT_LOADED)
sem_wait(&(music_data->semaphore));
if(music_data->status[_number_of_loops]==_CLOSED)
goto encerrando_thread;

if(!_music_thread_prepare_new_music(music_data,&rate,&channels,&encoding,
&bits,&current_format,&size)){

music_data->status[_number_of_loops]= _NOT_LOADED;
fprintf(stderr,"Error opening %s\n",
music_data->filename[last_loop]);
goto sem_musica_nenhuma;
}
tocando_musica:
if(!_music_thread_play_music(music_data,rate,current_format,size)){

_music_thread_end_music(music_data);
if(music_data->loop[last_loop]){

_music_thread_prepare_new_music(music_data,&rate,&channels,&encoding,
&bits,&current_format,&size);
goto tocando_musica;
}
else{
music_data->status[_number_of_loops]= _NOT_LOADED;
goto sem_musica_nenhuma;
}
}

if(last_loop!=_number_of_loops){
last_loop= _number_of_loops;
if(music_data->status[_number_of_loops]==_NOT_LOADED)
goto sem_musica_nenhuma;
if(music_data->status[_number_of_loops]==_CLOSED)
goto encerrando_thread;
}

else if(last_volume!=music_data->volume[_number_of_loops]){
_music_thread_update_volume(music_data);
last_volume= music_data->volume[_number_of_loops];
}

else if(music_data->status[_number_of_loops]==_PAUSED){
alSourcePause(music_data->sound_source);

while(music_data->status[_number_of_loops]==_PAUSED)
sem_wait(&(music_data->semaphore));

alSourcePlay(music_data->sound_source);
}

if(music_data->status[_number_of_loops]==_NOT_LOADED){

_music_thread_interrupt_music(music_data);
goto sem_musica_nenhuma;
}

if(music_data->status[_number_of_loops]==_CLOSED)
goto encerrando_thread;
goto tocando_musica;
encerrando_thread:
sem_destroy(&(music_data->semaphore));
return NULL;
}
#endif
/*:805*/
#line 11949 "./cweb/weaver.w"

/*:544*/
