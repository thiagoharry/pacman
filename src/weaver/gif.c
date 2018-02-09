/*598:*/
#line 12725 "cweb/weaver.w"

#include "weaver.h"

/*619:*/
#line 13264 "cweb/weaver.w"

#if W_TARGET != W_ELF || defined(W_MULTITHREAD)
static void free_img_list(struct _image_list*last){
struct _image_list*p= last,*tmp;
while(p!=NULL){
Wfree(p->rgba_image);
tmp= p;
p= p->prev;
Wfree(tmp);
}
}
#endif
/*:619*//*635:*/
#line 13820 "cweb/weaver.w"

void preenche_pixel(unsigned char*img,unsigned char**code_table,
unsigned code,
unsigned char*color_table,int size,
bool transparent_color_flag,unsigned transparency_index){
int i= 0;
for(i= 0;i<size;i++){


if(code_table[code]==NULL){
img[4*i]= color_table[3*code];
img[4*i+1]= color_table[3*code+1];
img[4*i+2]= color_table[3*code+2];
if(transparent_color_flag&&transparency_index==code){
img[4*i+3]= 0;
}
else{
img[4*i+3]= 255;
}
}


else{
img[4*i]= color_table[3*code_table[code][i]];
img[4*i+1]= color_table[3*code_table[code][i]+1];
img[4*i+2]= color_table[3*code_table[code][i]+2];
if(transparent_color_flag&&transparency_index==code_table[code][i]){
img[4*i+3]= 0;
}
else{
img[4*i+3]= 255;
}
}
}
}
/*:635*//*636:*/
#line 13863 "cweb/weaver.w"

unsigned char*produz_codigo(unsigned char*codigo,int size,char adicao){
int i;
unsigned char*ret= (unsigned char*)Walloc(size+1);
if(ret==NULL){
fprintf(stderr,"WARNING (0): No memory to parse image. Please, increase "
"the value of W_MAX_MEMORY at conf/conf.h.\n");
return NULL;
}
for(i= 0;i<size;i++)
ret[i]= codigo[i];
ret[size]= adicao;
return ret;
}
/*:636*/
#line 12728 "cweb/weaver.w"

/*602:*/
#line 12753 "cweb/weaver.w"

