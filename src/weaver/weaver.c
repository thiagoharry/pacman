/*72:*/
#line 2037 "cweb/weaver.w"

#include "weaver.h"
/*76:*/
#line 2098 "cweb/weaver.w"

struct _weaver_struct W;
/*:76*//*82:*/
#line 2166 "cweb/weaver.w"

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
/*:82*//*172:*/
#line 4243 "cweb/weaver.w"

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
/*180:*/
#line 4406 "cweb/weaver.w"

_lag= 0;
/*:180*//*290:*/
#line 6556 "cweb/weaver.w"

W.flush_input();
/*:290*//*357:*/
#line 8089 "cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins();
#endif
/*:357*//*509:*/
#line 11271 "cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:509*//*773:*/
#line 17048 "cweb/weaver.w"

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
/*:773*/
#line 4255 "cweb/weaver.w"

/*369:*/
#line 8250 "cweb/weaver.w"

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
/*:369*//*400:*/
#line 8858 "cweb/weaver.w"

_flush_interfaces();
/*:400*//*472:*/
#line 10558 "cweb/weaver.w"

_clean_interface_queue();
/*:472*//*514:*/
#line 11315 "cweb/weaver.w"

W.final_shader_integer= 0;
/*:514*//*591:*/
#line 12619 "cweb/weaver.w"

_finalize_all();
/*:591*//*771:*/
#line 16984 "cweb/weaver.w"

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
/*:771*/
#line 4256 "cweb/weaver.w"

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
/*:172*//*174:*/
#line 4281 "cweb/weaver.w"

