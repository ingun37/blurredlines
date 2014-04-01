#include "mynode.h"

mynode::mynode()
{
		mesh = NULL;
}
mynode::~mynode()
{
		int i;
		if(mesh)
		{
				for(i=0;i<childnodes.size();i++)
				{
						delete childnodes[i];
				}
				delete mesh;
				mesh=NULL;
		}
}

int mynode::render()
{
		int i;
		for(i=0;i<childnodes.size();i++)
		{
				childnodes[i]->render();
		}
		if(mesh)
				mesh->render();
		return 0;
}

int mynode::setMesh(myMesh* pmesh)
{
		mesh = pmesh;
}

int mynode::addNode(mynode* child)
{
		childnodes.push_back(child);
}

