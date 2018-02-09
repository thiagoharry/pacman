/*91:*/
#line 2396 "cweb/weaver.w"

#include "memory.h"
/*:91*//*98:*/
#line 2616 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 4
void _assert__arena_header(struct _arena_header*header){


if(header->total<header->max_used){
fprintf(stderr,
"ERROR (4): MEMORY: Arena header used more memory than allowed!\n");
exit(1);
}



if(header->max_used<header->used){
fprintf(stderr,
"ERROR (4): MEMORY: Arena header not registering max usage!\n");
exit(1);
}



if((void*)header->last_element<(void*)header->last_breakpoint){
fprintf(stderr,
"ERROR (4): MEMORY: Arena header storing in wrong location!\n");
exit(1);
}


if(!(header->empty_position==NULL||
(void*)header->empty_position> (void*)header->last_element)){
fprintf(stderr,
"ERROR (4): MEMORY: Arena header confused about empty position!\n");
exit(1);
}


if(header->used<=0){
fprintf(stderr,
"ERROR (4): MEMORY: Arena header not occupying space!\n");
exit(1);
}
}
#endif
/*:98*//*99:*/
#line 2666 "cweb/weaver.w"

static bool _initialize_arena_header(struct _arena_header*header,
size_t total
#if W_DEBUG_LEVEL >= 1
,char*filename,unsigned long line
#endif
){
header->total= total;
header->used= sizeof(struct _arena_header)-sizeof(struct _breakpoint);
header->last_breakpoint= (struct _breakpoint*)(header+1);
header->last_element= (void*)header->last_breakpoint;
header->empty_position= (void*)(header->last_breakpoint+1);
#ifdef W_MULTITHREAD
if(pthread_mutex_init(&(header->mutex),NULL)!=0){
return false;
}
#endif
#if W_DEBUG_LEVEL >= 1
header->line= line;
memcpy(header->file,filename,32);
header->file[31]= '\0';
#endif
#if W_DEBUG_LEVEL >= 3
header->max_used= header->used;
#endif
#if W_DEBUG_LEVEL >= 4
_assert__arena_header(header);
#endif
return true;
}
/*:99*//*104:*/
#line 2795 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _assert__breakpoint(struct _breakpoint*breakpoint){
if(breakpoint->type!=_BREAKPOINT_T){
fprintf(stderr,
"ERROR (1): Probable buffer overflow. We can't guarantee a "
"reliable error message in this case. But the "
"data where the buffer overflow happened may be "
"the place allocated at %s:%lu or before.\n",
((struct _breakpoint*)
breakpoint->last_element)->file,
((struct _breakpoint*)
breakpoint->last_element)->line);
exit(1);
}
#if W_DEBUG_LEVEL >= 4
if((void*)breakpoint->last_breakpoint> 
(void*)breakpoint->last_element){
fprintf(stderr,"ERROR (4): MEMORY: Breakpoint's previous breakpoint "
"found after breakpoint's last element.\n");
exit(1);
}
#endif
}
#endif
/*:104*//*105:*/
#line 2827 "cweb/weaver.w"

static void _initialize_breakpoint(struct _breakpoint*self,
void*last_element,
struct _arena_header*arena,
struct _breakpoint*last_breakpoint
#if W_DEBUG_LEVEL >= 1
,char*file,unsigned long line
#endif
){
self->type= _BREAKPOINT_T;
self->last_element= last_element;
self->arena= arena;
self->last_breakpoint= last_breakpoint;
self->size= arena->used-sizeof(struct _breakpoint);
#if W_DEBUG_LEVEL >= 1
memcpy(self->file,file,32);
self->file[31]= '\0';
self->line= line;
_assert__breakpoint(self);
#endif
}
/*:105*//*106:*/
#line 2860 "cweb/weaver.w"

