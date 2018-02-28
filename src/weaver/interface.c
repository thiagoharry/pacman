/*392:*/
#line 8714 "./cweb/weaver.w"

#include "interface.h"
#include <stdarg.h>  
#if W_TARGET == W_WEB
#include <sys/stat.h>  
#include <sys/types.h>  
#endif
/*649:*/
#line 14443 "./cweb/weaver.w"

#if W_TARGET == W_WEB
#include <SDL/SDL_image.h> 
#endif
/*:649*//*667:*/
#line 14918 "./cweb/weaver.w"

#if !defined(W_DISABLE_PNG) && (W_TARGET == W_ELF)
#include <png.h> 
#endif
/*:667*/
#line 8721 "./cweb/weaver.w"

/*648:*/
#line 14423 "./cweb/weaver.w"

#if W_TARGET == W_WEB
static void onerror_texture(unsigned undocumented,void*interface,
int error_code){
fprintf(stderr,"WARNING (0): Couldn't load a texture file. Code %d.\n",
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
/*:648*//*650:*/
#line 14449 "./cweb/weaver.w"

#if W_TARGET == W_WEB
static void onload_texture(unsigned undocumented,
void*inter,const char*filename){
char*ext;
bool ret= true;
struct interface*my_interface= (struct interface*)inter;
printf("DEBUG FILENAME: %s.\n",filename);

ext= strrchr(filename,'.');
if(!ext){
onerror_texture(0,inter,0);
return;
}
if(!strcmp(ext,".gif")||!strcmp(ext,".GIF")){
my_interface->_texture= 
_extract_gif((char*)filename,
&(my_interface->number_of_frames),
&(my_interface->frame_duration),
&(my_interface->max_repetition),&ret);
}
else{

SDL_Surface*tmp_surface= IMG_Load(filename);
unsigned char*pixels= (unsigned char*)
Walloc(sizeof(unsigned char)*4*tmp_surface->w*tmp_surface->h);
my_interface->_texture= (GLuint*)Walloc(sizeof(GLuint));
if(my_interface->_texture==NULL||pixels==NULL){
fprintf(stderr,"ERROR: Not enough memory to read %s. Please, increase "
"the value of W_MAX_MEMORY at conf/conf.h.\n",filename);
}
else{

int i,j,width= tmp_surface->w,height= tmp_surface->h;
for(i= 0;i<width;i++)
for(j= 0;j<height;j++){
pixels[4*(j*width+i)]= ((unsigned char*)tmp_surface->pixels)
[4*((height-1-j)*width+i)];
pixels[4*(j*width+i)+1]= 
((unsigned char*)tmp_surface->pixels)
[4*((height-1-j)*width+i)+1];
pixels[4*(j*width+i)+2]= 
((unsigned char*)tmp_surface->pixels)
[4*((height-1-j)*width+i)+2];
pixels[4*(j*width+i)+3]= 
((unsigned char*)tmp_surface->pixels)
[4*((height-1-j)*width+i)+3];
}
glGenTextures(1,my_interface->_texture);
glBindTexture(GL_TEXTURE_2D,*(my_interface->_texture));
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
GL_UNSIGNED_BYTE,pixels);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
glBindTexture(GL_TEXTURE_2D,0);
Wfree(pixels);
my_interface->number_of_frames= 1;
my_interface->frame_duration= NULL;
my_interface->max_repetition= 0;
ret= false;
}
SDL_FreeSurface(tmp_surface);
}
if(ret){
my_interface->type= W_NONE;
return;
}

if(my_interface->number_of_frames> 1)
my_interface->animate= true;
_finalize_after(my_interface,_finalize_interface_texture);

my_interface->_loaded_texture= true;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(W._pending_files_mutex));
#endif
W.pending_files--;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(W._pending_files_mutex));
#endif
}
#endif
/*:650*//*651:*/
#line 14540 "./cweb/weaver.w"

#if W_TARGET == W_WEB
static void onprogress_texture(unsigned int undocumented,void*snd,
int percent){
return;
}
#endif
/*:651*/
#line 8722 "./cweb/weaver.w"

/*399:*/
#line 8818 "./cweb/weaver.w"

