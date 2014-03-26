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
		free(materialproperties);
}
int myMesh::render()
{
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialproperties->diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialproperties->specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialproperties->ambient);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialproperties->shininess);
		
		glBindVertexArrayAPPLE(vao);
		glDrawElements(GL_TRIANGLES, numIndices * sizeof(short), GL_UNSIGNED_SHORT, NULL);
		glBindVertexArrayAPPLE(0);
}
int myMesh::setMaterial( MaterialProperties mat)
{
		memcpy(materialproperties, &mat, sizeof(MaterialProperties));
}
int myMesh::setShader(char* vshadername, char* fshadername)
{
		if(vshadername == NULL || fshadername == NULL)
				return -1;

		vshader = makeVertexShader(vshadername, NULL);
		fshader = makeVertexShader(fshadername, NULL);
		
		if(vshader <1 || fshader < 1)
				return -1;
		
		program = makeProgram(vshader, fshader);
		
		if(program < 1)
				return -1;
		return 0;
		
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
				makeVAOBufferToAttribute(getFixedVAOParameters(), 2, program, &vbo, pv, sizeof(Vertex), vnum);
		else
				makeVAOBufferOnly(&vbo, pv, sizeof(Vertex), vnum);
		
		
		if(pi)
				makeVertexArrayIndexBuffer( &ibo, pi, inum);
		
		glBindVertexArrayAPPLE(0);
		
		return 0;
}