void _exit_loop(void){
#if W_DEBUG_LEVEL >= 1
if(_first_loop){
fprintf(stderr,"ERROR (1): Using Wexit_loop outside a game loop.\n");
Wexit();
}
#endif
Wtrash();
if(_number_of_loops==0)
Wexit();
else{
/*370:*/
#line 8268 "cweb/weaver.w"

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
/*:370*//*401:*/
#line 8865 "cweb/weaver.w"

_flush_interfaces();
/*:401*//*473:*/
#line 10565 "cweb/weaver.w"

_clean_interface_queue();
/*:473*//*510:*/
#line 11280 "cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:510*//*513:*/
#line 11308 "cweb/weaver.w"

W.final_shader_integer= _final_shader_integer[_number_of_loops-1];
/*:513*//*592:*/
#line 12623 "cweb/weaver.w"

_finalize_all();
/*:592*//*770:*/
#line 16949 "cweb/weaver.w"

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
/*:770*/
#line 4293 "cweb/weaver.w"

_number_of_loops--;
/*180:*/
#line 4406 "cweb/weaver.w"

_lag= 0;
/*:180*//*290:*/
#line 6556 "cweb/weaver.w"

W.flush_input();
/*:290*//*357:*/
#line 8089 "cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins();
#endif
/*:357*//*509:*/
#line 11271 "cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:509*//*773:*/
#line 17048 "cweb/weaver.w"

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
/*:773*/
#line 4295 "cweb/weaver.w"

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
/*:174*//*176:*/
#line 4331 "cweb/weaver.w"

void Wsubloop(void(*f)(void)){
#if W_TARGET == W_WEB
emscripten_cancel_main_loop();
#endif
Wbreakpoint();
_loop_begin= 1;
_number_of_loops++;
/*180:*/
#line 4406 "cweb/weaver.w"

_lag= 0;
/*:180*//*290:*/
#line 6556 "cweb/weaver.w"

W.flush_input();
/*:290*//*357:*/
#line 8089 "cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins();
#endif
/*:357*//*509:*/
#line 11271 "cweb/weaver.w"

if(_changed_resolution){
_final_shader[_number_of_loops]= W_DEFAULT_SHADER;
}
else{
_final_shader[_number_of_loops]= W_NONE;
}
/*:509*//*773:*/
#line 17048 "cweb/weaver.w"

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
/*:773*/
#line 4339 "cweb/weaver.w"

/*512:*/
#line 11299 "cweb/weaver.w"

{
_final_shader_integer[_number_of_loops-1]= W.final_shader_integer;
W.final_shader_integer= 0;
}
/*:512*//*772:*/
#line 17022 "cweb/weaver.w"

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
/*:772*/
#line 4340 "cweb/weaver.w"

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
/*:176*//*271:*/
#line 6086 "cweb/weaver.w"

#if W_TARGET == W_ELF
static struct _k_translate{
unsigned original_symbol,new_symbol;
}_key_translate[100];
#endif


static unsigned _pressed_keys[20];
static unsigned _released_keys[20];

static unsigned _pressed_buttons[5];
static unsigned _released_buttons[5];
/*:271*//*276:*/
#line 6173 "cweb/weaver.w"

#if W_TARGET == W_ELF
static unsigned _translate_key(unsigned symbol){
int i;
for(i= 0;i<100;i++){
if(_key_translate[i].original_symbol==0)
return symbol%0xffff;
if(_key_translate[i].original_symbol==symbol)
return _key_translate[i].new_symbol%0xffff;
}
#if W_DEBUG_LEVEL >= 2
if(symbol>=0xffff)
fprintf(stderr,"WARNING (2): Key with unknown code pressed: %lu",
(unsigned long)symbol);
#endif
return symbol%0xffff;
}
#endif
/*:276*//*278:*/
#line 6205 "cweb/weaver.w"

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
/*:278*//*287:*/
#line 6519 "cweb/weaver.w"

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
/*316:*/
#line 7069 "cweb/weaver.w"

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
/*:316*/
#line 6536 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_input_mutex);
#endif
}
/*:287*//*310:*/
#line 7000 "cweb/weaver.w"

static int old_dx,old_dy;
/*:310*//*320:*/
#line 7101 "cweb/weaver.w"

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
/*:320*//*321:*/
#line 7147 "cweb/weaver.w"

#if W_TARGET == W_WEB
void _Whide_cursor(void){
SDL_ShowCursor(0);
emscripten_hide_mouse();
_using_original_cursor= false;
}
#endif
/*:321*//*373:*/
#line 8343 "cweb/weaver.w"

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
/*:373*//*374:*/
#line 8376 "cweb/weaver.w"

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
/*:374*//*375:*/
#line 8410 "cweb/weaver.w"

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
/*:375*//*376:*/
#line 8448 "cweb/weaver.w"

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
/*:376*//*588:*/
#line 12557 "cweb/weaver.w"

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
/*:588*//*590:*/
#line 12599 "cweb/weaver.w"

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
/*:590*//*594:*/
#line 12637 "cweb/weaver.w"

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
/*:594*/
#line 2039 "cweb/weaver.w"

void _awake_the_weaver(void){
/*80:*/
#line 2151 "cweb/weaver.w"

W.t= 0;
gettimeofday(&_last_time,NULL);
/*:80*//*86:*/
#line 2217 "cweb/weaver.w"

W.game= &_game;
/*:86*//*117:*/
#line 3085 "cweb/weaver.w"

W.create_arena= &_Wcreate_arena;
/*:117*//*125:*/
#line 3207 "cweb/weaver.w"

W.destroy_arena= &Wdestroy_arena;
/*:125*//*130:*/
#line 3336 "cweb/weaver.w"

W.alloc_arena= &_alloc;
/*:130*//*134:*/
#line 3448 "cweb/weaver.w"

W.free= &_free;
/*:134*//*140:*/
#line 3558 "cweb/weaver.w"

W.breakpoint_arena= &_new_breakpoint;
/*:140*//*144:*/
#line 3620 "cweb/weaver.w"

W.trash_arena= &Wtrash_arena;
/*:144*//*149:*/
#line 3688 "cweb/weaver.w"

_initialize_memory();
/*:149*//*154:*/
#line 3752 "cweb/weaver.w"

W.alloc= &_Walloc;
/*:154*//*159:*/
#line 3821 "cweb/weaver.w"

W.breakpoint= &_Wbreakpoint;
W.trash= &_Wtrash;
/*:159*//*171:*/
#line 4235 "cweb/weaver.w"

_first_loop= true;
_running_loop= false;
_number_of_loops= 0;
/*:171*//*179:*/
#line 4401 "cweb/weaver.w"

W.dt= 40000;
_lag= 0;
/*:179*//*192:*/
#line 4641 "cweb/weaver.w"

_initialize_numeric_functions();
/*:192*//*193:*/
#line 4647 "cweb/weaver.w"

_finalize_numeric_functions();
/*:193*//*203:*/
#line 4904 "cweb/weaver.w"

W.random= &_random;
/*:203*//*216:*/
#line 5174 "cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_init(&_window_mutex,NULL)!=0){
perror(NULL);
exit(1);
}
#endif
#if W_TARGET == W_ELF
_initialize_window();
#endif
/*:216*//*245:*/
#line 5694 "cweb/weaver.w"

#if W_TARGET == W_WEB
_initialize_canvas();
#endif
/*:245*//*255:*/
#line 5839 "cweb/weaver.w"

W.resize_window= &_Wresize_window;
/*:255*//*261:*/
#line 5885 "cweb/weaver.w"

W.move_window= &_Wmove_window;
/*:261*//*265:*/
#line 5928 "cweb/weaver.w"


glClearColor(W_DEFAULT_COLOR,1.0f);

glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LEQUAL);

glEnable(GL_CULL_FACE);

glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
/*:265*//*273:*/
#line 6111 "cweb/weaver.w"

{
int i;
for(i= 0;i<0xffff;i++)
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
/*:273*//*280:*/
#line 6260 "cweb/weaver.w"

W.key_translate= &_Wkey_translate;
W.erase_key_translations= &_Werase_key_translations;
/*:280*//*289:*/
#line 6549 "cweb/weaver.w"

W.flush_input= &_Wflush_input;
/*:289*//*301:*/
#line 6790 "cweb/weaver.w"

{
int i;
for(i= 0;i<5;i++)
W.mouse.buttons[i]= 0;
for(i= 0;i<5;i++){
_pressed_buttons[i]= 0;
_released_buttons[i]= 0;
}
}
/*:301*//*309:*/
#line 6969 "cweb/weaver.w"

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
/*:309*//*311:*/
#line 7004 "cweb/weaver.w"

old_dx= old_dy= 0;
/*:311*//*319:*/
#line 7097 "cweb/weaver.w"

_using_original_cursor= true;
/*:319*//*323:*/
#line 7163 "cweb/weaver.w"

W.hide_cursor= &_Whide_cursor;
/*:323*//*341:*/
#line 7744 "cweb/weaver.w"

W.reload_plugin= &_reload_plugin;
/*:341*//*352:*/
#line 7971 "cweb/weaver.w"

#if W_TARGET == W_ELF
W.get_plugin= &_Wget_plugin;
#endif
/*:352*//*356:*/
#line 8080 "cweb/weaver.w"

#if W_TARGET == W_ELF
W.reload_all_plugins= &_reload_all_plugins;
#endif
/*:356*//*367:*/
#line 8221 "cweb/weaver.w"

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
/*:367*//*378:*/
#line 8482 "cweb/weaver.w"

W.run_periodically= &_run_periodically;
W.run_futurelly= &_run_futurelly;
W.cancel= &_cancel;
W.period= &_period;
/*:378*//*383:*/
#line 8569 "cweb/weaver.w"

#if W_TARGET == W_ELF
W.enable_plugin= &_Wenable_plugin;
W.disable_plugin= &_Wdisable_plugin;
W.is_plugin_enabled= &_Wis_enabled;
#endif
/*:383*//*387:*/
#line 8618 "cweb/weaver.w"

#if W_TARGET == W_ELF
W.get_plugin_data= &_Wget_plugin_data;
W.set_plugin_data= &_Wset_plugin_data;
#endif
/*:387*//*396:*/
#line 8801 "cweb/weaver.w"

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
/*:396*//*402:*/
#line 8876 "cweb/weaver.w"

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
/*:402*//*404:*/
#line 8909 "cweb/weaver.w"

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
/*:404*//*409:*/
#line 9046 "cweb/weaver.w"

W.new_interface= &_new_interface;
/*:409*//*413:*/
#line 9093 "cweb/weaver.w"

W.destroy_interface= &_destroy_interface;
/*:413*//*417:*/
#line 9159 "cweb/weaver.w"

W.copy_interface= &_copy_interface;
/*:417*//*422:*/
#line 9258 "cweb/weaver.w"

W.move_interface= &_move_interface;
/*:422*//*426:*/
#line 9337 "cweb/weaver.w"

W.resize_interface= &_resize_interface;
/*:426*//*430:*/
#line 9417 "cweb/weaver.w"

W.rotate_interface= &_rotate_interface;
/*:430*//*453:*/
#line 9865 "cweb/weaver.w"

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
/*:453*//*457:*/
#line 9997 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
int number_of_shaders= 0;
char shader_directory[256];
DIR*d;
shader_directory[0]= '\0';
#if W_DEBUG_LEVEL == 0
strcat(shader_directory,W_INSTALL_DATA);
strcat(shader_directory,"/");
#endif
strcat(shader_directory,"shaders/");


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
err= stat(file,&s);
if(err==-1)continue;
if(!S_ISDIR(s.st_mode))continue;
}
#endif
number_of_shaders++;
}
}
#endif

