/*72:*/
#line 2059 "./cweb/weaver.w"

#include "weaver.h"
/*76:*/
#line 2120 "./cweb/weaver.w"

struct _weaver_struct W;
/*:76*//*82:*/
#line 2188 "./cweb/weaver.w"

unsigned long _update_time(void){
int nsec;
unsigned long result;
struct timeval _current_time;
gettimeofday(&_current_time,NULL);

if(_current_time.tv_usec<_last_time.tv_usec){
nsec= (_last_time.tv_usec-_current_time.tv_usec)/1000000+1;
_last_time.tv_usec-= 1000000*nsec;
_last_time.tv_sec+= nsec;
}
if(_current_time.tv_usec-_last_time.tv_usec> 1000000){
nsec= (_current_time.tv_usec-_last_time.tv_usec)/1000000;
_last_time.tv_usec+= 1000000*nsec;
_last_time.tv_sec-= nsec;
}
if(_current_time.tv_sec<_last_time.tv_sec){

result= (_current_time.tv_sec-_last_time.tv_sec)*(-1000000);
result+= (_current_time.tv_usec-_last_time.tv_usec);
}
else{
result= (_current_time.tv_sec-_last_time.tv_sec)*1000000;
result+= (_current_time.tv_usec-_last_time.tv_usec);
}
_last_time.tv_sec= _current_time.tv_sec;
_last_time.tv_usec= _current_time.tv_usec;
return result;
}
/*:82*//*173:*/
#line 4270 "./cweb/weaver.w"

void _Wloop(void(*f)(void)){
if(_first_loop)
_first_loop= false;
else{
#if W_TARGET == W_WEB
emscripten_cancel_main_loop();
#endif
Wtrash();
}
Wbreakpoint();
_loop_begin= 1;
/*181:*/
#line 4433 "./cweb/weaver.w"

_lag= 0;
/*:181*//*316:*/
#line 7064 "./cweb/weaver.w"

W.flush_input();
/*:316*//*383:*/
#line 8599 "./cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins();
#endif
/*:383*//*535:*/
#line 11822 "./cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:535*//*801:*/
#line 17646 "./cweb/weaver.w"

{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
#if W_TARGET == W_WEB
EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].play();
}
},i);
#else
if(_music[i].status[_number_of_loops]==_PLAYING){
sem_post(&(_music[i].semaphore));
}
#endif
}
}
/*:801*/
#line 4282 "./cweb/weaver.w"

/*395:*/
#line 8760 "./cweb/weaver.w"

{
int i;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
for(i= 0;i<W_MAX_SCHEDULING;i++)
_scheduled_functions[_number_of_loops][i].f= NULL;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_scheduler_mutex);
#endif
}
/*:395*//*426:*/
#line 9377 "./cweb/weaver.w"

_flush_interfaces();
/*:426*//*498:*/
#line 11109 "./cweb/weaver.w"

_clean_interface_queue();
/*:498*//*540:*/
#line 11866 "./cweb/weaver.w"

W.final_shader_integer= 0;
/*:540*//*617:*/
#line 13170 "./cweb/weaver.w"

_finalize_all();
/*:617*//*799:*/
#line 17582 "./cweb/weaver.w"

{
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
#if W_TARGET == W_WEB


EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].pause();
document["music"+$0]= undefined;
}
},i);
#else
if(_music[i].status[_number_of_loops]==_PLAYING){

sem_wait(&(_music[i].semaphore));
}
#endif
_music[i].volume[_number_of_loops]= 0.5;
_music[i].status[_number_of_loops]= _NOT_LOADED;
_music[i].filename[_number_of_loops][0]= '\0';
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
}
/*:799*/
#line 4283 "./cweb/weaver.w"

_loop_stack[_number_of_loops]= f;
_running_loop= true;
_update_time();
#if W_TARGET == W_WEB
while(1)
emscripten_set_main_loop(f,0,1);
#else
while(1)
f();
#endif
}
/*:173*//*175:*/
#line 4308 "./cweb/weaver.w"

void _exit_loop(void){
#if W_DEBUG_LEVEL >= 1
if(_first_loop){
fprintf(stderr,"ERROR (1): Using Wexit_loop outside a game loop.\n");
Wexit();
}
#endif
if(_number_of_loops==0)
Wexit();
else{
Wtrash();
/*396:*/
#line 8778 "./cweb/weaver.w"

{
int i;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
for(i= 0;i<W_MAX_SCHEDULING;i++)
_scheduled_functions[_number_of_loops][i].f= NULL;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_scheduler_mutex);
#endif
}
/*:396*//*427:*/
#line 9384 "./cweb/weaver.w"

_flush_interfaces();
/*:427*//*499:*/
#line 11116 "./cweb/weaver.w"

_clean_interface_queue();
/*:499*//*536:*/
#line 11831 "./cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:536*//*539:*/
#line 11859 "./cweb/weaver.w"

W.final_shader_integer= _final_shader_integer[_number_of_loops-1];
/*:539*//*618:*/
#line 13174 "./cweb/weaver.w"

_finalize_all();
/*:618*//*798:*/
#line 17547 "./cweb/weaver.w"

{
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_music_mutex);
#endif
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
#if W_TARGET == W_WEB


EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].pause();
document["music"+$0]= undefined;
}
},i);
#else
if(_music[i].status[_number_of_loops]==_PLAYING){

sem_wait(&(_music[i].semaphore));
}
#endif
_music[i].volume[_number_of_loops]= 0.5;
_music[i].status[_number_of_loops]= _NOT_LOADED;
_music[i].filename[_number_of_loops][0]= '\0';
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_music_mutex);
#endif
}
/*:798*/
#line 4320 "./cweb/weaver.w"

_number_of_loops--;
/*181:*/
#line 4433 "./cweb/weaver.w"

_lag= 0;
/*:181*//*316:*/
#line 7064 "./cweb/weaver.w"

W.flush_input();
/*:316*//*383:*/
#line 8599 "./cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins();
#endif
/*:383*//*535:*/
#line 11822 "./cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:535*//*801:*/
#line 17646 "./cweb/weaver.w"

{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
#if W_TARGET == W_WEB
EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].play();
}
},i);
#else
if(_music[i].status[_number_of_loops]==_PLAYING){
sem_post(&(_music[i].semaphore));
}
#endif
}
}
/*:801*/
#line 4322 "./cweb/weaver.w"

_running_loop= true;
_update_time();
#if W_TARGET == W_WEB
emscripten_cancel_main_loop();
while(1)
emscripten_set_main_loop(_loop_stack[_number_of_loops],0,1);
#else
while(1)
_loop_stack[_number_of_loops]();
#endif
}
}
/*:175*//*177:*/
#line 4358 "./cweb/weaver.w"

void Wsubloop(void(*f)(void)){
#if W_TARGET == W_WEB
emscripten_cancel_main_loop();
#endif
Wbreakpoint();
_loop_begin= 1;
_number_of_loops++;
/*181:*/
#line 4433 "./cweb/weaver.w"

_lag= 0;
/*:181*//*316:*/
#line 7064 "./cweb/weaver.w"

W.flush_input();
/*:316*//*383:*/
#line 8599 "./cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins();
#endif
/*:383*//*535:*/
#line 11822 "./cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:535*//*801:*/
#line 17646 "./cweb/weaver.w"

{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
#if W_TARGET == W_WEB
EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].play();
}
},i);
#else
if(_music[i].status[_number_of_loops]==_PLAYING){
sem_post(&(_music[i].semaphore));
}
#endif
}
}
/*:801*/
#line 4366 "./cweb/weaver.w"

/*538:*/
#line 11850 "./cweb/weaver.w"

{
_final_shader_integer[_number_of_loops-1]= W.final_shader_integer;
W.final_shader_integer= 0;
}
/*:538*//*800:*/
#line 17620 "./cweb/weaver.w"

