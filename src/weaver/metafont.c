/*818:*/
#line 18128 "./cweb/weaver.w"

#include "weaver.h"
#include <stdarg.h> 
/*872:*/
#line 19420 "./cweb/weaver.w"

#define HINT_ENDGROUP      1
/*:872*//*878:*/
#line 19525 "./cweb/weaver.w"

#define NOT_DECLARED -1
#define BOOLEAN       0
#define PATH          1


#define PEN           4
#define PICTURE       5
#define TRANSFORM     6
#define PAIR          7
/*:878*//*916:*/
#line 20589 "./cweb/weaver.w"

#define MACRO 8
/*:916*//*933:*/
#line 21318 "./cweb/weaver.w"

#define INTERNAL 9
/*:933*/
#line 18131 "./cweb/weaver.w"

/*821:*/
#line 18147 "./cweb/weaver.w"


/*:821*//*825:*/
#line 18183 "./cweb/weaver.w"

static void*metafont_arena;
/*:825*//*830:*/
#line 18240 "./cweb/weaver.w"

struct metafont{
char filename[256];
FILE*fp;
char buffer[4096];
int buffer_position;
int line;
bool error;
struct metafont*parent,*child;
/*845:*/
#line 18798 "./cweb/weaver.w"

struct token*pending_tokens,*past_tokens;
/*:845*//*854:*/
#line 19021 "./cweb/weaver.w"

struct _trie*internal_quantities;
/*:854*//*860:*/
#line 19233 "./cweb/weaver.w"

char*everyjob_token_name;
/*:860*//*869:*/
#line 19402 "./cweb/weaver.w"

int hint;
/*:869*//*879:*/
#line 19543 "./cweb/weaver.w"

struct _trie*variable_types;
/*:879*//*881:*/
#line 19560 "./cweb/weaver.w"

struct _trie*vars[8];
/*:881*//*887:*/
#line 19696 "./cweb/weaver.w"

struct _trie*delimiters;
/*:887*//*892:*/
#line 19784 "./cweb/weaver.w"

struct _trie*outer_tokens;
/*:892*//*896:*/
#line 19910 "./cweb/weaver.w"

struct _trie*def,*vardef,*primarydef,*secondarydef,*tertiarydef;
/*:896*//*907:*/
#line 20274 "./cweb/weaver.w"

struct _trie*macros;
/*:907*/
#line 18249 "./cweb/weaver.w"

};
/*:830*//*839:*/
#line 18500 "./cweb/weaver.w"


#define NUMERIC 1
#define STRING  2
#define SYMBOL  3
struct token{
int type;
float value;
char*name;
/*936:*/
#line 21523 "./cweb/weaver.w"

bool deterministic;
int known;
/*:936*//*958:*/
#line 21967 "./cweb/weaver.w"

float value2;
/*:958*/
#line 18509 "./cweb/weaver.w"

struct token*prev,*next;
};
/*:839*//*899:*/
#line 19967 "./cweb/weaver.w"

static struct _trie*primitive_sparks;
/*:899*//*905:*/
#line 20249 "./cweb/weaver.w"


#define PRIMARY    4
#define SECONDARY  5
#define TERTIARY   7
#define EXPR       8
#define SUFFIX     9
#define TEXT      10
/*:905*//*906:*/
#line 20264 "./cweb/weaver.w"

struct macro{
struct token*parameters;
struct token*replacement_text;
};
/*:906*//*910:*/
#line 20361 "./cweb/weaver.w"

#define UNDELIMITED_EXPR   11
#define UNDELIMITED_SUFFIX 12
#define UNDELIMITED_TEXT   13
/*:910*//*917:*/
#line 20601 "./cweb/weaver.w"


#define VARDEF_ARG 11
/*:917*//*930:*/
#line 21120 "./cweb/weaver.w"

struct string_variable{
char*name;
bool deterministic;
struct string_variable*prev,*next;
};
/*:930*//*957:*/
#line 21958 "./cweb/weaver.w"



/*:957*/
#line 18132 "./cweb/weaver.w"

/*884:*/
#line 19609 "./cweb/weaver.w"

struct metafont*get_scope(struct metafont*mf,char*type_name);
/*:884*//*925:*/
#line 20949 "./cweb/weaver.w"

static struct token*eval(struct metafont**,struct token**);
static struct token*eval_string(struct metafont**,struct token**);
static struct token*eval_numeric(struct metafont**,struct token**);
/*:925*//*971:*/
#line 22287 "./cweb/weaver.w"

static void expand_macro(struct metafont*,struct macro*,struct token**);
/*:971*//*981:*/
#line 22769 "./cweb/weaver.w"

static void remove_string_variable_from_equalty_list(void*string_var);
/*:981*//*983:*/
#line 22789 "./cweb/weaver.w"

static void end_scope(struct metafont*mf);
/*:983*/
#line 18133 "./cweb/weaver.w"

/*822:*/
#line 18151 "./cweb/weaver.w"


/*:822*//*834:*/
#line 18333 "./cweb/weaver.w"

char read_char(struct metafont*mf){
char ret;
size_t size;
while(mf->parent!=NULL)
mf= mf->parent;
ret= mf->buffer[mf->buffer_position];
if(ret!='\0'){
mf->buffer_position++;
}
else if(mf->fp!=NULL){
size= fread(mf->buffer,1,4095,mf->fp);
mf->buffer[size]= '\0';
if(size!=4095){
fclose(mf->fp);
mf->fp= NULL;
}
mf->buffer_position= 0;
ret= mf->buffer[mf->buffer_position];
if(ret!='\0')
mf->buffer_position++;
}
else
return'\0';

if(ret=='\n')
mf->line++;
return ret;
}
/*:834*//*835:*/
#line 18368 "./cweb/weaver.w"

char peek_char(struct metafont*mf){
char ret;
size_t size;
while(mf->parent!=NULL)
mf= mf->parent;
ret= mf->buffer[mf->buffer_position];
if(ret=='\0'){
if(mf->fp!=NULL){
size= fread(mf->buffer,1,4095,mf->fp);
mf->buffer[size]= '\0';
if(size!=4095){
fclose(mf->fp);
mf->fp= NULL;
}
mf->buffer_position= 0;
ret= mf->buffer[mf->buffer_position];
}
else
return'\0';
}
return ret;
}
/*:835*//*836:*/
#line 18398 "./cweb/weaver.w"

void mf_error(struct metafont*mf,char*message,...){
va_list args;
va_start(args,message);
while(mf->parent!=NULL)
mf= mf->parent;
if(!mf->error){
fprintf(stderr,"ERROR: Metafont: %s:%d: ",
mf->filename,mf->line);
vfprintf(stderr,message,args);
fprintf(stderr,"\n");
mf->error= true;

if(mf->fp!=NULL){
fclose(mf->fp);
mf->fp= NULL;
}
mf->buffer_position= 0;
mf->buffer[mf->buffer_position]= '\0';
}
va_end(args);
}
/*:836*//*837:*/
#line 18425 "./cweb/weaver.w"

void mf_end(struct metafont*mf){

while(mf->parent!=NULL)
mf= mf->parent;
if(mf->fp!=NULL){
fclose(mf->fp);
mf->fp= NULL;
}
mf->buffer_position= 0;
mf->buffer[mf->buffer_position]= '\0';
/*867:*/
#line 19368 "./cweb/weaver.w"

if(mf->parent!=NULL){
mf_error(mf,"A group begun and never ended.");
}
/*:867*/
#line 18436 "./cweb/weaver.w"

}
/*:837*//*840:*/
#line 18525 "./cweb/weaver.w"

static struct token*new_token(int type,float value,char*name,
void*memory_arena){
struct token*token;
token= (struct token*)Walloc_arena(memory_arena,sizeof(struct token));
if(token==NULL)
goto error_no_memory;
token->type= type;
token->value= value;
if(name!=NULL){
size_t name_size;
name_size= strlen(name)+1;
token->name= Walloc_arena(memory_arena,name_size);
if(token->name==NULL)
goto error_no_memory;
memcpy(token->name,name,name_size);
}
else
token->name= name;
token->prev= token->next= NULL;
/*937:*/
#line 21531 "./cweb/weaver.w"

token->deterministic= true;
token->known= 0;
/*:937*//*959:*/
#line 21973 "./cweb/weaver.w"

token->value2= 0.0;
/*:959*/
#line 18545 "./cweb/weaver.w"

return token;
error_no_memory:
fprintf(stderr,"ERROR (0): Not enough memory to parse METAFONT "
"source. Please, increase the value of %s "
"at conf/conf.h.\n",
(memory_arena==_user_arena)?"W_MAX_MEMORY":
"W_INTERNAL_MEMORY");
return NULL;
}
#define new_token_number(a) new_token(NUMERIC, a, NULL, _internal_arena)
#define new_token_string(a) new_token(STRING, 0.0, a, _internal_arena)
#define new_token_symbol(a) new_token(SYMBOL, 0.0, a, _internal_arena)
/*:840*//*841:*/
#line 18563 "./cweb/weaver.w"

#ifdef W_DEBUG_METAFONT
void debug_token_list(struct token*list){
struct token*tok= list;
while(tok!=NULL){
switch(tok->type){
case SYMBOL:
printf(" [%s] ",tok->name);
break;
case STRING:
printf(" \"%s\" ",tok->name);
break;
case NUMERIC:
printf(" %f ",tok->value);
break;
}

if(tok->next!=NULL)
if(tok->next->prev!=tok)
printf("\033[0;31mX\033[0m");
tok= tok->next;
}
}
#endif
/*:841*//*842:*/
#line 18595 "./cweb/weaver.w"

static struct token*next_token(struct metafont*mf){
char buffer[512];
int buffer_position= 0,number_of_dots= 0;
char current_char,next_char;
char family[56];
bool valid_char;
start:
current_char= read_char(mf);
switch(current_char){
case'\0':return NULL;
case' ':case'\n':goto start;
case'.':
next_char= peek_char(mf);
if(next_char=='.'){
memcpy(family,".",2);
break;
}
else if(isdigit(next_char))
goto numeric;
else
goto start;
case'%':
while(current_char!='\n'&&current_char!='\0')
current_char= read_char(mf);
goto start;
case'0':case'1':case'2':case'3':case'4':case'5':
case'6':case'7':case'8':case'9':
{
numeric:
for(;;){
buffer[buffer_position]= current_char;
buffer_position= (buffer_position+1)%512;
if(current_char=='.')
number_of_dots++;
next_char= peek_char(mf);
if((next_char=='.'&&number_of_dots==1)||
(next_char!='.'&&!isdigit(next_char))){
buffer[buffer_position]= '\0';
return new_token_number(atof(buffer));
}
current_char= read_char(mf);
}
}
case'"':
current_char= read_char(mf);
while(current_char!='"'&&current_char!='\0'){
if(current_char=='\n'){
mf_error(mf,"Incomplete string. "
"Strings should finish on the same line"
" as they began.");
return NULL;
}
buffer[buffer_position]= current_char;
buffer_position= (buffer_position+1)%512;
current_char= read_char(mf);
}
buffer[buffer_position]= '\0';
return new_token_string(buffer);
case'(':case')':case',':case';':
buffer[buffer_position]= current_char;
buffer_position= (buffer_position+1)%512;
buffer[buffer_position]= '\0';
return new_token_symbol(buffer);
case'a':case'b':case'c':case'd':case'e':case'f':case'g':
case'h':case'i':case'j':case'k':case'l':case'm':case'n':
case'o':case'p':case'q':case'r':case's':case't':case'u':
case'v':case'w':case'x':case'y':case'z':case'_':case'A':
case'B':case'C':case'D':case'E':case'F':case'G':case'H':
case'I':case'J':case'K':case'L':case'M':case'N':case'O':
case'P':case'Q':case'R':case'S':case'T':case'U':case'V':
case'W':case'X':case'Y':case'Z':
memcpy(family,"abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ",54);
break;
case'<':case'=':case'>':case':':case'|':
memcpy(family,"<=>:|",6);
break;
case'`':case'\'':
memcpy(family,"`'",3);
break;
case'+':case'-':
memcpy(family,"+-",3);
break;
case'/':case'*':case'\\':
memcpy(family,"/*\\",4);
break;
case'!':case'?':
memcpy(family,"!?",3);
break;
case'#':case'&':case'@':case'$':
memcpy(family,"#&@$",5);
break;
case'[':
memcpy(family,"[",2);
break;
case']':
memcpy(family,"]",2);
break;
case'{':case'}':
memcpy(family,"{}",3);
break;
case'~':case'^':
memcpy(family,"~^",3);
break;
default:
mf_error(mf,"Text line contains an invalid character.");
return NULL;
}

do{
char*c= family;
buffer[buffer_position]= current_char;
buffer_position= (buffer_position+1)%512;
next_char= peek_char(mf);
valid_char= false;
while(*c!='\0'){
if(*c==next_char){
valid_char= true;
current_char= read_char(mf);
break;
}
c++;
}
}while(valid_char);
buffer[buffer_position]= '\0';
return new_token_symbol(buffer);
}
/*:842*//*843:*/
#line 18730 "./cweb/weaver.w"