void _flush_interfaces(void){
int i;
for(i= 0;i<W_MAX_INTERFACES;i++){
switch(_interfaces[_number_of_loops][i].type){




default:
_interfaces[_number_of_loops][i].type= W_NONE;
}
#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&(_interfaces[_number_of_loops][i]._mutex))!=
0)
perror("Finalizing interface mutex:");
#endif
}
}
/*:399*//*407:*/
#line 8948 "./cweb/weaver.w"

struct interface*_new_interface(int type,int x,int y,int width,
int height,...){
int i;
va_list valist;
struct interface*new_interface= (struct interface*)1;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_interface_mutex);
#endif

for(i= 0;i<W_MAX_INTERFACES;i++)
if(_interfaces[_number_of_loops][i].type==W_NONE)
break;
if(i==W_MAX_INTERFACES){
#if W_DEBUG_LEVEL >  0
fprintf(stderr,"WARNING (1): Unable to create new interfaces. Please, "
"define W_MAX_INTERFACES as a value greater than %d at "
"conf/conf.h.\n",W_MAX_INTERFACES);
#endif
new_interface= NULL;
}
else{
_interfaces[_number_of_loops][i].type= type;
_interfaces[_number_of_loops][i].visible= true;
_interfaces[_number_of_loops][i].integer= 0;

_interfaces[_number_of_loops][i].r= 0.0;
_interfaces[_number_of_loops][i].g= 0.0;
_interfaces[_number_of_loops][i].b= 0.0;
_interfaces[_number_of_loops][i].a= 0.0;

_interfaces[_number_of_loops][i].x= (float)x;
_interfaces[_number_of_loops][i].y= (float)y;
_interfaces[_number_of_loops][i].rotation= 0.0;
_interfaces[_number_of_loops][i].width= (float)width;
_interfaces[_number_of_loops][i].height= (float)height;

_interfaces[_number_of_loops][i]._mode= GL_TRIANGLE_FAN;
/*645:*/
#line 14307 "./cweb/weaver.w"

{
_interfaces[_number_of_loops][i]._texture= &_empty_texture;




_interfaces[_number_of_loops][i]._loaded_texture= true;
_interfaces[_number_of_loops][i].animate= false;
_interfaces[_number_of_loops][i].number_of_frames= 1;
_interfaces[_number_of_loops][i].current_frame= 0;
_interfaces[_number_of_loops][i].frame_duration= NULL;
_interfaces[_number_of_loops][i]._t= W.t;
_interfaces[_number_of_loops][i].max_repetition= -1;
}
/*:645*/
#line 8986 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_init(&(_interfaces[_number_of_loops][i]._mutex),
NULL)!=0){
_interfaces[_number_of_loops][i].type= W_NONE;
perror("Initializing interface mutex:");
new_interface= NULL;
}
#endif
if(new_interface!=NULL){
switch(type){
case W_INTERFACE_PERIMETER:
_interfaces[_number_of_loops][i]._mode= GL_LINE_LOOP;

case W_INTERFACE_SQUARE:
va_start(valist,height);
_interfaces[_number_of_loops][i].r= va_arg(valist,double);
_interfaces[_number_of_loops][i].g= va_arg(valist,double);
_interfaces[_number_of_loops][i].b= va_arg(valist,double);
_interfaces[_number_of_loops][i].a= va_arg(valist,double);
va_end(valist);
break;
/*647:*/
#line 14338 "./cweb/weaver.w"

case W_INTERFACE_IMAGE:
_interfaces[_number_of_loops][i]._loaded_texture= false;
{
#if W_TARGET == W_WEB
char dir[]= "image/";
#elif W_DEBUG_LEVEL >= 1
char dir[]= "./image/";
#elif W_TARGET == W_ELF
char dir[]= W_INSTALL_DATA"/image/";
#endif
#if W_TARGET == W_ELF
char*ext;
bool ret= true;
#endif
char*filename,complete_path[256];
va_start(valist,height);
filename= va_arg(valist,char*);
va_end(valist);

strncpy(complete_path,dir,256);
complete_path[255]= '\0';
strncat(complete_path,filename,256-strlen(complete_path));
#if W_TARGET == W_WEB

if(mkdir("image/",0777)==-1)
perror(NULL);
W.pending_files++;

emscripten_async_wget2(complete_path,complete_path,
"GET","",
(void*)&(_interfaces[_number_of_loops][i]),
&onload_texture,&onerror_texture,
&onprogress_texture);
#else

ext= strrchr(filename,'.');
if(!ext){
fprintf(stderr,"WARNING (0): No file extension in %s.\n",
filename);
_interfaces[_number_of_loops][i].type= W_NONE;
return NULL;
}
if(!strcmp(ext,".gif")||!strcmp(ext,".GIF")){
_interfaces[_number_of_loops][i]._texture= 
_extract_gif(complete_path,
&(_interfaces[_number_of_loops][i].number_of_frames),
&(_interfaces[_number_of_loops][i].frame_duration),
&(_interfaces[_number_of_loops][i].max_repetition),
&ret);
if(ret){
_interfaces[_number_of_loops][i].type= W_NONE;
return NULL;
}

if(_interfaces[_number_of_loops][i].number_of_frames> 1)
_interfaces[_number_of_loops][i].animate= true;
_interfaces[_number_of_loops][i]._loaded_texture= true;



_finalize_after(&(_interfaces[_number_of_loops][i]),
_finalize_interface_texture);
}
/*685:*/
#line 15281 "./cweb/weaver.w"

#ifndef W_DISABLE_PNG
if(!strcmp(ext,".png")||!strcmp(ext,".PNG")){
_interfaces[_number_of_loops][i]._texture= 
_extract_png(complete_path,
&(_interfaces[_number_of_loops][i].number_of_frames),
&(_interfaces[_number_of_loops][i].frame_duration),
&(_interfaces[_number_of_loops][i].max_repetition),
&ret);
if(ret){
_interfaces[_number_of_loops][i].type= W_NONE;
return NULL;
}

_interfaces[_number_of_loops][i].animate= false;
_interfaces[_number_of_loops][i]._loaded_texture= true;

_finalize_after(&(_interfaces[_number_of_loops][i]),
_finalize_interface_texture);
}
#endif
/*:685*/
#line 14402 "./cweb/weaver.w"

#endif
}
break;
/*:647*/
#line 9008 "./cweb/weaver.w"

default:
/*663:*/
#line 14745 "./cweb/weaver.w"

{
#if W_TARGET == W_WEB
char dir[]= "image/";
#elif W_DEBUG_LEVEL >= 1
char dir[]= "./image/";
#elif W_TARGET == W_ELF
char dir[]= W_INSTALL_DATA"/image/";
#endif
#if W_TARGET == W_ELF
char*ext;
bool ret= true;
#endif
char*filename,complete_path[256];
va_start(valist,height);
filename= va_arg(valist,char*);
if(filename!=NULL&&filename[0]!='\0'){
_interfaces[_number_of_loops][i]._loaded_texture= false;

strncpy(complete_path,dir,256);
complete_path[255]= '\0';
strncat(complete_path,filename,256-strlen(complete_path));

#if W_TARGET == W_WEB
mkdir("images/",0777);
W.pending_files++;

emscripten_async_wget2(complete_path,complete_path,
"GET","",
(void*)&(_interfaces[_number_of_loops][i]),
&onload_texture,&onerror_texture,
&onprogress_texture);
#else

ext= strrchr(filename,'.');
if(!ext){
fprintf(stderr,"WARNING (0): No file extension in %s.\n",
filename);
_interfaces[_number_of_loops][i].type= W_NONE;
return NULL;
}
if(!strcmp(ext,".gif")||!strcmp(ext,".GIF")){
_interfaces[_number_of_loops][i]._texture= 
_extract_gif(complete_path,
&(_interfaces[_number_of_loops][i].number_of_frames),
&(_interfaces[_number_of_loops][i].frame_duration),
&(_interfaces[_number_of_loops][i].max_repetition),
&ret);
if(ret){
_interfaces[_number_of_loops][i].type= W_NONE;
return NULL;
}

if(_interfaces[_number_of_loops][i].number_of_frames> 1)
_interfaces[_number_of_loops][i].animate= true;
_interfaces[_number_of_loops][i]._loaded_texture= true;



_finalize_after(&(_interfaces[_number_of_loops][i]),
_finalize_interface_texture);
}
/*685:*/
#line 15281 "./cweb/weaver.w"

#ifndef W_DISABLE_PNG
if(!strcmp(ext,".png")||!strcmp(ext,".PNG")){
_interfaces[_number_of_loops][i]._texture= 
_extract_png(complete_path,
&(_interfaces[_number_of_loops][i].number_of_frames),
&(_interfaces[_number_of_loops][i].frame_duration),
&(_interfaces[_number_of_loops][i].max_repetition),
&ret);
if(ret){
_interfaces[_number_of_loops][i].type= W_NONE;
return NULL;
}

_interfaces[_number_of_loops][i].animate= false;
_interfaces[_number_of_loops][i]._loaded_texture= true;

_finalize_after(&(_interfaces[_number_of_loops][i]),
_finalize_interface_texture);
}
#endif
/*:685*/
#line 14807 "./cweb/weaver.w"

#endif
}
va_end(valist);
}
/*:663*/
#line 9010 "./cweb/weaver.w"

;
}
/*431:*/
#line 9484 "./cweb/weaver.w"

{
float nx,ny,cosine,sine,x1,y1;
nx= 2.0*((float)_interfaces[_number_of_loops][i].width);
ny= 2.0*((float)_interfaces[_number_of_loops][i].height);
cosine= cosf(_interfaces[_number_of_loops][i].rotation);
sine= sinf(_interfaces[_number_of_loops][i].rotation);
x1= (2.0*((float)_interfaces[_number_of_loops][i].x/
(float)W.width))-1.0;
y1= (2.0*((float)_interfaces[_number_of_loops][i].y/
(float)W.height))-1.0;
_interfaces[_number_of_loops][i]._transform_matrix[0]= nx*cosine/
(float)W.width;
_interfaces[_number_of_loops][i]._transform_matrix[4]= -(ny*sine)/
(float)W.width;
_interfaces[_number_of_loops][i]._transform_matrix[8]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[12]= x1;
_interfaces[_number_of_loops][i]._transform_matrix[1]= nx*sine/
(float)W.height;
_interfaces[_number_of_loops][i]._transform_matrix[5]= ny*cosine/
(float)W.height;
_interfaces[_number_of_loops][i]._transform_matrix[9]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[13]= y1;
_interfaces[_number_of_loops][i]._transform_matrix[2]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[3]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[10]= 1.0;
_interfaces[_number_of_loops][i]._transform_matrix[14]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[3]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[7]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[11]= 0.0;
_interfaces[_number_of_loops][i]._transform_matrix[15]= 1.0;
}
/*:431*/
#line 9013 "./cweb/weaver.w"

/*474:*/
#line 10557 "./cweb/weaver.w"


_insert_interface_queue(&(_interfaces[_number_of_loops][i]));
/*:474*/
#line 9014 "./cweb/weaver.w"

new_interface= &(_interfaces[_number_of_loops][i]);
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_interface_mutex);
#endif
return new_interface;
}
/*:407*//*411:*/
#line 9045 "./cweb/weaver.w"

