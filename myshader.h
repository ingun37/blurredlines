#ifndef myshader_h
#define myshader_h

#include <openGL/gl.h>
#define myshaderFailed(R) (R!=0)
#define printOpenGLError() printError(__FILE__,__LINE__)

typedef struct VAOparameterRaw
{
		char varname[128];
		int location;
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
GLint makeVertexArrayIndexBuffer( GLuint* buffer,unsigned short* data, unsigned int arraylen);

GLint makeVAOBufferToAttribute( VAOparameter* parameters, int parameternum, GLuint program, GLuint* buffer, void* data, int structSize, unsigned int arraylen);

GLint makeVAOBufferOnly( GLuint* buffer, void* data, int structSize, unsigned int arraylen);
VAOparameter* getFixedVAOParameters();
GLuint makeProgram(GLuint attachingVertexShader, GLuint attachingFragmentShader);
#ifdef __cplusplus
}
#endif
#endif