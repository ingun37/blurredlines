#include <fbxsdk.h>
#include "myfbx.h"
#include "Vertex.h"
#define safedestroy(p) {if(p){p->Destroy();} p=NULL;}
Vertex* getVerticesFromFBXpath(char* path)
{
	int i;
	FbxManager* sdkmanager = NULL;
	FbxScene* scene = NULL;
	FbxIOSettings* ios = NULL;
	FbxImporter* importer = NULL;
	bool status;
	FbxNode* rootnode = NULL;
	FbxNode* tmpnode = NULL;
	FbxMesh* mesh = NULL;
		FbxVector4* controlpoints;
		int *polygonvertices;
	try
	{
		sdkmanager = FbxManager::Create();
		scene = FbxScene::Create(sdkmanager, "");

		ios = FbxIOSettings::Create(sdkmanager, IOSROOT);
		/*
		ios->SetBoolProp(IMP_FBX_MATERIAL, false);
		ios->SetBoolProp(IMP_FBX_TEXTURE, false);
		ios->SetBoolProp(IMP_FBX_LINK, false);
		ios->SetBoolProp(IMP_FBX_SHAPE, false);
		ios->SetBoolProp(IMP_FBX_GOBO, false);
		ios->SetBoolProp(IMP_FBX_ANIMATION, false);
		ios->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, false);
		*/
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
		
		for(i=0;i<rootnode->GetChildCount();i++)
		{
				tmpnode = rootnode->GetChild(i);
		}
		
		if(tmpnode == NULL)
				throw("no child of root node");
			
			mesh = tmpnode->GetMesh();
			polygonvertices = mesh->GetPolygonVertices();
			controlpoints = mesh->GetControlPoints();
			
			printf("control points len : %d\n",mesh->GetControlPointsCount());
			printf("polygon count : %d\n",mesh->GetPolygonCount());
			printf("polygonvertexcount : %d\n",mesh->GetPolygonVertexCount());
			for(i=0;i<mesh->GetPolygonVertexCount();i++)
			{
					printf("%d : %f  %f  %f\n"
						   ,polygonvertices[i]
						   ,controlpoints[polygonvertices[i]][0]
						   ,controlpoints[polygonvertices[i]][1]
						   ,controlpoints[polygonvertices[i]][2]
						   );
			}
			
	}
	catch(char* err)
	{
		puts(err);
		
		safedestroy( tmpnode );
		safedestroy( rootnode );
		safedestroy( importer );
		safedestroy( ios );
		safedestroy( scene );
		safedestroy( sdkmanager );
				
		return NULL;
	}
		
		
	return NULL;
}