GLuint*_extract_gif(char*filename,unsigned*number_of_frames,
unsigned**frame_duration,
int*max_repetition,bool*error){

bool global_color_table_flag= false,local_color_table_flag= false;
bool interlace_flag= false;
bool transparent_color_flag= false;
unsigned local_color_table_size= 0,global_color_table_size= 0;
unsigned long image_size;
unsigned img_offset_x= 0,img_offset_y= 0,img_width= 0,img_height= 0;
unsigned number_of_loops= 0;
GLuint*returned_data= NULL;
unsigned background_color,delay_time= 0;
unsigned char transparency_index= 0;
unsigned char*global_color_table= NULL;
unsigned char*local_color_table= NULL;
unsigned long width,height;
int disposal_method= 0;
struct _image_list*img= NULL;
struct _image_list*last_img= NULL;
#ifdef W_MULTITHREAD
GLXContext thread_context;
#endif
*number_of_frames= 0;

FILE*fp= fopen(filename,"r");
*error= false;
#if W_TARGET == W_ELF && !defined(W_MULTITHREAD)
_iWbreakpoint();
#endif

if(fp==NULL){
fprintf(stderr,"ERROR: Can't open file %s.\n",filename);
goto error_gif;
}
/*604:*/
#line 12823 "cweb/weaver.w"

{
char data[4];
fread(data,1,3,fp);
data[3]= '\0';
if(strcmp(data,"GIF")){
fprintf(stderr,"WARNING: Not a GIF file: %s\n",filename);
goto error_gif;
}
fread(data,1,3,fp);
data[3]= '\0';
if(strcmp(data,"87a")&&strcmp(data,"89a")){
fprintf(stderr,"WARNING: Not supported GIF version: %s\n",filename);
goto error_gif;
}
}
/*:604*//*605:*/
#line 12851 "cweb/weaver.w"

{


unsigned char data[2];
fread(data,1,2,fp);
width= ((unsigned long)data[1])*256+((unsigned long)data[0]);


fread(data,1,2,fp);
height= ((unsigned long)data[1])*256+((unsigned long)data[0]);
image_size= (width)*(height);


fread(data,1,1,fp);

global_color_table_flag= (data[0]&128);


global_color_table_size= data[0]%8;
global_color_table_size= 3*(1<<(global_color_table_size+1));

fread(&background_color,1,1,fp);

fread(data,1,1,fp);
}
/*:605*//*606:*/
#line 12882 "cweb/weaver.w"

if(global_color_table_flag){
global_color_table= (unsigned char*)_iWalloc(global_color_table_size);
if(global_color_table==NULL){
fprintf(stderr,"WARNING: Not enough memory to read image. Please, increase "
"the value of W_MAX_MEMORY at conf/conf.h.\n");
goto error_gif;
}

fread(global_color_table,1,global_color_table_size,fp);
}
/*:606*//*608:*/
#line 12910 "cweb/weaver.w"

{
unsigned block_type;
unsigned char data[2];
fread(data,1,1,fp);
block_type= data[0];
while(block_type!=59){
switch(block_type){
case 33:
/*609:*/
#line 12950 "cweb/weaver.w"

{
unsigned extension_type;
fread(data,1,1,fp);
extension_type= (unsigned)data[0];
switch(extension_type){
case 1:
/*610:*/
#line 12991 "cweb/weaver.w"

{


unsigned char buffer[256];
fread(buffer,1,15,fp);




fread(buffer,1,1,fp);
while(buffer[0]!=0){
fread(buffer,1,buffer[0],fp);
fread(buffer,1,1,fp);
}
/*614:*/
#line 13127 "cweb/weaver.w"

{
disposal_method= 0;
transparent_color_flag= false;
delay_time= 0;
transparency_index= 0;
}
/*:614*/
#line 13006 "cweb/weaver.w"

}
/*:610*/
#line 12957 "cweb/weaver.w"

break;
case 249:
/*613:*/
#line 13078 "cweb/weaver.w"

{



unsigned char buffer[256];
fread(buffer,1,1,fp);






fread(buffer,1,1,fp);








disposal_method= (buffer[0]>>2)%8;

transparent_color_flag= buffer[0]%2;


fread(buffer,1,2,fp);
delay_time= ((unsigned)buffer[1])*256+((unsigned)buffer[0]);


fread(buffer,1,1,fp);
transparency_index= buffer[0];



fread(buffer,1,1,fp);
}
/*:613*/
#line 12960 "cweb/weaver.w"

break;
case 254:
/*612:*/
#line 13062 "cweb/weaver.w"

{
unsigned char buffer[256];
fread(buffer,1,1,fp);
while(buffer[0]!=0){
fread(buffer,1,buffer[0],fp);
fread(buffer,1,1,fp);
}
}
/*:612*/
#line 12963 "cweb/weaver.w"

break;
case 255:
/*611:*/
#line 13027 "cweb/weaver.w"

{
bool netscape_extension= false;
char buffer[12];
unsigned char buffer2[256];



fread(buffer,1,1,fp);


fread(buffer,1,11,fp);
buffer[11]= '\0';
if(!strcmp(buffer,"NETSCAPE2.0"))
netscape_extension= true;

fread(buffer2,1,1,fp);
while(buffer2[0]!=0){
fread(buffer2,1,buffer2[0],fp);
if(netscape_extension&&buffer2[0]==1){

number_of_loops= ((unsigned)buffer2[2])*256+((unsigned)buffer2[1]);
}
fread(buffer2,1,1,fp);
}
}
/*:611*/
#line 12966 "cweb/weaver.w"

break;
default:
fprintf(stderr,"WARNING: Couldn't interpret GIF file %s. Invalid extension "
"block (%d).\n",filename,extension_type);
goto error_gif;
}
}
/*:609*/
#line 12919 "cweb/weaver.w"

break;
case 44:
/*615:*/
#line 13145 "cweb/weaver.w"

{
int lzw_minimum_code_size;

unsigned char buffer[257];
fread(buffer,1,2,fp);
img_offset_x= ((unsigned)buffer[1])*256+((unsigned)buffer[0]);

fread(buffer,1,2,fp);
img_offset_y= ((unsigned)buffer[1])*256+((unsigned)buffer[0]);

fread(buffer,1,2,fp);
img_width= ((unsigned)buffer[1])*256+((unsigned)buffer[0]);

fread(buffer,1,2,fp);
img_height= ((unsigned)buffer[1])*256+((unsigned)buffer[0]);


fread(buffer,1,1,fp);



interlace_flag= (buffer[0]>>6)%2;

local_color_table_size= buffer[0]%8;
local_color_table_size= 3*(1<<(local_color_table_size+1));

local_color_table_flag= buffer[0]>>7;

if(local_color_table_flag){
/*616:*/
#line 13185 "cweb/weaver.w"

{
local_color_table= (unsigned char*)_iWalloc(local_color_table_size);
if(local_color_table==NULL){
fprintf(stderr,"WARNING: Not enough memory to read image. Please, increase "
"the value of W_MAX_MEMORY at conf/conf.h.\n");
goto error_gif;
}

fread(local_color_table,1,local_color_table_size,fp);
}
/*:616*/
#line 13175 "cweb/weaver.w"

}
/*617:*/
#line 13201 "cweb/weaver.w"

{
int buffer_size;
/*622:*/
#line 13410 "cweb/weaver.w"

unsigned char*code_table[4096];
int code_table_size[4096];
unsigned last_value_in_code_table;
/*:622*//*625:*/
#line 13457 "cweb/weaver.w"

unsigned clear_code,end_of_information_code;
bool end_of_image= false;
/*:625*//*627:*/
#line 13485 "cweb/weaver.w"

int bits;
/*:627*//*629:*/
#line 13503 "cweb/weaver.w"

int byte_offset= 0,bit_offset= 0;
unsigned code= 0,previous_code;



bool incomplete_code= false;

unsigned long pixel= 0;

bool first_pixel= true;
/*:629*//*631:*/
#line 13663 "cweb/weaver.w"

unsigned char*color_table;
/*:631*/
#line 13204 "cweb/weaver.w"








fread(buffer,1,1,fp);
lzw_minimum_code_size= buffer[0];
/*620:*/
#line 13282 "cweb/weaver.w"

{
struct _image_list*new_image;

*number_of_frames= (*number_of_frames)+1;


if(!local_color_table_flag&&!global_color_table_flag){
fprintf(stderr,"WARNING: GIF image without a color table: %s\n",filename);
goto error_gif;
}

new_image= (struct _image_list*)_iWalloc(sizeof(struct _image_list));
if(new_image==NULL){
fprintf(stderr,"WARNING (0): Not enough memory to read GIF file %s. "
"Please, increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
goto error_gif;
}
new_image->prev= new_image->next= NULL;
new_image->x_offset= img_offset_x;
new_image->y_offset= img_offset_y;
new_image->width= img_width;
new_image->height= img_height;
new_image->disposal_method= disposal_method;
new_image->rgba_image= (unsigned char*)_iWalloc(img_width*
img_height*4);
if(new_image->rgba_image==NULL){
fprintf(stderr,"WARNING (0): Not enough memory to read GIF file %s. "
"Please, increase the value of W_INTERNAL_MEMORY at conf/conf.h.\n",
filename);
goto error_gif;
}
if(img==NULL){
img= new_image;
last_img= img;
}
else{
last_img->next= new_image;
new_image->prev= last_img;
last_img= new_image;
}
last_img->delay_time= delay_time*10000;



if(img_offset_x!=0||img_offset_y!=0||img_width!=width||
img_height!=height){
unsigned long i;
unsigned long size= width*height;



for(i= 0;i<size;i+= 4){
if(local_color_table_flag&&background_color<local_color_table_size){
new_image->rgba_image[4*i]= local_color_table[3*background_color];
new_image->rgba_image[4*i+1]= 
local_color_table[3*background_color+1];
new_image->rgba_image[4*i+2]= 
local_color_table[3*background_color+2];
new_image->rgba_image[4*i+3]= 255;
}

else if(background_color<global_color_table_size){
new_image->rgba_image[4*i]= 
global_color_table[3*background_color];
new_image->rgba_image[4*i+1]= 
global_color_table[3*background_color+1];
new_image->rgba_image[4*i+2]= 
global_color_table[3*background_color+2];
new_image->rgba_image[4*i+3]= 255;
}
#if W_DEBUG_LEVEL >= 1




else{
fprintf(stderr,"WARNING: Image has invalid background color: %s.\n",
filename);
}
#endif
}
}
}
/*:620*//*623:*/
#line 13423 "cweb/weaver.w"

{
if(lzw_minimum_code_size<2||lzw_minimum_code_size> 8){
fprintf(stderr,"WARNING (0): Invalid GIF file %s. Not allowed LZW Minimim "
" code size.\n",filename);
goto error_gif;
}
}
/*:623*//*624:*/
#line 13437 "cweb/weaver.w"

{
unsigned i;
last_value_in_code_table= (1<<lzw_minimum_code_size)-1;
for(i= 0;i<=last_value_in_code_table;i++){
code_table[i]= NULL;
code_table_size[i]= 1;
}
}
/*:624*//*626:*/
#line 13462 "cweb/weaver.w"

{
clear_code= last_value_in_code_table+1;
end_of_information_code= last_value_in_code_table+2;
last_value_in_code_table= end_of_information_code;
code_table[clear_code]= NULL;
code_table_size[clear_code]= 0;
code_table[end_of_information_code]= NULL;
code_table_size[end_of_information_code]= 0;
}
/*:626*//*628:*/
#line 13489 "cweb/weaver.w"

{
bits= lzw_minimum_code_size+1;
}
/*:628*//*632:*/
#line 13667 "cweb/weaver.w"

if(local_color_table_flag)
color_table= local_color_table;
else
color_table= global_color_table;
/*:632*/
#line 13214 "cweb/weaver.w"




fread(buffer,1,1,fp);
while(buffer[0]!=0){
buffer_size= buffer[0];
buffer[buffer_size]= '\0';
fread(buffer,1,buffer[0],fp);

/*630:*/
#line 13519 "cweb/weaver.w"

byte_offset= 0;






if(!incomplete_code){
bit_offset= 0;
}

while(byte_offset<buffer_size&&!end_of_image&&pixel<image_size){



if(incomplete_code){
incomplete_code= false;

int still_need_to_read= bits+bit_offset;
unsigned tmp;


if(still_need_to_read<=8){



tmp= ((unsigned char)(buffer[0]<<(8-still_need_to_read)));

if(still_need_to_read-bit_offset<=8)
code+= (tmp>>(8-still_need_to_read+bit_offset));
else
code+= (tmp<<(still_need_to_read-bit_offset-8));
byte_offset+= (bits+bit_offset)/8;
bit_offset= (bits+bit_offset)%8;
}
else{


code+= buffer[byte_offset]<<(-bit_offset);


tmp= ((unsigned char)(buffer[byte_offset+1]<<
(16-still_need_to_read)));
tmp= tmp>>(16-still_need_to_read);

code+= tmp<<(8-bit_offset);
byte_offset+= (bits+bit_offset)/8;
bit_offset= (bits+bit_offset)%8;
}
}




else if(bit_offset+bits<=8*(buffer_size-byte_offset)){

if(bit_offset+bits<=8){

code= (buffer[byte_offset]>>bit_offset);

code= (unsigned)((unsigned char)(code<<(8-bits)));

code= code>>(8-bits);
}


else if(bit_offset+bits<=16){
unsigned tmp;

code= (buffer[byte_offset]>>bit_offset);


tmp= (unsigned char)
(buffer[byte_offset+1]<<(16-bit_offset-bits));

tmp= (tmp>>(16-bit_offset-bits));


code+= (tmp<<(8-bit_offset));
}


else{
unsigned tmp;

code= (buffer[byte_offset]>>bit_offset);

code+= buffer[byte_offset+1]<<(8-bit_offset);

tmp= (unsigned char)(buffer[byte_offset+2]<<
(24-bit_offset-bits));

tmp= tmp>>(24-bit_offset-bits);


code+= (tmp<<(16-bit_offset));
}
bit_offset+= bits;
if(bit_offset>=8){
byte_offset+= bit_offset/8;
bit_offset= bit_offset%8;
}
}
else{



if(byte_offset==buffer_size-1){

code= (buffer[byte_offset]>>bit_offset);



bit_offset= -(8-bit_offset);
byte_offset++;
}
else{


code= (buffer[byte_offset]>>bit_offset);

code+= buffer[byte_offset+1]<<(8-bit_offset);


bit_offset= -(16-bit_offset);
byte_offset+= 2;
}
incomplete_code= true;
continue;
}

if(code> last_value_in_code_table+1){
code= end_of_information_code;
}
/*633:*/
#line 13678 "cweb/weaver.w"

{


if(code==end_of_information_code){
end_of_image= true;
continue;
}

if(code==clear_code){
/*634:*/
#line 13801 "cweb/weaver.w"

{
for(;last_value_in_code_table> end_of_information_code;
last_value_in_code_table--){
Wfree(code_table[last_value_in_code_table]);
}
last_value_in_code_table= end_of_information_code;
bits= lzw_minimum_code_size+1;
first_pixel= true;
}
/*:634*/
#line 13688 "cweb/weaver.w"

continue;
}


else if(first_pixel){
first_pixel= false;
previous_code= code;





preenche_pixel(&(last_img->rgba_image[4*pixel]),
code_table,code,color_table,1,
transparent_color_flag,transparency_index);



pixel++;
}

else if(code> last_value_in_code_table){
if(previous_code<end_of_information_code){
if(last_value_in_code_table<4095){
code_table[last_value_in_code_table+1]= 
produz_codigo((unsigned char*)&previous_code,1,previous_code);
code_table_size[last_value_in_code_table+1]= 2;
last_value_in_code_table++;
}
}
else{
if(last_value_in_code_table<4095){
code_table[last_value_in_code_table+1]= 
produz_codigo(code_table[previous_code],
code_table_size[previous_code],
code_table[previous_code][0]);
code_table_size[last_value_in_code_table+1]= 
code_table_size[previous_code]+1;
last_value_in_code_table++;
}
}
preenche_pixel(&(last_img->rgba_image[4*pixel]),
code_table,code,color_table,code_table_size[code],
transparent_color_flag,transparency_index);
pixel+= code_table_size[code];
previous_code= code;
}


else{

if(code<end_of_information_code){
if(previous_code<end_of_information_code){
if(last_value_in_code_table<4095){
code_table[last_value_in_code_table+1]= 
produz_codigo((unsigned char*)&previous_code,1,code);
code_table_size[last_value_in_code_table+1]= 2;
last_value_in_code_table++;
}
}
else{
if(last_value_in_code_table<4095){
code_table[last_value_in_code_table+1]= 
produz_codigo(code_table[previous_code],
code_table_size[previous_code],code);
code_table_size[last_value_in_code_table+1]= 
code_table_size[previous_code]+1;
last_value_in_code_table++;
}
}
preenche_pixel(&(last_img->rgba_image[4*pixel]),
code_table,code,color_table,1,
transparent_color_flag,transparency_index);
pixel++;
previous_code= code;
}
else{
if(previous_code<end_of_information_code){
if(last_value_in_code_table<4095){
code_table[last_value_in_code_table+1]= 
produz_codigo((unsigned char*)&previous_code,1,
code_table[code][0]);
code_table_size[last_value_in_code_table+1]= 2;
last_value_in_code_table++;
}
}
else{
if(last_value_in_code_table<4095){
code_table[last_value_in_code_table+1]= 
produz_codigo(code_table[previous_code],
code_table_size[previous_code],
code_table[code][0]);
code_table_size[last_value_in_code_table+1]= 
code_table_size[previous_code]+1;
last_value_in_code_table++;
}
}
preenche_pixel(&(last_img->rgba_image[4*pixel]),
code_table,code,color_table,code_table_size[code],
transparent_color_flag,transparency_index);
pixel+= code_table_size[code];
previous_code= code;
}
}
if(last_value_in_code_table>=(unsigned)((1<<bits)-1)&&bits<12)
bits++;
}
/*:633*/
#line 13654 "cweb/weaver.w"

}
/*:630*/
#line 13224 "cweb/weaver.w"

fread(buffer,1,1,fp);
}
/*637:*/
#line 13883 "cweb/weaver.w"

{
unsigned i;
for(i= last_value_in_code_table;i!=end_of_information_code;i--)
Wfree(code_table[i]);
}
/*:637*/
#line 13227 "cweb/weaver.w"



disposal_method= 0;
transparent_color_flag= false;
delay_time= 0;
transparency_index= 0;
}
/*:617*/
#line 13177 "cweb/weaver.w"

}
/*:615*/
#line 12922 "cweb/weaver.w"

break;
default:
fprintf(stderr,"WARNING: Couldn't interpret GIF file %s. Invalid block "
"%u.\n",filename,block_type);
goto error_gif;
}

fread(data,1,1,fp);
block_type= data[0];
}
}
/*:608*/
#line 12789 "cweb/weaver.w"

/*638:*/
#line 13909 "cweb/weaver.w"

#ifdef W_MULTITHREAD
{
GLXFBConfig*fbConfigs;
int context_attribs[]= 
{
GLX_CONTEXT_MAJOR_VERSION_ARB,3,
GLX_CONTEXT_MINOR_VERSION_ARB,3,
None
};
glXCreateContextAttribsARBProc glXCreateContextAttribsARB= 0;
int return_value;
int doubleBufferAttributes[]= {
GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
GLX_RENDER_TYPE,GLX_RGBA_BIT,
GLX_DOUBLEBUFFER,True,
GLX_RED_SIZE,1,
GLX_GREEN_SIZE,1,
GLX_BLUE_SIZE,1,
GLX_ALPHA_SIZE,1,
GLX_DEPTH_SIZE,1,
None
};
fbConfigs= glXChooseFBConfig(_dpy,_screen,doubleBufferAttributes,
&return_value);
glXCreateContextAttribsARB= (glXCreateContextAttribsARBProc)
glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

thread_context= glXCreateContextAttribsARB(_dpy,*fbConfigs,_context,
GL_TRUE,context_attribs);
glXMakeCurrent(_dpy,_window,thread_context);
}
#endif
/*:638*//*639:*/
#line 13947 "cweb/weaver.w"

{
unsigned i,line_source,line_destiny,col;
unsigned long source_index,target_index;
struct _image_list*p;
int line_increment;
int current_disposal_method= 0;
unsigned char*current_image= NULL,*previous_image= NULL;




if(interlace_flag){
line_increment= 8;
}
else
line_increment= 1;


current_image= (unsigned char*)_iWalloc(4*width*height);
if(current_image==NULL){
fprintf(stderr,"WARNING (0): Not enough memory to read GIF file %s. "
"Please, increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
returned_data= NULL;
goto error_gif;
}



if(*number_of_frames> 1){
*frame_duration= (unsigned*)Walloc(*number_of_frames*
sizeof(unsigned));
if(*frame_duration==NULL){
fprintf(stderr,"WARNING (0): Not enough memory to read GIF file %s. "
"Please, increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
Wfree(current_image);
returned_data= NULL;
goto error_gif;
}
previous_image= (unsigned char*)_iWalloc(4*width*height);
if(previous_image==NULL){
fprintf(stderr,"WARNING (0): Not enough memory to read GIF file %s. "
"Please, increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
Wfree(current_image);
Wfree(frame_duration);
returned_data= NULL;
goto error_gif;
}
}

returned_data= (GLuint*)Walloc((*number_of_frames)*sizeof(GLuint));
if(returned_data==NULL){
fprintf(stderr,"WARNING (0): Not enough memory to read GIF file %s. "
"Please, increase the value of W_MAX_MEMORY at conf/conf.h.\n",
filename);
Wfree(current_image);
Wfree(frame_duration);
Wfree(previous_image);
goto error_gif;
}

glGenTextures(*number_of_frames,returned_data);
p= img;
for(i= 0;i<*number_of_frames;i++){
line_source= col= line_destiny= 0;
if(*number_of_frames> 1){
(*frame_duration)[i]= p->delay_time;
}
while(line_destiny<height){
while(col<width){
target_index= 4*width*(height-line_destiny-1)+
col*4;
source_index= (line_source-p->y_offset)*(p->width)*4+
(col-p->x_offset)*4;
if(col<p->x_offset||line_destiny<p->y_offset||
col>=p->x_offset+p->width||
line_destiny>=p->y_offset+p->height||
p->rgba_image[source_index+3]==0){
if(i==0||current_disposal_method==3){

current_image[target_index]= p->rgba_image[source_index];
current_image[target_index+1]= p->rgba_image[source_index+1];
current_image[target_index+2]= p->rgba_image[source_index+2];
current_image[target_index+3]= p->rgba_image[source_index+3];
}
else if(current_disposal_method==1){

current_image[target_index]= previous_image[target_index];
current_image[target_index+1]= previous_image[target_index+1];
current_image[target_index+2]= previous_image[target_index+2];
current_image[target_index+3]= previous_image[target_index+3];
}
else{

current_image[target_index]= 
global_color_table[background_color*3];
current_image[target_index+1]= 
global_color_table[background_color*3+1];
current_image[target_index+2]= 
global_color_table[background_color*3+2];
current_image[target_index+3]= 255;
}
}
else{

current_image[target_index]= p->rgba_image[source_index];
current_image[target_index+1]= p->rgba_image[source_index+1];
current_image[target_index+2]= p->rgba_image[source_index+2];
current_image[target_index+3]= p->rgba_image[source_index+3];
}
col++;
}
line_destiny= line_destiny+line_increment;
line_source++;
if(line_destiny>=height&&interlace_flag){
if(line_source<height/4){
line_destiny= line_increment/2;
}
else{
line_increment/= 2;
line_destiny= line_increment/2;
}
}
col= 0;
}
current_disposal_method= p->disposal_method;
p= p->next;
line_source= col= line_destiny= 0;

glBindTexture(GL_TEXTURE_2D,returned_data[i]);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
GL_UNSIGNED_BYTE,current_image);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
glBindTexture(GL_TEXTURE_2D,0);
{
unsigned char*tmp= previous_image;
previous_image= current_image;
current_image= tmp;
}
}

#ifdef W_MULTITHREAD
glXDestroyContext(_dpy,thread_context);
glXMakeCurrent(_dpy,_window,_context);
#endif


if(number_of_loops==0)
*max_repetition= -1;
else
*max_repetition= number_of_loops;

if(*number_of_frames%2){
if(current_image!=NULL)Wfree(current_image);
Wfree(previous_image);
}
else{
if(previous_image!=NULL)Wfree(previous_image);
Wfree(current_image);
}
}
/*:639*/
#line 12790 "cweb/weaver.w"


goto end_of_gif;
error_gif:

*error= true;
returned_data= NULL;
end_of_gif:

#if W_TARGET == W_ELF && !defined(W_MULTITHREAD)
fclose(fp);
_iWtrash();
#else
/*603:*/
#line 12812 "cweb/weaver.w"

if(fp!=NULL)fclose(fp);
/*:603*//*607:*/
#line 12899 "cweb/weaver.w"

if(local_color_table!=NULL)Wfree(local_color_table);
if(global_color_table!=NULL)Wfree(global_color_table);
/*:607*//*621:*/
#line 13372 "cweb/weaver.w"

if(img!=NULL)
free_img_list(last_img);
/*:621*/
#line 12803 "cweb/weaver.w"

#endif
return returned_data;
}
/*:602*/
#line 12729 "cweb/weaver.w"

/*:598*/