/*:457*//*458:*/
#line 10046 "cweb/weaver.w"

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
/*:458*//*459:*/
#line 10072 "cweb/weaver.w"

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
strcpy(path,shader_directory);
strcat(path,dir->d_name);
path[255]= '\0';
_compile_and_insert_new_shader(path,shader_number-1);
}
}
}
}
}
#endif
/*:459*//*460:*/
#line 10138 "cweb/weaver.w"

#if W_TARGET == W_WEB
#include "../../.hidden_code/initialize_shader.c"
#endif
/*:460*//*465:*/
#line 10404 "cweb/weaver.w"

{
int i,j;
for(i= 0;i<W_MAX_SUBLOOP;i++)
for(j= 0;j<W_MAX_INTERFACES;j++)
_interface_queue[i][j]= NULL;
}
/*:465*//*483:*/
#line 10826 "cweb/weaver.w"

{
_changed_resolution= false;


int i;
for(i= 0;i<W_MAX_SUBLOOP;i++)
_final_shader[i]= W_NONE;


glGenFramebuffers(1,&_framebuffer);


glBindFramebuffer(GL_FRAMEBUFFER,_framebuffer);
}
/*:483*//*486:*/
#line 10859 "cweb/weaver.w"

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
/*:486*//*488:*/
#line 10897 "cweb/weaver.w"