static void concat_token(struct token**before,struct token*after){
struct token*head= *before;
if(*before==NULL){
*before= after;
return;
}
if(after==NULL)
return;
while(head->next!=NULL)
head= head->next;
head->next= after;
after->prev= head;
}
/*:843*//*847:*/
#line 18814 "./cweb/weaver.w"

/*851:*/
#line 18986 "./cweb/weaver.w"

bool expand_token(struct metafont*mf,struct token**first_token){

(void)mf;
(void)first_token;
return false;
}
/*:851*/
#line 18815 "./cweb/weaver.w"

static struct token*get_token(struct metafont*mf){
struct token*first_token= NULL;

if(mf->pending_tokens){
first_token= mf->pending_tokens;
first_token->next->prev= NULL;
mf->pending_tokens= first_token->next;
first_token->next= NULL;
}
else{
first_token= next_token(mf);
}
while(expand_token(mf,&first_token));
return first_token;
}
/*:847*//*848:*/
#line 18835 "./cweb/weaver.w"

static struct token*get_statement(struct metafont*mf){
struct token*first_token,*current_token;
first_token= get_token(mf);
current_token= first_token;

if(current_token==NULL||
(current_token->type==SYMBOL&&
(!strcmp(current_token->name,"end")||
!strcmp(current_token->name,"dump")))){
mf_end(mf);
return NULL;
}

while(1){
if(current_token->type==SYMBOL&&
!strcmp(current_token->name,";"))
break;

if(current_token->next==NULL){
if(mf->pending_tokens==NULL){
current_token->next= next_token(mf);
}
else{
current_token->next= mf->pending_tokens;
mf->pending_tokens= mf->pending_tokens->next;
current_token->next->next= NULL;
}
}
if(current_token->next==NULL)
goto source_incomplete_or_with_error;
current_token->next->prev= current_token;
current_token= current_token->next;
while(expand_token(mf,&current_token));
}

if(current_token->next!=NULL){
current_token->next->prev= NULL;
if(mf->pending_tokens==NULL)
mf->pending_tokens= current_token->next;
else
concat_token(&(mf->pending_tokens),current_token->next);
}
current_token->next= NULL;
/*876:*/
#line 19489 "./cweb/weaver.w"

{
struct token*aux= current_token;
while(aux!=NULL){
if(aux->type==SYMBOL&&
!strcmp(aux->name,"endgroup")){
mf->hint= HINT_ENDGROUP;
}
else if(aux->type==SYMBOL&&
!strcmp(aux->name,"begingroup")){
mf->hint= 0;
}
aux= aux->prev;
}
}
/*:876*/
#line 18879 "./cweb/weaver.w"

return first_token;
source_incomplete_or_with_error:
mf_error(mf,"Source with error or incomplete, aborting.");
printf("current: %s first: %s\n",current_token->name,first_token->name);
return NULL;
}
/*:848*//*857:*/
#line 19161 "./cweb/weaver.w"

static struct token*symbolic_token_list(struct metafont*mf,
struct token**token){
struct token*first_token= *token,*current_token;
current_token= first_token;
while(1){

if(current_token==NULL||current_token->type!=SYMBOL){
mf_error(mf,"Missing symbolic token.");
return NULL;
}


if(current_token->next==NULL||
current_token->next->type!=SYMBOL||
strcmp(current_token->next->name,",")){
*token= current_token->next;
current_token->next= NULL;
return first_token;
}


if(current_token->next->next!=NULL)
current_token->next->next->prev= current_token;
current_token->next= current_token->next->next;
current_token= current_token->next;
}
}
/*:857*//*885:*/
#line 19619 "./cweb/weaver.w"

struct metafont*get_scope(struct metafont*mf,char*type_name){

bool got_prefix= false;
char*p= type_name;
struct metafont*scope= mf,*last_scope= mf;
void*dummy_result;
while(*p!='\0'){
if(*p==' '){
got_prefix= true;
*p= '\0';
break;
}
p++;
}
if(got_prefix){
while(scope!=NULL){
if(_search_trie(scope->variable_types,VOID_P,
type_name,&dummy_result)){
*p= ' ';
return scope;
}
scope= scope->parent;
}
}

if(got_prefix){
*p= ' ';
scope= mf;
}
while(scope!=NULL){
if(_search_trie(scope->variable_types,VOID_P,
type_name,&dummy_result)){
return scope;
}
last_scope= scope;
scope= scope->parent;
}
return last_scope;
}
/*:885*//*890:*/
#line 19750 "./cweb/weaver.w"

static char*delimiter(struct metafont*mf,struct token*tok){
char*result= NULL;
while(mf!=NULL){
bool ret= _search_trie(mf->delimiters,VOID_P,tok->name,
(void*)&result);
if(ret){
return result;
}
mf= mf->parent;
}
return NULL;
}
/*:890*//*898:*/
#line 19924 "./cweb/weaver.w"

static bool is_tag(struct metafont*mf,struct token*token){
struct metafont*scope= mf;
void*dummy;
if(token==NULL)
return false;
while(scope!=NULL){
if(_search_trie(scope->variable_types,VOID_P,
token->name,&dummy))
return true;
if(_search_trie(scope->internal_quantities,VOID_P,
token->name,&dummy))
return true;
if(_search_trie(scope->def,VOID_P,
token->name,&dummy))
return false;
if(_search_trie(scope->vardef,VOID_P,
token->name,&dummy))
return true;
if(_search_trie(scope->primarydef,VOID_P,
token->name,&dummy))
return false;
if(_search_trie(scope->secondarydef,VOID_P,
token->name,&dummy))
return false;
if(_search_trie(scope->tertiarydef,VOID_P,
token->name,&dummy))
return false;
scope= scope->parent;
}
if(_search_trie(primitive_sparks,VOID_P,
token->name,&dummy))
return false;
else
return true;
}
/*:898*//*901:*/
#line 20017 "./cweb/weaver.w"

void declared_variable(struct metafont*mf,struct token**token,
char*dst,size_t dst_size){
struct token*first_token= *token,*current_token;
size_t dst_length= 0,aux_length;

if(first_token==NULL||first_token->type!=SYMBOL){
mf_error(mf,"Missing symbolic token.");
return;
}
current_token= first_token->next;
dst_length= strlen(first_token->name);
if(dst_length+1> dst_size){
mf_error(mf,"Token too big: %s\n",first_token->name);
return;
}
memcpy(dst,first_token->name,dst_length+1);
while(current_token!=NULL){

if(current_token->type!=SYMBOL||
(!is_tag(mf,current_token)&&
(strcmp(current_token->name,"[")&&
strcmp(current_token->name,"]")))){

current_token= current_token->prev;
break;
}

if(!strcmp(current_token->name,"[")&&
(current_token->next==NULL||
strcmp(current_token->next->name,"]"))){
mf_error(mf,"Illegal suffix at token declaration.");
current_token= current_token->prev;
break;
}

if(!strcmp(current_token->name,"]")&&
(current_token->prev==NULL||
strcmp(current_token->prev->name,"["))){
mf_error(mf,"Illegal suffix at token declaration.");
current_token= current_token->prev;
break;
}

aux_length= strlen(current_token->name);
if(dst_length+aux_length+2> dst_size){
mf_error(mf,"Token too big: %s %s\n",dst,current_token->name);
return;
}
memcpy(&dst[dst_length]," ",2);
dst_length++;
memcpy(&dst[dst_length],current_token->name,aux_length+1);
dst_length+= aux_length;
current_token= current_token->next;
}
if(current_token==NULL&&first_token->prev==NULL)
*token= NULL;
else if(current_token==NULL){
first_token->prev->next= NULL;
*token= NULL;
}
else if(first_token->prev==NULL){
*token= current_token->next;
current_token->prev= NULL;
}
else{
*token= current_token->next;
current_token->next->prev= first_token->prev;
first_token->prev->next= current_token->next;
}
return;
}
/*:901*//*909:*/
#line 20295 "./cweb/weaver.w"

static struct token*delimited_parameters(struct metafont*mf,
struct token**token,
void*arena){
struct token*tok= *token,*parameter_list;
struct token*result= NULL,*last_result= NULL;
int type= NOT_DECLARED;

while(tok!=NULL&&tok->type==SYMBOL&&!strcmp(tok->name,"(")){
tok= tok->next;
if(tok==NULL||tok->type!=SYMBOL){
mf_error(mf,"Missing symbolic token.");
return NULL;
}
if(!strcmp(tok->name,"expr"))
type= EXPR;
else if(!strcmp(tok->name,"suffix"))
type= SUFFIX;
else if(!strcmp(tok->name,"text"))
type= TEXT;
else{
mf_error(mf,"Missing paramaeter type.");
return NULL;
}
tok= tok->next;
parameter_list= symbolic_token_list(mf,&tok);
while(parameter_list!=NULL){
char*name= (char*)
Walloc_arena(arena,
strlen(parameter_list->name)+1);
if(name==NULL)goto error_no_memory;
if(last_result!=NULL){
last_result->next= new_token(type,0.0,name,arena);
if(last_result->next==NULL)
return NULL;
last_result->next->prev= last_result;
last_result= last_result->next;
last_result->next= NULL;
}
else{
result= new_token(type,0.0,name,arena);
if(result==NULL)
return NULL;
last_result= result;
}
parameter_list= parameter_list->next;
}
if(tok==NULL||tok->type!=SYMBOL||strcmp(tok->name,")")){
mf_error(mf,"Missing ')' closing parameters.");
return NULL;
}
tok= tok->next;
}
*token= tok;
return result;
error_no_memory:
fprintf(stderr,"ERROR: Not enough memory. Please, increase"
" the value of W_%s_MEMORY at conf/conf.h\n",
(arena==_user_arena)?"MAX":"INTERNAL");
return NULL;
}
/*:909*//*911:*/
#line 20367 "./cweb/weaver.w"

static struct token*undelimited_parameters(struct metafont*mf,
struct token**token,
void*arena){
struct token*tok= *token;
int type= NOT_DECLARED;
if(tok!=NULL&&tok->type==SYMBOL){
if(!strcmp(tok->name,"primary"))
type= PRIMARY;
else if(!strcmp(tok->name,"secondary"))
type= SECONDARY;
else if(!strcmp(tok->name,"tertiary"))
type= TERTIARY;
else if(!strcmp(tok->name,"tertiary"))
type= TERTIARY;
else if(!strcmp(tok->name,"expr"))
type= UNDELIMITED_EXPR;
else if(!strcmp(tok->name,"suffix"))
type= UNDELIMITED_SUFFIX;
else if(!strcmp(tok->name,"text"))
type= UNDELIMITED_TEXT;
else return NULL;
}
tok= tok->next;
if(tok==NULL){
mf_error(mf,"Missing symbolic token.");
return NULL;
}
*token= tok->next;
return new_token(type,0.0,tok->name,arena);
}
/*:911*//*912:*/
#line 20421 "./cweb/weaver.w"