bool _destroy_interface(struct interface*inter){
int i;

for(i= 0;i<W_MAX_INTERFACES;i++)
if(&(_interfaces[_number_of_loops][i])==inter&&inter->type!=W_NONE)
break;
if(i==W_MAX_INTERFACES)
return false;
/*475:*/
#line 10565 "./cweb/weaver.w"


_remove_interface_queue(&(_interfaces[_number_of_loops][i]));
/*:475*//*654:*/
#line 14571 "./cweb/weaver.w"

{
_finalize_this(&_interfaces[_number_of_loops][i],true);
if(_interfaces[_number_of_loops][i]._texture!=&_empty_texture)
Wfree(_interfaces[_number_of_loops][i]._texture);
_finalize_interface_texture((void*)&_interfaces[_number_of_loops][i]);
}
/*:654*/
#line 9054 "./cweb/weaver.w"

switch(_interfaces[_number_of_loops][i].type){

case W_INTERFACE_SQUARE:
case W_INTERFACE_PERIMETER:
case W_NONE:
default:
_interfaces[_number_of_loops][i].type= W_NONE;
}
#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&(_interfaces[_number_of_loops][i]._mutex))!=0){
perror("Error destroying mutex from interface:");
Wexit();
}
#endif
return true;
}
/*:411*//*415:*/
#line 9099 "./cweb/weaver.w"

