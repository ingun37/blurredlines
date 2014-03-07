#ifndef glutsample_myshapes_h
#define glutsample_myshapes_h

#define PI 3.141592
#define spherePointnum(S) (2 + S*((2*S) + 2))
#define sphereVertNumPerSlice(S) (S*2+2)
#define sphereFaceNum(S) S*((S*2+2)*2)
#define sphereIndicesNum(S) (sphereFaceNum(S)*3)

#ifdef __cplusplus
extern "C"{
#endif
		
int makeSphereVerticesAndIndices(unsigned int smoothness,float radius, float **vertices, int *indices);
		
		
#ifdef __cplusplus
}
#endif

#endif