{
int i;
for(i= 0;i<W_MAX_MUSIC;i++){
#if W_TARGET == W_WEB
EM_ASM_({
if(document["music"+$0]!== undefined){
document["music"+$0].pause();
document["music"+$0]= undefined;
}
},i);
#else
if(_music[i].status[_number_of_loops-1]==_PLAYING){

sem_wait(&(_music[i].semaphore));
}
#endif
}
}
/*:800*/
#line 4367 "./cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
if(_number_of_loops>=W_MAX_SUBLOOP){
fprintf(stderr,"Error (1): Max number of subloops achieved.\n");
fprintf(stderr,"Please, increase W_MAX_SUBLOOP in conf/conf.h.\n");
}
#endif
_loop_stack[_number_of_loops]= f;
_running_loop= true;
_update_time();
#if W_TARGET == W_WEB
while(1)
emscripten_set_main_loop(f,0,1);
#else
while(1)
f();
#endif
}
/*:177*//*297:*/
#line 6594 "./cweb/weaver.w"

#if W_TARGET == W_ELF
static struct _k_translate{
unsigned original_symbol,new_symbol;
}_key_translate[100];
#endif


static unsigned _pressed_keys[20];
static unsigned _released_keys[20];

static unsigned _pressed_buttons[5];
static unsigned _released_buttons[5];
/*:297*//*302:*/
#line 6681 "./cweb/weaver.w"

#if W_TARGET == W_ELF
static unsigned _translate_key(unsigned symbol){
int i;
for(i= 0;i<100;i++){
if(_key_translate[i].original_symbol==0)
return symbol%0x10000;
if(_key_translate[i].original_symbol==symbol)
return _key_translate[i].new_symbol%0x10000;
}
#if W_DEBUG_LEVEL >= 2
if(symbol> 0xffff)
fprintf(stderr,"WARNING (2): Key with unknown code pressed: %lu",
(unsigned long)symbol);
#endif
return symbol%0x10000;
}
#endif
/*:302*//*304:*/
#line 6713 "./cweb/weaver.w"

int _Wkey_translate(unsigned old_value,unsigned new_value){
#if W_TARGET == W_ELF
int i;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_input_mutex);
#endif
for(i= 0;i<100;i++){
if(_key_translate[i].original_symbol==0||
_key_translate[i].original_symbol==old_value){
_key_translate[i].original_symbol= old_value;
_key_translate[i].new_symbol= new_value;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_input_mutex);
#endif
return 1;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_input_mutex);
#endif
#else


old_value= new_value;
new_value= old_value;
#endif
return 0;
}

void _Werase_key_translations(void){
#if W_TARGET == W_ELF
int i;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_input_mutex);
#endif
for(i= 0;i<100;i++){
_key_translate[i].original_symbol= 0;
_key_translate[i].new_symbol= 0;
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_input_mutex);
#endif
#endif
}
/*:304*//*313:*/
#line 7027 "./cweb/weaver.w"

void _Wflush_input(void){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_input_mutex);
#endif
{

int i,key;
for(i= 0;i<20;i++){
key= _pressed_keys[i];
_pressed_keys[i]= 0;
W.keyboard[key]= 0;
key= _released_keys[i];
_released_keys[i]= 0;
W.keyboard[key]= 0;
}
}
/*342:*/
#line 7577 "./cweb/weaver.w"

{
int i;
for(i= 0;i<5;i++){
_released_buttons[i]= 0;
_pressed_buttons[i]= 0;
}
for(i= 0;i<7;i++)
W.mouse.buttons[i]= 0;
W.mouse.dx= 0;
W.mouse.dy= 0;
}
/*:342*/
#line 7044 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_input_mutex);
#endif
}
/*:313*//*336:*/
#line 7508 "./cweb/weaver.w"

static int old_dx,old_dy;
/*:336*//*346:*/
#line 7609 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _Whide_cursor(void){
Colormap cmap;
Cursor no_ptr;
XColor black,dummy;
static char bm_no_data[]= {0,0,0,0,0,0,0,0};
Pixmap bm_no;
cmap= DefaultColormap(_dpy,DefaultScreen(_dpy));
XAllocNamedColor(_dpy,cmap,"black",&black,&dummy);
bm_no= XCreateBitmapFromData(_dpy,_window,bm_no_data,8,8);
no_ptr= XCreatePixmapCursor(_dpy,bm_no,bm_no,&black,&black,0,0);
XDefineCursor(_dpy,_window,no_ptr);
XFreeCursor(_dpy,no_ptr);
if(bm_no!=None)
XFreePixmap(_dpy,bm_no);
XFreeColors(_dpy,cmap,&black.pixel,1,0);
_using_original_cursor= false;
}
#endif
/*:346*//*347:*/
#line 7655 "./cweb/weaver.w"

#if W_TARGET == W_WEB
void _Whide_cursor(void){
SDL_ShowCursor(0);
emscripten_hide_mouse();
_using_original_cursor= false;
}
#endif
/*:347*//*399:*/
#line 8853 "./cweb/weaver.w"

void _run_periodically(void(*f)(void),float t){
int i;
unsigned long period= (unsigned long)(t*1000000);
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
for(i= 0;i<W_MAX_SCHEDULING;i++){
if(_scheduled_functions[_number_of_loops][i].f==NULL||
_scheduled_functions[_number_of_loops][i].f==f){
_scheduled_functions[_number_of_loops][i].f= f;
_scheduled_functions[_number_of_loops][i].period= period;
_scheduled_functions[_number_of_loops][i].periodic= true;
_scheduled_functions[_number_of_loops][i].last_execution= W.t;
break;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_scheduler_mutex);
#endif
if(i==W_MAX_SCHEDULING){
fprintf(stderr,"ERROR (1): Can't schedule more functions.");
fprintf(stderr,"Please, define W_MAX_SCHEDULING in conf/conf.h "
"with a value bigger than the current %d.\n",
W_MAX_SCHEDULING);
}
}
/*:399*//*400:*/
#line 8886 "./cweb/weaver.w"

void _run_futurelly(void(*f)(void),float t){
int i;
unsigned long period= (unsigned long)(t*1000000);
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
for(i= 0;i<W_MAX_SCHEDULING;i++){
if(_scheduled_functions[_number_of_loops][i].f==NULL||
_scheduled_functions[_number_of_loops][i].f==f){
_scheduled_functions[_number_of_loops][i].f= f;
_scheduled_functions[_number_of_loops][i].period= period;
_scheduled_functions[_number_of_loops][i].periodic= false;
_scheduled_functions[_number_of_loops][i].last_execution= W.t;
break;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_scheduler_mutex);
#endif
if(i==W_MAX_SCHEDULING){
fprintf(stderr,"ERROR (1): Can't schedule more functions.");
fprintf(stderr,"Please, define W_MAX_SCHEDULING in conf/conf.h "
"with a value bigger than the current %d.\n",
W_MAX_SCHEDULING);
}
}
/*:400*//*401:*/
#line 8920 "./cweb/weaver.w"

float _cancel(void(*f)(void)){
int i;
unsigned long period,last_execution;
float return_value= NAN;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
for(i= 0;i<W_MAX_SCHEDULING;i++){
if(_scheduled_functions[_number_of_loops][i].f==f){
period= _scheduled_functions[_number_of_loops][i].period;
last_execution= _scheduled_functions[_number_of_loops][i].last_execution;
return_value= ((float)(period-(W.t-last_execution)))/1000000.0;
for(;i<W_MAX_SCHEDULING-1;i++){
_scheduled_functions[_number_of_loops][i].f= 
_scheduled_functions[_number_of_loops][i+1].f;
_scheduled_functions[_number_of_loops][i].period= 
_scheduled_functions[_number_of_loops][i+1].period;
_scheduled_functions[_number_of_loops][i].last_execution= 
_scheduled_functions[_number_of_loops][i+1].last_execution;
}
_scheduled_functions[_number_of_loops][i].f= NULL;
break;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_scheduler_mutex);
#endif
return return_value;
}
/*:401*//*402:*/
#line 8958 "./cweb/weaver.w"

float _period(void(*f)(void)){
int i;
float result= -1.0;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
for(i= 0;i<W_MAX_SCHEDULING;i++)
if(_scheduled_functions[_number_of_loops][i].f==f){
if(_scheduled_functions[_number_of_loops][i].periodic==true)
result= (float)(_scheduled_functions[_number_of_loops][i].period)/
1000000.0;
else
result= INFINITY;
}
if(result<0.0)
result= NAN;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
return result;
}
/*:402*//*614:*/
#line 13108 "./cweb/weaver.w"

void _finalize_after(void*data,void(*finalizer)(void*)){
struct _finalize_element*el;
if(!_running_loop)
return;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_finalizing_mutex);
#endif
el= (struct _finalize_element*)Walloc(sizeof(struct _finalize_element));
if(el==NULL){
fprintf(stderr,"WARNING (0): Not enough memory. Error in an internal "
"operation. Please, increase the value of W_MAX_MEMORY at "
"conf/conf.h. Currently we won't be able to finalize some "
"resource.\n");
}
else if(_finalize_list[_number_of_loops]==NULL){
el->data= data;
el->finalize= finalizer;
el->prev= el->next= NULL;
_finalize_list[_number_of_loops]= el;
}
else{
el->data= data;
el->finalize= finalizer;
el->prev= NULL;
_finalize_list[_number_of_loops]->prev= el;
el->next= _finalize_list[_number_of_loops];
_finalize_list[_number_of_loops]= el;
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_finalizing_mutex);
#endif
}
/*:614*//*616:*/
#line 13150 "./cweb/weaver.w"

void _finalize_all(void){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_finalizing_mutex);
#endif
struct _finalize_element*p= _finalize_list[_number_of_loops];
while(p!=NULL){
p->finalize(p->data);
p= p->next;
}
_finalize_list[_number_of_loops]= NULL;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_finalizing_mutex);
#endif
}
/*:616*//*620:*/
#line 13188 "./cweb/weaver.w"

