#ifndef myshader_h
#define myshader_h

#include <openGL/gl.h>
#define myshaderFailed(R) (R!=0)
#define printOpenGLError() printError(__FILE__,__LINE__)

typedef struct VAOparameterRaw
{
		char varname[128];
		int elementnum;
		GLenum type;
		int willNormalize;
		GLvoid* offset;
		
}VAOparameter;

#ifdef __cplusplus
extern "C"{
#endif
int printError(char* file, int line);
GLuint makeVertexShader(char* path, GLint *perror);
GLuint makeFragmentShader(char* path, GLint *perror);
GLint makeVertexArrayIndexBuffer(char* varname, GLint *location, GLuint program, GLuint* buffer,unsigned short* data, unsigned int arraylen);
GLint makeVertexArrayPositionBufferToAttribute(char* varname, GLint *location, GLuint program, GLuint* buffer, float* data, unsigned int arraylen);
GLint makeVAOBufferToAttribute( VAOparameter* parameters, int parameternum, GLuint program, GLuint* buffer, void* data, int structSize, unsigned int arraylen);
GLuint makeProgram(GLuint attachingVertexShader, GLuint attachingFragmentShader);
#ifdef __cplusplus
}
#endif
#endif