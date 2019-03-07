/*715:*/
#line 16050 "./cweb/weaver.w"

#include "weaver.h"
/*717:*/
#line 16065 "./cweb/weaver.w"

#if W_TARGET == W_ELF
#include <sys/stat.h>   
#include <sys/types.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <pwd.h>  
#include "../misc/sqlite/sqlite3.h"
#endif
/*:717*/
#line 16052 "./cweb/weaver.w"

/*718:*/
#line 16079 "./cweb/weaver.w"

#if W_TARGET == W_ELF
static sqlite3*database;
#endif
/*:718*/
#line 16053 "./cweb/weaver.w"


/*720:*/
#line 16099 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _initialize_database(void){
char path[256];
size_t path_length= 0,w_prog_length= strlen(W_PROG);
int ret;
char*p,*zErrMsg= NULL;


p= getenv("HOME");
if(p!=NULL){
path_length= strlen(p);
if(path_length+2*w_prog_length+17> 255){
fprintf(stderr,"ERROR: Path too long: %s/.weaver_data/%s/%s.db\n",p,
W_PROG,W_PROG);
exit(1);
}
memcpy(path,p,path_length+1);
}
else{


struct passwd*pw= getpwuid(getuid());
if(pw!=NULL){
path_length= strlen(pw->pw_dir);
if(path_length+2*w_prog_length+17> 255){
fprintf(stderr,"ERROR: Path too long: %s/.weaver_data/%s/%s.db\n",p,
W_PROG,W_PROG);
exit(1);
}
memcpy(path,pw->pw_dir,path_length+1);
}
else{

fprintf(stderr,
"WARNING (0): Couldn't get home directory. Saving data in /tmp."
"\n");
path_length= 4;
memcpy(path,"/tmp",5);
}
}

memcpy(&path[path_length],"/.weaver_data/",15);
path_length+= 14;
mkdir(path,0755);

memcpy(&path[path_length],W_PROG,w_prog_length+1);
path_length+= w_prog_length;
memcpy(&path[path_length],"/",2);
path_length++;
mkdir(path,0755);

memcpy(&path[path_length],W_PROG,w_prog_length+1);
path_length+= w_prog_length;
memcpy(&path[path_length],".db",4);
path_length+= 3;

ret= sqlite3_open(path,&database);
if(ret!=SQLITE_OK){
fprintf(stderr,
"WARNING (0): Can't create or read database %s. "
"Data won't be saved: %s\n",
path,
sqlite3_errmsg(database));
}

ret= sqlite3_exec(database,
"CREATE TABLE IF NOT EXISTS "
"int_data(name TEXT PRIMARY KEY, value INT);",
NULL,NULL,&zErrMsg);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): SQL error: %s\n ",zErrMsg);
sqlite3_free(zErrMsg);
}

if(ret==SQLITE_OK){
ret= sqlite3_exec(database,
"CREATE TABLE IF NOT EXISTS "
"float_data(name TEXT PRIMARY KEY, value REAL);",
NULL,NULL,&zErrMsg);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): SQL error: %s\n ",zErrMsg);
sqlite3_free(zErrMsg);
}
}

if(ret==SQLITE_OK){
ret= sqlite3_exec(database,
"CREATE TABLE IF NOT EXISTS "
"string_data(name TEXT PRIMARY KEY, value TEXT);",
NULL,NULL,&zErrMsg);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): SQL error: %s\n ",zErrMsg);
sqlite3_free(zErrMsg);
}
}
}
#endif
/*:720*//*723:*/
#line 16218 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _finalize_database(void){
sqlite3_close(database);
}
#endif
/*:723*//*729:*/
#line 16262 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _write_integer(char*name,int value){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"INSERT OR REPLACE INTO int_data VALUES (?, ?);",
-1,&stmt,0);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_bind_int(stmt,2,value);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_step(stmt);
if(ret!=SQLITE_DONE){
fprintf(stderr,"WARNING (0): Possible problem saving data.\n");
return;
}

sqlite3_finalize(stmt);
}
#endif
/*:729*//*730:*/
#line 16301 "./cweb/weaver.w"

#if W_TARGET == W_WEB
void _write_integer(char*name,int value){
EM_ASM_({
document.cookie= "int_"+Pointer_stringify($0)+"="+$1+
"; expires=Fri, 31 Dec 9999 23:59:59 GMT";
},name,value);
}
#endif
/*:730*//*734:*/
#line 16334 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _write_float(char*name,float value){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"INSERT OR REPLACE INTO float_data VALUES (?, ?);",
-1,&stmt,0);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_bind_double(stmt,2,value);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_step(stmt);
if(ret!=SQLITE_DONE){
fprintf(stderr,"WARNING (0): Possible problem saving data.\n");
return;
}

