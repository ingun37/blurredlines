#include "myprogrammer.h"
#include "myshader.h"
shaderInfo* myprogrammer::currentlyRunningInfo = NULL;
unsigned int myprogrammer::currentlyRunningProgram = 0;

int myprogrammer::makeAndManageProgram(char* vpath, char* fpath)
{
		unsigned int program;
		shaderInfo info;
		
		info.v = makeVertexShader(vpath,NULL);
		info.f = makeFragmentShader(fpath,NULL);
		program = makeProgram(info.v, info.f);
		
		char eyeposname[] = "eyepos";
		info.leyepos = glGetUniformLocation (program, eyeposname);
		
		char texname[] = "tex";
		info.ltex = glGetUniformLocation(program, texname);
		
		if(program > 0)
				programs[program] = info;
		
		return program;
}

int myprogrammer::useProgram(int program)
{
		currentlyRunningInfo = &(programs[program]);
		
		if(currentlyRunningInfo)
		{
				printf("using program : %d\n",program);
				currentlyRunningProgram = program;
				printf("currently running program info\nltex : %d\nleyepos : %d\nv : %d\nf : %d\n", currentlyRunningInfo->ltex, currentlyRunningInfo->leyepos, currentlyRunningInfo->v, currentlyRunningInfo->f);
				glUseProgram(program);
		}
		else
		{
				printf("program : %d does not exists\n",program);
		}
}

myprogrammer::myprogrammer()
{
		currentlyRunningInfo = NULL;
		currentlyRunningProgram = 0;
}

myprogrammer::~myprogrammer()
{
		unsigned int programid;
		shaderInfo infoid;
		for( mapProgram::iterator it = programs.begin(); it!=programs.end();it++ )
		{
				if(it->first > 0)
				{
						programid = it->first;
						infoid = it->second;
						
						printf("deleting program : %d   v:%d   f:%d\n",programid, infoid.v,infoid.f);
						
						glDetachShader(programid, infoid.v);
						glDetachShader(programid, infoid.f);
						glDeleteShader(infoid.v);
						glDeleteShader(infoid.f);
						glDeleteProgram(programid);
				}
		}
}