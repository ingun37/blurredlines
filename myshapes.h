#ifndef glutsample_myshapes_h
#define glutsample_myshapes_h

#define PI 3.141592
#define spherePointnum(S) (2 + S*((2*S) + 2))
#define sphereVertNumPerSlice(S) (S*2+2)
#define sphereFaceNum(S) S*((S*2+2)*2)
#define sphereIndicesNum(S) (sphereFaceNum(S)*3)

#define planePointnum(W,H) ((W+1)*(H+1))
#define planeIndicesNum(W,H) (W*H*6)
#ifdef __cplusplus
extern "C"{
#endif
typedef struct Vertexraw Vertex;
int makeSphereVerticesAndIndices(unsigned int smoothness,float radius, float **vertices, unsigned short *indices, float*** normals);
int makePlane(float widthLen, float heightLen, unsigned int widthSeg, unsigned int heightSeg, float*** vertices, int** indices, int isClockwise);
int makeSphereObject(unsigned int smoothness,float radius, Vertex** vertices, unsigned short** indices);
#ifdef __cplusplus
}
#endif

#endif