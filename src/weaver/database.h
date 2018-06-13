/*688:*/
#line 15492 "cweb/weaver.w"

#ifndef _database_h_
#define _database_h_
#ifdef __cplusplus
extern"C"{
#endif
#include "weaver.h"
/*66:*/
#line 1928 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 15499 "cweb/weaver.w"

/*693:*/
#line 15549 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _initialize_database(void);
#endif
/*:693*//*696:*/
#line 15663 "cweb/weaver.w"

#if W_TARGET == W_ELF
void _finalize_database(void);
#endif
/*:696*//*700:*/
#line 15698 "cweb/weaver.w"

void _write_integer(char*name,int value);
/*:700*//*705:*/
#line 15770 "cweb/weaver.w"

void _write_float(char*name,float value);
/*:705*//*710:*/
#line 15838 "cweb/weaver.w"

void _write_string(char*name,char*value);
/*:710*//*716:*/
#line 15908 "cweb/weaver.w"

bool _read_integer(char*name,int*value);
bool _read_float(char*name,float*value);
bool _read_string(char*name,char*value,int n);
/*:716*//*726:*/
#line 16156 "cweb/weaver.w"

void _delete_integer(char*name);
void _delete_float(char*name);
void _delete_string(char*name);
void _delete_all(void);
/*:726*/
#line 15500 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:688*/
