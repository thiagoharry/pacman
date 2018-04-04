/*440:*/
#line 9649 "cweb/weaver.w"

#include <sys/types.h>  
#include <sys/stat.h>  
#include <string.h>  
#include <dirent.h>  
#include <ctype.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include "shaders.h"
/*448:*/
#line 9750 "cweb/weaver.w"

char _vertex_interface[]= {
#include "vertex_interface.data"
,0x00};
char _fragment_interface[]= {
#include "fragment_interface.data"
,0x00};
/*:448*//*450:*/
#line 9768 "cweb/weaver.w"

GLuint _compile_shader(char*source,bool vertex){
GLuint shader;
GLint success= 0,logSize= 0;

if(vertex)
shader= glCreateShader(GL_VERTEX_SHADER);
else
shader= glCreateShader(GL_FRAGMENT_SHADER);

glShaderSource(shader,1,(const GLchar**)&source,NULL);

glCompileShader(shader);

glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
if(success==GL_FALSE){
char*buffer;
glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logSize);
buffer= (char*)_iWalloc(logSize);
if(buffer==NULL){
fprintf(stderr,"ERROR (0): Shader failed to compile. "
"It wasn't possible to discover why because there's no "
"enough internal memory. Please, increase "
"the value of W_INTERNAL_MEMORY at conf/conf.h and try "
"to run this program again.\n");
exit(1);
}
glGetShaderInfoLog(shader,logSize,NULL,buffer);
fprintf(stderr,"ERROR (0): Failed to compile shader: %s\n",
buffer);
Wfree(buffer);
exit(1);
}
return shader;
}
/*:450*//*452:*/
#line 9813 "cweb/weaver.w"

GLuint _link_and_clean_shaders(GLuint vertex,GLuint fragment){
GLuint program= glCreateProgram();
glAttachShader(program,vertex);
glAttachShader(program,fragment);
glLinkProgram(program);

{
int isLinked= 0;
GLint logSize= 0;
glGetProgramiv(program,GL_LINK_STATUS,&isLinked);
if(isLinked==GL_FALSE){
char*buffer;
glGetShaderiv(program,GL_INFO_LOG_LENGTH,&logSize);
buffer= (char*)_iWalloc(logSize);
if(buffer==NULL){
fprintf(stderr,"ERROR (0): Shaders failed to link. It wasn't "
"possible to discover why because there's no enough "
"internal memory. Please, increase "
"the value of W_INTERNAL_MEMORY at conf/conf.h and try "
"to run this program again.\n");
exit(1);
}
glGetShaderInfoLog(program,logSize,NULL,buffer);
fprintf(stderr,"ERROR (0): Failed to link shader: %s\n",buffer);
Wfree(buffer);
exit(1);
}
}
glDetachShader(program,vertex);
glDetachShader(program,fragment);
return program;
}
/*:452*//*462:*/
#line 10143 "cweb/weaver.w"

void _compile_and_insert_new_shader(char*dir,int position){
char*vertex_file= NULL,*fragment_file= NULL;
char*vertex_source= NULL,*fragment_source= NULL;
off_t vertex_size= 0,fragment_size= 0;
GLuint vertex,fragment;
size_t size_t_ret;
bool read_error= false;
char*p;
int i;
FILE*fp;

_shader_list[position].initialized= true;


for(p= dir;*p!='\0';p++);
while(*(p-1)=='/')p--;
while(*(p-1)!='/'&&p-1!=dir)p--;
for(i= 0;p[i]!='\0'&&p[i]!='/'&&i<127;i++)
_shader_list[position].name[i]= p[i];
_shader_list[position].name[i]= '\0';

vertex_file= (char*)_iWalloc(strlen(dir)+strlen("/vertex.glsl")+1);
vertex_file[0]= '\0';
strcat(vertex_file,dir);
strcat(vertex_file,"/vertex.glsl");

if((fp= fopen(vertex_file,"r"))){
_shader_list[position].vertex_source= vertex_file;
fclose(fp);
}
else{
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): Vertex shader source code not found. "
"File %s was expected. Using a default shader instead.\n",
vertex_file);
#endif
_shader_list[position].vertex_source= NULL;
Wfree(vertex_file);
vertex_file= NULL;
}

