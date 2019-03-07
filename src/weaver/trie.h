/*213:*/
#line 5077 "./cweb/weaver.w"

#ifndef _trie_h_
#define _trie_h_
#ifdef __cplusplus
extern"C"{
#endif
#include "weaver.h"
/*66:*/
#line 1950 "./cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 5084 "./cweb/weaver.w"

/*216:*/
#line 5125 "./cweb/weaver.w"

#define INT    1
#define DOUBLE 2
#define VOID_P 3
/*:216*//*217:*/
#line 5133 "./cweb/weaver.w"

struct _trie{
char*string;
bool leaf;
struct _trie*child[256];
struct _trie*parent;
union{
int integer;
double real;
void*generic;
}value;
};
/*:217*//*218:*/
#line 5154 "./cweb/weaver.w"

struct _trie*_new_trie(void*arena);
/*:218*//*220:*/
#line 5188 "./cweb/weaver.w"

void _insert_trie(struct _trie*tree,void*arena,int type,char*name,...);
/*:220*//*224:*/
#line 5333 "./cweb/weaver.w"

bool _search_trie(struct _trie*tree,int type,char*name,...);
/*:224*//*226:*/
#line 5406 "./cweb/weaver.w"

void _remove_trie(struct _trie*tree,char*name);
/*:226*//*228:*/
#line 5438 "./cweb/weaver.w"

void _map_trie(void(*f)(void*),struct _trie*tree);
/*:228*//*230:*/
#line 5462 "./cweb/weaver.w"

#if W_DEBUG_LEVEL >= 1
void _debug_trie_values(char*prefix,struct _trie*tree);
#endif
/*:230*/
#line 5085 "./cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:213*/