struct interface*_copy_interface(struct interface*inter){
int i;
struct interface*new_interface= (struct interface*)1;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_interface_mutex);
#endif

for(i= 0;i<W_MAX_INTERFACES;i++)
if(_interfaces[_number_of_loops][i].type==W_NONE)
break;
if(i==W_MAX_INTERFACES){
#if W_DEBUG_LEVEL >  0
fprintf(stderr,"WARNING (1): Not enough space for interfaces. Please, "
"increase the value of W_MAX_INTERFACES at conf/conf.h.\n");
#endif
new_interface= NULL;
}
else{

new_interface= &(_interfaces[_number_of_loops][i]);
memcpy(new_interface,inter,sizeof(struct interface));


#ifdef W_MULTITHREAD
if(pthread_mutex_init(&(_interfaces[_number_of_loops][i]._mutex),
NULL)!=0){
_interfaces[_number_of_loops][i].type= W_NONE;
perror("Initializing interface mutex:");
new_interface= NULL;
}
#endif
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_interface_mutex);
#endif
return new_interface;
}
/*:415*//*420:*/
#line 9223 "./cweb/weaver.w"

void _move_interface(struct interface*inter,float x,float y){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(inter->_mutex));
#endif
inter->x= x;
inter->y= y;
/*432:*/
#line 9521 "./cweb/weaver.w"

