/*186:*/
#line 4546 "cweb/weaver.w"

#ifdef W_MULTITHREAD
#include <pthread.h> 
#endif
#include <stdint.h> 
#include <stdbool.h> 
#include <sys/timeb.h>  
#include <string.h>  
#include "numeric.h"
#if W_TARGET == W_ELF
#include <unistd.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#endif
/*189:*/
#line 4611 "cweb/weaver.w"


static uint32_t _sfmt_sequence[624];

static int _sfmt_index;
#if defined(W_MULTITHREAD)

static pthread_mutex_t _sfmt_mutex;
#endif
/*:189*/
#line 4560 "cweb/weaver.w"

/*201:*/
#line 4829 "cweb/weaver.w"

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
/*:201*/
#line 4561 "cweb/weaver.w"

/*191:*/
#line 4628 "cweb/weaver.w"

void _initialize_numeric_functions(void){
uint32_t seed;
/*194:*/
#line 4653 "cweb/weaver.w"

#if defined(W_MULTITHREAD)
if(pthread_mutex_init(&_sfmt_mutex,NULL)!=0){
fprintf(stderr,"ERROR (0): Can't initialize mutex for random numbers.\n");
exit(1);
}
#endif
/*:194*//*196:*/
#line 4677 "cweb/weaver.w"

{
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
struct timeb t;
ftime(&t);
seed= (uint32_t)t.time+(uint32_t)(t.millitm<<2);
}
#else
{
struct timeb t;
ftime(&t);
seed= (uint32_t)t.time+(uint32_t)(t.millitm<<2);
}
#endif

_sfmt_sequence[0]= seed;
#else
_sfmt_sequence[0]= seed= (uint32_t)W_SEED;
#endif
}
/*:196*//*197:*/
#line 4720 "cweb/weaver.w"

{
int i;
for(i= 1;i<624;i++){
_sfmt_sequence[i]= 1812433253ul*
(_sfmt_sequence[i-1]^(_sfmt_sequence[i-1]>>30))+i;
}


_sfmt_index= 0;
}
/*:197*//*198:*/
#line 4764 "cweb/weaver.w"

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
/*:198*//*204:*/
#line 4924 "cweb/weaver.w"

{
_sfmt_index= -1;
_regenerate_sequence();
}
/*:204*/
#line 4631 "cweb/weaver.w"

}
void _finalize_numeric_functions(void){
/*195:*/
#line 4662 "cweb/weaver.w"

#if defined(W_MULTITHREAD)
pthread_mutex_destroy(&_sfmt_mutex);
#endif
/*:195*/
#line 4634 "cweb/weaver.w"

}
/*:191*//*200:*/
#line 4787 "cweb/weaver.w"

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
/*:200*/
#line 4562 "cweb/weaver.w"

/*:186*/