static struct token*replacement_text(struct metafont*mf,struct token**token,
void*arena){
struct token*tok= *token,*result= NULL,*current_token= NULL;
int depth= 0,dummy;
for(;;){
if(tok==NULL||(depth<=0&&tok->type==SYMBOL&&
!strcmp(tok->name,"enddef")))
break;

if(tok->type==SYMBOL&&_search_trie(mf->outer_tokens,INT,
tok->name,&dummy)){
mf_error(mf,"Forbidden token at macro.");
return NULL;
}


if(tok->type==SYMBOL&&!strcmp(tok->name,"quote")){
if(tok->next==NULL){
mf_error(mf,"Missing token after 'quote'.");
return NULL;
}
current_token= current_token->next;
}

else if(tok->type==SYMBOL&&
(!strcmp(tok->name,"def")||!strcmp(tok->name,"vardef")||
!strcmp(tok->name,"primarydef")||
!strcmp(tok->name,"secondarydef")||
!strcmp(tok->name,"tertiarydef")))
depth++;
else if(tok->type==SYMBOL&&!strcmp(tok->name,"enddef"))
depth--;

if(result!=NULL){
current_token->next= new_token(tok->type,tok->value,
tok->name,arena);
if(current_token->next==NULL)
goto end_of_function;
current_token->next->prev= current_token;
current_token= current_token->next;
}
else{
result= new_token(tok->type,tok->value,tok->name,arena);
if(result==NULL)
goto end_of_function;
current_token= result;
}
if(tok->next==NULL){
tok->next= get_statement(mf);
if(tok->next!=NULL){
tok->next->prev= tok;
}
}
tok= tok->next;
}
end_of_function:
if(tok!=NULL){
if(tok->next==NULL){
tok->next= get_statement(mf);
if(tok->next!=NULL)
tok->next->prev= tok;
}
*token= tok->next;
}
else
*token= tok;

mf->hint= 0;
return result;
}
/*:912*//*931:*/
#line 21142 "./cweb/weaver.w"

void new_defined_string_variable(char*var_name,char*type_name,
struct token*string_token,
struct metafont*mf,bool overwrite){
struct metafont*scope= mf;
int current_type= -1;
void*current_arena;
struct string_variable*new_variable= NULL;
scope= get_scope(mf,type_name);
_search_trie(scope->variable_types,INT,type_name,&current_type);

switch(current_type){
case BOOLEAN:
mf_error(mf,"Equation cannot be performed (boolean=string).");
return;
case PATH:
mf_error(mf,"Equation cannot be performed (path=string).");
return;
case STRING:

break;
case PEN:
mf_error(mf,"Equation cannot be performed (pen=string).");
return;
case PICTURE:
mf_error(mf,"Equation cannot be performed (picture=string).");
return;
case TRANSFORM:
mf_error(mf,"Equation cannot be performed (transform=string).");
return;
case PAIR:
mf_error(mf,"Equation cannot be performed (pair=string).");
return;
default:
mf_error(mf,"Equation cannot be performed (numeric=string).");
return;
}

if(scope->parent==NULL)
current_arena= _user_arena;
else
current_arena= metafont_arena;

_search_trie(scope->vars[STRING],VOID_P,var_name,
(void*)&new_variable);
if(new_variable==NULL){

size_t name_size;
new_variable= (struct string_variable*)
Walloc_arena(current_arena,
sizeof(struct string_variable));
if(new_variable==NULL)
goto error_no_memory;
new_variable->name= 
(char*)Walloc_arena(current_arena,
name_size= strlen(string_token->name)+1);
if(new_variable->name==NULL)
goto error_no_memory;
memcpy(new_variable->name,string_token->name,name_size);
new_variable->deterministic= string_token->deterministic;
new_variable->prev= new_variable->next= NULL;
_insert_trie(scope->vars[STRING],current_arena,VOID_P,
var_name,(void*)new_variable);
return;
}
else{

if(overwrite){
size_t name_size;
if(new_variable->prev!=NULL)
new_variable->prev->next= new_variable->next;
if(new_variable->next!=NULL)
new_variable->next->prev= new_variable->prev;
new_variable->name= (char*)
Walloc_arena(current_arena,
name_size= strlen(string_token->name)+1);
if(new_variable->name==NULL)
goto error_no_memory;
memcpy(new_variable->name,string_token->name,name_size);
new_variable->deterministic= string_token->deterministic;
new_variable->prev= new_variable->next= NULL;
}
else{

if(new_variable->prev==NULL&&new_variable->next==NULL){
if(!strcmp(new_variable->name,string_token->name)){
mf_error(mf,"Redundant equation (%s=%s).",
new_variable->name,string_token->name);
return;
}
else{
mf_error(mf,"Inconsistent equation (%s=%s).",
new_variable->name,string_token->name);
return;
}
}


while(new_variable->prev!=NULL)
new_variable= new_variable->prev;
while(new_variable!=NULL){
size_t name_size;
struct string_variable*next_var;

new_variable->prev= NULL;

new_variable->name= 
(char*)
Walloc_arena(current_arena,
name_size= strlen(string_token->name)+1);
if(new_variable->name==NULL)
goto error_no_memory;
memcpy(new_variable->name,string_token->name,name_size);
new_variable->deterministic= string_token->deterministic;

next_var= new_variable->next;
new_variable->next= NULL;
new_variable= next_var;
}
}
}
return;
error_no_memory:
fprintf(stderr,"ERROR: Not enough memory. Please, increase "
"the value of W_%s_MEMORY at conf/conf.h.\n",
(current_arena==_user_arena)?"MAX":"INTERNAL");
exit(1);
}
/*:931*//*934:*/
#line 21322 "./cweb/weaver.w"

void variable(struct metafont**mf,struct token**token,
char*dst,int dst_size,
char*type_name,int*type,bool consume){
struct metafont*scope= *mf;
struct token*previous_token;
struct token*old_token= *token;

bool internal= false,vardef= false;
float dummy;
struct macro*mc= NULL;
dst[0]= '\0';
type_name[0]= '\0';
int type_size= dst_size;
int pos= 0,type_pos= 0,original_size= dst_size;
if(*token==NULL||(*token)->type!=SYMBOL)
return;
previous_token= (*token)->prev;

while(scope->parent!=NULL){
internal= _search_trie(scope->internal_quantities,DOUBLE,
(*token)->name,&dummy);
if(internal)
break;
scope= scope->parent;
}
if(!internal)
internal= _search_trie(scope->internal_quantities,DOUBLE,
(*token)->name,&dummy);
if(internal){
strncpy(dst,(*token)->name,dst_size);
if((*token)->prev!=NULL)
(*token)->prev->next= (*token)->next;
if((*token)->next!=NULL)
(*token)->next->prev= (*token)->prev;
*token= (*token)->next;
(*token)->prev= previous_token;
*type= INTERNAL;
strncpy(type_name,dst,type_size);
goto restore_token_if_not_consume_and_exit;
}

if(!is_tag(*mf,*token)){
*type= NOT_DECLARED;
return;
}
while(*token!=NULL){
{
scope= *mf;
while(scope!=NULL){
vardef= _search_trie(scope->vardef,VOID_P,
type_name,(void*)&mc);
if(vardef){


if((*token)->prev->prev!=NULL){
(*token)->prev->prev->next= (*token)->next;
(*token)->prev= (*token)->prev->prev;
}
if((*token)->next!=NULL)
(*token)->next->prev= (*token)->prev;
*type= MACRO;
expand_macro(*mf,mc,token);



eval(mf,token);
(*token)->prev= previous_token;
return;
}
scope= scope->parent;
}
}


if((*token)->type==SYMBOL&&
(!is_tag(*mf,*token)&&
strcmp((*token)->name,"[")&&
strcmp((*token)->name,"]"))){
break;
}


if((*token)->type==SYMBOL&&!strcmp((*token)->name,"[")){
struct token*result;
*token= (*token)->next;
result= eval_numeric(mf,token);
if(result==NULL||result->type!=NUMERIC){
mf_error(*mf,"Undefined numeric expression after '['.");
*type= NOT_DECLARED;
(*token)->prev= previous_token;
return;
}
if(*token==NULL||(*token)->type!=SYMBOL||
strcmp((*token)->name,"]")){
mf_error(*mf,"Missing ']' after '[' in variable name.");
*type= NOT_DECLARED;
(*token)->prev= previous_token;
return;
}
*token= (*token)->next;

snprintf(&(dst[pos]),dst_size,"%f ",result->value);
pos= strlen(dst);
dst_size= original_size-pos;
strncat(type_name," [ ]",type_size-type_pos);
type_pos= type_pos+4;
continue;
}

if((*token)->type==SYMBOL){
int size= strlen((*token)->name);
if(dst[0]!='\0'){
strncat(dst," ",dst_size);
strncat(type_name," ",type_size-type_pos);
type_pos++;
pos++;
dst_size-= 1;
}
strncat(dst,(*token)->name,dst_size);
pos+= size;
dst_size-= pos;
strncat(type_name,(*token)->name,type_size-type_pos);
type_pos+= size;
*token= (*token)->next;
continue;
}

if((*token)->type==NUMERIC){
snprintf(&(dst[pos]),dst_size,"%f",(*token)->value);
pos= strlen(dst);
dst_size= original_size-pos;
strncat(type_name," [ ]",type_size-type_pos);
type_pos+= 4;
*token= (*token)->next;
continue;
}


break;
}

*type= NUMERIC;
scope= get_scope(*mf,type_name);
_search_trie(scope->variable_types,INT,type_name,type);

if(dst_size> 0)
dst[pos]= '\0';
else
dst[original_size-1]= '\0';
if(*token!=NULL)
(*token)->prev= previous_token;
restore_token_if_not_consume_and_exit:
if(!consume&&old_token!=*token){

if(*token!=NULL&&(*token)->prev!=NULL)
(*token)->prev->next= old_token;
while(old_token->next!=(*token)&&
old_token->next!=NULL&&
old_token->next!=old_token){
old_token= old_token->next;
}
if(*token!=NULL)
(*token)->prev= old_token;
old_token->next= *token;
}
return;
}
/*:934*//*935:*/
#line 21497 "./cweb/weaver.w"

struct token*read_var(char*var_name,char*type_name,struct metafont*mf){
struct metafont*scope= mf;
struct token*ret= NULL;
int current_type= -1;
struct string_variable*var= NULL;
scope= get_scope(mf,type_name);
_search_trie(scope->variable_types,INT,type_name,&current_type);
if(current_type!=-1){
_search_trie(scope->vars[current_type],VOID_P,var_name,
(void*)&var);
if(var==NULL)
return NULL;
if(var->prev!=NULL||var->next!=NULL)
return NULL;
ret= new_token_string(var->name);
ret->deterministic= var->deterministic;
return ret;
}
return NULL;
}
/*:935*//*951:*/
#line 21774 "./cweb/weaver.w"

void number2utf8(uint32_t number,char*result){
int endian_probe_x= 1;
char*number_probe= (char*)&number;
char*little_endian= (char*)&endian_probe_x;
if(number<=127){
result[0]= (char)number;
result[1]= '\0';
return;
}
if(number<=2047){
if(*little_endian){
result[0]= number_probe[1];
result[1]= number_probe[0];
result[2]= '\0';
return;
}
else{
result[0]= number_probe[2];
result[1]= number_probe[3];
result[2]= '\0';
return;
}
}
if(number<=65535){
if(*little_endian){
result[0]= number_probe[2];
result[1]= number_probe[1];
result[2]= number_probe[0];
result[3]= '\0';
return;
}
else{
result[0]= number_probe[1];
result[1]= number_probe[2];
result[2]= number_probe[3];
result[3]= '\0';
return;
}
}
if(*little_endian){
result[0]= number_probe[3];
result[1]= number_probe[2];
result[2]= number_probe[1];
result[3]= number_probe[0];
result[4]= '\0';
return;
}
else{
result[0]= number_probe[0];
result[1]= number_probe[1];
result[2]= number_probe[2];
result[3]= number_probe[3];
result[4]= '\0';
return;
}
}
/*:951*//*952:*/
#line 21840 "./cweb/weaver.w"

struct token*numeric_primary(struct metafont**mf,struct token**token){
struct token*result;
if(token==NULL){
mf_error(*mf,"ERROR: Missing numeric primary.");
return NULL;
}
if((*token)->type==NUMERIC){
result= new_token_number((*token)->value);
result->next= (*token)->next;
if((*token)->next!=NULL)
(*token)->next->prev= result;
result->prev= (*token)->prev;
if((*token)->prev!=NULL)
(*token)->prev->next= result;
*token= (*token)->next;
return result;
}
mf_error(*mf,"ERROR: Unknown numeric primary.");
return NULL;
}
/*:952*//*960:*/
#line 21989 "./cweb/weaver.w"

