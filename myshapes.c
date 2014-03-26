#include <stdlib.h>
#include <math.h>
#include "myshapes.h"
#include "vertex.h"
int makePlane(float widthLen, float heightLen, unsigned int widthSeg, unsigned int heightSeg, float*** vertices, unsigned short** indices, int isClockwise)
{
		float** tmpv;
		short* tmpi;
		int i, j;
		int widthvnum = widthSeg + 1;
		int heightvnum = heightSeg + 1;
		*vertices = createp3dArr(widthvnum * heightvnum);
		tmpv = *vertices;
		for (i=0; i<heightvnum; i++)
		{
				for(j=0;j<widthvnum;j++)
				{
						tmpv[i*widthvnum + j][0] = j*(widthLen/widthSeg) - widthLen/2;
						tmpv[i*widthvnum + j][1] = 0;
						tmpv[i*widthvnum + j][2] = i*(heightLen/heightSeg) - heightLen/2;;
				}
		}
		*indices = (int*)malloc(sizeof(int) * widthSeg * heightSeg * 6);
		tmpi = *indices;
		for (i=0; i<heightSeg; i++)
		{
				for(j=0;j<widthSeg;j++)
				{
						if(isClockwise)
						{
								tmpi[6*(i*widthSeg + j) + 0] = (i+1)*widthvnum + j;
								tmpi[6*(i*widthSeg + j) + 1] = i*widthvnum + j;
								tmpi[6*(i*widthSeg + j) + 2] = i*widthvnum + j+1;
								tmpi[6*(i*widthSeg + j) + 3] = i*widthvnum + j+1;
								tmpi[6*(i*widthSeg + j) + 4] = (i+1)*widthvnum + j+1;
								tmpi[6*(i*widthSeg + j) + 5] = (i+1)*widthvnum + j;
						}
						else
						{
								tmpi[6*(i*widthSeg + j) + 0] = (i+1)*widthvnum + j;
								tmpi[6*(i*widthSeg + j) + 1] = (i+1)*widthvnum + j+1;
								tmpi[6*(i*widthSeg + j) + 2] = i*widthvnum + j+1;
								tmpi[6*(i*widthSeg + j) + 3] = i*widthvnum + j+1;
								tmpi[6*(i*widthSeg + j) + 4] = i*widthvnum + j;
								tmpi[6*(i*widthSeg + j) + 5] = (i+1)*widthvnum + j;
						}
				}
		}
		return 0;
}
int makePlaneObject(float widthLen, float heightLen, unsigned int widthSeg, unsigned int heightSeg, Vertex** vertices, unsigned short** indices, int isClockwise)
{
		float** tmpvert;
		int i;
		int vnum = (widthSeg + 1) * (heightSeg + 1);
		makePlane(widthLen,heightLen, widthSeg, heightSeg, &tmpvert, indices, isClockwise );
		
		*vertices = (Vertex*)malloc(sizeof(Vertex) * vnum);
		for(i=0;i<vnum;i++)
		{
				(*vertices)[i].position[0] = tmpvert[i][0];
				(*vertices)[i].position[1] = tmpvert[i][1];
				(*vertices)[i].position[2] = tmpvert[i][2];
				//memcpy((*vertices)[i].position, tmpvert[i], sizeof(float)*3);
				
				(*vertices)[i].normal[0] = 0;
				(*vertices)[i].normal[1] = 1;
				(*vertices)[i].normal[2] = 0;
				
				(*vertices)[i].uv[0] = ((float)(i%(widthSeg+1)))/widthSeg;
				(*vertices)[i].uv[1] = ((float)(i/(widthSeg+1)))/heightSeg;
				printf("%f %f\n",(*vertices)[i].uv[0],(*vertices)[i].uv[1]);
		}
		return 0;
}
int makeSphereVerticesAndIndices(unsigned int smoothness,float radius, float **vertices, unsigned short *indices, float*** normals)
{
		int i, j;
		float tmpangle, tmpradius, tmpheight;
		int pnum = spherePointnum(smoothness);
		int slicenum = sphereVertNumPerSlice(smoothness);
		int facenum = sphereFaceNum(smoothness);
		vertices[0][0] = 0;
		vertices[0][1] = radius;
		vertices[0][2] = 0;

		vertices[pnum-1][0] = 0;
		vertices[pnum-1][1] = -radius;
		vertices[pnum-1][2] = 0;

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
		if(normals)
		{
				*normals = createp3dArr(pnum);
				for(i=0;i<pnum;i++)
				{
						(*normals)[i] = createp3d(vertices[i][0]/radius,vertices[i][1]/radius,vertices[i][2]/radius);
				}
		}
		
		return 0;
}

int makeSphereObject(unsigned int smoothness,float radius, Vertex** vertices, unsigned short** indices)
{
		int i;
		unsigned int pnum = spherePointnum(smoothness);
		unsigned int slicenum = sphereVertNumPerSlice(smoothness);
		unsigned int facenum = sphereFaceNum(smoothness);
		
		p3d* spherepoints = createp3dArr(pnum);
		unsigned short* tmpindices = (unsigned short*)malloc(sizeof(unsigned short)*facenum*3);
		
		p3d* normals;
		makeSphereVerticesAndIndices(smoothness, radius, spherepoints, tmpindices, &normals);
		
		if(indices)
		{
				printf("fdas	%d\n",tmpindices);
				*indices = tmpindices;
		}
		if(vertices)
		{
				*vertices = (Vertex*)malloc(sizeof(Vertex) * pnum);
				for(i=0;i<pnum;i++)
				{
						(*vertices)[i].position[0] = spherepoints[i][0];
						(*vertices)[i].position[1] = spherepoints[i][1];
						(*vertices)[i].position[2] = spherepoints[i][2];
						//(*vertices)[i].position[3] = 1;
						
						(*vertices)[i].normal[0] = normals[i][0];
						(*vertices)[i].normal[1] = normals[i][1];
						(*vertices)[i].normal[2] = normals[i][2];
						//(*vertices)[i].normal[3] = 0;
				}
		}
		if(indices == NULL)
				free(tmpindices);
		releasep3darr(spherepoints, pnum);
}