void _finalize_this(void*data,bool remove){
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_finalizing_mutex);
#endif
{
struct _finalize_element*p= _finalize_list[_number_of_loops];
while(p!=NULL){
if(p->data==data){
if(p->prev!=NULL)
p->prev->next= p->next;
else
_finalize_list[_number_of_loops]= p->next;
if(p->next!=NULL)
p->next->prev= p->prev;
if(remove)
Wfree(p);
return;
}
p= p->next;
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_finalizing_mutex);
#endif
}
/*:620*/
#line 2061 "./cweb/weaver.w"

void _awake_the_weaver(void){
/*80:*/
#line 2173 "./cweb/weaver.w"

W.t= 0;
gettimeofday(&_last_time,NULL);
/*:80*//*86:*/
#line 2239 "./cweb/weaver.w"

W.game= &_game;
/*:86*//*117:*/
#line 3107 "./cweb/weaver.w"

W.create_arena= &_Wcreate_arena;
/*:117*//*125:*/
#line 3229 "./cweb/weaver.w"

W.destroy_arena= &Wdestroy_arena;
/*:125*//*130:*/
#line 3358 "./cweb/weaver.w"

W.alloc_arena= &_alloc;
/*:130*//*134:*/
#line 3470 "./cweb/weaver.w"

W.free= &_free;
/*:134*//*140:*/
#line 3580 "./cweb/weaver.w"

W.breakpoint_arena= &_new_breakpoint;
/*:140*//*144:*/
#line 3642 "./cweb/weaver.w"

W.trash_arena= &Wtrash_arena;
/*:144*//*150:*/
#line 3714 "./cweb/weaver.w"

_initialize_memory();
/*:150*//*155:*/
#line 3779 "./cweb/weaver.w"

W.alloc= &_Walloc;
/*:155*//*160:*/
#line 3848 "./cweb/weaver.w"

W.breakpoint= &_Wbreakpoint;
W.trash= &_Wtrash;
/*:160*//*172:*/
#line 4262 "./cweb/weaver.w"

_first_loop= true;
_running_loop= false;
_number_of_loops= 0;
/*:172*//*180:*/
#line 4428 "./cweb/weaver.w"

W.dt= 40000;
_lag= 0;
/*:180*//*194:*/
#line 4681 "./cweb/weaver.w"


_initialize_numeric_functions();
/*:194*//*195:*/
#line 4688 "./cweb/weaver.w"

_finalize_numeric_functions();
/*:195*//*206:*/
#line 4958 "./cweb/weaver.w"

W.random= &_random;
/*:206*//*242:*/
#line 5682 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_window_mutex,NULL)!=0){
perror(NULL);
exit(1);
}
#endif
#if W_TARGET == W_ELF
_initialize_window();
#endif
/*:242*//*271:*/
#line 6202 "./cweb/weaver.w"

#if W_TARGET == W_WEB
_initialize_canvas();
#endif
/*:271*//*281:*/
#line 6347 "./cweb/weaver.w"

W.resize_window= &_Wresize_window;
/*:281*//*287:*/
#line 6393 "./cweb/weaver.w"

W.move_window= &_Wmove_window;
/*:287*//*291:*/
#line 6436 "./cweb/weaver.w"


glClearColor(W_DEFAULT_COLOR,1.0f);

glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);

glEnable(GL_CULL_FACE);

glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
/*:291*//*299:*/
#line 6619 "./cweb/weaver.w"

{
int i;
for(i= 0;i<=0xffff;i++)
W.keyboard[i]= 0;
#if W_TARGET == W_ELF
for(i= 0;i<100;i++){
_key_translate[i].original_symbol= 0;
_key_translate[i].new_symbol= 0;
}
#endif
for(i= 0;i<20;i++){
_pressed_keys[i]= 0;
_released_keys[i]= 0;
}
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_input_mutex,NULL)!=0){
perror(NULL);
exit(1);
}
#endif
}
/*:299*//*306:*/
#line 6768 "./cweb/weaver.w"

W.key_translate= &_Wkey_translate;
W.erase_key_translations= &_Werase_key_translations;
/*:306*//*315:*/
#line 7057 "./cweb/weaver.w"

W.flush_input= &_Wflush_input;
/*:315*//*327:*/
#line 7298 "./cweb/weaver.w"

{
int i;
for(i= 0;i<5;i++)
W.mouse.buttons[i]= 0;
for(i= 0;i<5;i++){
_pressed_buttons[i]= 0;
_released_buttons[i]= 0;
}
}
/*:327*//*335:*/
#line 7477 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
Window root_return,child_return;
int root_x_return,root_y_return,win_x_return,win_y_return;
unsigned mask_return;
XQueryPointer(_dpy,_window,&root_return,&child_return,&root_x_return,
&root_y_return,&win_x_return,&win_y_return,&mask_return);




W.mouse.x= root_x_return;
W.mouse.y= W.height-1-root_y_return;
}
#endif
#if W_TARGET == W_WEB
SDL_GetMouseState(&(W.mouse.x),&(W.mouse.y));
W.mouse.y= W.height-1-W.mouse.y;
#endif
W.mouse.ddx= W.mouse.ddy= W.mouse.dx= W.mouse.dy= 0;
/*:335*//*337:*/
#line 7512 "./cweb/weaver.w"

old_dx= old_dy= 0;
/*:337*//*345:*/
#line 7605 "./cweb/weaver.w"

_using_original_cursor= true;
/*:345*//*349:*/
#line 7671 "./cweb/weaver.w"

W.hide_cursor= &_Whide_cursor;
/*:349*//*367:*/
#line 8248 "./cweb/weaver.w"

W.reload_plugin= &_reload_plugin;
/*:367*//*378:*/
#line 8478 "./cweb/weaver.w"

#if W_TARGET == W_ELF
W.get_plugin= &_Wget_plugin;
#endif
/*:378*//*382:*/
#line 8590 "./cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins= &_reload_all_plugins;
#endif
/*:382*//*393:*/
#line 8731 "./cweb/weaver.w"

{
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_scheduler_mutex,NULL)!=0){
perror(NULL);
exit(1);
}
#endif
int i,j;
for(i= 0;i<W_MAX_SUBLOOP;i++)
for(j= 0;j<W_MAX_SCHEDULING;j++)
_scheduled_functions[i][j].f= NULL;
}
/*:393*//*404:*/
#line 8992 "./cweb/weaver.w"

W.run_periodically= &_run_periodically;
W.run_futurelly= &_run_futurelly;
W.cancel= &_cancel;
W.period= &_period;
/*:404*//*409:*/
#line 9079 "./cweb/weaver.w"

#if W_TARGET == W_ELF
W.enable_plugin= &_Wenable_plugin;
W.disable_plugin= &_Wdisable_plugin;
W.is_plugin_enabled= &_Wis_enabled;
#endif
/*:409*//*413:*/
#line 9128 "./cweb/weaver.w"