static void _initialize_first_breakpoint(struct _breakpoint*self,
struct _arena_header*arena){
#if W_DEBUG_LEVEL >= 1
_initialize_breakpoint(self,self,arena,self,"",0);
#else
_initialize_breakpoint(self,self,arena,self);
#endif
}
/*:106*//*111:*/
#line 2946 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _assert__memory_header(struct _memory_header*mem){
if(mem->type!=_DATA_T){
fprintf(stderr,
"ERROR (1): Probable buffer overflow. We can't guarantee a "
"reliable error message in this case. But the "
"data where the buffer overflow happened may be "
"the place allocated at %s:%lu or before.\n",
((struct _memory_header*)
mem->last_element)->file,
((struct _memory_header*)
mem->last_element)->line);
exit(1);
}
#if W_DEBUG_LEVEL >= 4
if(mem->real_size<mem->requested_size){
fprintf(stderr,
"ERROR (4): MEMORY: Allocated less memory than requested in "
"data allocated in %s:%lu.\n",mem->file,mem->line);
exit(1);
}
#endif
}
#endif
/*:111*//*115:*/
#line 3010 "cweb/weaver.w"



#if W_DEBUG_LEVEL >= 1
void*_Wcreate_arena(size_t size,char*filename,unsigned long line){
#else
void*_Wcreate_arena(size_t size){
#endif
void*arena;
size_t real_size= 0;
struct _breakpoint*breakpoint;

long page_size= sysconf(_SC_PAGESIZE);
real_size= ((int)ceil((double)size/(double)page_size))*page_size;
arena= mmap(0,real_size,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,
-1,0);
if(arena==MAP_FAILED)
arena= NULL;
if(arena!=NULL){
if(!_initialize_arena_header((struct _arena_header*)arena,real_size
#if W_DEBUG_LEVEL >= 1
,filename,line
#endif
)){


munmap(arena,((struct _arena_header*)arena)->total);


return NULL;
}

breakpoint= ((struct _arena_header*)arena)->last_breakpoint;
_initialize_first_breakpoint(breakpoint,(struct _arena_header*)arena);
#if W_DEBUG_LEVEL >= 4
_assert__arena_header(arena);
#endif
}
return arena;
}
/*:115*//*121:*/
#line 3131 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _assert_no_memory_leak(void*arena){
struct _arena_header*header= (struct _arena_header*)arena;

struct _memory_header*p= (struct _memory_header*)header->last_element;



while(p->type!=_BREAKPOINT_T||
((struct _breakpoint*)p)->last_breakpoint!=
(struct _breakpoint*)p){
if(p->type==_DATA_T&&p->flags%2){
fprintf(stderr,"WARNING (1): Memory leak in data allocated in %s:%lu\n",
p->file,p->line);
}
p= (struct _memory_header*)p->last_element;
}
}
#endif
/*:121*//*123:*/
#line 3167 "cweb/weaver.w"

int Wdestroy_arena(void*arena){
#if W_DEBUG_LEVEL >= 1
_assert_no_memory_leak(arena);
#endif
#if W_DEBUG_LEVEL >= 4
_assert__arena_header(arena);
#endif
#if W_DEBUG_LEVEL >= 3
fprintf(stderr,
"WARNING (3): Max memory used in arena %s:%lu: %lu/%lu\n",
((struct _arena_header*)arena)->file,
((struct _arena_header*)arena)->line,
(unsigned long)((struct _arena_header*)arena)->max_used,
(unsigned long)((struct _arena_header*)arena)->total);
#endif
#ifdef W_MULTITHREAD
{
struct _arena_header*header= (struct _arena_header*)arena;
if(pthread_mutex_destroy(&(header->mutex))!=0)
return 0;
}
#endif

if(munmap(arena,((struct _arena_header*)arena)->total)==-1)
arena= NULL;
if(arena==NULL)return 0;
else return 1;
}
/*:123*//*128:*/
#line 3258 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void*_alloc(void*arena,size_t size,char*filename,unsigned long line){
#else
void*_alloc(void*arena,size_t size){
#endif
struct _arena_header*header= arena;
struct _memory_header*mem_header;
void*mem= NULL,*old_last_element;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(header->mutex));
#endif
#if W_DEBUG_LEVEL >= 4
_assert__arena_header(arena);
#endif
mem_header= header->empty_position;
old_last_element= header->last_element;