fragment_file= (char*)_iWalloc(strlen(dir)+strlen("/fragment.glsl")+
1);
fragment_file[0]= '\0';
strcat(fragment_file,dir);
strcat(fragment_file,"/fragment.glsl");
if((fp= fopen(fragment_file,"r"))){
_shader_list[position].fragment_source= fragment_file;
fclose(fp);
}
else{
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): Fragment shader source code not found. "
"File '%s' was expected. Using a default shader instead.\n",
fragment_file);
#endif
_shader_list[position].fragment_source= NULL;
Wfree(fragment_file);
fragment_file= NULL;
}


if(_shader_list[position].vertex_source!=NULL){
int fd;
fd= open(_shader_list[position].vertex_source,O_RDONLY);
if(fd<0){
fprintf(stderr,"WARNING (0): Can't read vertex shader source"
" code at %s. Using a default shader instead.\n",
_shader_list[position].vertex_source);





_shader_list[position].vertex_source= NULL;
}
else{
int ret;
struct stat attr;
ret= fstat(fd,&attr);
if(ret<0){
fprintf(stderr,"WARNING (0): Can't read shader source file"
" stats: %s. Ignoring source code and using a default"
"shader code.\n",
_shader_list[position].vertex_source);
_shader_list[position].vertex_source= NULL;
}
else{
#if W_TARGET == W_ELF
_shader_list[position].vertex_inode= attr.st_ino;
#endif
vertex_size= attr.st_size;
}
close(fd);
}
}

if(_shader_list[position].fragment_source!=NULL){
int fd;
struct stat attr;
fd= open(_shader_list[position].fragment_source,O_RDONLY);
if(fd<0){
fprintf(stderr,"WARNING (0): Can't read fragment shader source"
" code at %s. Using a default shader instead.\n",
_shader_list[position].fragment_source);
_shader_list[position].fragment_source= NULL;
}
else{
int ret;
ret= fstat(fd,&attr);
if(ret<0){
fprintf(stderr,"WARNING (0): Can't read shader source file"
" stats: %s. Ignoring source code and using a default"
"shader code.\n",
_shader_list[position].fragment_source);
_shader_list[position].fragment_source= NULL;
}
else{
#if W_TARGET == W_ELF
_shader_list[position].fragment_inode= attr.st_ino;
#endif
fragment_size= attr.st_size;
}
close(fd);
}
}

if(_shader_list[position].vertex_source!=NULL){
vertex_source= (char*)_iWalloc(vertex_size);
if(vertex_source==NULL){
fprintf(stderr,"WARNING (0): Can't read shader source code at %s."
" File too big.\n",vertex_file);
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): You should increase the value of "
"W_INTERNAL_MEMORY at conf/conf.h.\n");
#endif
_shader_list[position].vertex_source= NULL;
}
}
if(_shader_list[position].fragment_source!=NULL){
fragment_source= (char*)_iWalloc(fragment_size);
if(fragment_source==NULL){
fprintf(stderr,"WARNING (0): Can't read shader source code at %s."
" File too big.\n",fragment_file);
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): You should increase the value of "
"W_INTERNAL_MEMORY at conf/conf.h.\n");
#endif
_shader_list[position].fragment_source= NULL;
}
}

if(_shader_list[position].vertex_source!=NULL){
FILE*fd= fopen(_shader_list[position].vertex_source,"r");
if(fd==NULL){
fprintf(stderr,"WARNING (0): Can't read shader source code at"
" %s.\n",vertex_file);
perror(NULL);
_shader_list[position].vertex_source= NULL;
}
else{
size_t_ret= fread(vertex_source,sizeof(char),vertex_size,fd);
if(size_t_ret!=vertex_size*sizeof(char))
read_error= true;
vertex_source[vertex_size-1]= '\0';
fclose(fd);
}
}
if(_shader_list[position].fragment_source!=NULL){
FILE*fd= fopen(_shader_list[position].fragment_source,"r");
if(fd==NULL){
fprintf(stderr,"WARNING (0): Can't read shader source code at"
" %s.\n",fragment_file);
perror(NULL);
_shader_list[position].fragment_source= NULL;
}
else{
size_t_ret= fread(fragment_source,sizeof(char),fragment_size,fd);
if(size_t_ret!=vertex_size*sizeof(char))
read_error= true;
fragment_source[fragment_size-1]= '\0';
fclose(fd);
}
}
if(read_error){
#if W_DEBUG_LEVEL >= 4
fprintf(stderr,"WARNING (3): Something failed while reading shader file.\n");
#else
read_error= false;
#endif
}


