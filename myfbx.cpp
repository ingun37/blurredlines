#include <fbxsdk.h>
#include "myfbx.h"
#include "Vertex.h"
#include "mymesh.h"
#define safedestroy(p) {if(p){p->Destroy();} p=NULL;}

//#define useibo
mynode* getMyNodeFromFBXNode(FbxNode* fbxnode)
{
		int i;
		mynode* retnode=NULL;
		FbxNode* tmpfbxnode=NULL;
		mynode* tmpmynode=NULL;
		myMesh* mmesh=NULL;
		FbxMesh* fmesh=NULL;
		
		int indicesnum;
		int* polygonvertices=NULL;
		unsigned short* indices=NULL;
		
		int controlpointsnum;
		FbxVector4* controlpoints=NULL;
		FbxVector4 localpos;
		Vertex* vertices=NULL;
		
		FbxLayerElementNormal* normallayer=NULL;
		FbxLayerElementUV* uvlayer = NULL;
		
		if(fbxnode->GetNodeAttribute()->GetAttributeType() != FbxNodeAttribute::eMesh)
		{
				puts("not mesh...");
				return NULL;
		}
		
		puts("\nmaking new mesh node....");
		retnode=new mynode();
		mmesh = new myMesh();
		
		localpos = fbxnode->LclTranslation.Get();
		printf("local pos is..... %f %f %f\n", localpos[0], localpos[1], localpos[2]);
		retnode->setLocalPos(localpos[0], localpos[1], localpos[2]);
		
		//자식 노드 재귀함수로 집어넣기..
		for(i=0;i < fbxnode->GetChildCount();i++)
		{
				tmpfbxnode = fbxnode->GetChild(i);
				if(tmpfbxnode)
				{
						tmpmynode = getMyNodeFromFBXNode(tmpfbxnode);
						if(tmpmynode)
								retnode->addNode( tmpmynode );
				}
		}
		
		//메쉬받아온다.
		fmesh = fbxnode->GetMesh();
		
		
		//노말이랑 uv레이어 받아옴...
		if(fmesh->GetLayer(0,FbxLayerElement::eNormal))
		{
				puts("eNORMAL layer exists...");
				normallayer = fmesh->GetLayer(0,FbxLayerElement::eNormal)->GetNormals();
				if(normallayer)
				{
						printf("GetNormal exists\nlen : %d IndexLen : %d\n",normallayer->GetDirectArray().GetCount(), normallayer->GetIndexArray().GetCount());
				}
		}
		
		if(fmesh->GetLayer(0,FbxLayerElement::eUV))
		{
				puts("eUV layer exists...");
				uvlayer = fmesh->GetLayer(0,FbxLayerElement::eUV)->GetUVs();
				if(uvlayer)
				{
						printf("GetUVs exists\nlen : %d IndexLen : %d\n",uvlayer->GetDirectArray().GetCount(), uvlayer->GetIndexArray().GetCount());
				}
		}
		
		
		//인덱스...
		indicesnum = fmesh->GetPolygonVertexCount();
		polygonvertices = fmesh->GetPolygonVertices();
		
		//버텍스...
		controlpointsnum = fmesh->GetControlPointsCount();
		controlpoints = fmesh->GetControlPoints();
		
		/*
		 puts("uv indices and polygon indices");
		 for (i=0; i<indicesnum; i++)
		 {
		 printf("%d ",uvlayer->GetIndexArray()[i]);
		 }
		 
		 putchar('\n');
		 for (i=0; i<indicesnum; i++)
		 {
		 printf("%d ",polygonvertices[i]);
		 }
		 
		 putchar('\n');
		 */
		
#ifdef useibo
		indices = new unsigned short[indicesnum];
		
		for(i=0;i<indicesnum;i++)
		{
				indices[i] = polygonvertices[i];
		}
		
		vertices = new Vertex[controlpointsnum];
		for(i=0;i<controlpointsnum;i++)
		{
				vertices[i].position[0]=controlpoints[i][0];
				vertices[i].position[1]=controlpoints[i][1];
				vertices[i].position[2]=controlpoints[i][2];
				
				if(normallayer)//확인해보니 normal의 갯수가 index갯수랑 똑같고 normallayer의 indexarray는 없더라
				{
						vertices[i].normal[0]=normallayer->GetDirectArray()[i][0];
						vertices[i].normal[1]=normallayer->GetDirectArray()[i][1];
						vertices[i].normal[2]=normallayer->GetDirectArray()[i][2];
				}
				if(uvlayer)
				{
						vertices[i].uv[0]=uvlayer->GetDirectArray()[uvlayer->GetIndexArray()[i]][0];
						vertices[i].uv[1]=uvlayer->GetDirectArray()[uvlayer->GetIndexArray()[i]][1];
				}
				//printf("%d.\n   position %.2f %.2f %.2f\n   normal %.2f %.2f %.2f\n   uv %.2f %.2f\n",i,vertices[i].position[0],vertices[i].position[1],vertices[i].position[2],vertices[i].normal[0],vertices[i].normal[1],vertices[i].normal[2],vertices[i].uv[0],vertices[i].uv[1]);
		}
		mmesh->setVAO(vertices, controlpointsnum, indices,indicesnum);
#else
		vertices = new Vertex[indicesnum];
		for(i=0;i<indicesnum;i++)
		{
				vertices[i].position[0]=controlpoints[polygonvertices[i]][0];
				vertices[i].position[1]=controlpoints[polygonvertices[i]][1];
				vertices[i].position[2]=controlpoints[polygonvertices[i]][2];
		
				if(normallayer)//확인해보니 normal의 갯수가 index갯수랑 똑같고 normallayer의 indexarray는 없더라
				{
						vertices[i].normal[0]=normallayer->GetDirectArray()[i][0];
						vertices[i].normal[1]=normallayer->GetDirectArray()[i][1];
						vertices[i].normal[2]=normallayer->GetDirectArray()[i][2];
				}
				if(uvlayer)
				{
						vertices[i].uv[0]=uvlayer->GetDirectArray()[uvlayer->GetIndexArray()[i]][0];
						vertices[i].uv[1]=uvlayer->GetDirectArray()[uvlayer->GetIndexArray()[i]][1];
				}
				//printf("%d.\n   position %.2f %.2f %.2f\n   normal %.2f %.2f %.2f\n   uv %.2f %.2f\n",i,vertices[i].position[0],vertices[i].position[1],vertices[i].position[2],vertices[i].normal[0],vertices[i].normal[1],vertices[i].normal[2],vertices[i].uv[0],vertices[i].uv[1]);
		}
		mmesh->setVAO(vertices, indicesnum, NULL,0);
		 
#endif

		retnode->setMesh(mmesh);
		return retnode;
}

