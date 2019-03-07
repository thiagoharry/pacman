/*68:*/
#line 1974 "./cweb/weaver.w"

#ifndef W_DEBUG_LEVEL
#define W_DEBUG_LEVEL 0
#endif
#if W_TARGET == W_WEB && defined(W_MULTITHREAD)
#undef W_MULTITHREAD
#warning "Threads won't be used when compiling the game to a web browser."
#endif
#ifndef W_SOURCE
#warning "Not W_SOURCE defined at conf/conf.h. Assuming W_C (C)."
#define W_SOURCE W_C
#endif
#ifndef W_TARGET
#warning "Not W_TARGET defined at conf/conf.h. Assuming W_ELF (linux executable)."
#define W_TARGET W_ELF
#endif
/*:68*//*93:*/
#line 2468 "./cweb/weaver.w"

#ifndef W_MAX_MEMORY
#warning "W_MAX_MEMORY not defined at conf/conf.h. Assuming the smallest value possible"
#define W_MAX_MEMORY 1
#endif
#ifndef W_INTERNAL_MEMORY
#define W_INTERNAL_MEMORY \
  (((W_MAX_MEMORY)/10000> 16384)?((W_MAX_MEMORY)/10000):(16384))
#endif
#if !defined(W_WEB_MEMORY) && W_TARGET == W_ELF
#warning "W_WEB_MEMORY not defined at conf/conf.h."
#endif
#ifndef W_MAX_SUBLOOP
#warning "W_MAX_SUBLOOP not defined at conf/conf.h. Assuming 1."
#define W_MAX_SUBLOOP 1
#endif
/*:93*//*235:*/
#line 5549 "./cweb/weaver.w"

#ifndef W_DEFAULT_COLOR
#define W_DEFAULT_COLOR 0.0, 0.0, 0.0
#endif
#ifndef W_HEIGHT
#define W_HEIGHT 0
#endif
#ifndef W_WIDTH
#define W_WIDTH 0
#endif
/*:235*//*354:*/
#line 7840 "./cweb/weaver.w"

#ifndef W_INSTALL_DATA
#define W_INSTALL_DATA "/usr/share/games/"W_PROG
#endif
#ifndef W_INSTALL_PROG
#define W_INSTALL_PROG "/usr/games/"
#endif
#ifndef W_PLUGIN_PATH
#if W_DEBUG_LEVEL == 0
#define W_PLUGIN_PATH  W_INSTALL_DATA"/plugins"
#else
#define W_PLUGIN_PATH  "compiled_plugins"
#endif
#endif
#ifndef W_MAX_SCHEDULING
#define W_MAX_SCHEDULING 8
#endif
/*:354*//*763:*/
#line 16968 "./cweb/weaver.w"


#ifndef W_MAX_MUSIC
#define W_MAX_MUSIC 1
#endif
/*:763*/
