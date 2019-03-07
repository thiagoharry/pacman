/*817:*/
#line 18110 "./cweb/weaver.w"

#ifndef _metafont_h_
#define _metafont_h_
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
#line 18117 "./cweb/weaver.w"

#if W_DEBUG_LEVEL == 0 && defined(W_DEBUG_METAFONT)
#error "Use W_DEBUG_METAFONT only with W_DABUG_LEVEL > 0"
#endif
/*820:*/
#line 18143 "./cweb/weaver.w"


/*:820*//*826:*/
#line 18190 "./cweb/weaver.w"

void _initialize_metafont(void);
/*:826*//*831:*/
#line 18262 "./cweb/weaver.w"

struct metafont*_new_metafont(struct metafont*,char*);
/*:831*/
#line 18121 "./cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:817*/