#if W_TARGET == W_ELF
W.get_plugin_data= &_Wget_plugin_data;
W.set_plugin_data= &_Wset_plugin_data;
#endif
/*:413*//*422:*/
#line 9311 "./cweb/weaver.w"

{
int i,j;
for(i= 0;i<W_MAX_SUBLOOP;i++)
for(j= 0;j<W_MAX_INTERFACES;j++)
_interfaces[i][j].type= W_NONE;
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_interface_mutex,NULL)!=0){
perror("Initializing interface mutex:");
exit(1);
}
#endif
}
/*:422*//*428:*/
#line 9395 "./cweb/weaver.w"

{
GLenum dummy;
glewExperimental= GL_TRUE;
GLenum err= glewInit();
if(err!=GLEW_OK){
fprintf(stderr,"ERROR: GLW not supported.\n");
exit(1);
}






dummy= glGetError();
glewExperimental+= dummy;
glewExperimental-= dummy;
}
/*:428*//*430:*/
#line 9428 "./cweb/weaver.w"

{
_interface_vertices[0]= -0.5;
_interface_vertices[1]= -0.5;
_interface_vertices[2]= 0.0;
_interface_vertices[3]= 0.5;
_interface_vertices[4]= -0.5;
_interface_vertices[5]= 0.0;
_interface_vertices[6]= 0.5;
_interface_vertices[7]= 0.5;
_interface_vertices[8]= 0.0;
_interface_vertices[9]= -0.5;
_interface_vertices[10]= 0.5;
_interface_vertices[11]= 0.0;

glGenBuffers(1,&_interface_VBO);

glGenVertexArrays(1,&_interface_VAO);

glBindVertexArray(_interface_VAO);

glBindBuffer(GL_ARRAY_BUFFER,_interface_VBO);

glBufferData(GL_ARRAY_BUFFER,sizeof(_interface_vertices),
_interface_vertices,GL_STATIC_DRAW);

glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(GLvoid*)0);

glEnableVertexAttribArray(0);

glBindVertexArray(0);
}
/*:430*//*435:*/
#line 9571 "./cweb/weaver.w"

W.new_interface= &_new_interface;
/*:435*//*439:*/
#line 9618 "./cweb/weaver.w"

W.destroy_interface= &_destroy_interface;
/*:439*//*443:*/
#line 9684 "./cweb/weaver.w"

W.copy_interface= &_copy_interface;
/*:443*//*448:*/
#line 9783 "./cweb/weaver.w"

W.move_interface= &_move_interface;
/*:448*//*452:*/
#line 9862 "./cweb/weaver.w"

W.resize_interface= &_resize_interface;
/*:452*//*456:*/
#line 9942 "./cweb/weaver.w"

W.rotate_interface= &_rotate_interface;
/*:456*//*479:*/
#line 10390 "./cweb/weaver.w"

{
GLuint vertex,fragment;
vertex= _compile_vertex_shader(_vertex_interface);
fragment= _compile_fragment_shader(_fragment_interface);


_default_interface_shader.program_shader= 
_link_and_clean_shaders(vertex,fragment);
_default_interface_shader._uniform_object_color= 
glGetUniformLocation(_default_interface_shader.program_shader,
"object_color");
_default_interface_shader._uniform_model_view= 
glGetUniformLocation(_default_interface_shader.program_shader,
"model_view_matrix");
_default_interface_shader._uniform_object_size= 
glGetUniformLocation(_default_interface_shader.program_shader,
"object_size");
_default_interface_shader._uniform_integer= 
glGetUniformLocation(_default_interface_shader.program_shader,
"integer");
_default_interface_shader._uniform_time= 
glGetUniformLocation(_default_interface_shader.program_shader,
"time");
_default_interface_shader._attribute_vertex_position= 
glGetAttribLocation(_default_interface_shader.program_shader,
"vertex_position");
}
/*:479*//*483:*/
#line 10522 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
int number_of_shaders= 0;
char shader_directory[256];
size_t dir_length= 0;
DIR*d;
shader_directory[0]= '\0';
#if W_DEBUG_LEVEL == 0
dir_length= strlen(W_INSTALL_DATA);
if(dir_length+9> 256){
fprintf(stderr,"ERROR (0): Shader directory name is too big: %s\n",
W_INSTALL_DATA);
exit(1);
}
memcpy(shader_directory,W_INSTALL_DATA,dir_length+1);
memcpy(&shader_directory[dir_length],"/",2);
dir_length++;
#endif
memcpy(&shader_directory[dir_length],"shaders/",9);
dir_length+= 8;


d= opendir(shader_directory);
if(d){
struct dirent*dir;


while((dir= readdir(d))!=NULL){
if(dir->d_name[0]=='.')continue;
if(atoi(dir->d_name)==0){
fprintf(stderr,"WARNING (0): Shader being ignored. "
"%s%s should start with number different than zero.\n",
shader_directory,dir->d_name);
continue;
}
#if (defined(__linux__) || defined(_BSD_SOURCE)) && defined(DT_DIR)
if(dir->d_type!=DT_DIR)continue;
#else
{

struct stat s;
int err;
char file[256];
strlcpy(file,shader_directory,256);
strlcat(file,dir->d_name,256);
err= stat(file,&s);
if(err==-1)continue;
if(!S_ISDIR(s.st_mode))continue;
}
#endif
number_of_shaders++;
}
}
#endif

/*:483*//*484:*/
#line 10583 "./cweb/weaver.w"

#if W_TARGET == W_ELF

_shader_list= (struct _shader*)_iWalloc(sizeof(struct _shader)*
number_of_shaders);
if(_shader_list==NULL){
fprintf(stderr,"ERROR (0): Not enough memory to compile shaders. "
"Please, increase the value of W_INTERNAL_MEMORY "
"at conf/conf.h.");
exit(1);
}
{
int i;
for(i= 0;i<number_of_shaders;i++)
_shader_list[i].initialized= false;
}

#endif
/*:484*//*485:*/
#line 10609 "./cweb/weaver.w"

#if W_TARGET == W_ELF

if(d)closedir(d);
d= opendir(shader_directory);
if(d){
struct dirent*dir;


while((dir= readdir(d))!=NULL){
if(dir->d_name[0]=='.')continue;
if(!isdigit(dir->d_name[0])||dir->d_name[0]=='0')
continue;
#if (defined(__linux__) || defined(_BSD_SOURCE)) && defined(DT_DIR)
if(dir->d_type!=DT_DIR)continue;
#else
{

struct stat s;
int err;
char file[256];
size_t name_size= strlen(shader_directory)+1;
if(name_size> 256)
name_size= 256;
memcpy(file,shader_directory,name_size);
file[255]= '\0';
strlcat(file,dir->d_name,256);
err= stat(file,&s);
if(err==-1)continue;
if(!S_ISDIR(s.st_mode))continue;
}
#endif


{
int shader_number= atoi(dir->d_name);
if(shader_number> number_of_shaders){
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): Non-sequential shader "
"enumeration at %s.\n",shader_directory);
#endif
continue;
}
if(_shader_list[shader_number-1].initialized==true){
#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): Two shaders enumerated "
"with number %d at %s.\n",shader_number,
shader_directory);
#endif
continue;
}



{
char path[256];
size_t name_size= strlen(shader_directory);
size_t d_name_size= strlen(dir->d_name);
if(name_size+d_name_size> 255){
fprintf(stderr,"ERROR (0): path is "
"too big: %s%s\n",shader_directory,
dir->d_name);
exit(1);
}
memcpy(path,shader_directory,name_size+1);
memcpy(&path[name_size],dir->d_name,d_name_size+1);
_compile_and_insert_new_shader(path,shader_number-1);
}
}
}
}
}
#endif
/*:485*//*486:*/
#line 10689 "./cweb/weaver.w"

#if W_TARGET == W_WEB
#include "../../.hidden_code/initialize_shader.c"
#endif
/*:486*//*491:*/
#line 10955 "./cweb/weaver.w"

{
int i,j;
for(i= 0;i<W_MAX_SUBLOOP;i++)
for(j= 0;j<W_MAX_INTERFACES;j++)
_interface_queue[i][j]= NULL;
}
/*:491*//*509:*/
#line 11377 "./cweb/weaver.w"

