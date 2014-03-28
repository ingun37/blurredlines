#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myshader.h"
#include "lodepng.h"
int printError(char* file, int line)
{
		GLenum glErr;
		int retCode = 0;
		glErr = glGetError();
		if(glErr != GL_NO_ERROR)
		{
				printf("glError in file %s @ line %d: %s",file,line,gluErrorString(glErr));
				retCode = 1;
		}
		return retCode;
}
char* getshadersourcefromfile(char* path,unsigned int* plen)
{
		FILE* fsource = fopen(path,"r");
		char c;
		unsigned int len=0;
		char* source, *pc;
		
		while((c=getc(fsource))!=EOF)
		{
				len++;
		}
		len++;
		source = (char*)malloc(sizeof(char)*len);
		pc = source;
		fseek(fsource,0,SEEK_SET);
		while((c=getc(fsource))!=EOF)
		{
				*(pc++)=c;
		}
		*pc = '\0';
		if(plen) *plen = len;
		
		fclose(fsource);
		
		return source;
}

GLuint makeShader(char* path, GLenum type, GLint* perror)
{
		char* source;
		char** sources;
		char buflog[1024]={0,};
		GLint result;
		GLuint iShader;
		
		
		iShader = glCreateShader(type);
		
		sources = (char**)malloc(sizeof(char*)*1);
		
		source = getshadersourcefromfile(path,NULL);
		
		//printf("%s\n",source);

		sources[0] = source;

		glShaderSource(iShader,1,(const char**)sources,NULL);
		puts("shadersourcecomplete");
		glCompileShader(iShader);
		puts("compile complete");
		glGetShaderiv(iShader,GL_COMPILE_STATUS,&result);
		if(result != GL_TRUE)
		{
				if(perror)
						*perror = result;
				puts("compile error");
				glGetShaderInfoLog(iShader, 1024, NULL, buflog);
				printf("%s\n",buflog);
				return -999;
		}
		free(source);
		free(sources);
		printf("shader %d complete.\n",iShader);
		return iShader;
	
}

GLuint makeVertexShader(char* path, GLint *perror)
{
		return makeShader(path, GL_VERTEX_SHADER, perror);
}

GLuint makeFragmentShader(char* path, GLint *perror)
{
		return makeShader(path, GL_FRAGMENT_SHADER, perror);
}
GLuint makeProgram(GLuint attachingVertexShader, GLuint attachingFragmentShader)
{
		GLuint prog = glCreateProgram();
		printOpenGLError();
		glAttachShader(prog, attachingVertexShader);
		printOpenGLError();
		glAttachShader(prog, attachingFragmentShader);
		printOpenGLError();
		glLinkProgram(prog);
		printOpenGLError();
		
		return prog;
}

GLint makeVertexArrayBufferToAttribute(char* varname, GLint *location, GLuint program, GLuint* buffer, void* data, unsigned int size, unsigned int buffertype, unsigned int elementnum, unsigned int vartype)
{
		if(location == NULL) return 1;
		if(buffer == NULL) return 1;
		//attribute변수 위치 받아오고...
		*location = glGetAttribLocation( program, varname);

		//버퍼 한개 만들어서 아이디 buffer에 집어넣고..
		glGenBuffers(1,buffer);
		printOpenGLError();
		//그 버퍼는 GL_ARRAY_BUFFER형식이라고 말하고..
		glBindBuffer(buffertype, *buffer);
		printOpenGLError();
		//버퍼에 데이타쌓고
		glBufferData(buffertype, size, data, GL_STATIC_DRAW);
		printOpenGLError();
		//키고..
		glEnableVertexAttribArray(*location);
		printOpenGLError();
		//변수형 알려주고..
		glVertexAttribPointer(*location, elementnum, vartype, 0, 0, 0);
		printOpenGLError();
		
		
		return 0;
}
/*
 char varname[128];
 int elementnum;
 GLenum type;
 int willNormalize;
 GLvoid* offset;
 */
