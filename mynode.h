#ifndef mynode_h
#define mynode_h
#include <vector>

class myMesh;
class mynode
{
		std::vector<mynode*> childnodes;
		
		float localpos[3];
public:
		myMesh* mesh;
		int render();
		int setMesh(myMesh* mesh);
		int addNode(mynode* child);
		int setLocalPos(float x, float y, float z);
		mynode* getChildAt(int n);
		mynode();
		~mynode();
};

#endif