sqlite3_finalize(stmt);
}
#endif
/*:734*//*735:*/
#line 16373 "./cweb/weaver.w"

#if W_TARGET == W_WEB
void _write_float(char*name,float value){
EM_ASM_({
document.cookie= "float_"+Pointer_stringify($0)+"="+$1+
"; expires=Fri, 31 Dec 9999 23:59:59 GMT";
},name,value);
}
#endif
/*:735*//*739:*/
#line 16402 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _write_string(char*name,char*value){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"INSERT OR REPLACE INTO string_data VALUES (?, ?);",
-1,&stmt,0);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_bind_text(stmt,2,value,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
fprintf(stderr,"WARNING (0): Can't save data.\n");
return;
}

ret= sqlite3_step(stmt);
if(ret!=SQLITE_DONE){
fprintf(stderr,"WARNING (0): Possible problem saving data.\n");
return;
}

sqlite3_finalize(stmt);
}
#endif
/*:739*//*740:*/
#line 16441 "./cweb/weaver.w"

#if W_TARGET == W_WEB
void _write_string(char*name,char*value){
EM_ASM_({
document.cookie= "string_"+Pointer_stringify($0)+"="+
Pointer_stringify($1)+"; expires=Fri, 31 Dec 9999 23:59:59 GMT";
},name,value);
}
#endif
/*:740*//*745:*/
#line 16483 "./cweb/weaver.w"

#if W_TARGET == W_ELF
bool _read_integer(char*name,int*value){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"SELECT value FROM int_data WHERE name = ?;",
-1,&stmt,0);
if(ret!=SQLITE_OK){
return false;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
return false;
}

ret= sqlite3_step(stmt);
if(ret==SQLITE_ROW){
*value= sqlite3_column_int(stmt,0);
sqlite3_finalize(stmt);
return true;
}
else{
sqlite3_finalize(stmt);
return false;
}
}
#endif
/*:745*//*746:*/
#line 16517 "./cweb/weaver.w"

#if W_TARGET == W_ELF
bool _read_float(char*name,float*value){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"SELECT value FROM float_data WHERE name = ?;",
-1,&stmt,0);
if(ret!=SQLITE_OK){
return false;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
return false;
}

ret= sqlite3_step(stmt);
if(ret==SQLITE_ROW){
*value= sqlite3_column_double(stmt,0);
sqlite3_finalize(stmt);
return true;
}
else{
sqlite3_finalize(stmt);
return false;
}
}
#endif
/*:746*//*747:*/
#line 16554 "./cweb/weaver.w"

#if W_TARGET == W_ELF
bool _read_string(char*name,char*value,int size){
int ret;
sqlite3_stmt*stmt;
const unsigned char*p;

ret= sqlite3_prepare_v2(database,
"SELECT value FROM string_data WHERE name = ?;",
-1,&stmt,0);
if(ret!=SQLITE_OK){
return false;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
return false;
}

ret= sqlite3_step(stmt);
if(ret==SQLITE_ROW){
p= sqlite3_column_text(stmt,0);
strncpy(value,(const char*)p,size);
sqlite3_finalize(stmt);
return true;
}
else{
sqlite3_finalize(stmt);
return false;
}
}
#endif
/*:747*//*748:*/
#line 16590 "./cweb/weaver.w"

#if W_TARGET == W_WEB
bool _read_integer(char*name,int*value){

int exists= EM_ASM_INT({
var nameEQ= "int_"+Pointer_stringify($0)+"=";
var ca= document.cookie.split(';');
for(var i= 0;i<ca.length;i++){
var c= ca[i];
while(c.charAt(0)==' ')c= c.substring(1,c.length);
if(c.indexOf(nameEQ)==0)
return 1;
}
return 0;
},name);
if(!exists)
return false;

*value= EM_ASM_INT({
var nameEQ= "int_"+Pointer_stringify($0)+"=";
var ca= document.cookie.split(';');
for(var i= 0;i<ca.length;i++){
var c= ca[i];
while(c.charAt(0)==' ')c= c.substring(1,c.length);
if(c.indexOf(nameEQ)==0)
return parseInt(c.substring(nameEQ.length,c.length),10);
}
},name);
return true;
}
#endif
/*:748*//*749:*/
#line 16627 "./cweb/weaver.w"