{
glGenRenderbuffers(1,&_depth_stencil);
glBindRenderbuffer(GL_RENDERBUFFER,_depth_stencil);
glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,
W.width,W.height);

glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,
GL_RENDERBUFFER,_depth_stencil);

glBindFramebuffer(GL_FRAMEBUFFER,0);
}
/*:488*//*495:*/
#line 11003 "cweb/weaver.w"

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
/*:495*//*500:*/
#line 11206 "cweb/weaver.w"

W.change_resolution= &_change_resolution;
/*:500*//*505:*/
#line 11239 "cweb/weaver.w"

W.change_final_shader= &_change_final_shader;
/*:505*//*507:*/
#line 11254 "cweb/weaver.w"

W.final_shader_integer= 0;
/*:507*//*526:*/
#line 11460 "cweb/weaver.w"

{
_initialize_sound();
}
/*:526*//*537:*/
#line 11593 "cweb/weaver.w"

W.select_sound_device= &_select_sound_device;
/*:537*//*541:*/
#line 11626 "cweb/weaver.w"

W.current_sound_device= &_current_sound_device;
/*:541*//*550:*/
#line 11799 "cweb/weaver.w"

W.pending_files= 0;
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&(W._pending_files_mutex),NULL)!=0){
fprintf(stderr,"ERROR (0): Can't initialize mutex for file loading.\n");
exit(1);
}
#endif
/*:550*//*574:*/
#line 12406 "cweb/weaver.w"

