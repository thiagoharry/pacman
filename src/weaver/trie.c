/*214:*/
#line 5094 "./cweb/weaver.w"

#include "trie.h"
#include <stdarg.h>  

/*222:*/
#line 5244 "./cweb/weaver.w"

struct _trie*_new_node(void*arena,char*string,struct _trie*parent){
int i,size= strlen(string);
struct _trie*ret;
ret= (struct _trie*)Walloc_arena(arena,sizeof(struct _trie));
if(ret==NULL)
goto no_memory_error;
ret->string= (char*)Walloc_arena(arena,size+1);
if(ret->string==NULL)
goto no_memory_error;
memcpy(ret->string,string,size+1);
ret->leaf= false;
for(i= 0;i<256;i++)
ret->child[i]= NULL;
ret->parent= parent;
return ret;
no_memory_error:
fprintf(stderr,"ERROR (0): No memory enough. Please increase the value of "
"%s at conf/conf.h.\n",(arena==_user_arena)?"W_MAX_MEMORY":
"W_INTERNAL_MEMORY");
return NULL;
}
/*:222*//*223:*/
#line 5289 "./cweb/weaver.w"







void _split_trie(void*arena,struct _trie**origin,char*C,
char*D){
struct _trie*node_C,*node_D;
int i;
bool BC_was_a_leaf= (*origin)->leaf;
(*origin)->leaf= (*C=='\0')&&BC_was_a_leaf;
if(*C!='\0'){
node_C= _new_node(arena,C,*origin);

for(i= 0;i<256;i++){
node_C->child[i]= (*origin)->child[i];
(*origin)->child[i]= NULL;
}
node_C->leaf= BC_was_a_leaf;
if(BC_was_a_leaf)
node_C->value= (*origin)->value;
(*origin)->child[(int)*C]= node_C;
*C= '\0';
}
if(*D!='\0'){
node_D= _new_node(arena,D,*origin);
node_D->leaf= true;
(*origin)->child[(int)*D]= node_D;
*origin= node_D;
}
}
/*:223*/
#line 5098 "./cweb/weaver.w"

/*219:*/
#line 5158 "./cweb/weaver.w"

struct _trie*_new_trie(void*arena){
int i;
struct _trie*ret;
ret= (struct _trie*)Walloc_arena(arena,sizeof(struct _trie));
if(ret==NULL)
goto no_memory_error;
ret->string= (char*)Walloc(1);
if(ret->string==NULL)
goto no_memory_error;
ret->string[0]= '\0';
ret->leaf= false;
for(i= 0;i<256;i++)
ret->child[i]= NULL;
ret->parent= NULL;
return ret;
no_memory_error:
fprintf(stderr,"ERROR: No memory enough. Please increase the value of "
"%s at conf/conf.h.\n",(arena==_user_arena)?"W_MAX_MEMORY":
"W_INTERNAL_MEMORY");
return NULL;
}
/*:219*//*221:*/
#line 5192 "./cweb/weaver.w"

void _insert_trie(struct _trie*tree,void*arena,int type,char*name,...){
va_list arguments;
va_start(arguments,name);
struct _trie*current_prefix= tree;
char*match= name,*p= current_prefix->string;
while(*match!='\0'||*p!='\0'){
if(*p=='\0'){

if(current_prefix->child[(int)*match]!=NULL){
current_prefix= current_prefix->child[(int)*match];
p= current_prefix->string;
}
else{

current_prefix->child[(int)*match]= 
_new_node(arena,match,current_prefix);
current_prefix= current_prefix->child[(int)*match];
break;
}
}
else if(*p!=*match){

_split_trie(arena,&current_prefix,p,match);
break;
}
else{

p++;
match++;
}
}

current_prefix->leaf= true;
switch(type){
case INT:
current_prefix->value.integer= va_arg(arguments,int);
break;
case DOUBLE:
current_prefix->value.real= va_arg(arguments,double);
break;
default:
current_prefix->value.generic= va_arg(arguments,void*);
break;
}
}
/*:221*//*225:*/
#line 5337 "./cweb/weaver.w"

bool _search_trie(struct _trie*tree,int type,char*name,...){
va_list arguments;
va_start(arguments,name);
struct _trie*current_prefix= tree;
char*match= name,*p= current_prefix->string;
while(*match!='\0'||*p!='\0'||!(current_prefix->leaf)){
if(*p=='\0'){

if(current_prefix->child[(int)*match]!=NULL){
current_prefix= current_prefix->child[(int)*match];
p= current_prefix->string;
continue;
}
else
return false;
}
else if(*p==*match){
p++;
match++;
}
else{
return false;
}
}
switch(type){
int*ret;
double*ret2;
void**ret3;
case INT:
ret= va_arg(arguments,int*);
*ret= current_prefix->value.integer;
return true;
case DOUBLE:
ret2= va_arg(arguments,double*);
*ret2= current_prefix->value.real;
return true;
default:
ret3= va_arg(arguments,void**);
*ret3= current_prefix->value.generic;
return true;
}
}
/*:225*//*227:*/
#line 5410 "./cweb/weaver.w"

void _remove_trie(struct _trie*tree,char*name){
struct _trie*current_prefix= tree;
char*match= name,*p= current_prefix->string;
while(*match!='\0'){
if(*p=='\0'){

if(current_prefix->child[(int)*match]!=NULL){
current_prefix= current_prefix->child[(int)*match];
p= current_prefix->string;
}
else
return;
}
else if(*p==*match){
p++;
match++;
}
else
return;
}
current_prefix->leaf= false;
}
/*:227*//*229:*/
#line 5446 "./cweb/weaver.w"

void _map_trie(void(*f)(void*),struct _trie*tree){
int i;
if(tree->leaf)
f(tree->value.generic);
for(i= 0;i<256;i++)
if(tree->child[i]!=NULL)
_map_trie(f,tree->child[i]);
}
/*:229*//*231:*/
#line 5468 "./cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _debug_trie_values(char*prefix,struct _trie*tree){
int i;
char buffer[1024];
strncpy(buffer,prefix,1024);
strncat(buffer,tree->string,1024-strlen(prefix));
if(tree->leaf){
bool error;
int dummy;
struct _trie*parent= tree;
while(parent->parent!=NULL)
parent= parent->parent;
error= !_search_trie(parent,INT,buffer,&dummy);

printf(" '");
if(error)
printf("\033[0;31m");
printf("%s",buffer);
if(error)
printf("\033[0m");
printf("'");
}
for(i= 0;i<256;i++)
if(tree->child[i]!=NULL){
_debug_trie_values(buffer,tree->child[i]);
}
}
#endif
/*:231*/
#line 5099 "./cweb/weaver.w"

/*:214*/
