#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vertex.h"

p2d createp2d(float x, float y)
{
		p2d p = (p2d)malloc(sizeof(float)*2);
		
		p[0]=x;
		p[1]=y;
		
		return p;
}
p3d createp3d(float x, float y, float z)
{
		p3d p = (p3d)malloc(sizeof(float)*3);
		
		p[0]=x;
		p[1]=y;
		p[2]=z;
		
		return p;
}

p2d linearInterpolate(p2d p1, p2d p2, float f)
{
		return createp2d(p1[0]*(1-f) + p2[0]*f, p1[1]*(1-f) + p2[1]*f );
}

p3d linearInterpolate3(p3d p1, p3d p2, float f)
{
		return createp3d(p1[0]*(1-f) + p2[0]*f, p1[1]*(1-f) + p2[1]*f, p1[2]*(1-f) + p2[2]*f);
}

p3d direction3(p3d from, p3d to)
{
		p3d dir = createp3d(to[0]-from[0],to[1]-from[1],to[2]-from[2]);
		float len = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
		dir[0]/=len;
		dir[1]/=len;
		dir[2]/=len;
		
		return dir;
}
float distance3(p3d p1, p3d p2)
{
		float lx = p1[0] - p2[0];
		float ly = p1[1] - p2[1];
		float lz = p1[2] - p2[2];
		
		return sqrtf(lx*lx + ly*ly + lz*lz);
}
p2d copyp(p2d p)
{
		return createp2d(p[0], p[1]);
}
p2d normalize(p2d p)
{
		float len = sqrt(p[0]*p[0] + p[1]*p[1]);
		return createp2d(p[0]/sqrt(len), p[1]/sqrt(len));
}
p2d scalarmul(p2d p, float f)
{
		return createp2d(p[0]*f, p[1]*f);
}
p2d addp2d(p2d a, p2d b)
{
		return createp2d(a[0] + b[0] , a[1] + b[1]);
}
p2d* copyp2darr(p2d* arr, unsigned int num)
{
		p2d* ret = (p2d*)malloc(sizeof(p2d)*num);
		
		int i=0;
		for(i=0;i<num;i++)
		{
				ret[i] = createp2d(arr[i][0], arr[i][1]);
		}
		return ret;
}
void releasp2d(p2d p)
{
		free(p);
		
}
void releasep2darr(p2d* arr, unsigned int num)
{
		int i;
		for(i=0;i<num;i++)
				releasp2d(arr[i]);
		free(arr);
		
}

void releasep3d(p3d p)
{
		free(p);
		
}
void releasep3darr(p3d* arr, unsigned int num)
{
		int i;
		for(i=0;i<num;i++)
				releasep3d(arr[i]);
		free(arr);
		
}

p2d bezierInterpolation(p2d* arr, unsigned int pnum, float f)
{
		unsigned int linecnt = pnum-1;
		p2d* tmparr;
		p2d tmpp;
		int i;
		
		tmparr = copyp2darr(arr, pnum);
		
		while(linecnt > 0)
		{
				for(i=1;i<linecnt+1;i++)
				{
						tmpp = linearInterpolate(tmparr[i-1], tmparr[i], f);
						releasp2d(tmparr[i-1]);
						tmparr[i-1]=tmpp;
				}
				
				--linecnt;
		}
		
		tmpp = copyp(tmparr[0]);
		releasep2darr(tmparr, pnum);
		return tmpp;
}