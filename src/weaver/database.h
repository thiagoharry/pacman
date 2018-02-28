/*688:*/
#line 15360 "./cweb/weaver.w"

#ifndef _database_h_
#define _database_h_
#ifdef __cplusplus
extern"C"{
#endif
#include "weaver.h"
/*66:*/
#line 1927 "./cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 15367 "./cweb/weaver.w"

/*693:*/
#line 15417 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _initialize_database(void);
#endif
/*:693*//*696:*/
#line 15531 "./cweb/weaver.w"

#if W_TARGET == W_ELF
void _finalize_database(void);
#endif
/*:696*//*700:*/
#line 15566 "./cweb/weaver.w"

void _write_integer(char*name,int value);
/*:700*//*705:*/
#line 15638 "./cweb/weaver.w"

void _write_float(char*name,float value);
/*:705*//*710:*/
#line 15706 "./cweb/weaver.w"

void _write_string(char*name,char*value);
/*:710*//*716:*/
#line 15776 "./cweb/weaver.w"

bool _read_integer(char*name,int*value);
bool _read_float(char*name,float*value);
bool _read_string(char*name,char*value,int n);
/*:716*//*726:*/
#line 16024 "./cweb/weaver.w"

void _delete_integer(char*name);
void _delete_float(char*name);
void _delete_string(char*name);
void _delete_all(void);
/*:726*/
#line 15368 "./cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:688*/