struct token*pair_primary(struct metafont**mf,struct token**token){
struct token*result,*tok= *token;
bool deterministic= true;
if(tok==NULL){
mf_error(*mf,"ERROR: Missing pair primary.");
return NULL;
}
if(tok->type==SYMBOL&&!strcmp(tok->name,"(")&&
tok->next!=NULL&&(tok->next->type!=SYMBOL||
strcmp(tok->next->name,"("))){
struct token*n1,*n2;
tok= tok->next;
if(tok==NULL){
mf_error(*mf,"Missing numeric expression.");
return NULL;
}
n1= eval_numeric(mf,&tok);
if(n1==NULL)
return NULL;
deterministic= deterministic&&n1->deterministic;
if(n1->type!=NUMERIC){
mf_error(*mf,"Unknown numeric expression result.");
return NULL;
}
if(tok==NULL||tok->type!=SYMBOL||strcmp(tok->name,",")){
mf_error(*mf,"Missing ',' at pair.");
return NULL;
}
tok= tok->next;
if(tok==NULL){
mf_error(*mf,"Missing numeric expression.");
return NULL;
}
n2= eval_numeric(mf,&tok);
if(n2==NULL)
return NULL;
deterministic= deterministic&&n2->deterministic;
if(n2->type!=NUMERIC){
mf_error(*mf,"Unknown numeric expression result.");
return NULL;
}
if(tok==NULL||tok->type!=SYMBOL||strcmp(tok->name,")")){
mf_error(*mf,"Missing ')' at pair.");
return NULL;
}
result= new_token_number(n1->value);
result->type= PAIR;
result->deterministic= deterministic;
result->value2= n2->value;
if((*token)->prev!=NULL)
(*token)->prev->next= tok->next;
if(tok->next!=NULL)
tok->next->prev= (*token)->prev;
*token= tok->next;
return result;
}
mf_error(*mf,"ERROR: Unknown numeric primary.");
return NULL;
}
/*:960*//*972:*/
#line 22290 "./cweb/weaver.w"

static void expand_macro(struct metafont*mf,struct macro*mc,
struct token**tok){
struct token*expansion= NULL,*current_token= NULL,*replacement;
replacement= mc->replacement_text;
{
struct token*arg= mc->parameters;
while(arg!=NULL){
/*975:*/
#line 22419 "./cweb/weaver.w"

{

arg->prev= NULL;
if(arg->type==VARDEF_ARG){
while(*tok!=NULL&&is_tag(mf,*tok)){
struct token*next_token= (*tok)->next;
if((*tok)->prev!=NULL)
(*tok)->prev->next= (*tok)->next;
if((*tok)->next!=NULL)
(*tok)->next->prev= (*tok)->prev;
(*tok)->prev= (*tok)->next= NULL;
concat_token(&(arg->prev),*tok);
*tok= next_token;
}
}
/*986:*/
#line 22833 "./cweb/weaver.w"

else if(arg->type==EXPR){
struct token*begin_delim,*end_delim;
struct token*next_token= (*tok)->next;
bool last_arg= (arg->next==NULL);
char*delim;
int number_of_delimiters= 0;

begin_delim= *tok;
delim= delimiter(mf,begin_delim);
if(delim==NULL){
mf_error(mf,"Missing argument.");
return;
}

number_of_delimiters++;
end_delim= begin_delim->next;
while(end_delim!=NULL){
if(end_delim->type==SYMBOL&&!strcmp(end_delim->name,delim))
break;
end_delim= end_delim->next;
}
if(end_delim==NULL||end_delim==begin_delim->next){
mf_error(mf,"Missing or invalid argument.");
return;
}
end_delim->prev->next= NULL;
next_token= begin_delim->next;
arg->prev= eval(&mf,&next_token);
end_delim->prev->next= end_delim;
if(!last_arg&&next_token->next!=NULL&&
next_token->next->type==SYMBOL&&
!strcmp(next_token->next->name,",")){

begin_delim->next= next_token->next->next;
next_token->next->next->prev= begin_delim;
*tok= begin_delim;
}
else{
*tok= end_delim->next;
begin_delim->prev->next= end_delim->next;
end_delim->next->prev= begin_delim->prev;
}
}
/*:986*/
#line 22435 "./cweb/weaver.w"

}
/*:975*/
#line 22298 "./cweb/weaver.w"

arg= arg->next;
}
}
while(replacement!=NULL){
/*973:*/
#line 22355 "./cweb/weaver.w"

{
struct token*arg= mc->parameters;
while(arg!=NULL){
if(replacement->type==SYMBOL&&!strcmp(replacement->name,
arg->name)){

arg= arg->prev;
while(arg!=NULL){
if(current_token!=NULL){
current_token->next= new_token(arg->type,
arg->value,
arg->name,
_internal_arena);
if(current_token->next==NULL)
goto error_no_memory;
current_token->next->prev= current_token;
current_token= current_token->next;
}
else{
current_token= new_token(arg->type,
arg->value,
arg->name,
_internal_arena);
if(current_token==NULL)
goto error_no_memory;
expansion= current_token;
expansion->prev= (*tok)->prev;
expansion->prev->next= expansion;
}
arg= arg->next;
}
replacement= replacement->next;
break;
}
arg= arg->next;
}
if(arg!=NULL)
continue;
}
/*:973*/
#line 22303 "./cweb/weaver.w"

if(current_token!=NULL){
current_token->next= new_token(replacement->type,
replacement->value,
replacement->name,
_internal_arena);
if(current_token->next==NULL)
goto error_no_memory;
current_token->next->prev= current_token;
current_token= current_token->next;
}
else{
current_token= new_token(replacement->type,
replacement->value,
replacement->name,
_internal_arena);
if(current_token==NULL)
goto error_no_memory;
expansion= current_token;
if(*tok!=NULL)
current_token->prev= (*tok)->prev;
if(expansion->prev!=NULL)
expansion->prev->next= expansion;
}
replacement= replacement->next;
}
if(current_token!=NULL){
current_token->next= *tok;
if(*tok!=NULL){
(*tok)->prev= current_token;
}
if(expansion!=NULL)
*tok= expansion;
}
/*974:*/
#line 22400 "./cweb/weaver.w"

{
struct token*arg= mc->parameters;
if(arg!=NULL){
arg->prev= NULL;
while(arg->next!=NULL){
arg->next->prev= arg;
arg= arg->next;
}
}
}
/*:974*/
#line 22337 "./cweb/weaver.w"

return;
error_no_memory:
fprintf(stderr,"ERROR: Not enough memory. Please increase the value of "
"W_INTERNAL_MEMORY at conf/conf.h.\n");
exit(1);
}
/*:972*//*979:*/
#line 22500 "./cweb/weaver.w"

static void equal_variables(struct metafont*mf,char*name1,char*name2,
char*declared1,char*declared2,
bool overwrite){
struct string_variable*var1= NULL,*var2= NULL;
void*arena1= _user_arena,*arena2= _user_arena;
int var1_type= NOT_DECLARED,var2_type= NOT_DECLARED;
struct metafont*scope1,*scope2;
char types[8][10]= {"boolean","path","string","numeric","pen",
"picture","transform","pair"};
scope1= get_scope(mf,declared1);
if(scope1->parent!=NULL)
arena1= metafont_arena;
_search_trie(scope1->variable_types,INT,declared1,&var1_type);
if(var1_type==NOT_DECLARED)
var1_type= NUMERIC;
scope2= get_scope(mf,declared2);
if(scope2->parent!=NULL)
arena2= metafont_arena;
_search_trie(scope2->variable_types,INT,declared2,&var2_type);
if(var2_type==NOT_DECLARED)
var2_type= NUMERIC;
if(var1_type!=var2_type){
mf_error(mf,"Equation cannot be performed (%s=%s).",types[var1_type],
types[var2_type]);
return;
}

if(var1_type==STRING){
_search_trie(scope1->vars[STRING],VOID_P,name1,(void*)&var1);
_search_trie(scope2->vars[STRING],VOID_P,name2,(void*)&var2);

if(var2==NULL){
var2= (struct string_variable*)
Walloc_arena(arena2,sizeof(struct string_variable));
var2->name= NULL;
var2->deterministic= true;
var2->prev= var2->next= NULL;
_insert_trie(scope2->vars[STRING],arena2,VOID_P,name2,
(void*)var2);
}
if(var1==NULL){

var1= (struct string_variable*)
Walloc_arena(arena1,sizeof(struct string_variable));
var1->name= NULL;
var1->deterministic= var2->deterministic;
var1->prev= NULL;
var1->next= var2;
if(var2->prev!=NULL)
var2->prev->next= var1;
var2->prev= var1;
_insert_trie(scope1->vars[STRING],arena1,VOID_P,name1,
(void*)var1);
return;
}
else if(var1!=NULL){






if(var1->prev!=NULL||var1->next!=NULL){


while(var1->next!=NULL)
var1= var1->next;
while(var2->prev!=NULL)
var2= var2->prev;
var1->next= var2;
var2->prev= var1;

return;
}
else{
if(overwrite){


var1->next= var2;
var1->prev= var2->prev;
if(var2->prev!=NULL)
var2->prev->next= var1;
var2->prev= var1;
return;
}
else{
int tamanho= strlen(var1->name);


while(var2->prev!=NULL)
var2= var2->prev;
while(var2!=NULL){
struct string_variable*next_var;
var2->prev= NULL;
var2->name= (char*)Walloc_arena(arena2,
tamanho+1);
memcpy(var1->name,var2->name,tamanho+1);
next_var= var2->next;
var2->next= NULL;
var2= next_var;
}
return;
}
}
}
}
}
/*:979*//*982:*/
#line 22773 "./cweb/weaver.w"

static void remove_string_variable_from_equalty_list(void*string_var){
struct string_variable*string= (struct string_variable*)string_var;
if(string->prev!=NULL)
string->prev->next= string->next;
if(string->next!=NULL)
string->next->prev= string->prev;
}
/*:982*//*984:*/
#line 22793 "./cweb/weaver.w"

static void end_scope(struct metafont*mf){
_map_trie(remove_string_variable_from_equalty_list,mf->vars[STRING]);
}
/*:984*/
#line 18134 "./cweb/weaver.w"

/*926:*/
#line 20955 "./cweb/weaver.w"

/*932:*/
#line 21294 "./cweb/weaver.w"

static struct token*eval_numeric(struct metafont**mf,
struct token**expression){
(void)mf;
struct token*ret= *expression;
if(ret->next!=NULL)
ret->next->prev= ret->prev;
if(ret->prev!=NULL)
ret->prev->next= ret->next;
*expression= ret->next;
return ret;
}
/*:932*/
#line 20956 "./cweb/weaver.w"

/*927:*/
#line 21033 "./cweb/weaver.w"

