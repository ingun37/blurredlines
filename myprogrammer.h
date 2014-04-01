#ifndef programmer_h
#define programmer_h
#include <map>

typedef struct shaderInforaw
{
		unsigned int v;
		unsigned int f;
		
		unsigned int leyepos;
		unsigned int ltex;
}shaderInfo;

typedef std::map<unsigned int, shaderInfo> mapProgram;
class myprogrammer
{
		mapProgram programs;
public:
		static unsigned int currentlyRunningProgram;
		static shaderInfo* currentlyRunningInfo;
		int makeAndManageProgram(char* vpath, char* fpath);
		int useProgram(int program);
		myprogrammer();
		~myprogrammer();
};

#endif