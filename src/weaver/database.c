/*689:*/
#line 15506 "cweb/weaver.w"

#include "weaver.h"
/*691:*/
#line 15521 "cweb/weaver.w"

#if W_TARGET == W_ELF
#include <sys/stat.h>   
#include <sys/types.h>  
#include <unistd.h>  
#include <stdlib.h>  
#include <pwd.h>  
#include "../misc/sqlite/sqlite3.h"
#endif
/*:691*/
#line 15508 "cweb/weaver.w"

/*692:*/
#line 15535 "cweb/weaver.w"

#if W_TARGET == W_ELF
static sqlite3*database;
#endif
/*:692*/
#line 15509 "cweb/weaver.w"


/*694:*/
#line 15555 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _initialize_database(void){
char path[256];
int ret,size;
char*p,*zErrMsg= NULL;


p= getenv("HOME");
if(p!=NULL){
size= strlen(p);
strncpy(path,p,255);
path[255]= '\0';
}
else{


struct passwd*pw= getpwuid(getuid());
if(pw!=NULL){
size= strlen(pw->pw_dir);
strncpy(path,pw->pw_dir,255);
path[255]= '\0';
}
else{

fprintf(stderr,
"WARNING (0): Couldn't get home directory. Saving data in /tmp."
"\n");
size= 4;
strncpy(path,"/tmp",255);
}
}

if(size+9<256){
size+= 14;
strcat(path,"/.weaver_data/");
mkdir(path,0755);
}

size+= strlen(W_PROG)+1;
if(size<256){
strcat(path,W_PROG);
strcat(path,"/");
mkdir(path,0755);
}

size+= strlen(W_PROG)+3;
if(size<256){
strcat(path,W_PROG);
strcat(path,".db");
}

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
/*:694*//*697:*/
#line 15669 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _finalize_database(void){
sqlite3_close(database);
}
#endif
/*:697*//*703:*/
#line 15713 "cweb/weaver.w"

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
/*:703*//*704:*/
#line 15752 "cweb/weaver.w"

#if W_TARGET == W_WEB
void _write_integer(char*name,int value){
EM_ASM_({
document.cookie= "int_"+Pointer_stringify($0)+"="+$1+
"; expires=Fri, 31 Dec 9999 23:59:59 GMT";
},name,value);
}
#endif
/*:704*//*708:*/
#line 15785 "cweb/weaver.w"

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
/*:708*//*709:*/
#line 15824 "cweb/weaver.w"

#if W_TARGET == W_WEB
void _write_float(char*name,float value){
EM_ASM_({
document.cookie= "float_"+Pointer_stringify($0)+"="+$1+
"; expires=Fri, 31 Dec 9999 23:59:59 GMT";
},name,value);
}
#endif
/*:709*//*713:*/
#line 15853 "cweb/weaver.w"

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
/*:713*//*714:*/
#line 15892 "cweb/weaver.w"

#if W_TARGET == W_WEB
void _write_string(char*name,char*value){
EM_ASM_({
document.cookie= "string_"+Pointer_stringify($0)+"="+
Pointer_stringify($1)+"; expires=Fri, 31 Dec 9999 23:59:59 GMT";
},name,value);
}
#endif
/*:714*//*719:*/
#line 15934 "cweb/weaver.w"

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
/*:719*//*720:*/
#line 15968 "cweb/weaver.w"

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
/*:720*//*721:*/
#line 16005 "cweb/weaver.w"

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
/*:721*//*722:*/
#line 16041 "cweb/weaver.w"

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
/*:722*//*723:*/
#line 16078 "cweb/weaver.w"

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
/*:723*//*724:*/
#line 16115 "cweb/weaver.w"

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
/*:724*//*729:*/
#line 16180 "cweb/weaver.w"

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
/*:729*//*730:*/
#line 16207 "cweb/weaver.w"

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
/*:730*//*731:*/
#line 16234 "cweb/weaver.w"

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
/*:731*//*732:*/
#line 16261 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _delete_all(void){
sqlite3_exec(database,"DELETE * FROM int_data; ",NULL,NULL,NULL);
sqlite3_exec(database,"DELETE * FROM float_data; ",NULL,NULL,NULL);
sqlite3_exec(database,"DELETE * FROM string_data; ",NULL,NULL,NULL);
}
#endif
/*:732*//*733:*/
#line 16274 "cweb/weaver.w"

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
/*:733*//*734:*/
#line 16300 "cweb/weaver.w"

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
/*:734*/
#line 15511 "cweb/weaver.w"

/*:689*/