static struct token*eval_string(struct metafont**mf,
struct token**expression){
bool delimited= false;
struct token*current_token= *expression;
char*delim= delimiter(*mf,*expression);
if(delim!=NULL){
current_token= current_token->next;
delimited= true;
}

while(current_token!=NULL&&
(current_token->type!=SYMBOL||
(strcmp(current_token->name,";")&&
strcmp(current_token->name,"=")&&
strcmp(current_token->name,",")&&
strcmp(current_token->name,":=")))&&
(!delimited||strcmp(current_token->name,delim))){
/*938:*/
#line 21539 "./cweb/weaver.w"

if(current_token->type==SYMBOL){
char variable_name[1024],type_name[1024];
int type= NOT_DECLARED;
struct token*replacement= NULL;
struct token*possible_var= current_token;
variable(mf,&current_token,variable_name,1024,type_name,&type,true);
if(type==MACRO){
return NULL;
}
if(type!=NOT_DECLARED){

if(type!=STRING){
mf_error(*mf,"Variable '%s' isn't a string.",variable_name);
return NULL;
}
replacement= read_var(variable_name,type_name,*mf);
if(replacement!=NULL){
if(current_token!=NULL)
replacement->prev= current_token->prev;
replacement->next= current_token;
if(current_token!=NULL&&current_token->prev!=NULL)
current_token->prev->next= replacement;
else
*expression= replacement;
if(current_token!=NULL)
current_token->prev= replacement;
current_token= replacement;
continue;
}
}
if(replacement==NULL||type==NOT_DECLARED){
if(current_token!=possible_var){

if(current_token->prev==NULL)
*expression= possible_var;
else
current_token->prev->next= possible_var;
while(possible_var->next!=current_token&&
possible_var->next!=NULL&&
possible_var->next!=possible_var){
possible_var= possible_var->next;
}
current_token->prev= possible_var;
possible_var->next= current_token;
}
}
}
/*:938*//*941:*/
#line 21601 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"jobname")){
struct token*jobname= new_token_string((*mf)->filename);
if(jobname==NULL)
goto error_no_memory_internal;
jobname->prev= current_token->prev;
jobname->next= current_token->next;
current_token= jobname;
if(current_token->prev!=NULL)
current_token->prev->next= current_token;
else
*expression= current_token;
if(current_token->next!=NULL)
current_token->next->prev= NULL;
}
/*:941*//*943:*/
#line 21626 "./cweb/weaver.w"

{
char*current_delim= delimiter(*mf,current_token);
if(current_delim!=NULL){
eval_string(mf,&current_token);
}
}
/*:943*//*945:*/
#line 21640 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"begingroup")){
eval(mf,&current_token);
return NULL;
}
/*:945*//*947:*/
#line 21653 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"readstring")){
struct token*token;
char*buffer= NULL,*begin;
size_t size;
if(getline(&buffer,&size,stdin)==-1||buffer==NULL){
fprintf(stderr,"ERROR: Not enough memory.\n");
exit(1);
}
begin= buffer;
while(isspace(*begin))
begin++;
while(size!=0&&(buffer[size]=='\n'||isspace(buffer[size])))
size--;
buffer[size+1]= '\0';
token= new_token_string(begin);
free(buffer);
token->next= current_token->next;
token->prev= current_token->prev;
if(token->prev==NULL)
*expression= token;
else
token->prev->next= token;
if(token->next!=NULL)
token->next->prev= token;
}
/*:947*//*949:*/
#line 21689 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"str")){
char buffer[2048];
buffer[0]= '\0';
int remaining_size= 2047;
bool deterministic= true;
struct token*last_token= current_token->next,*new_result;
while(last_token!=NULL){
if(last_token->type==SYMBOL&&!strcmp(last_token->name,"[")){
char buffer_number[16];
struct token*result;
strncat(buffer,"[",remaining_size);
remaining_size--;
last_token= last_token->next;
if(last_token==NULL){
mf_error(*mf,"Missing numeric expression.");
return NULL;
}
result= eval_numeric(mf,&last_token);
if(result==NULL)
return NULL;
if(result->type!=NUMERIC){
mf_error(*mf,"Undefined numeric expression.");
return NULL;
}
deterministic= deterministic&&result->deterministic;
snprintf(buffer_number,16,"%f",result->value);
strncat(buffer,buffer_number,remaining_size);
remaining_size-= strlen(buffer_number);
if(last_token==NULL||last_token->type!=SYMBOL||
strcmp(last_token->name,"]")){
mf_error(*mf,"Missing ']'.");
return NULL;
}
strncat(buffer,"]",remaining_size);
remaining_size--;
last_token= last_token->next;
continue;
}
if(last_token->type==SYMBOL&&!is_tag(*mf,last_token))
break;
if(last_token->type==SYMBOL&&is_tag(*mf,last_token)){
if(buffer[0]!='\0'){
strncat(buffer,".",remaining_size);
remaining_size--;
}
strncat(buffer,last_token->name,remaining_size);
remaining_size-= strlen(last_token->name);
}
last_token= last_token->next;
}
new_result= new_token_string(buffer);
new_result->deterministic= deterministic;
new_result->prev= current_token->prev;
if(new_result->prev==NULL)
*expression= new_result;
else
new_result->prev->next= new_result;
new_result->next= last_token;
if(new_result->next!=NULL)
new_result->next->prev= new_result;
current_token= new_result;
}
/*:949*//*953:*/
#line 21865 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"char")){
char buffer[5]= {0x00,0x00,0x00,0x00};
unsigned long number;
bool deterministic;
struct token*result;
if(current_token->next==NULL){
mf_error(*mf,"Missing numeric primary.");
return NULL;
}
result= numeric_primary(mf,&(current_token->next));
if(result==NULL)
return NULL;
if(result->type!=NUMERIC){
mf_error(*mf,"Not recognized numeric primary.");
return NULL;
}
deterministic= result->deterministic;
number= (unsigned long)round(result->value);
number2utf8((uint32_t)number,buffer);
result= new_token_string(buffer);
result->deterministic= deterministic;
result->next= current_token->next;
result->prev= current_token->prev;
if(result->next!=NULL)
result->next->prev= result;
if(result->prev!=NULL)
result->prev->next= result;
else
*expression= result;
}
/*:953*//*955:*/
#line 21907 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"decimal")){
struct token*result;
char buffer[32];
bool deterministic;
int n;
if(current_token->next==NULL){
mf_error(*mf,"Missing numeric primary.");
return NULL;
}
result= numeric_primary(mf,&(current_token->next));
if(result==NULL)
return NULL;
if(result->type!=NUMERIC){
mf_error(*mf,"Not recognized numeric primary.");
return NULL;
}
deterministic= result->deterministic;
snprintf(buffer,32,"%f",result->value);

for(n= 0;buffer[n]!='\0';n++);
n--;
while(buffer[n]=='0'&&n>=0){
buffer[n]= '\0';
n--;
}
if(n>=0&&buffer[n]=='.')
buffer[n]= '\0';

result= new_token_string(buffer);
result->deterministic= deterministic;
result->next= current_token->next;
result->prev= current_token->prev;
if(result->next!=NULL)
result->next->prev= result;
if(result->prev!=NULL)
result->prev->next= result;
else
*expression= result;
}
/*:955*//*961:*/
#line 22060 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"substring")){
struct token*pair,*substring_token;
substring_token= current_token;
current_token= current_token->next;
pair= pair_primary(mf,&current_token);
if(pair==NULL){
return NULL;
}
pair->next= current_token;
pair->prev= substring_token;
substring_token->next= pair;
if(current_token!=NULL)
current_token->prev= pair;
}
/*:961*//*962:*/
#line 22088 "./cweb/weaver.w"

while(current_token!=NULL&&current_token->prev!=NULL&&
current_token->prev->prev!=NULL&&
current_token->prev->prev->prev!=NULL&&
current_token->prev->prev->prev->type==SYMBOL&&
!strcmp(current_token->prev->prev->prev->name,"substring")){
struct token*result;
int i;
long n1,n2,max_size;
char*buffer;
bool reversed,deterministic= true;
if(current_token->type!=STRING){
mf_error(*mf,"Can't get substring from an unknown string.");
return NULL;
}
deterministic= deterministic&&current_token->deterministic;
if(current_token->prev->type!=SYMBOL||
strcmp(current_token->prev->name,"of")){
mf_error(*mf,"Missing 'of' in substring expression.");
return NULL;
}
if(current_token->prev->prev->type!=PAIR){
mf_error(*mf,"Unknown pair after substring expression.");
return NULL;
}
deterministic= deterministic&&
current_token->prev->prev->deterministic;
max_size= (long)strlen(current_token->name);
n1= (long)round(current_token->prev->prev->value);
if(n1<0)
n1= 0;
if(n1> max_size)
n1= max_size;
n2= (long)round(current_token->prev->prev->value2);
if(n2> max_size)
n2= max_size;
if(n2<0)
n2= 0;
if(n1> n2){
reversed= true;
max_size= n1-n2+1;
}
else{
reversed= false;
max_size= n2-n1+1;
}
buffer= (char*)Walloc_arena(_internal_arena,max_size);
if(buffer==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_INTERNAL_MEMORY at conf/conf.h.\n");
exit(1);
}
if(!reversed)
for(i= 0;i<max_size-1;i++)
buffer[i]= current_token->name[n1+i];
else
for(i= 0;i<max_size-1;i++)
buffer[i]= current_token->name[n1-i-1];
buffer[max_size-1]= '\0';
result= new_token_string(buffer);
result->deterministic= deterministic;
result->prev= current_token->prev->prev->prev->prev;
result->next= current_token->next;
if(result->prev!=NULL)
result->prev->next= result;
else
*expression= result;
if(result->next!=NULL)
result->next->prev= result;
current_token= result;
}
/*:962*/
#line 21051 "./cweb/weaver.w"

current_token= current_token->next;
}

current_token= *expression;
if(delim!=NULL)
current_token= current_token->next;
while(current_token!=NULL&&
(current_token->type!=SYMBOL||
(strcmp(current_token->name,";")&&
strcmp(current_token->name,"=")&&
strcmp(current_token->name,",")&&
strcmp(current_token->name,":=")))&&
(!delimited||strcmp(current_token->name,delim))){
/*965:*/
#line 22173 "./cweb/weaver.w"

if(current_token->type==SYMBOL&&
!strcmp(current_token->name,"&")){
size_t new_string_size;
size_t last_token_size,next_token_size;
char*buffer;
struct token*result;

if(current_token->prev==NULL||current_token->prev->type!=STRING){
mf_error(*mf,"Missing known string before '&'.");
return NULL;
}

if(current_token->next==NULL||current_token->next->type!=STRING){
mf_error(*mf,"Missing known string after '&'.");
return NULL;
}
last_token_size= strlen(current_token->prev->name);
next_token_size= strlen(current_token->next->name);
new_string_size= last_token_size+next_token_size+1;
buffer= Walloc_arena(_internal_arena,new_string_size);
if(buffer==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_INTERNAL_MEMORY at conf/conf.h.\n");
exit(1);
}
memcpy(buffer,current_token->prev->name,last_token_size+1);
memcpy(&buffer[last_token_size],current_token->next->name,
next_token_size);
result= new_token_string(buffer);
result->deterministic= current_token->next->deterministic&&
current_token->prev->deterministic;
result->prev= current_token->prev->prev;
result->next= current_token->next->next;
if(result->prev!=NULL)
result->prev->next= result;
else
*expression= result;
if(result->next!=NULL)
result->next->prev= result;
current_token= result;
}
/*:965*/
#line 21065 "./cweb/weaver.w"

current_token= current_token->next;
}


if(delimited){
if(*expression!=NULL&&(*expression)->next!=NULL&&
(*expression)->next->next!=NULL&&
!strcmp((*expression)->next->next->name,
delim)){
*expression= (*expression)->next;
(*expression)->prev= (*expression)->prev->prev;
if((*expression)->prev!=NULL)
(*expression)->prev->next= *expression;
(*expression)->next= (*expression)->next->next;
if((*expression)->next!=NULL)
(*expression)->next->prev= *expression;
}
}
return*expression;
error_no_memory_internal:
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_INTERNAL_MEMORY at conf/conf.h\n");
exit(1);


}
/*:927*/
#line 20957 "./cweb/weaver.w"

struct token*eval(struct metafont**mf,struct token**expression){
struct token*aux= *expression;
int type= -1;
char var_name[1024],type_name[1024];
if((*expression)->type==SYMBOL&&!strcmp((*expression)->name,";"))
return NULL;

while(aux!=NULL&&aux->type==SYMBOL&&delimiter(*mf,aux)!=NULL)
aux= aux->next;

if(aux==NULL){
mf_error(*mf,"Missing expression.");
return NULL;
}
if(aux->type==STRING)
return eval_string(mf,expression);

else if(aux->type==SYMBOL){
if(!strcmp(aux->name,"begingroup")){
struct token*expr_begin= *expression;
while(expr_begin!=NULL&&expr_begin->prev!=NULL)
expr_begin= expr_begin->prev;
if(expr_begin!=NULL){
(*mf)->past_tokens= expr_begin;
}


if(aux->prev!=NULL){
aux->prev->next= NULL;
aux->prev= NULL;
}

Wbreakpoint_arena(metafont_arena);
*mf= _new_metafont(*mf,(*mf)->filename);
(*mf)->pending_tokens= aux->next;
concat_token(&((*mf)->pending_tokens),
(*mf)->parent->pending_tokens);
if(aux->next!=NULL){
aux->next->prev= NULL;
aux->next= NULL;
}
(*mf)->parent->pending_tokens= NULL;

return NULL;
}

if(!strcmp(aux->name,"jobname"))
return eval_string(mf,expression);
if(!strcmp(aux->name,"readstring"))
return eval_string(mf,expression);
if(!strcmp(aux->name,"str"))
return eval_string(mf,expression);
if(!strcmp(aux->name,"char"))
return eval_string(mf,expression);
if(!strcmp(aux->name,"decimal"))
return eval_string(mf,expression);
if(!strcmp(aux->name,"substring"))
return eval_string(mf,expression);

variable(mf,&aux,var_name,1024,type_name,&type,false);
if(type==STRING){
struct token*teste= eval_string(mf,expression);
return teste;
}

}
mf_error(*mf,"Undetermined expression.");
return NULL;
}
/*:926*/
#line 18135 "./cweb/weaver.w"

