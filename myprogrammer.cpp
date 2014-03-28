#include "myprogrammer.h"
#include "myshader.h"

int myprogrammer::makeAndManageProgram(char* vpath, char* fpath)
{
		unsigned int program;
		shaderpair pair;
		
		pair.v = makeVertexShader(vpath,NULL);
		pair.f = makeFragmentShader(fpath,NULL);
		program = makeProgram(pair.v, pair.f);
		
		if(program > 0)
				programs[program] = pair;
		
		return program;
}

myprogrammer::myprogrammer()
{
}

myprogrammer::~myprogrammer()
{
		unsigned int programid;
		shaderpair pairid;
		for( mapProgram::iterator it = programs.begin(); it!=programs.end();it++ )
		{
				if(it->first > 0)
				{
						programid = it->first;
						pairid = it->second;
						
						printf("deleting program : %d   v:%d   f:%d\n",programid, pairid.v,pairid.f);
						
						glDetachShader(programid, pairid.v);
						glDetachShader(programid, pairid.f);
						glDeleteShader(pairid.v);
						glDeleteShader(pairid.f);
						glDeleteProgram(programid);
				}
		}
}