#if W_TARGET == W_WEB
bool _read_float(char*name,float*value){

int exists= EM_ASM_INT({
var nameEQ= "float_"+Pointer_stringify($0)+"=";
var ca= document.cookie.split(';');
for(var i= 0;i<ca.length;i++){
var c= ca[i];
while(c.charAt(0)==' ')c= c.substring(1,c.length);
if(c.indexOf(nameEQ)==0)
return 1;
}
return 0;
},name);
if(!exists)
return false;

*value= EM_ASM_DOUBLE({
var nameEQ= "float_"+Pointer_stringify($0)+"=";
var ca= document.cookie.split(';');
for(var i= 0;i<ca.length;i++){
var c= ca[i];
while(c.charAt(0)==' ')c= c.substring(1,c.length);
if(c.indexOf(nameEQ)==0)
return parseInt(c.substring(nameEQ.length,c.length),10);
}
},name);
return true;
}
#endif
/*:749*//*750:*/
#line 16664 "./cweb/weaver.w"

#if W_TARGET == W_WEB
bool _read_string(char*name,char*value,int size){

int exists= EM_ASM_INT({
var nameEQ= "string_"+Pointer_stringify($0)+"=";
var ca= document.cookie.split(';');
for(var i= 0;i<ca.length;i++){
var c= ca[i];
while(c.charAt(0)==' ')c= c.substring(1,c.length);
if(c.indexOf(nameEQ)==0)
return 1;
}
return 0;
},name);
if(!exists)
return false;

EM_ASM_({
var nameEQ= "string_"+Pointer_stringify($0)+"=";
var ca= document.cookie.split(';');
for(var i= 0;i<ca.length;i++){
var c= ca[i];
while(c.charAt(0)==' ')c= c.substring(1,c.length);
if(c.indexOf(nameEQ)==0){
stringToUTF8(c.substring(nameEQ.length,c.length),$1,$2);
}
}
},name,value,size);
return true;
}
#endif
/*:750*//*755:*/
#line 16729 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _delete_integer(char*name){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"DELETE FROM int_data WHERE name = ?;",
-1,&stmt,0);
if(ret!=SQLITE_OK){
return;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
sqlite3_finalize(stmt);
return;
}

ret= sqlite3_step(stmt);
sqlite3_finalize(stmt);
}
#endif
/*:755*//*756:*/
#line 16756 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _delete_float(char*name){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"DELETE FROM float_data WHERE name = ?;",
-1,&stmt,0);
if(ret!=SQLITE_OK){
return;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
sqlite3_finalize(stmt);
return;
}

ret= sqlite3_step(stmt);
sqlite3_finalize(stmt);
}
#endif
/*:756*//*757:*/
#line 16783 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _delete_string(char*name){
int ret;
sqlite3_stmt*stmt;

ret= sqlite3_prepare_v2(database,
"DELETE FROM string_data WHERE name = ?;",
-1,&stmt,0);
if(ret!=SQLITE_OK){
return;
}

ret= sqlite3_bind_text(stmt,1,name,-1,SQLITE_STATIC);
if(ret!=SQLITE_OK){
sqlite3_finalize(stmt);
return;
}

ret= sqlite3_step(stmt);
sqlite3_finalize(stmt);
}
#endif
/*:757*//*758:*/
#line 16810 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _delete_all(void){
sqlite3_exec(database,"DELETE * FROM int_data; ",NULL,NULL,NULL);
sqlite3_exec(database,"DELETE * FROM float_data; ",NULL,NULL,NULL);
sqlite3_exec(database,"DELETE * FROM string_data; ",NULL,NULL,NULL);
}
#endif
/*:758*//*759:*/
#line 16823 "./cweb/weaver.w"

#if W_TARGET == W_WEB
void _delete_integer(char*name){
EM_ASM_({
document.cookie= "int_"+Pointer_stringify($0)+"=0"+
";expires=Thu, 01 Jan 1970 00:00:01 GMT";
},name);
}
void _delete_float(char*name){
EM_ASM_({
document.cookie= "float_"+Pointer_stringify($0)+"=0"+
";expires=Thu, 01 Jan 1970 00:00:01 GMT";
},name);
}
void _delete_string(char*name){
EM_ASM_({
document.cookie= "string_"+Pointer_stringify($0)+"=0"+
";expires=Thu, 01 Jan 1970 00:00:01 GMT";
},name);
}
#endif
/*:759*//*760:*/
#line 16849 "./cweb/weaver.w"

#if W_TARGET == W_WEB
void _delete_all(void){
EM_ASM({
var cookies= document.cookie.split(";");
for(var i= 0;i<cookies.length;i++){
var cookie= cookies[i];
var eqPos= cookie.indexOf("=");
var name= eqPos> -1?cookie.substr(0,eqPos):cookie;
document.cookie= name+"=;expires=Thu, 01 Jan 1970 00:00:00 GMT";
}
});
}
#endif
/*:760*/
#line 16055 "./cweb/weaver.w"

/*:715*/
