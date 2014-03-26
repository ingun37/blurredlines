
#ifndef glutsample_vertex_h
#define glutsample_vertex_h

typedef struct Vertexraw
{
		float position[3];
		float normal[3];
}Vertex;

#ifdef __cplusplus
extern "C"{
#endif
typedef float* p2d;
typedef float* p3d;
		

		
p2d createp2d(float x, float y);
p3d createp3d(float x, float y, float z);
float* create1df4ArrayFromP3dArray(p3d* arr, unsigned int pnum, float w);

p3d* createp3dArr(int num);
p2d linearInterpolate(p2d p1, p2d p2, float f);
p2d copyp(p2d p);
p2d normalize(p2d p);
p2d scalarmul(p2d p, float f);
p2d addp2d(p2d a, p2d b);
p2d* copyp2darr(p2d* arr, unsigned int num);
		
p3d linearInterpolate3(p3d p1, p3d p2, float f);
p3d direction3(p3d from, p3d to);
float distance3(p3d p1,p3d p2);
void releasp2d(p2d p);
void releasep2darr(p2d* arr, unsigned int num);

void releasep3d(p3d p);
void releasep3darr(p3d* arr, unsigned int num);

p2d bezierInterpolation(p2d* arr, unsigned int pnum, float f);
#ifdef __cplusplus
}
#endif
#endif