static VAOparameter fixedparameters[3];
unsigned int getNumFixedVAOParameters()
{
		return sizeof(fixedparameters)/sizeof(VAOparameter);
}
VAOparameter* getFixedVAOParameters()
{
		static int inited = 0;
		if(inited == 0)
		{
				memset(fixedparameters[0].varname, 0, sizeof(fixedparameters[0].varname));
				strncpy(fixedparameters[0].varname, "position", 8);
				fixedparameters[0].location = 0;
				fixedparameters[0].elementnum = 3;
				fixedparameters[0].type = GL_FLOAT;
				fixedparameters[0].willNormalize = 0;
				fixedparameters[0].offset = (unsigned char*)NULL + 0;
				
				memset(fixedparameters[1].varname, 0, sizeof(fixedparameters[1].varname));
				strncpy(fixedparameters[1].varname, "normal", 6);
				fixedparameters[1].location = 1;
				fixedparameters[1].elementnum = 3;
				fixedparameters[1].type = GL_FLOAT;
				fixedparameters[1].willNormalize = 1;
				fixedparameters[1].offset = (unsigned char*)NULL + (sizeof(float) * 3);
				
				memset(fixedparameters[2].varname, 0, sizeof(fixedparameters[1].varname));
				strncpy(fixedparameters[2].varname, "uv", 2);
				fixedparameters[2].location = 2;
				fixedparameters[2].elementnum = 2;
				fixedparameters[2].type = GL_FLOAT;
				fixedparameters[2].willNormalize = 0;
				fixedparameters[2].offset = (unsigned char*)NULL + (sizeof(float) * 6);
				
				inited = 1;
		}
		return fixedparameters;
}
GLint makeVAOBufferToAttribute( VAOparameter* parameters, int parameternum, GLuint* buffer, void* data, int structSize, unsigned int arraylen)
{
		int i;
		//int location;
		/*
		glGenBuffers(1, buffer);
		glBindBuffer(GL_ARRAY_BUFFER, *buffer);
		glBufferData(GL_ARRAY_BUFFER, arraylen * structSize, data, GL_STATIC_DRAW);
		 */
		makeVAOBufferOnly(buffer, data, structSize, arraylen);
		for(i=0;i<parameternum;i++)
		{
				//location = glGetAttribLocation(program, parameters[i].varname);

				glEnableVertexAttribArray(parameters[i].location);

				glBindBuffer(GL_ARRAY_BUFFER, *buffer);

				glVertexAttribPointer(parameters[i].location, parameters[i].elementnum, parameters[i].type,parameters[i].willNormalize, structSize, parameters[i].offset);
		}
}

GLint makeVAOBufferOnly( GLuint* buffer, void* data, int structSize, unsigned int arraylen)
{
		int i;
		int location;
		
		glGenBuffers(1, buffer);
		glBindBuffer(GL_ARRAY_BUFFER, *buffer);
		glBufferData(GL_ARRAY_BUFFER, arraylen * structSize, data, GL_STATIC_DRAW);
}

GLint makeVertexArrayIndexBuffer( GLuint* buffer,unsigned short* data, unsigned int arraylen)
{
		//return makeVertexArrayBufferToAttribute(varname, location, program, buffer, data, arraylen * sizeof(int), GL_ELEMENT_ARRAY_BUFFER, 1, GL_FLOAT);
		if(buffer == NULL) return 1;

		//attribute변수 위치 받아오고...
		//*location = glGetAttribLocation(program,varname);
		//printf("%d %s %d\n", program, varname, *location);
		//버퍼 한개 만들어서 아이디 buffer에 집어넣고..
		glGenBuffers(1,buffer);
		printOpenGLError();
		//그 버퍼는 GL_ARRAY_BUFFER형식이라고 말하고..
		printf("binding index buffer : %d\n", *buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *buffer);
		printOpenGLError();
		//버퍼에 데이타쌓고
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, arraylen * sizeof(short), data, GL_STATIC_DRAW);
		printOpenGLError();
		//키고
		//glEnableVertexAttribArray(*location);
		//printOpenGLError();
		//변수형 알려주고..
		//glVertexAttribPointer(*location, 1, GL_UNSIGNED_SHORT, 0, 0, 0);
		//printOpenGLError();
}

GLuint makeTexture(char* path, unsigned int *pwidth, unsigned int *pheight)
{
		unsigned error;
		unsigned char* image;
		unsigned width, height;
		GLuint textureID;
		//lodepng사이트 가보면 파일을 메모리로 먼저 로드하고 하는 방법도있음.
		error = lodepng_decode32_file(&image, &width, &height, path);
		if(error)
		{
				printf("lodepng error : %u : %s\n", error, lodepng_error_text(error));
				return -1;
		}
		else
				printf("loadpng success width : %d height : %d\n",width,height);

		if(pwidth)
				*pwidth = width;
		if(pheight)
				*pheight = height;
		
		glGenTextures(1,&textureID);
		printf("gen tex success texid : %d\n", textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		
		puts("freeing image.....");
		free(image);
		return textureID;
		
}