W.new_sound= &_new_sound;
/*:574*//*578:*/
#line 12438 "cweb/weaver.w"

W.play_sound= &_play_sound;
/*:578*//*582:*/
#line 12487 "cweb/weaver.w"

W.destroy_sound= &_destroy_sound;
/*:582*//*585:*/
#line 12525 "cweb/weaver.w"

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
/*:585*//*643:*/
#line 14418 "cweb/weaver.w"

{
_empty_image[0]= _empty_image[1]= _empty_image[2]= _empty_image[3]= '\0';
glGenTextures(1,&_empty_texture);
glBindTexture(GL_TEXTURE_2D,_empty_texture);
glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,1,1,0,GL_RGBA,
GL_UNSIGNED_BYTE,&_empty_texture);
glBindTexture(GL_TEXTURE_2D,0);
}
/*:643*//*660:*/
#line 14791 "cweb/weaver.w"

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
/*:660*//*695:*/
#line 15653 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
_initialize_database();
}
#endif
/*:695*//*702:*/
#line 15707 "cweb/weaver.w"

W.write_integer= &_write_integer;
/*:702*//*707:*/
#line 15779 "cweb/weaver.w"

W.write_float= &_write_float;
/*:707*//*712:*/
#line 15847 "cweb/weaver.w"

W.write_string= &_write_string;
/*:712*//*718:*/
#line 15921 "cweb/weaver.w"

W.read_integer= &_read_integer;
W.read_float= &_read_float;
W.read_string= &_read_string;
/*:718*//*728:*/
#line 16171 "cweb/weaver.w"

W.delete_integer= &_delete_integer;
W.delete_float= &_delete_float;
W.delete_string= &_delete_string;
W.delete_all= &_delete_all;
/*:728*//*751:*/
#line 16684 "cweb/weaver.w"

W.play_music= &_play_music;
/*:751*//*755:*/
#line 16739 "cweb/weaver.w"

W.pause_music= &_pause_music;
/*:755*//*761:*/
#line 16843 "cweb/weaver.w"

W.stop_music= &_stop_music;
/*:761*//*765:*/
#line 16874 "cweb/weaver.w"

W.get_volume= &_get_volume;
/*:765*//*769:*/
#line 16934 "cweb/weaver.w"

W.increase_volume= &_increase_volume;
/*:769*/
#line 2041 "cweb/weaver.w"

/*345:*/
#line 7797 "cweb/weaver.w"

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

/*346:*/
#line 7850 "cweb/weaver.w"

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
/*:346*//*347:*/
#line 7879 "cweb/weaver.w"

{
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

directory= opendir(dir);
if(directory==NULL){




begin= end+1;
continue;
}

while((dp= readdir(directory))!=NULL){
if(dp->d_name[0]!='.'&&dp->d_type==DT_REG){
if(strlen(dir)+1+strlen(dp->d_name)> 255){
fprintf(stderr,"Ignoring plugin with too long path: %s/%s.\n",
dir,dp->d_name);
continue;
}
if(i>=_max_number_of_plugins){
fprintf(stderr,"Ignoring plugin %s/%s, not prepared for so much "
"new plugins being added.\n",dir,dp->d_name);
continue;
}
strcat(dir,"/");
strcat(dir,dp->d_name);
_initialize_plugin(&(_plugins[i]),dir);
i++;
}
}

begin= end+1;
}
}
/*:347*/
#line 7840 "cweb/weaver.w"

}
}
#endif
/*:345*//*361:*/
#line 8139 "cweb/weaver.w"

#if W_TARGET == W_WEB
{
_max_number_of_plugins= _W_NUMBER_OF_PLUGINS;
_plugins= (struct _plugin_data*)Walloc(sizeof(struct _plugin_data)*
_max_number_of_plugins);
#include "../../.hidden_code/initialize_plugin.c"
}
#endif
/*:361*/
#line 2042 "cweb/weaver.w"

}
void _may_the_weaver_sleep(void){
/*73:*/
#line 2074 "cweb/weaver.w"


/*:73*//*150:*/
#line 3692 "cweb/weaver.w"


/*217:*/
#line 5185 "cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&_window_mutex)!=0){
perror(NULL);
exit(1);
}
#endif
#if W_TARGET == W_ELF
_finalize_window();
#endif
/*:217*//*246:*/
#line 5700 "cweb/weaver.w"