{
_changed_resolution= false;


int i;
for(i= 0;i<W_MAX_SUBLOOP;i++)
_final_shader[i]= W_NONE;


glGenFramebuffers(1,&_framebuffer);


glBindFramebuffer(GL_FRAMEBUFFER,_framebuffer);
}
/*:509*//*512:*/
#line 11410 "./cweb/weaver.w"

{

glGenTextures(1,&_texture);
glBindTexture(GL_TEXTURE_2D,_texture);
glTexImage2D(
GL_TEXTURE_2D,
0,
GL_RGB,
W.width,
W.height,
0,
GL_RGB,GL_UNSIGNED_BYTE,
NULL);


glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);


glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
GL_TEXTURE_2D,_texture,0);
}
/*:512*//*514:*/
#line 11448 "./cweb/weaver.w"

{
glGenRenderbuffers(1,&_depth_stencil);
glBindRenderbuffer(GL_RENDERBUFFER,_depth_stencil);
glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,
W.width,W.height);

glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
GL_RENDERBUFFER,_depth_stencil);

glBindFramebuffer(GL_FRAMEBUFFER,0);
}
/*:514*//*521:*/
#line 11554 "./cweb/weaver.w"

{
GLuint vertex,fragment;


vertex= _compile_vertex_shader(_vertex_interface_texture);
fragment= _compile_fragment_shader(_fragment_interface_texture);

_framebuffer_shader.program_shader= 
_link_and_clean_shaders(vertex,fragment);
_framebuffer_shader._uniform_texture1= 
glGetUniformLocation(_framebuffer_shader.program_shader,
"texture1");
_framebuffer_shader._uniform_object_color= 
glGetUniformLocation(_framebuffer_shader.program_shader,
"object_color");
_framebuffer_shader._uniform_model_view= 
glGetUniformLocation(_framebuffer_shader.program_shader,
"model_view_matrix");
_framebuffer_shader._uniform_object_size= 
glGetUniformLocation(_framebuffer_shader.program_shader,
"object_size");
_framebuffer_shader._uniform_time= 
glGetUniformLocation(_framebuffer_shader.program_shader,
"time");
_framebuffer_shader._uniform_integer= 
glGetUniformLocation(_framebuffer_shader.program_shader,
"integer");
_framebuffer_shader._attribute_vertex_position= 
glGetAttribLocation(_framebuffer_shader.program_shader,
"vertex_position");





_framebuffer_matrix[0]= _framebuffer_matrix[5]= 
_framebuffer_matrix[10]= 2.0;
_framebuffer_matrix[15]= 1.0;
_framebuffer_matrix[1]= _framebuffer_matrix[2]= 
_framebuffer_matrix[3]= _framebuffer_matrix[4]= 
_framebuffer_matrix[6]= _framebuffer_matrix[7]= 
_framebuffer_matrix[8]= _framebuffer_matrix[9]= 
_framebuffer_matrix[11]= _framebuffer_matrix[12]= 
_framebuffer_matrix[13]= _framebuffer_matrix[14]= 0.0;
}
/*:521*//*526:*/
#line 11757 "./cweb/weaver.w"

W.change_resolution= &_change_resolution;
/*:526*//*531:*/
#line 11790 "./cweb/weaver.w"

W.change_final_shader= &_change_final_shader;
/*:531*//*533:*/
#line 11805 "./cweb/weaver.w"

W.final_shader_integer= 0;
/*:533*//*552:*/
#line 12011 "./cweb/weaver.w"

{
_initialize_sound();
}
/*:552*//*563:*/
#line 12144 "./cweb/weaver.w"

W.select_sound_device= &_select_sound_device;
/*:563*//*567:*/
#line 12177 "./cweb/weaver.w"

W.current_sound_device= &_current_sound_device;
/*:567*//*576:*/
#line 12350 "./cweb/weaver.w"

W.pending_files= 0;
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&(W._pending_files_mutex),NULL)!=0){
fprintf(stderr,"ERROR (0): Can't initialize mutex for file loading.\n");
exit(1);
}
#endif
/*:576*//*600:*/
#line 12957 "./cweb/weaver.w"

W.new_sound= &_new_sound;
/*:600*//*604:*/
#line 12989 "./cweb/weaver.w"

W.play_sound= &_play_sound;
/*:604*//*608:*/
#line 13038 "./cweb/weaver.w"

W.destroy_sound= &_destroy_sound;
/*:608*//*611:*/
#line 13076 "./cweb/weaver.w"

{
int i;
for(i= 0;i<W_MAX_SUBLOOP;i++){
_finalize_list[i]= NULL;
}
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_finalizing_mutex,NULL)!=0){
fprintf(stderr,"ERROR (0): Can't initialize mutex.\n");
exit(1);
}
#endif
}
/*:611*//*669:*/
#line 14961 "./cweb/weaver.w"

{
_empty_image[0]= _empty_image[1]= _empty_image[2]= _empty_image[3]= '\0';
glGenTextures(1,&_empty_texture);
glBindTexture(GL_TEXTURE_2D,_empty_texture);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,
GL_UNSIGNED_BYTE,&_empty_texture);
glBindTexture(GL_TEXTURE_2D,0);
}
/*:669*//*686:*/
#line 15334 "./cweb/weaver.w"

{
GLuint vertex,fragment;
vertex= _compile_vertex_shader(_vertex_image_interface);
fragment= _compile_fragment_shader(_fragment_image_interface);

_image_interface_shader.program_shader= 
_link_and_clean_shaders(vertex,fragment);
_image_interface_shader._uniform_texture1= 
glGetUniformLocation(_image_interface_shader.program_shader,
"texture1");
_image_interface_shader._uniform_object_color= 
glGetUniformLocation(_image_interface_shader.program_shader,
"object_color");
_image_interface_shader._uniform_model_view= 
glGetUniformLocation(_image_interface_shader.program_shader,
"model_view_matrix");
_image_interface_shader._uniform_object_size= 
glGetUniformLocation(_image_interface_shader.program_shader,
"object_size");
_image_interface_shader._uniform_time= 
glGetUniformLocation(_image_interface_shader.program_shader,
"time");
_image_interface_shader._uniform_integer= 
glGetUniformLocation(_image_interface_shader.program_shader,
"integer");
_image_interface_shader._attribute_vertex_position= 
glGetAttribLocation(_image_interface_shader.program_shader,
"vertex_position");
}
/*:686*//*721:*/
#line 16202 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
_initialize_database();
}
#endif
/*:721*//*728:*/
#line 16256 "./cweb/weaver.w"

W.write_integer= &_write_integer;
/*:728*//*733:*/
#line 16328 "./cweb/weaver.w"

W.write_float= &_write_float;
/*:733*//*738:*/
#line 16396 "./cweb/weaver.w"

W.write_string= &_write_string;
/*:738*//*744:*/
#line 16470 "./cweb/weaver.w"

W.read_integer= &_read_integer;
W.read_float= &_read_float;
W.read_string= &_read_string;
/*:744*//*754:*/
#line 16720 "./cweb/weaver.w"

W.delete_integer= &_delete_integer;
W.delete_float= &_delete_float;
W.delete_string= &_delete_string;
W.delete_all= &_delete_all;
/*:754*//*777:*/
#line 17245 "./cweb/weaver.w"

W.play_music= &_play_music;
/*:777*//*781:*/
#line 17300 "./cweb/weaver.w"

W.pause_music= &_pause_music;
/*:781*//*787:*/
#line 17404 "./cweb/weaver.w"

W.stop_music= &_stop_music;
/*:787*//*793:*/
#line 17472 "./cweb/weaver.w"

W.get_volume= &_get_volume;
/*:793*//*797:*/
#line 17532 "./cweb/weaver.w"

W.increase_volume= &_increase_volume;
/*:797*//*828:*/
#line 18214 "./cweb/weaver.w"

{
Wbreakpoint();
_initialize_metafont();
}
/*:828*/
#line 2063 "./cweb/weaver.w"

