#ifndef mymesh_h
#define mymesh_h

#include <list>


typedef struct MaterialPropertiesraw MaterialProperties;
typedef struct Vertexraw Vertex;

typedef struct texidUnitPairraw
{
		unsigned int texid;
		unsigned int unit;
}texidUnitPair;

class myMesh
{
		Vertex* vertices;
		unsigned short *indices;
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;

		unsigned int vshader;
		unsigned int fshader;
		
		unsigned int numVertices;
		unsigned int numIndices;

		MaterialProperties* materialproperties;
		
		std::list<texidUnitPair*> texunitpairs;
		int drawOption;
public:
		enum eDrawOption{ eTRIANGLES=1, eTRIANGLESTRIP=2, eQUADS=4};
		
		
		unsigned int getVnum();
		unsigned int getInum(	);
		unsigned int getVAO();

		int setVAO(Vertex* pv, int vnum, unsigned short* pi, int inum);
		int setDrawOption(eDrawOption op);
		//int setShader(char* vshadername, char* fshadername);
		
		int render();
		
		int setMaterial( MaterialProperties mat);

		void setTexid( unsigned int itexid, unsigned int texUnitnum );
		void setTexidByPath( char* path, unsigned int texUnitnum );
		void release();
		myMesh();
		~myMesh();
};

#endif