if(_shader_list[position].vertex_source!=NULL)
vertex= _compile_vertex_shader(vertex_source);
else
vertex= _compile_vertex_shader(_vertex_interface);
if(_shader_list[position].fragment_source!=NULL)
fragment= _compile_fragment_shader(fragment_source);
else
fragment= _compile_fragment_shader(_fragment_interface);
_shader_list[position].program_shader= _link_and_clean_shaders(vertex,
fragment);

_shader_list[position]._uniform_object_color= 
glGetUniformLocation(_shader_list[position].program_shader,
"object_color");
_shader_list[position]._uniform_object_size= 
glGetUniformLocation(_shader_list[position].program_shader,
"object_size");
_shader_list[position]._uniform_time= 
glGetUniformLocation(_shader_list[position].program_shader,
"time");
_shader_list[position]._uniform_integer= 
glGetUniformLocation(_shader_list[position].program_shader,
"integer");
_shader_list[position]._uniform_texture1= 
glGetUniformLocation(_shader_list[position].program_shader,
"texture1");
_shader_list[position]._uniform_model_view= 
glGetUniformLocation(_shader_list[position].program_shader,
"model_view_matrix");

_shader_list[position]._attribute_vertex_position= 
glGetAttribLocation(_shader_list[position].program_shader,
"vertex_position");

if(fragment_source!=NULL)Wfree(fragment_source);
if(vertex_source!=NULL)Wfree(vertex_source);
}
/*:462*//*492:*/
#line 10937 "cweb/weaver.w"

char _vertex_interface_texture[]= {
#include "vertex_interface_texture.data"
,0x00};
char _fragment_interface_texture[]= {
#include "fragment_interface_texture.data"
,0x00};
/*:492*//*498:*/
#line 11118 "cweb/weaver.w"

void _change_resolution(int resolution_x,int resolution_y){
#if W_WIDTH != 0 || W_HEIGHT != 0
W.resize_window(resolution_x,resolution_y);
#else
int width,height,old_width= W.width,old_height= W.height;
int i,j;
_changed_resolution= true;

for(i= 0;i<W_MAX_SUBLOOP;i++)
if(_final_shader[i]==W_NONE)
_final_shader[i]= W_DEFAULT_SHADER;
width= W.width= ((resolution_x> 0)?(resolution_x):(W.width));
height= W.height= ((resolution_y> 0)?(resolution_y):(W.height));



glBindFramebuffer(GL_FRAMEBUFFER,_framebuffer);
glDeleteTextures(1,&_texture);
glGenTextures(1,&_texture);
glBindTexture(GL_TEXTURE_2D,_texture);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,W.width,W.height,0,GL_RGB,
GL_UNSIGNED_BYTE,NULL);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
GL_TEXTURE_2D,_texture,0);

glDeleteRenderbuffers(1,&_depth_stencil);
glGenRenderbuffers(1,&_depth_stencil);
glBindRenderbuffer(GL_RENDERBUFFER,_depth_stencil);
glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,
W.width,W.height);

glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
GL_RENDERBUFFER,_depth_stencil);
glBindFramebuffer(GL_FRAMEBUFFER,0);


for(i= 0;i<W_MAX_SUBLOOP;i++){
for(j= 0;j<W_MAX_INTERFACES;j++){
if(_interfaces[i][j].type==W_NONE)continue;
W.move_interface(&_interfaces[i][j],
_interfaces[i][j].x*
((float)width)/((float)old_width),
_interfaces[i][j].y*
((float)height)/((float)old_height));
W.rotate_interface(&_interfaces[i][j],
_interfaces[i][j].rotation);


{
float new_height= _interfaces[i][j].height;
float new_width= _interfaces[i][j].width;
new_width*= (float)width/(float)old_width;
new_height*= (float)height/(float)old_height;
W.resize_interface(&_interfaces[i][j],new_width,new_height);
}
}
}
#endif
}
/*:498*//*503:*/
#line 11212 "cweb/weaver.w"

void _change_final_shader(int type){
_final_shader[_number_of_loops]= type;
}
/*:503*//*659:*/
#line 14764 "cweb/weaver.w"

char _vertex_image_interface[]= {
#include "vertex_image_interface.data"
,0x00};
char _fragment_image_interface[]= {
#include "fragment_image_interface.data"
,0x00};
/*:659*/
#line 9658 "cweb/weaver.w"

/*:440*/