/*371:*/
#line 8301 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
int i= 0;
if(strcmp(W_PLUGIN_PATH,"")){
char*begin= W_PLUGIN_PATH,*end= W_PLUGIN_PATH;
char dir[256];
DIR*directory;
struct dirent*dp;
_max_number_of_plugins= 0;
while(*end!='\0'){
end++;
while(*end!=':'&&*end!='\0')end++;

if(end-begin> 255){
fprintf(stderr,"ERROR: Path too big in W_PLUGIN_PATH.\n");
begin= end+1;
continue;
}
strncpy(dir,begin,(size_t)(end-begin));
dir[(end-begin)]= '\0';

directory= opendir(dir);
if(directory==NULL){
#if W_DEBUG_LEVEL >= 2
fprintf(stderr,"WARNING (2): Trying to access plugin directory %s: "
"%s\n",dir,strerror(errno));
#endif


begin= end+1;
continue;
}

while((dp= readdir(directory))!=NULL){
if(dp->d_name[0]!='.'&&dp->d_type==DT_REG)
_max_number_of_plugins++;

}

begin= end+1;
}

/*372:*/
#line 8354 "./cweb/weaver.w"

{
_max_number_of_plugins+= 25;
#if W_DEBUG_LEVEL >= 3
printf("WARNING (3): Supporting maximum of %d plugins.\n",
_max_number_of_plugins);
#endif
_plugins= (struct _plugin_data*)_iWalloc(sizeof(struct _plugin_data)*
(_max_number_of_plugins));
if(_plugins==NULL){
fprintf(stderr,"ERROR (0): Too many plugins. Not enough memory!\n");
Wexit();
}
for(i= 0;i<_max_number_of_plugins;i++){
_plugins[i].defined= false;
}
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_plugin_mutex,NULL)!=0){
perror("Initializing plugin mutex:");
Wexit();
}
#endif
}
/*:372*//*373:*/
#line 8383 "./cweb/weaver.w"

{
size_t dir_length,d_name_length;
begin= end= W_PLUGIN_PATH;
i= 0;
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
d_name_length= strlen(dp->d_name);
if(dir_length+1+d_name_length> 255){
fprintf(stderr,"Ignoring plugin with too long path: %s/%s.\n",
dir,dp->d_name);
continue;
}
if(i>=_max_number_of_plugins){
fprintf(stderr,"Ignoring plugin %s/%s, not prepared for so much "
"new plugins being added.\n",dir,dp->d_name);
continue;
}
memcpy(&dir[dir_length],"/",2);
memcpy(&dir[dir_length+1],dp->d_name,d_name_length+1);
_initialize_plugin(&(_plugins[i]),dir);
i++;
}
}

begin= end+1;
}
}
/*:373*/
#line 8344 "./cweb/weaver.w"

}
}
#endif
/*:371*//*387:*/
#line 8649 "./cweb/weaver.w"

#if W_TARGET == W_WEB
{
_max_number_of_plugins= _W_NUMBER_OF_PLUGINS;
_plugins= (struct _plugin_data*)Walloc(sizeof(struct _plugin_data)*
_max_number_of_plugins);
#include "../../.hidden_code/initialize_plugin.c"
}
#endif
/*:387*/
#line 2064 "./cweb/weaver.w"

}
void _may_the_weaver_sleep(void){
/*73:*/
#line 2096 "./cweb/weaver.w"


/*:73*//*151:*/
#line 3718 "./cweb/weaver.w"


/*829:*/
#line 18224 "./cweb/weaver.w"

{
Wtrash();
}
/*:829*/
#line 3720 "./cweb/weaver.w"

/*184:*/
#line 4501 "./cweb/weaver.w"

{
int i;
for(i= 0;i<=_number_of_loops;i++)
Wtrash();
}
/*:184*//*243:*/
#line 5693 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&_window_mutex)!=0){
perror(NULL);
exit(1);
}
#endif
#if W_TARGET == W_ELF
_finalize_window();
#endif
/*:243*//*272:*/
#line 6208 "./cweb/weaver.w"

#if W_TARGET == W_WEB
_finalize_canvas();
#endif
/*:272*//*374:*/
#line 8441 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
int j;
for(j= 0;j<_max_number_of_plugins;j++)
if(_plugins[j].defined)
_finalize_plugin(&(_plugins[j]));
}
#endif
/*:374*//*388:*/
#line 8666 "./cweb/weaver.w"

#if W_TARGET == W_WEB
Wfree(_plugins);
#endif
/*:388*//*390:*/
#line 8686 "./cweb/weaver.w"

#if W_TARGET == W_WEB
{
int i;
for(i= 0;i<_max_number_of_plugins;i++)
_plugins[i]._fini_plugin(&W);
}
#endif
/*:390*/
#line 3721 "./cweb/weaver.w"

/*553:*/
#line 12017 "./cweb/weaver.w"

{
_finalize_sound();
}
/*:553*/
#line 3722 "./cweb/weaver.w"



_finalize_memory();
/*:151*//*300:*/
#line 6646 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&_input_mutex)!=0){
perror(NULL);
exit(1);
}
#endif
/*:300*//*394:*/
#line 8748 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_destroy(&_scheduler_mutex);
#endif
/*:394*//*423:*/
#line 9329 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&_interface_mutex)!=0)
perror("Finalizing interface mutex:");
#endif
/*:423*//*480:*/
#line 10422 "./cweb/weaver.w"

glDeleteProgram(_default_interface_shader.program_shader);
/*:480*//*510:*/
#line 11394 "./cweb/weaver.w"

glDeleteFramebuffers(1,&_framebuffer);
/*:510*//*515:*/
#line 11464 "./cweb/weaver.w"

glDeleteTextures(1,&_texture);
glDeleteRenderbuffers(1,&_depth_stencil);
/*:515*//*577:*/
#line 12359 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_destroy(&(W._pending_files_mutex));
#endif
/*:577*//*612:*/
#line 13094 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_destroy(&_finalizing_mutex);
#endif
/*:612*//*670:*/
#line 14973 "./cweb/weaver.w"

{
glDeleteTextures(1,&_empty_texture);
}
/*:670*//*724:*/
#line 16229 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
_finalize_database();
}
#endif
/*:724*/
#line 2067 "./cweb/weaver.w"

exit(0);
}