#if W_TARGET == W_WEB
_finalize_canvas();
#endif
/*:246*//*348:*/
#line 7934 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
int j;
for(j= 0;j<_max_number_of_plugins;j++)
if(_plugins[j].defined)
_finalize_plugin(&(_plugins[j]));
}
#endif
/*:348*//*362:*/
#line 8156 "cweb/weaver.w"

#if W_TARGET == W_WEB
Wfree(_plugins);
#endif
/*:362*//*364:*/
#line 8176 "cweb/weaver.w"

#if W_TARGET == W_WEB
{
int i;
for(i= 0;i<_max_number_of_plugins;i++)
_plugins[i]._fini_plugin(&W);
}
#endif
/*:364*/
#line 3694 "cweb/weaver.w"

/*527:*/
#line 11466 "cweb/weaver.w"

{
_finalize_sound();
}
/*:527*/
#line 3695 "cweb/weaver.w"



_finalize_memory();
/*:150*//*274:*/
#line 6138 "cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&_input_mutex)!=0){
perror(NULL);
exit(1);
}
#endif
/*:274*//*368:*/
#line 8238 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_destroy(&_scheduler_mutex);
#endif
/*:368*//*397:*/
#line 8819 "cweb/weaver.w"

#ifdef W_MULTITHREAD
if(pthread_mutex_destroy(&_interface_mutex)!=0)
perror("Finalizing interface mutex:");
#endif
/*:397*//*454:*/
#line 9897 "cweb/weaver.w"

glDeleteProgram(_default_interface_shader.program_shader);
/*:454*//*484:*/
#line 10843 "cweb/weaver.w"

glDeleteFramebuffers(1,&_framebuffer);
/*:484*//*489:*/
#line 10913 "cweb/weaver.w"

glDeleteTextures(1,&_texture);
glDeleteRenderbuffers(1,&_depth_stencil);
/*:489*//*551:*/
#line 11808 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_destroy(&(W._pending_files_mutex));
#endif
/*:551*//*586:*/
#line 12543 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_destroy(&_finalizing_mutex);
#endif
/*:586*//*644:*/
#line 14430 "cweb/weaver.w"

{
glDeleteTextures(1,&_empty_texture);
}
/*:644*//*698:*/
#line 15680 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
_finalize_database();
}
#endif
/*:698*/
#line 2045 "cweb/weaver.w"

exit(0);
}