/*849:*/
#line 18892 "./cweb/weaver.w"

/*850:*/
#line 18928 "./cweb/weaver.w"

void run_single_statement(struct metafont**mf,struct token*statement){
#ifdef W_DEBUG_METAFONT
{
int depth= 0;
struct metafont*p= *mf;
while(p->parent!=NULL){
p= p->parent;
depth++;
}
printf("Global vardefs:");
_debug_trie_values("",p->vardef);
printf("\n");
printf("Declared variables:");
p= *mf;
while(p->parent!=NULL){
_debug_trie_values("",p->variable_types);
p= p->parent;
}
_debug_trie_values("",p->variable_types);
printf("\n");
printf("METAFONT: Statement:  (Depth: %d)\n",depth);
while(p!=*mf){
printf("                     ");
debug_token_list(p->past_tokens);
printf(" [begingroup]\n");
p= p->child;
}
printf("                  -> ");
debug_token_list(statement);
printf("\n");
printf("                     ");
debug_token_list((*mf)->pending_tokens);
printf("\n");
}
#endif
if(statement->type==SYMBOL&&!strcmp(statement->name,";"))
return;
/*873:*/
#line 19428 "./cweb/weaver.w"

{
struct token*aux= statement;
if((*mf)->hint==HINT_ENDGROUP){
while(aux!=NULL){
if(aux->type==SYMBOL&&!strcmp(aux->name,"endgroup")){
if(aux->prev!=NULL)
aux->prev->next= NULL;
else
statement= aux->next;
concat_token(&(aux->next),(*mf)->pending_tokens);
if(aux->next!=NULL){
(*mf)->pending_tokens= aux->next;
aux->next->prev= NULL;
}
break;
}
aux= aux->next;
}
}
}
/*:873*/
#line 18966 "./cweb/weaver.w"

/*858:*/
#line 19195 "./cweb/weaver.w"

if(statement->type==SYMBOL&&!strcmp(statement->name,"newinternal")){
struct token*current_token= statement->next;
struct token*list= symbolic_token_list(*mf,&current_token);

if(current_token==NULL||current_token->type!=SYMBOL||
strcmp(current_token->name,";")){
mf_error(*mf,"Extra token at newinternal command.");
return;
}

while(list!=NULL){
_insert_trie((*mf)->internal_quantities,_user_arena,DOUBLE,
list->name,0.0);
list= list->next;
}
return;
}
/*:858*//*862:*/
#line 19246 "./cweb/weaver.w"

if(statement->type==SYMBOL&&!strcmp(statement->name,"everyjob")){

if(statement->next==NULL||statement->next->type!=SYMBOL){
mf_error(*mf,"Missing symbolic token.");
return;
}

if(!strcmp(statement->next->name,";")){
statement->next->next= get_statement(*mf);
if(statement->next->next!=NULL)
statement->next->next->prev= statement->next;
}

if(statement->next->next==NULL||
statement->next->next->type!=SYMBOL||
strcmp(statement->next->next->name,";")){
mf_error(*mf,"Extra tokens found after everyjob.");
return;
}

if((*mf)->everyjob_token_name==NULL||
strlen((*mf)->everyjob_token_name)<strlen(statement->next->name))
(*mf)->everyjob_token_name= (char*)
Walloc(strlen(statement->next->name)+1);
memcpy((*mf)->everyjob_token_name,statement->next->name,
strlen(statement->next->name)+1);
return;
}
/*:862*//*865:*/
#line 19308 "./cweb/weaver.w"

if(statement->type==SYMBOL&&
(!strcmp(statement->name,"batchmode")||
!strcmp(statement->name,"nonstopmode")||
!strcmp(statement->name,"scrollmode")||
!strcmp(statement->name,"errorstopmode"))){

if(statement->next==NULL||statement->next->type!=SYMBOL||
strcmp(statement->next->name,";")){
mf_error(*mf,"Extra tokens found after mode command.");
return;
}
return;
}
/*:865*//*868:*/
#line 19379 "./cweb/weaver.w"

if(statement->type==SYMBOL&&!strcmp(statement->name,"begingroup")){
Wbreakpoint_arena(metafont_arena);
*mf= _new_metafont(*mf,(*mf)->filename);
statement= statement->next;
statement->prev= NULL;
(*mf)->pending_tokens= statement;
concat_token(&((*mf)->pending_tokens),
(*mf)->parent->pending_tokens);
(*mf)->parent->pending_tokens= NULL;
return;
}
/*:868*//*883:*/
#line 19576 "./cweb/weaver.w"

if(statement->type==SYMBOL&&!strcmp(statement->name,"save")){
struct token*current_token= statement->next;
struct token*list= symbolic_token_list(*mf,&current_token);

if(current_token==NULL||current_token->type!=SYMBOL||
strcmp(current_token->name,";")){
mf_error(*mf,"Extra token at save command.");
return;
}

while(list!=NULL){
void*current_arena= 
((*mf)->parent==NULL)?_user_arena:metafont_arena;
int current_type;
bool already_declared= _search_trie((*mf)->variable_types,INT,
list->name,&current_type);
if(already_declared&&current_type!=NOT_DECLARED)
_remove_trie((*mf)->vars[current_type],list->name);
_insert_trie((*mf)->variable_types,current_arena,INT,list->name,
NOT_DECLARED);
list= list->next;
}
return;
}
/*:883*//*889:*/
#line 19707 "./cweb/weaver.w"

if(statement->type==SYMBOL&&!strcmp(statement->name,"delimiters")){
char*end_delimiter;
void*current_arena;
size_t name_size;
struct metafont*scope= (*mf);
statement= statement->next;
if(statement==NULL||statement->type!=SYMBOL){
mf_error(*mf,"Missing symbolic token.");
return;
}
scope= get_scope(*mf,statement->name);
current_arena= (scope->parent==NULL)?_user_arena:metafont_arena;
statement= statement->next;
if(statement==NULL||statement->type!=SYMBOL){
mf_error(*mf,"Missing symbolic token.");
return;
}
end_delimiter= (char*)
Walloc_arena(current_arena,
name_size= strlen(statement->name)+1);
if(end_delimiter==NULL){
fprintf(stderr,"ERROR: Not enough memory to parse METAFONT. "
"Please, increase the value of %s at conf/conf.h.\n",
(current_arena==_user_arena)?"W_MAX_MEMORY":"W_INTERNAL_MEMORY");
return;
}
memcpy(end_delimiter,statement->name,name_size);
_insert_trie(scope->delimiters,current_arena,VOID_P,
statement->prev->name,(void*)end_delimiter);
statement= statement->next;
if(statement==NULL||statement->type!=SYMBOL||
strcmp(statement->name,";")){
mf_error(*mf,"Missing symbolic token.");
return;
}
return;
}
/*:889*//*894:*/
#line 19794 "./cweb/weaver.w"

if(statement->type==SYMBOL&&
(!strcmp(statement->name,"inner")||
!strcmp(statement->name,"outer"))){
bool inner_command= (statement->name[0]=='i');
statement= statement->next;
struct token*list= symbolic_token_list(*mf,&statement);

if(statement==NULL||statement->type!=SYMBOL||
strcmp(statement->name,";")){
mf_error(*mf,"Extra token at save command");
return;
}

while(list!=NULL){
void*current_arena;
struct metafont*scope= get_scope(*mf,list->name);
current_arena= (scope->parent==NULL)?_user_arena:metafont_arena;
if(inner_command)
_remove_trie((*mf)->outer_tokens,list->name);
else
_insert_trie((*mf)->outer_tokens,current_arena,INT,
list->name,0);
list= list->next;
}
return;
}
/*:894*//*902:*/
#line 20093 "./cweb/weaver.w"

if(statement->type==SYMBOL&&
(!strcmp(statement->name,"boolean")||
!strcmp(statement->name,"string")||
!strcmp(statement->name,"path")||
!strcmp(statement->name,"pen")||
!strcmp(statement->name,"picture")||
!strcmp(statement->name,"transform")||
!strcmp(statement->name,"pair")||
!strcmp(statement->name,"numeric"))){
int type;
char buffer[1024];

switch(statement->name[0]){
case'b':
type= BOOLEAN;
break;
case's':
type= STRING;
break;
case't':
type= TRANSFORM;
break;
case'n':
type= NUMERIC;
break;
default:
switch(statement->name[2]){
case't':
type= PATH;
break;
case'n':
type= PEN;
break;
case'c':
type= PICTURE;
break;
default:
type= PAIR;
break;
}
}
statement= statement->next;
while(1){
bool already_declared= false;
int current_type_if_already_declared;
void*current_arena;
struct metafont*scope= *mf;

declared_variable(*mf,&statement,buffer,1024);
if(!strcmp(buffer,"")){
mf_error(*mf,"Missing symbolic token.");
return;
}

if(scope->parent==NULL)
already_declared= _search_trie(scope->variable_types,INT,
buffer,
&current_type_if_already_declared);
scope= get_scope(*mf,buffer);
already_declared= _search_trie(scope->variable_types,INT,
buffer,
&current_type_if_already_declared);

if(scope->parent==NULL)
current_arena= _user_arena;
else
current_arena= metafont_arena;

if(already_declared&&current_type_if_already_declared!=NOT_DECLARED)
_remove_trie(scope->vars[current_type_if_already_declared],
buffer);

_insert_trie(scope->variable_types,current_arena,INT,buffer,type);

if(statement!=NULL&&statement->type==SYMBOL&&
!strcmp(statement->name,";"))
break;

if(statement!=NULL&&statement->type==SYMBOL&&
!strcmp(statement->name,",")){
statement= statement->next;
continue;
}

else{
mf_error(*mf,"Illegal suffix or missing symbolic token.");
return;
}
}
return;
}
/*:902*//*913:*/
#line 20497 "./cweb/weaver.w"

if(statement->type==SYMBOL&&!strcmp(statement->name,"def")){
char*name;
struct macro*new_macro;
struct token*delimited_headers,*undelimited_header;
statement= statement->next;
struct metafont*scope= *mf;
void*current_arena= _user_arena;

if(statement==NULL||statement->type!=SYMBOL){
mf_error(*mf,"Missing symbolic token.");
return;
}
name= statement->name;

scope= get_scope(*mf,name);
if(scope->parent!=NULL)
current_arena= metafont_arena;
statement= statement->next;
delimited_headers= delimited_parameters(*mf,&statement,current_arena);
undelimited_header= undelimited_parameters(*mf,&statement,current_arena);
new_macro= (struct macro*)Walloc_arena(current_arena,sizeof(struct macro));
if(new_macro==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_%s_MEMORY at conf/conf.h\n",
(current_arena==_user_arena)?"MAX":"INTERNAL");
exit(1);
}
new_macro->parameters= undelimited_header;
if(new_macro->parameters==NULL)
new_macro->parameters= delimited_headers;
else
new_macro->parameters->next= delimited_headers;
if(delimited_headers!=NULL)
delimited_headers->prev= undelimited_header;

if(statement==NULL||statement->type!=SYMBOL||
(strcmp(statement->name,"=")&&strcmp(statement->name,":="))){
mf_error(*mf,"Missing '=' or ':=' at macro definition.");
return;
}
statement= statement->next;

new_macro->replacement_text= replacement_text(*mf,&statement,
current_arena);

_insert_trie(scope->macros,current_arena,VOID_P,name,
(void*)new_macro);

if(statement==NULL||statement->type!=SYMBOL||
strcmp(statement->name,";")){
mf_error(*mf,"Extra token after enddef");
return;
}
return;
}
/*:913*//*918:*/
#line 20610 "./cweb/weaver.w"