size_t real_size= (size_t)(ceil((float)size/(float)sizeof(long))*
sizeof(long));
if(header->used+real_size+sizeof(struct _memory_header)> 
header->total){

#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): No memory enough to allocate in %s:%lu.\n",
filename,line);
#endif
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(header->mutex));
#endif
return NULL;
}

header->used+= real_size+sizeof(struct _memory_header);
mem= (void*)((char*)header->empty_position+
sizeof(struct _memory_header));
header->last_element= header->empty_position;
header->empty_position= (void*)((char*)mem+real_size);
#if W_DEBUG_LEVEL >= 3

if(header->used> header->max_used)
header->max_used= header->used;
#endif



mem_header->type= _DATA_T;
mem_header->last_element= old_last_element;
mem_header->real_size= real_size;
mem_header->requested_size= size;
mem_header->flags= 0x1;
mem_header->arena= arena;
#if W_DEBUG_LEVEL >= 1
memcpy(mem_header->file,filename,32);
mem_header->file[31]= '\0';
mem_header->line= line;
_assert__memory_header(mem_header);
#endif
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(header->mutex));
#endif
return mem;
}
/*:128*//*132:*/
#line 3375 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 2 && !defined(W_MULTITHREAD)
void _free(void*mem,char*filename,unsigned long line){
#else
void _free(void*mem){
#endif
struct _memory_header*mem_header= ((struct _memory_header*)mem)-1;
struct _arena_header*arena= mem_header->arena;
void*last_freed_element;
size_t memory_freed= 0;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(arena->mutex));
#endif
#if W_DEBUG_LEVEL >= 4
_assert__arena_header(arena);
#endif
#if W_DEBUG_LEVEL >= 1
_assert__memory_header(mem_header);
#endif



if((struct _memory_header*)arena->last_element!=mem_header){
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(arena->mutex));
#endif
mem_header->flags= 0x0;
#if W_DEBUG_LEVEL >= 2 && !defined(W_MULTITHREAD)


fprintf(stderr,
"WARNING (2): %s:%lu: Memory allocated in %s:%lu should be"
" freed first to prevent fragmentation.\n",filename,line,
((struct _memory_header*)(arena->last_element))->file,
((struct _memory_header*)(arena->last_element))->line);
#endif
return;
}


memory_freed= mem_header->real_size+sizeof(struct _memory_header);
last_freed_element= mem_header;
mem_header= mem_header->last_element;


while(mem_header->type!=_BREAKPOINT_T&&mem_header->flags==0x0){
memory_freed+= mem_header->real_size+sizeof(struct _memory_header);
last_freed_element= mem_header;
mem_header= mem_header->last_element;
}


arena->last_element= mem_header;
arena->empty_position= last_freed_element;
arena->used-= memory_freed;
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(arena->mutex));
#endif
}
/*:132*//*138:*/
#line 3483 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
int _new_breakpoint(void*arena,char*filename,unsigned long line){
#else
int _new_breakpoint(void*arena){
#endif
struct _arena_header*header= (struct _arena_header*)arena;
struct _breakpoint*breakpoint,*old_breakpoint;
void*old_last_element;
size_t old_size;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(header->mutex));
#endif
#if W_DEBUG_LEVEL >= 4
_assert__arena_header(arena);
#endif
if(header->used+sizeof(struct _breakpoint)> header->total){

#if W_DEBUG_LEVEL >= 1
fprintf(stderr,"WARNING (1): No memory enough to allocate in %s:%lu.\n",
filename,line);
#endif
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(header->mutex));
#endif
return 0;
}