void _update(void){
/*274:*/
#line 6222 "./cweb/weaver.w"

/*309:*/
#line 6864 "./cweb/weaver.w"

{
int i,key;

for(i= 0;i<20;i++){
key= _released_keys[i];











while(W.keyboard[key]> 0){
int j;
for(j= i;j<19;j++){
_released_keys[j]= _released_keys[j+1];
}
_released_keys[19]= 0;
key= _released_keys[i];
}
if(key==0)break;

if(key==W_LEFT_CTRL||key==W_RIGHT_CTRL)W.keyboard[W_CTRL]= 0;
else if(key==W_LEFT_SHIFT||key==W_RIGHT_SHIFT)
W.keyboard[W_SHIFT]= 0;
else if(key==W_LEFT_ALT||key==W_RIGHT_ALT)W.keyboard[W_ALT]= 0;

W.keyboard[key]= 0;
_released_keys[i]= 0;
}

for(i= 0;i<20;i++){
key= _pressed_keys[i];
if(key==0)break;

if(key==W_LEFT_CTRL||key==W_RIGHT_CTRL)
W.keyboard[W_CTRL]+= W.dt;
else if(key==W_LEFT_SHIFT||key==W_RIGHT_SHIFT)
W.keyboard[W_SHIFT]+= W.dt;
else if(key==W_LEFT_ALT||key==W_RIGHT_ALT)
W.keyboard[W_ALT]+= W.dt;

W.keyboard[key]+= W.dt;
}
}
/*:309*//*322:*/
#line 7232 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_lock(&_input_mutex);
#endif
/*:322*//*328:*/
#line 7315 "./cweb/weaver.w"

{
int i,button;

for(i= 0;i<5;i++){
button= _released_buttons[i];
while(W.mouse.buttons[button]> 0){
int j;
for(j= i;j<4;j++){
_released_buttons[j]= _released_buttons[j+1];
}
_released_buttons[4]= 0;
button= _released_buttons[i];
}
if(button==0)break;
#if W_TARGET == W_ELF


if(button==W_MOUSE_LEFT)
XSetInputFocus(_dpy,_window,RevertToParent,CurrentTime);
#endif
W.mouse.buttons[button]= 0;
_released_buttons[i]= 0;
}

for(i= 0;i<5;i++){
button= _pressed_buttons[i];
if(button==0)break;
W.mouse.buttons[button]+= W.dt;
}
}
/*:328*//*338:*/
#line 7516 "./cweb/weaver.w"

old_dx= W.mouse.dx;
old_dy= W.mouse.dy;
W.mouse.dx= W.mouse.dy= 0;
/*:338*/
#line 6223 "./cweb/weaver.w"

#if W_TARGET == W_ELF
{
XEvent event;
while(XPending(_dpy)){
XNextEvent(_dpy,&event);

/*275:*/
#line 6260 "./cweb/weaver.w"

if(event.type==ConfigureNotify){
XConfigureEvent config= event.xconfigure;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_window_mutex);
#endif
W.width= config.width;
W.height= config.height;
W.x= config.x+W.width/2;
W.y= W.resolution_y-config.y-W.height/2;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_window_mutex);
#endif
continue;
}
/*:275*//*289:*/
#line 6415 "./cweb/weaver.w"

#if W_WIDTH == 0 && W_HEIGHT == 0
if(event.type==FocusOut){
XSetInputFocus(_dpy,_window,RevertToParent,CurrentTime);
continue;
}
#endif
/*:289*//*307:*/
#line 6784 "./cweb/weaver.w"

if(event.type==KeyPress){
unsigned int code= _translate_key(XkbKeycodeToKeysym(_dpy,
event.xkey.keycode,0,
0));
int i;

for(i= 0;i<20;i++){
if(_pressed_keys[i]==0||_pressed_keys[i]==code){
_pressed_keys[i]= code;
break;
}
}











if(W.keyboard[code]==0)
W.keyboard[code]= 1;
else if(W.keyboard[code]<0)
W.keyboard[code]*= -1;
continue;
}
/*:307*//*308:*/
#line 6820 "./cweb/weaver.w"

if(event.type==KeyRelease){
unsigned int code= _translate_key(XkbKeycodeToKeysym(_dpy,
event.xkey.keycode,
0,0));
int i;

for(i= 0;i<20;i++){
if(_pressed_keys[i]==code){
_pressed_keys[i]= 0;
break;
}
}
for(;i<19;i++){
_pressed_keys[i]= _pressed_keys[i+1];
}
_pressed_keys[19]= 0;

for(i= 0;i<20;i++){
if(_released_keys[i]==0||_released_keys[i]==code){
_released_keys[i]= code;
break;
}
}

W.keyboard[code]*= -1;
continue;
}
/*:308*//*329:*/
#line 7352 "./cweb/weaver.w"

if(event.type==ButtonPress){
unsigned int code= event.xbutton.button;
int i;

for(i= 0;i<5;i++){
if(_pressed_buttons[i]==0||_pressed_buttons[i]==code){
_pressed_buttons[i]= code;
break;
}
}




if(W.mouse.buttons[code]==0)
W.mouse.buttons[code]= 1;
else if(W.mouse.buttons[code]<0)
W.mouse.buttons[code]*= -1;
continue;
}
/*:329*//*330:*/
#line 7377 "./cweb/weaver.w"

if(event.type==ButtonRelease){
unsigned int code= event.xbutton.button;
int i;

for(i= 0;i<5;i++){
if(_pressed_buttons[i]==code){
_pressed_buttons[i]= 0;
break;
}
}
for(;i<4;i++){
_pressed_buttons[i]= _pressed_buttons[i+1];
}
_pressed_buttons[4]= 0;

for(i= 0;i<5;i++){
if(_released_buttons[i]==0||_released_buttons[i]==code){
_released_buttons[i]= code;
break;
}
}

W.mouse.buttons[code]*= -1;
continue;
}
/*:330*//*340:*/
#line 7540 "./cweb/weaver.w"

if(event.type==MotionNotify){
int x,y,dx,dy;
x= event.xmotion.x;
y= W.height-1-event.xmotion.y;
dx= x-W.mouse.x;
dy= y-W.mouse.y;
W.mouse.dx= ((float)dx/W.dt)*1000000;
W.mouse.dy= ((float)dy/W.dt)*1000000;
W.mouse.x= x;
W.mouse.y= y;
continue;
}
/*:340*/
#line 6230 "./cweb/weaver.w"

}
}
#endif
#if W_TARGET == W_WEB
{
SDL_Event event;
while(SDL_PollEvent(&event)){

/*318:*/
#line 7086 "./cweb/weaver.w"

if(event.type==SDL_KEYDOWN){
unsigned int code= event.key.keysym.sym;
int i;

for(i= 0;i<20;i++){
if(_pressed_keys[i]==0||_pressed_keys[i]==code){
_pressed_keys[i]= code;
break;
}
}





if(W.keyboard[code]==0)
W.keyboard[code]= 1;
else if(W.keyboard[code]<0)
W.keyboard[code]*= -1;
continue;
}
/*:318*//*319:*/
#line 7112 "./cweb/weaver.w"

if(event.type==SDL_KEYUP){
unsigned int code= event.key.keysym.sym;
int i;

for(i= 0;i<20;i++){
if(_pressed_keys[i]==code){
_pressed_keys[i]= 0;
break;
}
}
for(;i<19;i++){
_pressed_keys[i]= _pressed_keys[i+1];
}
_pressed_keys[19]= 0;

for(i= 0;i<20;i++){
if(_released_keys[i]==0||_released_keys[i]==code){
_released_keys[i]= code;
break;
}
}

W.keyboard[code]*= -1;
continue;
}

/*:319*//*331:*/
#line 7408 "./cweb/weaver.w"

if(event.type==SDL_MOUSEBUTTONDOWN){
unsigned int code= event.button.button;
int i;

for(i= 0;i<5;i++){
if(_pressed_buttons[i]==0||_pressed_buttons[i]==code){
_pressed_buttons[i]= code;
break;
}
}


if(W.mouse.buttons[code]==0)
W.mouse.buttons[code]= 1;
else if(W.mouse.buttons[code]<0)
W.mouse.buttons[code]*= -1;
continue;
}
/*:331*//*332:*/
#line 7431 "./cweb/weaver.w"

if(event.type==SDL_MOUSEBUTTONUP){
unsigned int code= event.button.button;
int i;

for(i= 0;i<5;i++){
if(_pressed_buttons[i]==code){
_pressed_buttons[i]= 0;
break;
}
}
for(;i<4;i++){
_pressed_buttons[i]= _pressed_buttons[i+1];
}
_pressed_buttons[4]= 0;

for(i= 0;i<5;i++){
if(_released_buttons[i]==0||_released_buttons[i]==code){
_released_buttons[i]= code;
break;
}
}

W.mouse.buttons[code]*= -1;
continue;
}
/*:332*//*341:*/
#line 7557 "./cweb/weaver.w"

if(event.type==SDL_MOUSEMOTION){
int x,y,dx,dy;
x= event.motion.x;
y= W.height-1-event.motion.y;
dx= x-W.mouse.x;
dy= y-W.mouse.y;
W.mouse.dx= ((float)dx/W.dt)*1000000;
W.mouse.dy= ((float)dy/W.dt)*1000000;
W.mouse.x= x;
W.mouse.y= y;
continue;
}
/*:341*/
#line 6239 "./cweb/weaver.w"

}
}
#endif
/*339:*/
#line 7522 "./cweb/weaver.w"


W.mouse.ddx= (int)((float)(W.mouse.dx-old_dx)/W.dt)*
1000000;
W.mouse.ddy= (int)((float)(W.mouse.dy-old_dy)/W.dt)*
1000000;
#ifdef W_MULTITHREAD

pthread_mutex_unlock(&_input_mutex);
#endif
/*:339*/
#line 6243 "./cweb/weaver.w"

/*:274*//*292:*/
#line 6449 "./cweb/weaver.w"

glClear(GL_COLOR_BUFFER_BIT);
/*:292*//*310:*/
#line 6919 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_lock(&_input_mutex);
#endif
W.keyboard[W_ANY]= (_pressed_keys[0]!=0);
/*:310*//*333:*/
#line 7462 "./cweb/weaver.w"

W.mouse.buttons[W_ANY]= (_pressed_buttons[0]!=0);
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_input_mutex);
#endif
/*:333*//*385:*/
#line 8620 "./cweb/weaver.w"

