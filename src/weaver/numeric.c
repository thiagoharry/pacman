/*188:*/
#line 4586 "./cweb/weaver.w"

#ifdef W_MULTITHREAD
#include <pthread.h> 
#endif
#include <stdint.h> 
#include <stdbool.h> 
#include <sys/time.h>  
#include <string.h>  
#include "numeric.h"
#if W_TARGET == W_ELF
#include <unistd.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#endif
static bool initialized= false;
/*191:*/
#line 4652 "./cweb/weaver.w"


static uint32_t _sfmt_sequence[624];

static int _sfmt_index;
#if defined(W_MULTITHREAD)

static pthread_mutex_t _sfmt_mutex;
#endif
/*:191*//*199:*/
#line 4758 "./cweb/weaver.w"

static bool use_runtime_seed= false;
static unsigned int runtime_seed= 0;
/*:199*/
#line 4601 "./cweb/weaver.w"

/*204:*/
#line 4884 "./cweb/weaver.w"

static void _regenerate_sequence(void){
int i;


uint32_t r1[4],r2[4];





memcpy(r2,&(_sfmt_sequence[620]),16);
memcpy(r1,&(_sfmt_sequence[616]),16);

for(i= 0;i<156;i++){



uint32_t x[4],y[4];
uint64_t n1,n2,aux1,aux2;
int j;
n1= ((uint64_t)_sfmt_sequence[i*4+3]<<32)|
((uint64_t)_sfmt_sequence[i*4+2]);
n2= ((uint64_t)_sfmt_sequence[i*4+1]<<32)|
((uint64_t)_sfmt_sequence[i*4]);
aux1= n1<<8;
aux2= n2<<8;
aux1|= n2>>56;
x[1]= (uint32_t)(aux2>>32);
x[0]= (uint32_t)aux2;
x[3]= (uint32_t)(aux1>>32);
x[2]= (uint32_t)aux1;


n1= ((uint64_t)r1[3]<<32)|((uint64_t)r1[2]);
n2= ((uint64_t)r1[1]<<32)|((uint64_t)r1[0]);
aux1= n1>>8;
aux2= n2>>8;
aux2|= n1<<56;
y[1]= (uint32_t)(aux2>>32);
y[0]= (uint32_t)aux2;
y[3]= (uint32_t)(aux1>>32);
y[2]= (uint32_t)aux1;

if(i<34)
j= i+122;
else
j= i-34;


_sfmt_sequence[i*4]= _sfmt_sequence[i*4]^x[0]^
((_sfmt_sequence[j*4]>>11)&3758096367ul)^y[0]^(r2[0]<<18);
_sfmt_sequence[i*4+1]= _sfmt_sequence[i*4+1]^x[1]^
((_sfmt_sequence[4*j+1]>>11)&3724462975ul)^y[1]^(r2[1]<<18);
_sfmt_sequence[i*4+2]= _sfmt_sequence[i*4+2]^x[2]^
((_sfmt_sequence[4*j+2]>>11)&3220897791ul)^y[2]^(r2[2]<<18);
_sfmt_sequence[i*4+3]= _sfmt_sequence[i*4+3]^x[3]^
((_sfmt_sequence[4*j+3]>>11)&3221225462ul)^y[3]^(r2[3]<<18);

memcpy(r1,r2,16);
memcpy(r2,&(_sfmt_sequence[4*i]),16);
}
}
/*:204*/
#line 4602 "./cweb/weaver.w"

/*193:*/
#line 4669 "./cweb/weaver.w"

void _initialize_numeric_functions(void){
uint32_t seed;
/*196:*/
#line 4695 "./cweb/weaver.w"

#if defined(W_MULTITHREAD)
if(!initialized&&pthread_mutex_init(&_sfmt_mutex,NULL)!=0){
fprintf(stderr,"ERROR (0): Can't initialize mutex for random numbers.\n");
exit(1);
}
#endif
/*:196*//*198:*/
#line 4722 "./cweb/weaver.w"

if(use_runtime_seed)
seed= runtime_seed;
else{
#ifndef W_SEED
#if W_TARGET == W_ELF
bool got_seed= false;
int file= open("/dev/urandom",O_RDONLY);
if(file!=-1){
if(read(file,&seed,sizeof(seed))!=-1)
got_seed= true;
close(file);
}
if(!got_seed){
struct timeval t;
gettimeofday(&t,NULL);
seed= (uint32_t)t.tv_usec+(uint32_t)(t.tv_sec<<9);
}
#else
{
struct timeval t;
gettimeofday(&t,NULL);
seed= (uint32_t)t.tv_usec+(uint32_t)(t.tv_sec<<9);
}
#endif

_sfmt_sequence[0]= seed;
#else
_sfmt_sequence[0]= seed= (uint32_t)W_SEED;
#endif
}
/*:198*//*200:*/
#line 4775 "./cweb/weaver.w"

{
int i;
for(i= 1;i<624;i++){
_sfmt_sequence[i]= 1812433253ul*
(_sfmt_sequence[i-1]^(_sfmt_sequence[i-1]>>30))+i;
}


_sfmt_index= 0;
}
/*:200*//*201:*/
#line 4819 "./cweb/weaver.w"

{
int i;
uint32_t r= (1&_sfmt_sequence[0])^(331998852ul&_sfmt_sequence[3]);
for(i= 16;i>=1;i/= 2)
r^= (r>>i);
if(!(r%2)){

if(_sfmt_sequence[0]%2)
_sfmt_sequence[0]--;
else
_sfmt_sequence[0]++;
}
}
/*:201*//*207:*/
#line 4979 "./cweb/weaver.w"

{
_sfmt_index= -1;
_regenerate_sequence();
initialized= true;
}
/*:207*/
#line 4672 "./cweb/weaver.w"

}
void _finalize_numeric_functions(void){
/*197:*/
#line 4704 "./cweb/weaver.w"

#if defined(W_MULTITHREAD)
pthread_mutex_destroy(&_sfmt_mutex);
#endif
/*:197*/
#line 4675 "./cweb/weaver.w"

}
/*:193*//*203:*/
#line 4842 "./cweb/weaver.w"

unsigned long _random(void){
unsigned long number;
#ifdef W_MULTITHREAD
pthread_mutex_lock(&_sfmt_mutex);
#endif
if(_sfmt_index<623){
_sfmt_index++;
}
else{
_sfmt_index= 0;

_regenerate_sequence();
}
number= (unsigned long)_sfmt_sequence[_sfmt_index];
#ifdef W_MULTITHREAD
pthread_mutex_unlock(&_sfmt_mutex);
#endif
return number;
}
/*:203*//*209:*/
#line 4995 "./cweb/weaver.w"

void _set_random_number_seed(unsigned int seed){
use_runtime_seed= true;
runtime_seed= seed;
_initialize_numeric_functions();
}
/*:209*/
#line 4603 "./cweb/weaver.w"

/*:188*/
