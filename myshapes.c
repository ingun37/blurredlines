#include <stdio.h>
#include <math.h>
#include "myshapes.h"
int makeSphereVerticesAndIndices(unsigned int smoothness,float radius, float **vertices, int *indices)
{
		int i, j;
		float tmpangle, tmpradius, tmpheight;
		int pnum = spherePointnum(smoothness);
		int slicenum = sphereVertNumPerSlice(smoothness);
		int facenum = sphereFaceNum(smoothness);
		vertices[0][0] = 0;
		vertices[0][1] = radius;
		vertices[0][2] = 0;
		puts("fffff");
		vertices[pnum-1][0] = 0;
		vertices[pnum-1][1] = -radius;
		vertices[pnum-1][2] = 0;
		puts("vertices num right");
		tmpangle = PI/(smoothness+1);
		
		for (i=0; i<smoothness; i++)
		{
				tmpradius = cosf(PI/2 - (i+1)*(tmpangle)) * radius;
				tmpheight = sinf(PI/2 - (i+1)*(tmpangle)) * radius;
				for(j=0;j<slicenum;j++)
				{
						vertices[1 + i*slicenum + j][0] = cosf(j*tmpangle)*tmpradius;
						vertices[1 + i*slicenum + j][1] = tmpheight;
						vertices[1 + i*slicenum + j][2] = sinf(j*tmpangle)*tmpradius;
				}
		}
		
		for(i=0;i<slicenum;i++)
		{
				indices[i*3 + 0]=0;
				indices[i*3 + 1]=((i+1)%slicenum)+1;
				indices[i*3 + 2]=i+1;
		}
		
		for(i=0;i<smoothness-1;i++)
		{
				for(j=0;j<slicenum;j++)
				{
						indices[slicenum*3 + (slicenum*3*2)*i + j*6 + 0] = 1 + (i*slicenum) + j;
						indices[slicenum*3 + (slicenum*3*2)*i + j*6 + 1] = 1 + (i*slicenum) + ((j + 1)%slicenum);
						indices[slicenum*3 + (slicenum*3*2)*i + j*6 + 2] = 1 + (i*slicenum) + j + slicenum;
						
						indices[slicenum*3 + (slicenum*3*2)*i + j*6 + 3] = 1 + (i*slicenum) + j + slicenum;
						indices[slicenum*3 + (slicenum*3*2)*i + j*6 + 4] = 1 + (i*slicenum) + ((j + 1)%slicenum);
						indices[slicenum*3 + (slicenum*3*2)*i + j*6 + 5] = 1 + (i*slicenum) + ((j + 1)%slicenum) + slicenum;
				}
		}
		//for (i=1 + ((sphereSmoothness-1)*slicenum); i< 1 + ((sphereSmoothness-1)*slicenum) + slicenum ; i++)
		for(i=0;i<slicenum;i++)
		{
				indices[facenum*3 - (slicenum*3) + i*3 + 0] = (pnum-1);
				indices[facenum*3 - (slicenum*3) + i*3 + 1] = (pnum-1) -slicenum + i;
				indices[facenum*3 - (slicenum*3) + i*3 + 2] = (pnum-1) -slicenum + ((i + 1)%slicenum);
		}
		
		return 0;
}