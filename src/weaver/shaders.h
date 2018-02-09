/*439:*/
#line 9629 "cweb/weaver.w"

#ifndef _shaders_h_
#define _shaders_h_
#ifdef __cplusplus
extern"C"{
#endif
#include "weaver.h"
/*66:*/
#line 1927 "cweb/weaver.w"

#include "conf_begin.h"
#include "../../conf/conf.h"
#include "conf_end.h"
/*:66*/
#line 9636 "cweb/weaver.w"

/*447:*/
#line 9739 "cweb/weaver.w"

extern char _vertex_interface[];
extern char _fragment_interface[];
struct _shader _default_interface_shader;
/*:447*//*449:*/
#line 9756 "cweb/weaver.w"

GLuint _compile_shader(char*source,bool vertex);
#define _compile_vertex_shader(source) _compile_shader(source, true)
#define _compile_fragment_shader(source) _compile_shader(source, false)
/*:449*//*451:*/
#line 9803 "cweb/weaver.w"

GLuint _link_and_clean_shaders(GLuint vertex,GLuint fragment);
/*:451*//*456:*/
#line 9917 "cweb/weaver.w"

struct _shader{
bool initialized;
GLuint program_shader;
char name[128];

GLint _uniform_object_color,_uniform_model_view,_uniform_object_size;
GLint _uniform_time,_uniform_texture1,_uniform_integer;

GLint _attribute_vertex_position;
char*vertex_source,*fragment_source;
#if W_TARGET == W_ELF


ino_t vertex_inode,fragment_inode;
#endif
};

#if W_TARGET == W_ELF


struct _shader*_shader_list;
#else


#include "../../.hidden_code/shader.h"
#endif
/*:456*//*461:*/
#line 10133 "cweb/weaver.w"

void _compile_and_insert_new_shader(char*dir,int position);
/*:461*//*491:*/
#line 10911 "cweb/weaver.w"

extern char _vertex_interface_texture[];
extern char _fragment_interface_texture[];
struct _shader _framebuffer_shader;
GLfloat _framebuffer_matrix[16];
/*:491*//*497:*/
#line 11095 "cweb/weaver.w"

void _change_resolution(int resolution_x,int resolution_y);
/*:497*//*502:*/
#line 11189 "cweb/weaver.w"

void _change_final_shader(int type);
/*:502*//*658:*/
#line 14514 "cweb/weaver.w"

extern char _vertex_image_interface[];
extern char _fragment_image_interface[];
struct _shader _image_interface_shader;
/*:658*/
#line 9637 "cweb/weaver.w"

#ifdef __cplusplus
}
#endif
#endif
/*:439*/
