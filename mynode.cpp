#include "mynode.h"
#include <string.h>
#include "mymesh.h"
#include <OpenGL/gl.h>

mynode::mynode()
{
		mesh = NULL;
		memset(localpos, 0, sizeof(float)*3);
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
		
		glPushMatrix();
		
		//printf("\n local pos is... %f %f %f\n", localpos[0], localpos[1], localpos[2]);
		glTranslatef(localpos[0], localpos[2], localpos[1]);
		
		if(mesh)
				mesh->render();
		
		for(i=0;i<childnodes.size();i++)
		{
				//								if(i==5)
				childnodes[i]->render();
		}
		
		glPopMatrix();

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

int mynode::setLocalPos(float x, float y, float z)
{
		localpos[0] = x;
		localpos[1] = y;
		localpos[2] = z;
}

mynode* mynode::getChildAt(int n)
{
		return childnodes[n];
}