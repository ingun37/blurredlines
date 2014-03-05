#include <stdio.h>
#include <stdlib.h>
#include "myshader.h"

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
		
		printf("%s\n",source);

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