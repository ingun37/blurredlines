#include <openGL/gl.h>

#ifdef __cplusplus
extern "C"{
#endif
GLuint makeVertexShader(char* path, GLint *perror);
GLuint makeFragmentShader(char* path, GLint *perror);
#ifdef __cplusplus
}
#endif