mynode* getNodeFromFBXpath(char* path)
{
	int i;
	FbxManager* sdkmanager = NULL;
	FbxScene* scene = NULL;
	FbxIOSettings* ios = NULL;
	FbxImporter* importer = NULL;
	bool status;
	FbxNode* rootnode = NULL;
	FbxNode* tmpfbxnode;
	mynode* myrootnode,*tmpmynode;



	try
	{
		sdkmanager = FbxManager::Create();
		scene = FbxScene::Create(sdkmanager, "");

		ios = FbxIOSettings::Create(sdkmanager, IOSROOT);
		
		sdkmanager->SetIOSettings(ios);
		
		importer = FbxImporter::Create(sdkmanager, "");
		status = importer->Initialize(path, -1, sdkmanager->GetIOSettings());
		
		if(status == false)
				throw("fbx fileimport fail!!");

		status = importer->Import(scene);
		
		if(status == false)
				throw("importing to scene fail");

		
		rootnode = scene->GetRootNode();
		
		if(rootnode == NULL)
				throw("no root node");

		
		printf("root node's child num : %d\n",rootnode->GetChildCount());
		
		myrootnode = new mynode();
			
		for(i=0;i < rootnode->GetChildCount();i++)
		{
			tmpfbxnode = rootnode->GetChild(i);
			if(tmpfbxnode)
			{
				tmpmynode = getMyNodeFromFBXNode(tmpfbxnode);
				if(tmpmynode)
					myrootnode->addNode( tmpmynode );
			}
		}
		
		
		return myrootnode;
	}
	catch(char* err)
	{
		puts(err);
		
		safedestroy( rootnode );
		safedestroy( importer );
		safedestroy( ios );
		safedestroy( scene );
		safedestroy( sdkmanager );
				
		return NULL;
	}
		
		
	return NULL;
}