old_breakpoint= header->last_breakpoint;
old_last_element= header->last_element;
old_size= header->used;
header->used+= sizeof(struct _breakpoint);
breakpoint= (struct _breakpoint*)header->empty_position;
header->last_breakpoint= breakpoint;
header->empty_position= ((struct _breakpoint*)header->empty_position)+
1;
header->last_element= header->last_breakpoint;
#if W_DEBUG_LEVEL >= 3
if(header->used> header->max_used){
header->max_used= header->used;
}
#endif
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(header->mutex));
#endif
breakpoint->type= _BREAKPOINT_T;
breakpoint->last_element= old_last_element;
breakpoint->arena= arena;
breakpoint->last_breakpoint= (void*)old_breakpoint;
breakpoint->size= old_size;
#if W_DEBUG_LEVEL >= 1
memcpy(breakpoint->file,filename,32);
breakpoint->file[31]= '\0';
breakpoint->line= line;
#endif
#if W_DEBUG_LEVEL >= 4
_assert__breakpoint(breakpoint);
#endif
return 1;
}
/*:138*//*142:*/
#line 3575 "cweb/weaver.w"

void Wtrash_arena(void*arena){
struct _arena_header*header= (struct _arena_header*)arena;
struct _breakpoint*previous_breakpoint= 
((struct _breakpoint*)header->last_breakpoint)->last_breakpoint;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&(header->mutex));
#endif
#if W_DEBUG_LEVEL >= 4
_assert__arena_header(arena);
_assert__breakpoint(header->last_breakpoint);
#endif
if(header->last_breakpoint==previous_breakpoint){

header->last_element= previous_breakpoint;
header->empty_position= (void*)(previous_breakpoint+1);
header->used= previous_breakpoint->size+sizeof(struct _breakpoint);
}
else{

struct _breakpoint*last= (struct _breakpoint*)header->last_breakpoint;
header->used= last->size;
header->empty_position= last;
header->last_element= last->last_element;
header->last_breakpoint= previous_breakpoint;
}
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&(header->mutex));
#endif
}
/*:142*//*146:*/
#line 3637 "cweb/weaver.w"

static void*_user_arena,*_internal_arena;
/*:146*//*148:*/
#line 3654 "cweb/weaver.w"

void _initialize_memory(void){
_user_arena= Wcreate_arena(W_MAX_MEMORY);
if(_user_arena==NULL){
fprintf(stderr,"ERROR: This system have no enough memory to "
"run this program.\n");
exit(1);
}
_internal_arena= Wcreate_arena(W_INTERNAL_MEMORY);
if(_internal_arena==NULL){
fprintf(stderr,"ERROR: This system have no enough memory to "
"run this program.\n");
exit(1);
}
}
void _finalize_memory(){
Wdestroy_arena(_user_arena);
Wtrash_arena(_internal_arena);
Wdestroy_arena(_internal_arena);
}


/*:148*//*152:*/
#line 3722 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void*_Walloc(size_t size,char*filename,unsigned long line){
return _alloc(_user_arena,size,filename,line);
}
void*_Winternal_alloc(size_t size,char*filename,unsigned long line){
return _alloc(_internal_arena,size,filename,line);
}
#else
void*_Walloc(size_t size){
return _alloc(_user_arena,size);
}
void*_Winternal_alloc(size_t size){
return _alloc(_internal_arena,size);
}
#endif
/*:152*//*157:*/
#line 3784 "cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
int _Wbreakpoint(char*filename,unsigned long line){
return _new_breakpoint(_user_arena,filename,line);
}
int _iWbreakpoint_(char*filename,unsigned long line){
return _new_breakpoint(_internal_arena,filename,line);
}
#else
int _Wbreakpoint(void){
return _new_breakpoint(_user_arena);
}
int _iWbreakpoint_(void){
return _new_breakpoint(_internal_arena);
}
#endif
void _Wtrash(void){
Wtrash_arena(_user_arena);
}
void _iWtrash(void){
Wtrash_arena(_internal_arena);
}
/*:157*/