{
float x1,y1;
x1= (2.0*((float)inter->x/(float)W.width))-1.0;
y1= (2.0*((float)inter->y/(float)W.height))-1.0;
inter->_transform_matrix[12]= x1;
inter->_transform_matrix[13]= y1;
}
/*:432*/
#line 9230 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(inter->_mutex));
#endif
}
/*:420*//*424:*/
#line 9302 "./cweb/weaver.w"

void _resize_interface(struct interface*inter,float size_x,float size_y){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(inter->_mutex));
#endif
inter->height= size_y;
inter->width= size_x;
/*433:*/
#line 9534 "./cweb/weaver.w"

{
float nx,ny,cosine,sine;
nx= 2.0*((float)inter->width);
ny= 2.0*((float)inter->height);
cosine= cosf(inter->rotation);
sine= sinf(inter->rotation);
inter->_transform_matrix[0]= (nx*cosine)/(float)W.width;
inter->_transform_matrix[4]= -(ny*sine)/(float)W.width;
inter->_transform_matrix[1]= (nx*sine)/(float)W.height;
inter->_transform_matrix[5]= (ny*cosine)/(float)W.height;
}
/*:433*/
#line 9309 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(inter->_mutex));
#endif
}
/*:424*//*428:*/
#line 9383 "./cweb/weaver.w"

void _rotate_interface(struct interface*inter,float rotation){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(inter->_mutex));
#endif
inter->rotation= rotation;
/*433:*/
#line 9534 "./cweb/weaver.w"

{
float nx,ny,cosine,sine;
nx= 2.0*((float)inter->width);
ny= 2.0*((float)inter->height);
cosine= cosf(inter->rotation);
sine= sinf(inter->rotation);
inter->_transform_matrix[0]= (nx*cosine)/(float)W.width;
inter->_transform_matrix[4]= -(ny*sine)/(float)W.width;
inter->_transform_matrix[1]= (nx*sine)/(float)W.height;
inter->_transform_matrix[5]= (ny*cosine)/(float)W.height;
}
/*:433*/
#line 9389 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(inter->_mutex));
#endif
}
/*:428*//*435:*/
#line 9556 "./cweb/weaver.w"

