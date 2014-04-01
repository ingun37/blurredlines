#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mymesh.h"
#include "materialProperties.h"
#include "Vertex.h"
#include "myshader.h"

myMesh::myMesh()
{
		vertices = NULL;
		vao = 0;
		materialproperties = (MaterialProperties*)malloc(sizeof(MaterialProperties));
		memset(materialproperties, 0, sizeof(MaterialProperties));

		vshader = 0;
		fshader = 0;
		ibo = 0;
		vbo = 0;
		numVertices = 0;
		numIndices = 0;

}

myMesh::~myMesh()
{
		puts("deleteing mesh");
		free(materialproperties);
		this->release();
}
int myMesh::render()
{
		std::list<texidUnitPair*>::iterator itor;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialproperties->diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialproperties->specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialproperties->ambient);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialproperties->shininess);
		

		//텍스쳐들 입혀준다.
		for(itor = texunitpairs.begin(); itor !=texunitpairs.end(); itor++)
		{
				glActiveTexture((*itor)->unit);
				glBindTexture(GL_TEXTURE_2D, (*itor)->texid );
		}

		glBindVertexArrayAPPLE(vao);
		if(ibo > 0)
		{
				glDrawElements(GL_TRIANGLES, numIndices * sizeof(short), GL_UNSIGNED_SHORT, NULL);
		}
		else
		{

				glDrawArrays(GL_TRIANGLES,0,numVertices);
		}
		glBindVertexArrayAPPLE(0);
		return 0;
}

int myMesh::setMaterial( MaterialProperties mat)
{
		memcpy(materialproperties, &mat, sizeof(MaterialProperties));
		return 0;
}
void myMesh::setTexid(unsigned int texid, unsigned int texUnitnum)
{
		texidUnitPair* pair = (texidUnitPair*)malloc(sizeof(texidUnitPair));
		pair->texid = texid;
		pair->unit = texUnitnum;
		texunitpairs.push_back(pair);
		
}
void myMesh::setTexidByPath(char* path, unsigned int texUnitnum)
{
		int texid;
		if(path)
		{
				texid = makeTexture(path,NULL,NULL);
				this->setTexid(texid, texUnitnum);
		}
		
}

unsigned int myMesh::getVnum(){ return numVertices; }
unsigned int myMesh::getInum(){ return numIndices; }
unsigned int myMesh::getVAO()
{
		return vao;
}

int myMesh::setVAO(Vertex* pv, int vnum, unsigned short* pi, int inum)
{
		if(pv == NULL || vnum == 0)
				return -1;
		
		glGenVertexArraysAPPLE(1, &vao);
		glBindVertexArrayAPPLE(vao);
		
		if(1)
		{
				numVertices = vnum;
				vertices = pv;
				makeVAOBufferToAttribute(getFixedVAOParameters(), getNumFixedVAOParameters(), &vbo, pv, sizeof(Vertex), vnum);
		}
		else
		{
				numVertices = vnum;
				vertices = pv;
				makeVAOBufferOnly(&vbo, pv, sizeof(Vertex), vnum);
		}
		
		
		if(pi)
		{
				numIndices = inum;
				indices = pi;
				makeVertexArrayIndexBuffer( &ibo, pi, inum);
		}
		else
		{
				puts("pi is null not making index buffer...");
		}
		glBindVertexArrayAPPLE(0);
		
		return 0;
}

void myMesh::release()
{

		if(vbo)
		{
				glDeleteBuffers(1,&vbo);
				vbo = 0;
		}
		if(ibo)
		{
				glDeleteBuffers(1,&ibo);
				ibo = 0;
		}
		if(vao)
		{
				glDeleteVertexArraysAPPLE(1,&vao);
				vao = 0;
		}
		
		std::list<texidUnitPair*>::iterator itor;
		
		
		for(itor = texunitpairs.begin(); itor !=texunitpairs.end(); itor++)
		{
				free(*itor);
		}

}