void _update(void){
/*248:*/
#line 5714 "cweb/weaver.w"

/*283:*/
#line 6356 "cweb/weaver.w"

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
/*:283*//*296:*/
#line 6724 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_lock(&_input_mutex);
#endif
/*:296*//*302:*/
#line 6807 "cweb/weaver.w"

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
/*:302*//*312:*/
#line 7008 "cweb/weaver.w"

old_dx= W.mouse.dx;
old_dy= W.mouse.dy;
W.mouse.dx= W.mouse.dy= 0;
/*:312*/
#line 5715 "cweb/weaver.w"

#if W_TARGET == W_ELF
{
XEvent event;
while(XPending(_dpy)){
XNextEvent(_dpy,&event);

/*249:*/
#line 5752 "cweb/weaver.w"

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
/*:249*//*263:*/
#line 5907 "cweb/weaver.w"

#if W_WIDTH == 0 && W_HEIGHT == 0
if(event.type==FocusOut){
XSetInputFocus(_dpy,_window,RevertToParent,CurrentTime);
continue;
}
#endif
/*:263*//*281:*/
#line 6276 "cweb/weaver.w"

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
/*:281*//*282:*/
#line 6312 "cweb/weaver.w"

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
/*:282*//*303:*/
#line 6844 "cweb/weaver.w"

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
/*:303*//*304:*/
#line 6869 "cweb/weaver.w"

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
/*:304*//*314:*/
#line 7032 "cweb/weaver.w"

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
/*:314*/
#line 5722 "cweb/weaver.w"

}
}
#endif
#if W_TARGET == W_WEB
{
SDL_Event event;
while(SDL_PollEvent(&event)){

/*292:*/
#line 6578 "cweb/weaver.w"

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
/*:292*//*293:*/
#line 6604 "cweb/weaver.w"

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

/*:293*//*305:*/
#line 6900 "cweb/weaver.w"

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
/*:305*//*306:*/
#line 6923 "cweb/weaver.w"

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
/*:306*//*315:*/
#line 7049 "cweb/weaver.w"

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
/*:315*/
#line 5731 "cweb/weaver.w"

}
}
#endif
/*313:*/
#line 7014 "cweb/weaver.w"


W.mouse.ddx= (int)((float)(W.mouse.dx-old_dx)/W.dt)*
1000000;
W.mouse.ddy= (int)((float)(W.mouse.dy-old_dy)/W.dt)*
1000000;
#ifdef W_MULTITHREAD

pthread_mutex_unlock(&_input_mutex);
#endif
/*:313*/
#line 5735 "cweb/weaver.w"

/*:248*//*266:*/
#line 5941 "cweb/weaver.w"

glClear(GL_COLOR_BUFFER_BIT);
/*:266*//*284:*/
#line 6411 "cweb/weaver.w"

#ifdef W_MULTITHREAD
pthread_mutex_lock(&_input_mutex);
#endif
W.keyboard[W_ANY]= (_pressed_keys[0]!=0);
/*:284*//*307:*/
#line 6954 "cweb/weaver.w"

W.mouse.buttons[W_ANY]= (_pressed_buttons[0]!=0);
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_input_mutex);
#endif
/*:307*//*359:*/
#line 8110 "cweb/weaver.w"

{
int i;
for(i= 0;i<_max_number_of_plugins;i++)
if(_plugins[i].defined&&_plugins[i].enabled)
_plugins[i]._run_plugin(&W);
}
/*:359*//*371:*/
#line 8286 "cweb/weaver.w"

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
/*:371*/
#line 2050 "cweb/weaver.w"

}

void _render(void){

glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
/*490:*/
#line 10923 "cweb/weaver.w"

if(_final_shader[_number_of_loops]!=W_NONE){
glBindFramebuffer(GL_FRAMEBUFFER,_framebuffer);
glViewport(0,0,W.width,W.height);
glEnable(GL_DEPTH_TEST);
glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
/*:490*/
#line 2056 "cweb/weaver.w"


/*476:*/
#line 10590 "cweb/weaver.w"

{

glBindBuffer(GL_ARRAY_BUFFER,_interface_VBO);


int last_type;
int i;
bool first_element= true;
float time_float;
struct _shader*current_shader;



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
/*661:*/
#line 14826 "cweb/weaver.w"

else if(_interface_queue[_number_of_loops][i]->type==W_INTERFACE_IMAGE){
current_shader= &_image_interface_shader;
}
/*:661*/
#line 10626 "cweb/weaver.w"

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
/*662:*/
#line 14835 "cweb/weaver.w"


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
/*:662*/
#line 10649 "cweb/weaver.w"


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
/*:476*/
#line 2058 "cweb/weaver.w"

/*496:*/
#line 11061 "cweb/weaver.w"

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
/*508:*/
#line 11261 "cweb/weaver.w"

glUniform1i(current_shader->_uniform_integer,
W.final_shader_integer);
/*:508*/
#line 11092 "cweb/weaver.w"

glDrawArrays(GL_TRIANGLE_FAN,0,4);
glDisableVertexAttribArray(current_shader->_attribute_vertex_position);
}
/*:496*/
#line 2059 "cweb/weaver.w"

#if W_TARGET == W_ELF
glXSwapBuffers(_dpy,_window);
#else
glFlush();
#endif
}
/*:72*/
