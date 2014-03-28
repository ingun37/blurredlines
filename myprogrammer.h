#ifndef programmer_h
#define programmer_h
#include <map>
typedef struct shaderpairraw
{
		unsigned int v;
		unsigned int f;
}shaderpair;

typedef std::map<unsigned int, shaderpair> mapProgram;
class myprogrammer
{
		mapProgram programs;
public:
		int makeAndManageProgram(char* vpath, char* fpath);
		myprogrammer();
		~myprogrammer();
};

#endif