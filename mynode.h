#ifndef mynode_h
#define mynode_h
#include <vector>
#include "mymesh.h"
class mynode
{
		std::vector<mynode*> childnodes;
		myMesh* mesh;
public:
		int render();
		int setMesh(myMesh* mesh);
		int addNode(mynode* child);
		mynode();
		~mynode();
};

#endif