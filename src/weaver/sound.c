/*518:*/
#line 11378 "cweb/weaver.w"

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

/*521:*/
#line 11411 "cweb/weaver.w"

static ALCdevice*default_device;
/*:521*//*542:*/
#line 11641 "cweb/weaver.w"

static ALCcontext*default_context= NULL;
/*:542*//*544:*/
#line 11666 "cweb/weaver.w"

static ALuint default_source[5];
/*:544*//*552:*/
#line 11825 "cweb/weaver.w"

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
/*553:*/
#line 11845 "cweb/weaver.w"

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
/*:553*//*554:*/
#line 11870 "cweb/weaver.w"

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
/*:554*//*555:*/
#line 11895 "cweb/weaver.w"

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
/*:555*//*556:*/
#line 11922 "cweb/weaver.w"

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
/*:556*//*557:*/
#line 11948 "cweb/weaver.w"

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
/*:557*//*558:*/
#line 11980 "cweb/weaver.w"

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
/*:558*//*559:*/
#line 12005 "cweb/weaver.w"

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
/*:559*//*560:*/
#line 12032 "cweb/weaver.w"

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
/*:560*//*561:*/
#line 12052 "cweb/weaver.w"

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
/*:561*//*562:*/
#line 12079 "cweb/weaver.w"

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
/*:562*//*563:*/
#line 12099 "cweb/weaver.w"

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
/*:563*//*564:*/
#line 12123 "cweb/weaver.w"

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
/*:564*/
#line 11836 "cweb/weaver.w"

return returned_buffer;
}
/*:552*//*742:*/
#line 16486 "cweb/weaver.w"

struct _music_data _music[W_MAX_MUSIC];
#ifdef W_MULTITHREAD


pthread_mutex_t _music_mutex;
#endif
/*:742*/
#line 11394 "cweb/weaver.w"

/*569:*/
#line 12299 "cweb/weaver.w"



static void _finalize_openal(void*data){
ALuint*p= (ALuint*)data;
alDeleteBuffers(1,p);
}
/*:569*//*570:*/
#line 12325 "cweb/weaver.w"

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
/*:570*//*571:*/
#line 12347 "cweb/weaver.w"

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
/*:571*//*572:*/
#line 12389 "cweb/weaver.w"

#if W_TARGET == W_WEB
static void onprogress_sound(unsigned int undocumented,void*snd,
int percent){
return;
}
#endif
/*:572*//*739:*/
#line 16439 "cweb/weaver.w"

#if W_TARGET == W_WEB
static char*basename(char*path){
char*p= path,*c;
for(c= path;c!='\0';c++)
if(*c=='/'&&*(c+1)!='\0')
p= c+1;
return p;
}
#endif
/*:739*//*778:*/
#line 17196 "cweb/weaver.w"

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
/*:778*//*779:*/
#line 17240 "cweb/weaver.w"

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
/*:779*//*780:*/
#line 17270 "cweb/weaver.w"

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
/*:780*//*781:*/
#line 17292 "cweb/weaver.w"

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
/*:781*//*782:*/
#line 17317 "cweb/weaver.w"

#if W_TARGET == W_ELF && !defined(W_DISABLE_MP3)
void _music_thread_update_volume(struct _music_data*music_data){
alSourcef(music_data->sound_source,AL_GAIN,
music_data->volume[_number_of_loops]);
}
#endif
/*:782*/
#line 11395 "cweb/weaver.w"

/*523:*/
#line 11426 "cweb/weaver.w"

void _initialize_sound(void){
default_device= alcOpenDevice(NULL);
if(default_device==NULL)
fprintf(stderr,"WARNING (0): No sound device detected.\n");
/*530:*/
#line 11487 "cweb/weaver.w"

W.number_of_sound_devices= 0;
W.sound_device_name= NULL;
/*:530*//*531:*/
#line 11502 "cweb/weaver.w"

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
/*:531*//*532:*/
#line 11526 "cweb/weaver.w"

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
/*:532*//*543:*/
#line 11645 "cweb/weaver.w"

{
if(default_device){

default_context= alcCreateContext(default_device,NULL);
alcMakeContextCurrent(default_context);
}
alGetError();
}
/*:543*//*545:*/
#line 11672 "cweb/weaver.w"

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
/*:545*//*744:*/
#line 16506 "cweb/weaver.w"

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
/*:744*//*774:*/
#line 17076 "cweb/weaver.w"

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
/*:774*/
#line 11431 "cweb/weaver.w"

AFTER_SOUND_INITIALIZATION:
return;
}
/*:523*//*525:*/
#line 11445 "cweb/weaver.w"