if(statement->type==SYMBOL&&!strcmp(statement->name,"vardef")){
struct macro*new_macro;
struct token*tok;
char variable_name[1024];
struct token*delimited_headers= NULL,*undelimited_header= NULL,
*suffix_header= NULL;
void*current_arena= _user_arena;
struct metafont*scope= *mf;
statement= statement->next;

variable_name[0]= '\0';
declared_variable(*mf,&statement,variable_name,1024);

scope= get_scope(*mf,variable_name);
if(scope->parent!=NULL)
current_arena= metafont_arena;

if(statement!=NULL&&statement->type==SYMBOL&&
!strcmp(statement->name,"@#")){
suffix_header= new_token(VARDEF_ARG,0.0,"@#",current_arena);
statement= statement->next;
}

delimited_headers= delimited_parameters(*mf,&statement,current_arena);
undelimited_header= undelimited_parameters(*mf,&statement,current_arena);
new_macro= (struct macro*)Walloc_arena(current_arena,sizeof(struct macro));
if(new_macro==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_%s_MEMORY at conf/conf.h\n",
(current_arena==_user_arena)?"MAX":"INTERNAL");
exit(1);
}
new_macro->parameters= suffix_header;
concat_token(&(new_macro->parameters),undelimited_header);
concat_token(&(new_macro->parameters),delimited_headers);

if(statement==NULL||statement->type!=SYMBOL||
(strcmp(statement->name,"=")&&strcmp(statement->name,":="))){
mf_error(*mf,"Missing '=' or ':=' at macro definition.");
return;
}
statement= statement->next;

new_macro->replacement_text= new_token(SYMBOL,0.0,"begingroup",
current_arena);
if(new_macro->replacement_text==NULL){
if(current_arena==_user_arena)
goto error_no_memory_user;
else
goto error_no_memory_internal;
}
new_macro->replacement_text->next= replacement_text(*mf,&statement,
current_arena);
if(new_macro->replacement_text->next!=NULL)
new_macro->replacement_text->next->prev= 
new_macro->replacement_text;
tok= new_token(SYMBOL,0.0,"endgroup",current_arena);
if(tok==NULL){
if(current_arena==_user_arena)
goto error_no_memory_user;
else
goto error_no_memory_internal;
}
concat_token(&(new_macro->replacement_text),tok);

_insert_trie(scope->variable_types,current_arena,INT,variable_name,
MACRO);
_insert_trie(scope->vardef,current_arena,VOID_P,variable_name,
(void*)new_macro);

if(statement==NULL||statement->type!=SYMBOL||
strcmp(statement->name,";")){
mf_error(*mf,"Extra token after enddef");
return;
}
return;
}
/*:918*//*921:*/
#line 20723 "./cweb/weaver.w"

if(statement->type==SYMBOL&&
(!strcmp(statement->name,"primarydef")||
!strcmp(statement->name,"secondarydef")||
!strcmp(statement->name,"tertiarydef"))){
struct _trie*destiny[3];
int precedence;
char*name;
struct token*arg1,*arg2;
struct metafont*scope= *mf;
void*current_arena= _user_arena;
struct macro*new_macro;

switch(statement->name[0]){
case'p':
precedence= 0;
break;
case's':
precedence= 1;
break;
default:
precedence= 2;
break;
}

if(statement->next==NULL){
mf_error(*mf,"Missing symbolic token.");
return;
}
statement= statement->next->next;
if(statement==NULL||statement->type!=SYMBOL){
mf_error(*mf,"Missing symbolic token.");
return;
}
name= statement->name;

scope= get_scope(*mf,name);
if(scope->parent!=NULL)
current_arena= metafont_arena;
destiny[0]= scope->primarydef;
destiny[1]= scope->secondarydef;
destiny[2]= scope->tertiarydef;

new_macro= (struct macro*)Walloc_arena(current_arena,
sizeof(struct macro));
if(new_macro==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_%s_MEMORY at conf/conf.h\n",
(current_arena==_user_arena)?"MAX":"INTERNAL");
exit(1);
}

statement= statement->prev;
if(statement->type!=SYMBOL){
mf_error(*mf,"Missing symbolic token.");
return;
}
arg1= new_token(SYMBOL,0.0,statement->name,current_arena);
if(arg1==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_%s_MEMORY at conf/conf.h\n",
(current_arena==_user_arena)?"MAX":"INTERNAL");
exit(1);
}

statement= statement->next->next;
if(statement->type!=SYMBOL){
mf_error(*mf,"Missing symbolic token.");
return;
}
arg2= new_token(SYMBOL,0.0,statement->name,current_arena);
if(arg1==NULL){
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_%s_MEMORY at conf/conf.h\n",
(current_arena==_user_arena)?"MAX":"INTERNAL");
exit(1);
}
arg1->next= arg2;
arg2->prev= arg1;
new_macro->parameters= arg1;

statement= statement->next;
if(statement==NULL||statement->type!=SYMBOL||
(strcmp(statement->name,"=")&&strcmp(statement->name,":="))){
mf_error(*mf,"Missing '=' or ':=' at macro definition.");
return;
}
statement= statement->next;

new_macro->replacement_text= new_token(SYMBOL,0.0,"begingroup",
current_arena);
if(new_macro->replacement_text==NULL){
if(current_arena==_user_arena)
goto error_no_memory_user;
else
goto error_no_memory_internal;
}
new_macro->replacement_text= replacement_text(*mf,&statement,
current_arena);


_insert_trie(destiny[precedence],current_arena,VOID_P,name,
(void*)new_macro);

if(statement==NULL||statement->type!=SYMBOL||
strcmp(statement->name,";")){
mf_error(*mf,"Extra token after enddef");
return;
}
return;
}
/*:921*//*968:*/
#line 22243 "./cweb/weaver.w"

if(statement->type==SYMBOL&&(!strcmp(statement->name,"message")||
!strcmp(statement->name,"errmessage")||
!strcmp(statement->name,"errhelp"))){
struct token*expr_result;

if(statement->next==NULL){
mf_error(*mf,"Missing string expression.");
return;
}
expr_result= eval_string(mf,&(statement->next));
if(expr_result==NULL){
return;
}
if(expr_result->type!=STRING){
mf_error(*mf,"Tried to print unknown string (%s).",
expr_result->name);
return;
}
if(statement->name[0]=='m')
printf("%s\n",expr_result->name);
else if(statement->name[3]=='m')
fprintf(stderr,"%s\n",expr_result->name);
return;
}
/*:968*//*980:*/
#line 22654 "./cweb/weaver.w"

{
struct token*tok= statement,*last_separator= NULL;
struct token*last_semicolon= NULL;
bool found_equation_or_attribution= false;
int type= -1;
struct token*token_stack[512];
int token_stack_position= -1;

while(tok!=NULL){
if(tok->type==SYMBOL&&
(!strcmp(tok->name,":=")||!strcmp(tok->name,"="))){
token_stack_position++;
token_stack[token_stack_position]= tok;
last_separator= tok;
found_equation_or_attribution= true;
}
last_semicolon= tok;
tok= tok->next;
}

while(last_separator!=NULL){
char left_var[1024],left_var_type[1024];
char right_var[1024],right_var_type[1024];
struct token*left,*right= eval(mf,&(last_separator->next));
if(right==NULL){
for(tok= statement;tok->next!=NULL;tok= tok->next)
tok->next->prev= tok;
return;
}
if(last_separator->name[0]==':'){
if(token_stack_position> 0){
mf_error(*mf,"Not a variable before ':='.");
return;
}
variable(mf,&statement,left_var,1024,left_var_type,&type,true);
if(statement->type!=SYMBOL||(strcmp(statement->name,"=")&&
strcmp(statement->name,":="))){
mf_error(*mf,"Not a variable before ':='.");
return;
}
if(right->type==SYMBOL){

variable(mf,&right,right_var,1024,right_var_type,&type,true);
equal_variables(*mf,left_var,right_var,left_var_type,
right_var_type,true);

}
else{

if(right->type==STRING)
new_defined_string_variable(left_var,left_var_type,right,*mf,
true);
}
return;
}
else{

if(token_stack_position> 0)
left= eval(mf,&(token_stack[token_stack_position-1]->next));
else
left= eval(mf,&statement);
if(right->type==SYMBOL&&left->type==SYMBOL){
variable(mf,&left,left_var,1024,left_var_type,&type,false);
variable(mf,&right,right_var,1024,right_var_type,&type,false);
equal_variables(*mf,left_var,right_var,left_var_type,
right_var_type,false);
}
else if(right->type==SYMBOL){
variable(mf,&right,right_var,1024,right_var_type,&type,false);
new_defined_string_variable(right_var,right_var_type,left,*mf,
false);
}
else if(left->type==SYMBOL){

variable(mf,&left,left_var,1024,left_var_type,&type,false);
new_defined_string_variable(left_var,left_var_type,right,*mf,
false);
}
else{

if(left->type==right->type&&!strcmp(left->name,
right->name)){
mf_error(*mf,"Redundant equation.");
return;
}
else{
mf_error(*mf,"Inconsistent equation (%s = %s).",left->name,
right->name);
return;
}
}
}
last_separator->prev->next= last_semicolon;
last_semicolon->prev= last_separator->prev;
token_stack_position--;
if(token_stack_position>=0)
last_separator= token_stack[token_stack_position];
else
last_separator= NULL;
}
if(found_equation_or_attribution)
return;
}
/*:980*/
#line 18967 "./cweb/weaver.w"

/*924:*/
#line 20905 "./cweb/weaver.w"

{
struct token*new_tokens= NULL;
struct token*expression_result= eval(mf,&statement);
if((*mf)->hint==HINT_ENDGROUP){
if(expression_result!=NULL)
if((*mf)->parent==NULL){
mf_error(*mf,"Extra 'endgroup' while not in 'begingroup'.");
return;
}
new_tokens= (*mf)->parent->past_tokens;
(*mf)->parent->past_tokens= NULL;
if(expression_result!=NULL){
concat_token(&new_tokens,expression_result);
concat_token(&new_tokens,(*mf)->pending_tokens);
(*mf)->parent->pending_tokens= new_tokens;
}
end_scope(*mf);
*mf= (*mf)->parent;
Wtrash_arena(metafont_arena);
}
else{


if(expression_result==NULL)
return;
if(expression_result->type!=STRING)
mf_error(*mf,"Isolated expression.");
else if(expression_result->type==STRING){
if(statement->next->type!=SYMBOL||
strcmp(statement->next->name,";"))
mf_error(*mf,"Missing ';' after title.");
;
}
}
return;
}
/*:924*/
#line 18968 "./cweb/weaver.w"

mf_error(*mf,"Isolated expression. I couldn't find a = or := after it.");
return;
error_no_memory_user:
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_MAX_MEMORY at conf/conf.h\n");
exit(1);
error_no_memory_internal:
fprintf(stderr,"ERROR: Not enough memory. Please, increase the "
"value of W_INTERNAL_MEMORY at conf/conf.h\n");
exit(1);
}
/*:850*/
#line 18893 "./cweb/weaver.w"

void run_statements(struct metafont*mf){
struct token*statement;
bool end_execution= false,first_loop= (mf->parent==NULL);
while(!end_execution){
if(mf->pending_tokens==NULL&&mf->parent==NULL)
_iWbreakpoint();
if(first_loop){
/*863:*/
#line 19281 "./cweb/weaver.w"

if(mf->everyjob_token_name!=NULL)
mf->pending_tokens= new_token_symbol(mf->everyjob_token_name);
/*:863*/
#line 18901 "./cweb/weaver.w"

first_loop= false;
}
/*871:*/
#line 19413 "./cweb/weaver.w"

mf->hint= 0;
/*:871*/
#line 18904 "./cweb/weaver.w"

statement= get_statement(mf);
if(statement==NULL)
end_execution= true;
else{
run_single_statement(&mf,statement);
}
/*874:*/
#line 19453 "./cweb/weaver.w"

if(mf->hint==HINT_ENDGROUP){


if(mf->parent==NULL)
mf_error(mf,"Extra 'endgroup' while not in 'begingroup'.");
else{
end_scope(mf);
mf= mf->parent;
Wtrash_arena(metafont_arena);
}
}
/*:874*/
#line 18911 "./cweb/weaver.w"

if(mf->pending_tokens==NULL&&mf->parent==NULL)
_iWtrash();
}
/*875:*/
#line 19471 "./cweb/weaver.w"

while(mf->parent!=NULL){
end_scope(mf);
mf= mf->parent;
Wtrash_arena(metafont_arena);
}
/*:875*/
#line 18915 "./cweb/weaver.w"

}
/*:849*/
#line 18136 "./cweb/weaver.w"