void _update_interface_screen_size(void){
int i,j;
float nx,ny,cosine,sine;
for(i= 0;i<_number_of_loops;i++)
for(j= 0;j<W_MAX_INTERFACES;j++){
if(_interfaces[i][j].type==W_NONE)continue;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_interfaces[i][j]._mutex);
#endif
nx= 2.0*_interfaces[i][j].width;
ny= 2.0*_interfaces[i][j].height;
cosine= cosf(_interfaces[i][j].rotation);
sine= sinf(_interfaces[i][j].rotation);
_interfaces[i][j]._transform_matrix[0]= (nx*cosine)/
(float)W.width;
_interfaces[i][j]._transform_matrix[4]= -(ny*sine)/
(float)W.width;
_interfaces[i][j]._transform_matrix[1]= (nx*sine)/
(float)W.height;
_interfaces[i][j]._transform_matrix[5]= (ny*cosine)/
(float)W.height;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_interfaces[i][j]._mutex);
#endif
}
}
/*:435*//*467:*/
#line 10414 "./cweb/weaver.w"

void _insert_interface_queue(struct interface*inter){
int begin,end,middle,tmp;
int type= inter->type;
if(_interface_queue[_number_of_loops][W_MAX_INTERFACES-1]!=NULL){
fprintf(stderr,"WARNING (0): Couldn't create new interface. You "
"should increase the value of W_MAX_INTERFACES at cont/conf.h "
"or decrease the number of inerfaces created.\n");
return;
}
begin= 0;
end= W_MAX_INTERFACES-1;
middle= (begin+end)/2;
while((_interface_queue[_number_of_loops][middle]==NULL||
_interface_queue[_number_of_loops][middle]->type!=type)&&
begin!=end){
if(_interface_queue[_number_of_loops][middle]==NULL||
_interface_queue[_number_of_loops][middle]->type> type){
tmp= (middle+end)/2;
if(tmp==end)end--;
else end= tmp;
middle= (begin+end)/2;
}
else{
tmp= (middle+begin)/2;
if(tmp==begin)begin++;
else begin= tmp;
middle= (begin+end)/2;
}
}


for(tmp= W_MAX_INTERFACES-1;tmp>=middle;tmp--)
_interface_queue[_number_of_loops][tmp]= 
_interface_queue[_number_of_loops][tmp-1];

_interface_queue[_number_of_loops][middle]= inter;
}
/*:467*//*469:*/
#line 10462 "./cweb/weaver.w"

void _remove_interface_queue(struct interface*inter){
int begin,end,middle,tmp;
int type= inter->type;
begin= 0;
end= W_MAX_INTERFACES-1;
middle= (begin+end)/2;
while((_interface_queue[_number_of_loops][middle]==NULL||
_interface_queue[_number_of_loops][middle]->type!=type)
&&begin!=end){
if(_interface_queue[_number_of_loops][middle]==NULL||
_interface_queue[_number_of_loops][middle]->type> type){
tmp= (middle+end)/2;
if(tmp==end)end--;
else end= tmp;
middle= (begin+end)/2;
}
else{
tmp= (middle+begin)/2;
if(tmp==begin)begin++;
else begin= tmp;
middle= (begin+end)/2;
}
}

while(middle> 0&&_interface_queue[_number_of_loops][middle]!=NULL&&
_interface_queue[_number_of_loops][middle]->type==
_interface_queue[_number_of_loops][middle-1]->type)
middle--;
if(_interface_queue[_number_of_loops][middle]->type!=type){
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,
"WARNING (1): Tried to erase a non-existent interface.\n");
#endif
return;
}


while(_interface_queue[_number_of_loops][middle]!=NULL&&
_interface_queue[_number_of_loops][middle]->type==type&&
_interface_queue[_number_of_loops][middle]!=inter)
middle++;


if(_interface_queue[_number_of_loops][middle]==inter){
while(_interface_queue[_number_of_loops][middle]!=NULL&&
middle!=W_MAX_INTERFACES-1){
_interface_queue[_number_of_loops][middle]= 
_interface_queue[_number_of_loops][middle+1];
middle++;
}
_interface_queue[_number_of_loops][W_MAX_INTERFACES-1]= NULL;
}
else{
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,
"WARNING (1): Tried to erase a non-existent interface.\n");
#endif
return;
}
}
/*:469*//*471:*/
#line 10531 "./cweb/weaver.w"