{
int i;
for(i= 0;i<_max_number_of_plugins;i++)
if(_plugins[i].defined&&_plugins[i].enabled)
_plugins[i]._run_plugin(&W);
}
/*:385*//*397:*/
#line 8796 "./cweb/weaver.w"

{
int i;
void(*f)(void);
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_scheduler_mutex);
#endif
for(i= 0;i<W_MAX_SCHEDULING;i++){
if(_scheduled_functions[_number_of_loops][i].f==NULL)
break;
if(_scheduled_functions[_number_of_loops][i].period<
W.t-_scheduled_functions[_number_of_loops][i].last_execution){
f= _scheduled_functions[_number_of_loops][i].f;
if(!_scheduled_functions[_number_of_loops][i].periodic){
int j;
_scheduled_functions[_number_of_loops][i].f= NULL;
for(j= i+1;j<W_MAX_SCHEDULING;j++){
_scheduled_functions[_number_of_loops][j-1].periodic= 
_scheduled_functions[_number_of_loops][j].periodic;
_scheduled_functions[_number_of_loops][j-1].last_execution= 
_scheduled_functions[_number_of_loops][j].last_execution;
_scheduled_functions[_number_of_loops][j-1].period= 
_scheduled_functions[_number_of_loops][j].period;
_scheduled_functions[_number_of_loops][j-1].f= 
_scheduled_functions[_number_of_loops][j].f;
}
_scheduled_functions[_number_of_loops][W_MAX_SCHEDULING-1].f= NULL;
i--;
}
else
_scheduled_functions[_number_of_loops][i].last_execution= W.t;
f();
}
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_scheduler_mutex);
#endif
}
/*:397*/
#line 2072 "./cweb/weaver.w"

}

void _render(void){

glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
/*516:*/
#line 11474 "./cweb/weaver.w"

if(_final_shader[_number_of_loops]!=W_NONE){
glBindFramebuffer(GL_FRAMEBUFFER,_framebuffer);
glViewport(0,0,W.width,W.height);
glEnable(GL_DEPTH_TEST);
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
/*:516*/
#line 2078 "./cweb/weaver.w"


/*502:*/
#line 11141 "./cweb/weaver.w"

{

glBindBuffer(GL_ARRAY_BUFFER,_interface_VBO);


int last_type= W_NONE;
int i;
bool first_element= true;
float time_float;
struct _shader*current_shader= NULL;



i= W.t%300000000;
time_float= (float)i/1000000;
time_float+= ((float)(i-1000000*time_float))/1000000.0;

glClear(GL_DEPTH_BUFFER_BIT);

glBindVertexArray(_interface_VAO);



for(i= 0;i<W_MAX_INTERFACES;i++){

if(_interface_queue[_number_of_loops][i]==NULL)break;
if(!(_interface_queue[_number_of_loops][i]->visible))continue;
if(first_element||
_interface_queue[_number_of_loops][i]->type!=last_type){
last_type= _interface_queue[_number_of_loops][i]->type;
if(_interface_queue[_number_of_loops][i]->type> 0){
current_shader= 
&(_shader_list[_interface_queue[_number_of_loops][i]->
type-1]);
}
/*687:*/
#line 15369 "./cweb/weaver.w"

else if(_interface_queue[_number_of_loops][i]->type==W_INTERFACE_IMAGE){
current_shader= &_image_interface_shader;
}
/*:687*/
#line 11177 "./cweb/weaver.w"

else{
current_shader= &_default_interface_shader;
}
glUseProgram(current_shader->program_shader);
first_element= false;
}


glUniform4f(current_shader->_uniform_object_color,
_interface_queue[_number_of_loops][i]->r,
_interface_queue[_number_of_loops][i]->g,
_interface_queue[_number_of_loops][i]->b,
_interface_queue[_number_of_loops][i]->a);
glUniform2f(current_shader->_uniform_object_size,
_interface_queue[_number_of_loops][i]->width,
_interface_queue[_number_of_loops][i]->height);
glUniform1f(current_shader->_uniform_time,time_float);
glUniform1i(current_shader->_uniform_integer,
_interface_queue[_number_of_loops][i]->integer);
glUniformMatrix4fv(current_shader->_uniform_model_view,1,false,
_interface_queue[_number_of_loops][i]->
_transform_matrix);
/*688:*/
#line 15378 "./cweb/weaver.w"


if(_interface_queue[_number_of_loops][i]->animate&&
_interface_queue[_number_of_loops][i]->number_of_frames> 1&&
_interface_queue[_number_of_loops][i]->max_repetition!=0){
if(W.t+_lag-_interface_queue[_number_of_loops][i]->_t> 
_interface_queue[_number_of_loops][i]->
frame_duration[_interface_queue[_number_of_loops][i]->current_frame]){
if(_interface_queue[_number_of_loops][i]->current_frame+1==
_interface_queue[_number_of_loops][i]->number_of_frames){

if(_interface_queue[_number_of_loops][i]->max_repetition> 0){
_interface_queue[_number_of_loops][i]->max_repetition--;
}
if(_interface_queue[_number_of_loops][i]->max_repetition!=0){
_interface_queue[_number_of_loops][i]->current_frame= 0;
_interface_queue[_number_of_loops][i]->_t= W.t+_lag;
}
}
else{

_interface_queue[_number_of_loops][i]->current_frame++;
_interface_queue[_number_of_loops][i]->_t= W.t+_lag;
}
}
}

glBindTexture(GL_TEXTURE_2D,
_interface_queue[_number_of_loops][i]->
_texture[_interface_queue[_number_of_loops][i]->current_frame]);
/*:688*/
#line 11200 "./cweb/weaver.w"


glEnableVertexAttribArray(current_shader->_attribute_vertex_position);
glVertexAttribPointer(current_shader->_attribute_vertex_position,
3,GL_FLOAT,GL_FALSE,0,(void*)0);
glDrawArrays(_interface_queue[_number_of_loops][i]->_mode,0,4);
glDisableVertexAttribArray(current_shader->
_attribute_vertex_position);
glBindTexture(GL_TEXTURE_2D,0);
}


glBindVertexArray(0);
glEnable(GL_DEPTH_TEST);
}
/*:502*/
#line 2080 "./cweb/weaver.w"

/*522:*/
#line 11612 "./cweb/weaver.w"

if(_final_shader[_number_of_loops]!=W_NONE){
struct _shader*current_shader;

glBindBuffer(GL_ARRAY_BUFFER,_interface_VBO);
glBindFramebuffer(GL_FRAMEBUFFER,0);
glViewport(0,0,W.resolution_x,W.resolution_y);
glBindVertexArray(_interface_VAO);
glDisable(GL_DEPTH_TEST);
if(_final_shader[_number_of_loops]> 0){
glUseProgram(_shader_list[_final_shader[_number_of_loops]-
1].program_shader);
current_shader= &(_shader_list[_final_shader[_number_of_loops]-1]);
}
else{
glUseProgram(_framebuffer_shader.program_shader);
current_shader= &(_framebuffer_shader);
}
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,_texture);
glEnableVertexAttribArray(current_shader->_attribute_vertex_position);
glVertexAttribPointer(current_shader->_attribute_vertex_position,
3,GL_FLOAT,GL_FALSE,0,(void*)0);

glUniform2f(current_shader->_uniform_object_size,W.width,W.height);
glUniform4f(current_shader->_uniform_object_color,W_DEFAULT_COLOR,
1.0);
glUniform1f(current_shader->_uniform_time,
(float)W.t/(float)1000000);
glUniformMatrix4fv(current_shader->_uniform_model_view,1,false,
_framebuffer_matrix);
/*534:*/
#line 11812 "./cweb/weaver.w"

glUniform1i(current_shader->_uniform_integer,
W.final_shader_integer);
/*:534*/
#line 11643 "./cweb/weaver.w"

glDrawArrays(GL_TRIANGLE_FAN,0,4);
glDisableVertexAttribArray(current_shader->_attribute_vertex_position);
}
/*:522*/
#line 2081 "./cweb/weaver.w"

#if W_TARGET == W_ELF
glXSwapBuffers(_dpy,_window);
#else
glFlush();
#endif
}
/*:72*/