/*823:*/
#line 18155 "./cweb/weaver.w"


/*:823*//*827:*/
#line 18197 "./cweb/weaver.w"

void _initialize_metafont(void){
struct metafont*mf;
metafont_arena= Wcreate_arena(W_INTERNAL_MEMORY/4);
if(metafont_arena==NULL){
fprintf(stderr,"ERROR: This system have no enough memory to "
"run this program.\n");
exit(1);
}
/*900:*/
#line 19974 "./cweb/weaver.w"

primitive_sparks= _new_trie(_user_arena);
_insert_trie(primitive_sparks,_user_arena,INT,"end",0);
_insert_trie(primitive_sparks,_user_arena,INT,"dump",0);
_insert_trie(primitive_sparks,_user_arena,INT,";",0);
_insert_trie(primitive_sparks,_user_arena,INT,",",0);
_insert_trie(primitive_sparks,_user_arena,INT,"newinternal",0);
_insert_trie(primitive_sparks,_user_arena,INT,"everyjob",0);
_insert_trie(primitive_sparks,_user_arena,INT,"batchmode",0);
_insert_trie(primitive_sparks,_user_arena,INT,"nonstopmode",0);
_insert_trie(primitive_sparks,_user_arena,INT,"scrollmode",0);
_insert_trie(primitive_sparks,_user_arena,INT,"errorstopmode",0);
_insert_trie(primitive_sparks,_user_arena,INT,"begingroup",0);
_insert_trie(primitive_sparks,_user_arena,INT,"endgroup",0);
_insert_trie(primitive_sparks,_user_arena,INT,"save",0);
_insert_trie(primitive_sparks,_user_arena,INT,"delimiters",0);
_insert_trie(primitive_sparks,_user_arena,INT,"outer",0);
_insert_trie(primitive_sparks,_user_arena,INT,"inner",0);
_insert_trie(primitive_sparks,_user_arena,INT,"[",0);
_insert_trie(primitive_sparks,_user_arena,INT,"]",0);
_insert_trie(primitive_sparks,_user_arena,INT,"boolean",0);
_insert_trie(primitive_sparks,_user_arena,INT,"string",0);
_insert_trie(primitive_sparks,_user_arena,INT,"path",0);
_insert_trie(primitive_sparks,_user_arena,INT,"pen",0);
_insert_trie(primitive_sparks,_user_arena,INT,"picture",0);
_insert_trie(primitive_sparks,_user_arena,INT,"transform",0);
_insert_trie(primitive_sparks,_user_arena,INT,"pair",0);
_insert_trie(primitive_sparks,_user_arena,INT,"numeric",0);
/*904:*/
#line 20224 "./cweb/weaver.w"

_insert_trie(primitive_sparks,_user_arena,INT,"expr",0);
_insert_trie(primitive_sparks,_user_arena,INT,"suffix",0);
_insert_trie(primitive_sparks,_user_arena,INT,"text",0);
_insert_trie(primitive_sparks,_user_arena,INT,"primary",0);
_insert_trie(primitive_sparks,_user_arena,INT,"secondary",0);
_insert_trie(primitive_sparks,_user_arena,INT,"tertiary",0);
_insert_trie(primitive_sparks,_user_arena,INT,"=",0);
_insert_trie(primitive_sparks,_user_arena,INT,":=",0);
_insert_trie(primitive_sparks,_user_arena,INT,"(",0);
_insert_trie(primitive_sparks,_user_arena,INT,")",0);
/*:904*//*915:*/
#line 20575 "./cweb/weaver.w"

_insert_trie(primitive_sparks,_user_arena,INT,"@#",0);
/*:915*//*920:*/
#line 20715 "./cweb/weaver.w"

_insert_trie(primitive_sparks,_user_arena,INT,"primarydef",0);
_insert_trie(primitive_sparks,_user_arena,INT,"secondarydef",0);
_insert_trie(primitive_sparks,_user_arena,INT,"tertiarydef",0);
/*:920*//*923:*/
#line 20876 "./cweb/weaver.w"

_insert_trie(primitive_sparks,_user_arena,INT,"&",0);
_insert_trie(primitive_sparks,_user_arena,INT,"jobname",0);
_insert_trie(primitive_sparks,_user_arena,INT,"readstring",0);
_insert_trie(primitive_sparks,_user_arena,INT,"str",0);
_insert_trie(primitive_sparks,_user_arena,INT,"char",0);
_insert_trie(primitive_sparks,_user_arena,INT,"decimal",0);
_insert_trie(primitive_sparks,_user_arena,INT,"substring",0);
/*:923*//*964:*/
#line 22166 "./cweb/weaver.w"

_insert_trie(primitive_sparks,_user_arena,INT,"&",0);
/*:964*//*967:*/
#line 22235 "./cweb/weaver.w"

_insert_trie(primitive_sparks,_user_arena,INT,"message",0);
_insert_trie(primitive_sparks,_user_arena,INT,"errmessage",0);
_insert_trie(primitive_sparks,_user_arena,INT,"errhelp",0);
/*:967*/
#line 20002 "./cweb/weaver.w"

#ifdef W_DEBUG_METAFONT
printf("METAFONT Sparks:");
_debug_trie_values("",primitive_sparks);
printf("\n");
#endif
/*:900*/
#line 18206 "./cweb/weaver.w"

/*833:*/
#line 18321 "./cweb/weaver.w"

mf= _new_metafont(NULL,"fonts/init.mf");
/*:833*/
#line 18207 "./cweb/weaver.w"

}
/*:827*//*832:*/
#line 18266 "./cweb/weaver.w"

struct metafont*_new_metafont(struct metafont*parent,char*filename){
void*arena;
struct metafont*structure;
size_t ret;
if(parent==NULL)
arena= _user_arena;
else
arena= metafont_arena;
structure= (struct metafont*)Walloc_arena(arena,
sizeof(struct metafont));
if(structure==NULL)
goto error_no_memory;
structure->parent= parent;
strncpy(structure->filename,filename,255);
if(parent==NULL){
#ifdef W_DEBUG_METAFONT
printf("METAFONT: Opening file '%s'.\n",filename);
#endif
structure->fp= fopen(filename,"r");
if(structure->fp==NULL)
goto error_no_file;
else{
ret= fread(structure->buffer,1,4095,structure->fp);
structure->buffer[ret]= '\0';
if(ret!=4095){
fclose(structure->fp);
structure->fp= NULL;
}
}
}
structure->buffer_position= 0;
structure->line= 1;
structure->error= false;
/*846:*/
#line 18805 "./cweb/weaver.w"

structure->pending_tokens= NULL;
structure->past_tokens= NULL;
/*:846*//*855:*/
#line 19028 "./cweb/weaver.w"

structure->internal_quantities= _new_trie(arena);
if(structure->internal_quantities==NULL)
goto error_no_memory;
/*:855*//*856:*/
#line 19042 "./cweb/weaver.w"

if(structure->parent==NULL){
struct _trie*T= structure->internal_quantities;
time_t current_time;
unsigned int year,month,day,time_in_minutes;
struct tm*date;
time(&current_time);
date= localtime(&current_time);
year= date->tm_year+1900;
month= date->tm_mon+1;
day= date->tm_mday;
time_in_minutes= 60*date->tm_hour+date->tm_min;
_insert_trie(T,arena,DOUBLE,"tracingtitles",0.0);
_insert_trie(T,arena,DOUBLE,"tracingequations",0.0);
_insert_trie(T,arena,DOUBLE,"tracingcapsules",0.0);
_insert_trie(T,arena,DOUBLE,"tracingchoices",0.0);
_insert_trie(T,arena,DOUBLE,"tracingspecs",0.0);
_insert_trie(T,arena,DOUBLE,"tracingpens",0.0);
_insert_trie(T,arena,DOUBLE,"tracingcommands",0.0);
_insert_trie(T,arena,DOUBLE,"tracingrestores",0.0);
_insert_trie(T,arena,DOUBLE,"tracingmacros",0.0);
_insert_trie(T,arena,DOUBLE,"tracingedges",0.0);
_insert_trie(T,arena,DOUBLE,"tracingoutput",0.0);
_insert_trie(T,arena,DOUBLE,"tracingonline",0.0);
_insert_trie(T,arena,DOUBLE,"tracingstats",0.0);
_insert_trie(T,arena,DOUBLE,"pausing",0.0);
_insert_trie(T,arena,DOUBLE,"showstopping",0.0);
_insert_trie(T,arena,DOUBLE,"proofing",0.0);
_insert_trie(T,arena,DOUBLE,"turningcheck",0.0);
_insert_trie(T,arena,DOUBLE,"warningcheck",0.0);
_insert_trie(T,arena,DOUBLE,"smoothing",0.0);
_insert_trie(T,arena,DOUBLE,"autorounding",0.0);
_insert_trie(T,arena,DOUBLE,"glanularity",0.0);
_insert_trie(T,arena,DOUBLE,"glanularity",0.0);
_insert_trie(T,arena,DOUBLE,"fillin",0.0);
_insert_trie(T,arena,DOUBLE,"year",(double)year);
_insert_trie(T,arena,DOUBLE,"month",(double)month);
_insert_trie(T,arena,DOUBLE,"day",(double)day);
_insert_trie(T,arena,DOUBLE,"time",(double)time_in_minutes);
_insert_trie(T,arena,DOUBLE,"charcode",0.0);
_insert_trie(T,arena,DOUBLE,"charext",0.0);
_insert_trie(T,arena,DOUBLE,"charwd",0.0);
_insert_trie(T,arena,DOUBLE,"charht",0.0);
_insert_trie(T,arena,DOUBLE,"chardp",0.0);
_insert_trie(T,arena,DOUBLE,"charic",0.0);
_insert_trie(T,arena,DOUBLE,"chardx",0.0);
_insert_trie(T,arena,DOUBLE,"chardy",0.0);
_insert_trie(T,arena,DOUBLE,"designsize",0.0);
_insert_trie(T,arena,DOUBLE,"hppp",0.0);
_insert_trie(T,arena,DOUBLE,"vppp",0.0);
_insert_trie(T,arena,DOUBLE,"xoffset",0.0);
_insert_trie(T,arena,DOUBLE,"yoffset",0.0);
_insert_trie(T,arena,DOUBLE,"boundarychar",-1.0);
}
/*:856*//*861:*/
#line 19239 "./cweb/weaver.w"

structure->everyjob_token_name= NULL;
/*:861*//*870:*/
#line 19409 "./cweb/weaver.w"

structure->hint= 0;
/*:870*//*880:*/
#line 19547 "./cweb/weaver.w"

structure->variable_types= _new_trie(arena);
/*:880*//*882:*/
#line 19564 "./cweb/weaver.w"

{
int i;

for(i= 0;i<8;i++)
structure->vars[i]= _new_trie(arena);
}
/*:882*//*888:*/
#line 19700 "./cweb/weaver.w"

structure->delimiters= _new_trie(arena);
/*:888*//*893:*/
#line 19788 "./cweb/weaver.w"

structure->outer_tokens= _new_trie(arena);
/*:893*//*897:*/
#line 19914 "./cweb/weaver.w"

structure->def= _new_trie(arena);
structure->vardef= _new_trie(arena);
structure->primarydef= _new_trie(arena);
structure->secondarydef= _new_trie(arena);
structure->tertiarydef= _new_trie(arena);
/*:897*//*908:*/
#line 20278 "./cweb/weaver.w"

structure->macros= _new_trie(arena);
/*:908*/
#line 18300 "./cweb/weaver.w"

/*852:*/
#line 18998 "./cweb/weaver.w"

if(parent==NULL){
run_statements(structure);
}
/*:852*/
#line 18301 "./cweb/weaver.w"

if(parent!=NULL)
parent->child= structure;
else
structure->child= NULL;
return structure;
error_no_file:
fprintf(stderr,"ERROR (0): File %s don't exist.\n",filename);
return NULL;
error_no_memory:
fprintf(stderr,"ERROR: Not enough memory to parse METAFONT "
"source. Please, increase the value of W_%s_MEMORY "
"at conf/conf.h.\n",(arena==_user_arena)?"MAX":"INTERNAL");
return NULL;
}
/*:832*/
#line 18137 "./cweb/weaver.w"

/*:818*/