void _clean_interface_queue(void){
int i;
for(i= 0;i<W_MAX_INTERFACES;i++)
_interface_queue[_number_of_loops][i]= NULL;
}
/*:471*//*653:*/
#line 14556 "./cweb/weaver.w"



void _finalize_interface_texture(void*data){
struct interface*p= (struct interface*)data;
glDeleteTextures(p->number_of_frames,p->_texture);
}
/*:653*//*669:*/
#line 14941 "./cweb/weaver.w"

#if !defined(W_DISABLE_PNG) && (W_TARGET == W_ELF)
GLuint*_extract_png(char*filename,unsigned*number_of_frames,
unsigned**frame_duration,int*max_repetition,
bool*error){
int width,height;
unsigned char*pixel_array= NULL;
png_structp png_ptr;
png_infop info_ptr;
png_bytep*row_pointers= NULL;
png_byte color_type,bit_depth;
GLuint*returned_data= NULL;
*number_of_frames= 1;
*frame_duration= NULL;
*max_repetition= 0;
FILE*fp= fopen(filename,"r");
*error= false;

if(fp==NULL){
fprintf(stderr,"ERROR: Can't open file %s.\n",filename);
goto error_png;
}
/*670:*/
#line 14986 "./cweb/weaver.w"

{
size_t size_t_ret;
unsigned char header[8];






size_t_ret= fread(header,1,8,fp);
if(png_sig_cmp(header,0,8)||size_t_ret!=8){
fprintf(stderr,"ERROR: %s don't have a PNG header.\n",filename);
goto error_png;
}
}
/*:670*//*671:*/
#line 15007 "./cweb/weaver.w"

{






png_ptr= png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
if(!png_ptr){
fprintf(stderr,"ERROR: Can't create structure to read PNG.\n");
goto error_png;
}
info_ptr= png_create_info_struct(png_ptr);
if(!info_ptr){
fprintf(stderr,"ERROR: Can't create structure to read PNG.\n");
goto error_png;
}
}
/*:671*//*672:*/
#line 15035 "./cweb/weaver.w"

{
if(setjmp(png_jmpbuf(png_ptr))){
fprintf(stderr,"ERROR: %s initialization failed.\n",filename);
goto error_png;
}
}
/*:672*//*673:*/
#line 15048 "./cweb/weaver.w"

{
png_init_io(png_ptr,fp);
}
/*:673*//*674:*/
#line 15059 "./cweb/weaver.w"

{
png_set_sig_bytes(png_ptr,8);
}
/*:674*//*675:*/
#line 15070 "./cweb/weaver.w"

{
png_read_info(png_ptr,info_ptr);
width= png_get_image_width(png_ptr,info_ptr);
height= png_get_image_height(png_ptr,info_ptr);
color_type= png_get_color_type(png_ptr,info_ptr);
bit_depth= png_get_bit_depth(png_ptr,info_ptr);
}
/*:675*//*676:*/
#line 15083 "./cweb/weaver.w"

{


if(color_type==PNG_COLOR_TYPE_PALETTE)
png_set_palette_to_rgb(png_ptr);


if(color_type==PNG_COLOR_TYPE_GRAY&&
bit_depth<8)png_set_expand_gray_1_2_4_to_8(png_ptr);


if(png_get_valid(png_ptr,info_ptr,
PNG_INFO_tRNS))png_set_tRNS_to_alpha(png_ptr);

if(bit_depth==16)
png_set_strip_16(png_ptr);

if(bit_depth<8)
png_set_packing(png_ptr);

if(color_type==PNG_COLOR_TYPE_GRAY||
color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
png_set_gray_to_rgb(png_ptr);
}
/*:676*//*677:*/
#line 15115 "./cweb/weaver.w"

{
png_read_update_info(png_ptr,info_ptr);
if(setjmp(png_jmpbuf(png_ptr))){
fprintf(stderr,"ERROR: Failed to interpret %s .\n",filename);
goto error_png;
}
}
/*:677*//*678:*/
#line 15131 "./cweb/weaver.w"

{
int y,z;
returned_data= (GLuint*)Walloc(sizeof(GLuint));
if(returned_data==NULL){
fprintf(stderr,"ERROR: Not enough memory to read %s. Please, increase "
"the value of W_MAX_MEMORY at conf/conf.h.\n",filename);
goto error_png;
}
pixel_array= (unsigned char*)Walloc(width*height*4);
if(pixel_array==NULL){
fprintf(stderr,"ERROR: No enough memory to load %s. "
"Please increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
goto error_png;
}
row_pointers= (png_bytep*)Walloc(sizeof(png_bytep)*height);
if(row_pointers==NULL){
Wfree(pixel_array);
fprintf(stderr,"ERROR: No enough memory to load %s. "
"Please increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
goto error_png;
}
for(y= 0;y<height;y++){
row_pointers[y]= (png_byte*)Walloc(png_get_rowbytes(png_ptr,info_ptr));
if(row_pointers[y]==NULL){
for(z= y-1;z>=0;z--)
Wfree(row_pointers[z]);
Wfree(row_pointers);
row_pointers= NULL;
Wfree(pixel_array);
pixel_array= NULL;
fprintf(stderr,"ERROR: No enough memory to load %s. "
"Please increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
goto error_png;
}
}

png_read_image(png_ptr,row_pointers);
}
/*:678*//*680:*/
#line 15195 "./cweb/weaver.w"

{
color_type= png_get_color_type(png_ptr,info_ptr);
switch(color_type){
case PNG_COLOR_TYPE_RGB:
/*682:*/
#line 15229 "./cweb/weaver.w"

{
int x,y;
for(y= 0;y<height;y++){
png_byte*row= row_pointers[y];
for(x= 0;x<width;x++){
png_byte*ptr= &(row[x*3]);
pixel_array[4*width*(height-y-1)+x*4]= ptr[0];
pixel_array[4*width*(height-y-1)+x*4+1]= ptr[1];
pixel_array[4*width*(height-y-1)+x*4+2]= ptr[2];
pixel_array[4*width*(height-y-1)+x*4+3]= 255;
}
}
}
/*:682*/
#line 15200 "./cweb/weaver.w"

break;
case PNG_COLOR_TYPE_RGBA:
/*681:*/
#line 15211 "./cweb/weaver.w"

{
int x,y;
for(y= 0;y<height;y++){
png_byte*row= row_pointers[y];
for(x= 0;x<width;x++){
png_byte*ptr= &(row[x*4]);
pixel_array[4*width*(height-y-1)+x*4]= ptr[0];
pixel_array[4*width*(height-y-1)+x*4+1]= ptr[1];
pixel_array[4*width*(height-y-1)+x*4+2]= ptr[2];
pixel_array[4*width*(height-y-1)+x*4+3]= ptr[3];
}
}
}
/*:681*/
#line 15203 "./cweb/weaver.w"

break;
}
}
/*:680*//*683:*/
#line 15249 "./cweb/weaver.w"

{
int z;
for(z= height-1;z>=0;z--)
Wfree(row_pointers[z]);
Wfree(row_pointers);
row_pointers= NULL;
}
/*:683*//*684:*/
#line 15262 "./cweb/weaver.w"

{
glGenTextures(1,returned_data);
glBindTexture(GL_TEXTURE_2D,*returned_data);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
GL_UNSIGNED_BYTE,pixel_array);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
glBindTexture(GL_TEXTURE_2D,0);
}
/*:684*/
#line 14963 "./cweb/weaver.w"

goto end_of_png;
error_png:

*error= true;
returned_data= NULL;
end_of_png:

#if W_TARGET == W_ELF && !defined(W_MULTITHREAD)
fclose(fp);
#else
/*679:*/
#line 15178 "./cweb/weaver.w"

{
if(row_pointers!=NULL){
int z;
for(z= height-1;z>=0;z--)
Wfree(row_pointers[z]);
Wfree(row_pointers);
}
if(pixel_array!=NULL)
Wfree(pixel_array);
}
/*:679*/
#line 14974 "./cweb/weaver.w"

#endif
return returned_data;
}
#endif
/*:669*/
#line 8723 "./cweb/weaver.w"

/*:392*/