void _finalize_sound(void){
/*745:*/
#line 16549 "cweb/weaver.w"

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
/*:745*/
#line 11447 "cweb/weaver.w"

/*533:*/
#line 11556 "cweb/weaver.w"

{
if(W.sound_device_name!=NULL)
Wfree(W.sound_device_name);
}

/*:533*//*546:*/
#line 11688 "cweb/weaver.w"

{
alDeleteSources(5,default_source);
if(default_context!=NULL)
alcDestroyContext(default_context);
}
/*:546*//*775:*/
#line 17104 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
sem_destroy(&(_music[i].semaphore));
#ifndef W_DISABLE_MP3
pthread_cancel(_music[i].thread);
#endif
}
}
#endif
/*:775*/
#line 11448 "cweb/weaver.w"



alcCloseDevice(default_device);
}
/*:525*//*535:*/
#line 11572 "cweb/weaver.w"

bool _select_sound_device(int position){
if(position<0||position>=W.number_of_sound_devices)
return false;


/*547:*/
#line 11700 "cweb/weaver.w"

{
alDeleteSources(5,default_source);
if(default_context!=NULL)
alcDestroyContext(default_context);
}
/*:547*//*746:*/
#line 16577 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
alGenSources(1,&_music[i].sound_source);
}
}
#endif
/*:746*/
#line 11578 "cweb/weaver.w"

alcCloseDevice(default_device);
default_device= alcOpenDevice(W.sound_device_name[position]);
/*747:*/
#line 16591 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
alDeleteSources(1,&_music[i].sound_source);
}
}
#endif
/*:747*/
#line 11581 "cweb/weaver.w"

return true;
}
/*:535*//*539:*/
#line 11605 "cweb/weaver.w"

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
/*:539*//*568:*/
#line 12223 "cweb/weaver.w"

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
/*784:*/
#line 17338 "cweb/weaver.w"

#ifndef W_DISABLE_MP3
else if(!strcmp(ext,".mp3")||!strcmp(ext,".MP3")){
/*785:*/
#line 17356 "cweb/weaver.w"

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
/*:785*//*786:*/
#line 17432 "cweb/weaver.w"

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
/*:786*/
#line 17341 "cweb/weaver.w"

}
#endif
/*:784*/
#line 12277 "cweb/weaver.w"

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
/*:568*//*576:*/
#line 12415 "cweb/weaver.w"

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
/*:576*//*580:*/
#line 12450 "cweb/weaver.w"

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
/*:580*//*749:*/
#line 16617 "cweb/weaver.w"

bool _play_music(char*name,bool loop){
int i;
bool success= false;



if(_resume_music(name))
return true;

#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
for(i= 0;i<W_MAX_MUSIC;i++){
if(_music[i].status[_number_of_loops]==_NOT_LOADED){
#if W_TARGET == W_WEB

EM_ASM_({
document["music"+$0]= new Audio(Pointer_stringify($1));
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
strncpy(_music[i].filename[_number_of_loops],W_INSTALL_DATA,256);
strcat(_music[i].filename[_number_of_loops],"/");
#endif
strncat(_music[i].filename[_number_of_loops],"music/",
256-strlen(_music[i].filename[_number_of_loops]));
strncat(_music[i].filename[_number_of_loops],name,
256-strlen(_music[i].filename[_number_of_loops]));
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
/*:749*//*753:*/
#line 16698 "cweb/weaver.w"

bool _pause_music(char*name){
int i;
bool success= false;
printf("Pausando %s\n",name);
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
#if W_TARGET == W_ELF
if(_music[i].status[_number_of_loops]==_PLAYING){

sem_wait(&(_music[i].semaphore));
}
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
/*:753*//*757:*/
#line 16755 "cweb/weaver.w"

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
#if W_TARGET == W_ELF

sem_post(&(_music[i].semaphore));
#endif
_music[i].status[_number_of_loops]= _PLAYING;
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
/*:757*//*759:*/
#line 16802 "cweb/weaver.w"

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
if(_music[i].status[_number_of_loops]==_PLAYING){

sem_wait(&(_music[i].semaphore));
}
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
/*:759*//*763:*/
#line 16863 "cweb/weaver.w"

float _get_volume(char*name){
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
if(!strcmp(name,_music[i].filename[_number_of_loops])){
return _music[i].volume[_number_of_loops];
}
}
return-1.0;
}
/*:763*//*767:*/
#line 16897 "cweb/weaver.w"

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
/*:767*//*777:*/
#line 17131 "cweb/weaver.w"

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
}

else if(last_volume!=music_data->volume[_number_of_loops]){
_music_thread_update_volume(music_data);
last_volume= music_data->volume[_number_of_loops];
}

else if(music_data->status[_number_of_loops]==_NOT_LOADED){

_music_thread_interrupt_music(music_data);
goto sem_musica_nenhuma;
}
goto tocando_musica;
}
#endif
/*:777*/
#line 11396 "cweb/weaver.w"

/*:518*/
