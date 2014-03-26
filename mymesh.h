#ifndef mymesh_h
#define mymesh_h

typedef struct MaterialPropertiesraw MaterialProperties;
typedef struct Vertexraw Vertex;
class myMesh
{
		Vertex* vertices;
		unsigned short *indices;
		unsigned int vao;
		unsigned int vbo;
		unsigned int ibo;
		unsigned int program;
		unsigned int vshader;
		unsigned int fshader;
		
		unsigned int numVertices;
		unsigned int numIndices;
		MaterialProperties* materialproperties;
public:

		unsigned int getVnum();
		unsigned int getInum();
		unsigned int getVAO();
		void setItsShaderProgram(unsigned int progid);
		int setVAO(Vertex* pv, int vnum, unsigned short* pi, int inum);
		int setShader(char* vshadername, char* fshadername);
		
		int render();
		int setMaterial( MaterialProperties mat);
		
		myMesh();
		~myMesh();
};

#endif