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
		program = 0;
		vshader = 0;
		fshader = 0;
		ibo = NULL;
		vbo = NULL;
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
		
		if(program)
		{
				for(itor = texunitpairs.begin(); itor !=texunitpairs.end(); itor++)
				{
						glActiveTexture((*itor)->unit);
						glBindTexture(GL_TEXTURE_2D, (*itor)->texid );
				}
		}
		glBindVertexArrayAPPLE(vao);
		glDrawElements(GL_TRIANGLES, numIndices * sizeof(short), GL_UNSIGNED_SHORT, NULL);
		glBindVertexArrayAPPLE(0);
}

int myMesh::setMaterial( MaterialProperties mat)
{
		memcpy(materialproperties, &mat, sizeof(MaterialProperties));
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

void myMesh::setItsShaderProgram(unsigned int progid)
{
		program = progid;
}

int myMesh::setVAO(Vertex* pv, int vnum, unsigned short* pi, int inum)
{
		if(pv == NULL || inum == 0 || vnum == 0)
				return -1;
		
		numVertices = vnum;
		numIndices = inum;
		
		glGenVertexArraysAPPLE(1, &vao);
		glBindVertexArrayAPPLE(vao);
		
		if(program)
		{
				makeVAOBufferToAttribute(getFixedVAOParameters(), getNumFixedVAOParameters(), &vbo, pv, sizeof(Vertex), vnum);
		}
		
		else
				makeVAOBufferOnly(&vbo, pv, sizeof(Vertex), vnum);
		
		
		if(pi)
				makeVertexArrayIndexBuffer( &ibo, pi, inum);
		
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
		
		if(program)
		{
				for(itor = texunitpairs.begin(); itor !=texunitpairs.end(); itor++)
				{
						free(*itor);
				}
		}
}