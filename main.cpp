//#define renderBezier
//#define renderGlow
#include <stdio.h>
#include <stdlib.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#include <math.h>
#include <time.h>
#include <string.h>

#include "myprogrammer.h"
#include "vertex.h"
#include "myshader.h"
#include "myshapes.h"
#include "mymesh.h"
#include "materialProperties.h"

#include "myfbx.h"
#include "mynode.h"
#define KEY_ESCAPE 27
//#define PI 3.141592
#define D360 (PI*2)
#define A2R(a) (D360*(((float)a)/360))

const int winwidth = 2048;
const int winheight = 512;
const float winangle = 45;
const float winnear = 1;
const float winfar = 2000;
static float eyepos[3] = {2,100,200};
static float lookat[3] = {0,100,0};

static int beziersmoothness = 100;
static int pointnum=20;
static float linewidth = 1;
static p2d* points;
static unsigned int exlinesnum=10;
static p2d** exlines;


static int moonsmoothness = 30;
static int moonpnum=6;
static float moonradius = 0.4;
static p2d* moonpoints;
static unsigned int exmoonlinesnum=10;
static p2d** moonexlines;

static int sunmoothness = 30;
static int sunpnum=13;//to make circle with bezier, startpoint and end point must be same
static float sunradius = 0.8;
static p2d* sunpoints;
static unsigned int sunexlinesnum=10;
static p2d** sunexlines;

static p3d* glowingline;


static clock_t clockstart;
static clock_t clockcurrent;
static GLuint shaderprogram;
static GLuint shaderV;
static GLuint shaderF;


static GLint lv3start, lv3dir, lfdist, lv3mycolor, lfmypow;

//shaders

static myprogrammer programs;

static GLuint shaderLightprogram;//, shaderVLight, shaderFLight;
static GLint lfUNIv3lightdir, lfUNIv3eyepos, lfUNIv3specc, lfUNIv3spherecenter;

static GLuint shaderCheckProgram;//shaderFChecker, shaderVChecker, ;

static GLuint shaderTexUnlitProgram;// shaderFTexUnlit, shaderVTexUnlit, ;

static GLint lfUNIitex;
//meshes
static myMesh* meshSphere;
static myMesh* meshTexSphere;
static myMesh* meshPlane;

//locations

//fbxthings...
static mynode* box;

static MaterialProperties boxmat =
{
		{0.6f,0.2f,0.9f,1},
		{0.6f,0.2f,0.9f,1},
		{1.f,1.f,1.f,1},
		20.f,
};

//sphere things...
static unsigned short* sphereindices;
static const unsigned int sphereSmoothness = 2;
static const float sphereradius = 2;
static Vertex* spherevertices;
static MaterialProperties spheremat =
{
		{0.7f,0.2f,0.4f,1},
		{0.7f,0.2f,0.4f,1},
		{1.f,1.f,1.f,1},
		20.f,
};


//plane things...
static unsigned short* planeindices;
static const float planeWidthlen=20, planeHeightlen=20, planeWidthSeg=4, planeHeightSeg = 4;
static Vertex* planevertices;
static MaterialProperties planemat =
{
		{0.7f,0.2f,0.4f,1},
		{0.7f,0.2f,0.4f,1},
		{1.f,1.f,1.f,1},
		20.f,
};



//light1은 directional
static GLfloat light1Ambient[] = {0.2f,0.2f,0.2f,1};
static GLfloat light1Diffuse[] = {1,1,1,1};
static GLfloat light1specular[] = {0.3f,0.3f,0.3f,1};
static GLfloat light1position[] = {0,0,0,0};
static GLfloat light1direction[] = {0,1,0};

void markAt(p2d *arr, unsigned int num)
{
		int i;
		static float ra=0;
		ra +=0.1f;
		for(i=0;i<num;i++)
		{
				glPushMatrix();
				
				
				glTranslatef(arr[i][0], arr[i][1], 0);
				glRotatef(ra, 0, 0, 1);
				glBegin(GL_TRIANGLES);
				
				glColor3f(0.0f,0.0f,1.0f);
				
				glVertex3f( 0.0f, 0.1f * sqrt(3)*(2.f/3), 0.0f);
				glColor3f(0.0f,1.f,0.0f);
				glVertex3f(-0.1f,-0.1f * sqrt(3)*(1.f/3), 0.0f);
				glColor3f(1.0f,0.0f,0.0f);
				glVertex3f( 0.1f,-0.1f * sqrt(3)*(1.f/3), 0.0f);
				
				glEnd();
				
				glPopMatrix();
		}
}
void markAt3(p3d p)
{
		
		static float ra=0;
		ra +=0.1f;
		
		{
				glPushMatrix();
				
				
				glTranslatef(p[0], p[1], p[2] + 0.05);
				glRotatef(ra, 0, 0, 1);
				glBegin(GL_TRIANGLES);
				
				glColor3f(0.0f,0.0f,1.0f);
				
				glVertex3f( 0.0f, 0.1f * sqrt(3)*(2.f/3), 0.0f);
				glColor3f(0.0f,1.f,0.0f);
				glVertex3f(-0.1f,-0.1f * sqrt(3)*(1.f/3), 0.0f);
				glColor3f(1.0f,0.0f,0.0f);
				glVertex3f( 0.1f,-0.1f * sqrt(3)*(1.f/3), 0.0f);
				
				glEnd();
				
				glPopMatrix();
		}
}

void drawBezier(p2d* bezierpoints, unsigned int num, int smooth)
{
		int i;
		p2d tmpp;
		glBegin(GL_LINE_STRIP);
		
		for(i=0;i<=smooth;i++)
		{
				tmpp = bezierInterpolation(bezierpoints, num, ((float)i)/smooth);
				
				glVertex3f(tmpp[0], tmpp[1], 0);
				releasp2d(tmpp);
		}
		glEnd();
}


void update()
{
		int i=0, j;
		int randres;
		int tmp = -1;
		float tmpf;
		p2d tmpp, tmpp2;
#ifdef renderBezier
		{
				for (i=0;i<exlinesnum;i++)
				{
						exlines[i] = copyp2darr(points, pointnum);
				}
				
				for (i=0;i<pointnum;i++)
				{
						randres = rand()%2;
						tmp = powf(-1, (float)randres);
						
						points[i][1] += tmp*0.04f;
						
						for(j=0;j<exlinesnum;j++)
						{
								//exlines[j][i][1] += tmp* 0.04*(pow(j*5,1 + ((float)j/exlinesnum)*0.5)) * ((i!=pointnum-1) & 1);
								exlines[j][i][1] += tmp* 0.04*(j*1) * ((i%(pointnum-1)) & 1);
						}
				}
				
				
				for (i=0;i<exmoonlinesnum;i++)
				{
						moonexlines[i] = copyp2darr(moonpoints, moonpnum);
				}
				
				for (i=0;i<moonpnum;i++)
				{
						randres = rand()%2;
						tmp = powf(-1, (float)randres);
						
						moonpoints[i][0] += tmp*0.01f;
						
						for(j=0;j<exmoonlinesnum;j++)
						{
								
								moonexlines[j][i][0] += tmp* 0.02*(pow(j*1,1 + ((float)j/exmoonlinesnum)*0.5))* ((i%(moonpnum-1)) & 1);
						}
				}
				
				for (i=0;i<sunexlinesnum;i++)
				{
						sunexlines[i] = copyp2darr(sunpoints, sunpnum);
				}
				
				for (i=0;i<sunpnum;i++)
				{
						randres = rand()%2;
						tmp = powf(-1, (float)randres);
						
						tmpp = normalize(sunpoints[i]);
						tmpp2 = scalarmul(tmpp, tmp*0.01f);
						releasp2d(tmpp);
						tmpp = addp2d(tmpp2, sunpoints[i]);
						releasp2d(sunpoints[i]);
						sunpoints[i] = tmpp;
						
				}
		}
#endif
#ifdef renderGlow
		{
				p3d start;
				p3d end;
				p3d dir;
				float dist;
				
				
				start = linearInterpolate3(glowingline[0],glowingline[1],0.5);
				end = linearInterpolate3(glowingline[2],glowingline[3],0.5);
				dir = direction3(start,end);
				dist = distance3(glowingline[0],start);
				
				
				glUniform1f(lfdist, dist);
				glUniform3f(lv3start, start[0],start[1],start[2]);
				glUniform3f(lv3dir,dir[0],dir[1],dir[2]);
				
				releasep3d(start);
				releasep3d(end);
				releasep3d(dir);
		}
#endif
		/*
		 releasep3d(start);
		 releasep3d(end);
		 releasep3d(dir);
		 */
		
		glLightfv(GL_LIGHT0, GL_AMBIENT, light1Ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light1Diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light1specular);
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light1direction);
		
		clockcurrent = clock();
		
}

void drawfinish()
{
#ifdef renderBezier
		{
				int i;
				for(i=0;i<exlinesnum;i++)
				{
						releasep2darr(exlines[i], pointnum);
						
				}
				//releasep2darr(points, pointnum);
				
				for(i=0;i<exmoonlinesnum;i++)
				{
						releasep2darr(moonexlines[i], moonpnum);
				}
		}
#endif
}

static bool drawbezier=false;
void display()
{
		int i, tmp;
		float rotation;
		rotation =360 * ((float)(clockcurrent%10000000))/10000000;
		
		update();
		
		glClearColor(0.1f,0.5f,0.2f,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer
		glLoadIdentity();
		
		//glLoadIdentity();
		gluLookAt(eyepos[0], eyepos[1], eyepos[2], lookat[0], lookat[1], lookat[2], 0, 1, 0);
		
		//glTranslatef(0.0f,0.0f,-10.0f);
		
		//markAt(points, pointnum);
		//markAt(moonpoints, moonpnum);
		
		
#ifdef renderBezier
		{
				glPushMatrix();
				
				glUseProgram(0);
				
				//glTranslatef(-3, 1, 0);
				
				//printf("%f\n",rotation);
				glRotatef(rotation, 1, 0, 0);
				glTranslatef(0.0f,0.0f,-10.0f);
				
				
				
				glColor3f(0.9, 0.9, 0.3);
				
				
				drawBezier(moonpoints, moonpnum, moonsmoothness);
				for(i=0;i<exmoonlinesnum;i++)
				drawBezier(moonexlines[i], moonpnum, moonsmoothness);
				glPopMatrix();
				
				glPushMatrix();
				
				
				//glTranslatef(-3, 1, 0);
				rotation =360 * ((float)(clockcurrent%10000000))/10000000 + 180;
				glRotatef(rotation, 1, 0, 0);
				glTranslatef(0.0f,0.0f,-10.0f);
				
				glColor3f(0.9, 0.2, 0.3);
				drawBezier(sunpoints, sunpnum, sunmoothness);
				glPopMatrix();
				
				glTranslatef(0.0f,0.0f,-10.0f);
				glColor3f(0.2, 0.3, 0.6);
				drawBezier(points, pointnum, beziersmoothness);
				for(i=0;i<exlinesnum;i++)
				drawBezier(exlines[i], pointnum, beziersmoothness);
		}
#endif
		
		/*
		 glUseProgram(0);
		 
		 glPushMatrix();
		 glTranslatef(0, sphereradius*2, 0);
		 glRotatef(sinf(rotation*0.1)*260, 0, 1, 0);
		 for(i=0;i<sphereForTexturePointnum(sphereSmoothness);i++)
		 {
		 markAt3(sphereTexvertices[i].position);
		 }
		 glPopMatrix();
		 */
		
		
		//render box....
		//puts("\n\nstarting render fbx hierarchy......");

		
		//glUseProgram(shaderTexUnlitProgram);
		programs.useProgram(shaderTexUnlitProgram);
		
		glUniform1i(lfUNIitex, 0);
		glPushMatrix();
		glRotatef(-90,1,0,0);
		
		
		box->render();
		glPopMatrix();
		//puts("\nrender fbx hierarchy end......");
		
		
		
		//render plane....
		//glUseProgram(shaderTexUnlitProgram);
		
		//glProgramUniform1i((GLuint)shaderTexUnlitProgram, lfUNIitex, 0);
		glUniform1i(lfUNIitex, 0);
		glPushMatrix();
		glTranslatef(0,0,0);
		glColor3f(0.1f, 0.4f, 0.1f);
		
		//meshPlane->render();
		
		glPopMatrix();
		
		
		
		
		//render objects with light rpogram
		//glUseProgram(shaderLightprogram);
		programs.useProgram(shaderLightprogram);
		
		glUniform3f(lfUNIv3eyepos, eyepos[0], eyepos[1], eyepos[2]);
		glUniform3f(lfUNIv3spherecenter, 0,0,-10);
		
		//render sphere...
		glPushMatrix();
		glTranslatef(7,sphereradius*2,0.0f);
		glRotatef(sinf(rotation*0.1)*260,0,1,0);
		//meshSphere->render();
		glPopMatrix();
		
		
		
		
#ifdef renderGlow
		{
				glUseProgram(shaderprogram);
				
				glPushMatrix();
				
				glTranslatef(0,sinf(rotation/1.5),-20);
				glRotatef(rotation*4, 0,0,1);
				
				glUniform3f(lv3mycolor,0.9,0.2,0.6);
				glUniform1f(lfmypow,2+sinf(rotation*3.2));
				
				glBegin(GL_QUADS);
				
				for( i=0;i<4;i++)
				{
						glVertex4f(glowingline[i][0], glowingline[i][1], glowingline[i][2],1);
				}
				
				glEnd();
				
				glPopMatrix();
				
				
				
				glPushMatrix();
				
				glTranslatef(sinf(rotation*0.5),sinf(rotation*1.2),-20);
				glRotatef(sinf(rotation*1.4)*40, 0,0,1);
				
				glUniform3f(lv3mycolor,0.1,0.9,0.4);
				glUniform1f(lfmypow,2+sinf(rotation*5));
				
				glBegin(GL_QUADS);
				
				for( i=0;i<4;i++)
				{
						glVertex4f(glowingline[i][0], glowingline[i][1], glowingline[i][2],1);
				}
				
				glEnd();
				
				glPopMatrix();
				
				
				
				glPushMatrix();
				
				glTranslatef(1.6*sinf(rotation*1.3),2*sinf(rotation*2),-20);
				glRotatef(-sinf(rotation*0.2)*720, 0,0,1);
				
				glUniform3f(lv3mycolor,0.3,0.2,0.6);
				glUniform1f(lfmypow,2+sinf(rotation*5));
				
				glBegin(GL_QUADS);
				
				for( i=0;i<4;i++)
				{
						glVertex4f(glowingline[i][0], glowingline[i][1], glowingline[i][2],1);
				}
				
				glEnd();
				
				glPopMatrix();
				
				
				
				glPushMatrix();
				
				glTranslatef(10,0,-30);
				glRotatef(-85, 0,1,0);
				glScalef(40,1,1);
				glUniform3f(lv3mycolor,1,1,1);
				glUniform1f(lfmypow,2);
				
				glBegin(GL_QUADS);
				
				for( i=0;i<4;i++)
				{
						glVertex4f(glowingline[i][0], glowingline[i][1], glowingline[i][2],1);
				}
				
				glEnd();
				
				glPopMatrix();
		}
#endif

}

void initialize ()
{
		int i, j;
		float angle=(30.f/360)*2*(3.141592);
		float range = 8;
		
		float term;
		float tmp;
		/*
		 glewExperimental = GL_TRUE;
		 glewInit();
		 if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
		 printf("ready for glsl\n");
		 if(glewIsSupported("GL_VERSION_2_0"))
		 puts("ready for opengl 2");
		 */
		
		printf("\n\n\n\ngl_texture0 is.... %d %d\n\n\n\n", GL_TEXTURE0, GL_TEXTURE1);
		glMatrixMode(GL_PROJECTION);												// select projection matrix
		glViewport(0, 0, winwidth, winheight);									// set the viewport
		glMatrixMode(GL_PROJECTION);												// set matrix mode
		glLoadIdentity();															// reset projection matrix
		GLfloat aspect = (GLfloat) winwidth / winheight;
		gluPerspective(winangle, aspect, winnear, winfar);		// set up a perspective projection matrix
		glMatrixMode(GL_MODELVIEW);													// specify which matrix is the current matrix
																					//glLoadIdentity();
																					//gluLookAt(0, 0, 0, 0, 0, 100, 0, 1, 0);
		glShadeModel( GL_SMOOTH );
		glClearDepth( 1.0f );														// specify the clear value for the depth buffer
																					//glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );
		glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );						// specify implementation-specific hints
		glClearColor(0.0, 0.0, 0.0, 1.0);											// specify clear values for the color buffers
#ifdef renderBezier
		{
				points = (p2d*)malloc(sizeof(p2d)*pointnum);
				
				for (i=0; i<pointnum; i++)
				{
						points[i] = createp2d(linewidth * (i-pointnum/2), powf(-1,  (float)(i%2))* abs( ((float)(rand()%((int)range*10)))/(range*10)*range - range/2));
				}
				exlines = (p2d**)malloc(sizeof(p2d*)*exlinesnum);
				
				
				moonpoints = (p2d*)malloc(sizeof(p2d)*moonpnum);
				for(i=0;i<moonpnum;i++)
				{
						moonpoints[i] = createp2d(cosf(angle + A2R(60)*i) * moonradius, sinf(angle+A2R(60)*i) * moonradius);
						//printf("%f %f\n", moonpoints[i][0], moonpoints[i][1]);
				}
				
				moonpoints[0][1]*=0.8;
				moonpoints[moonpnum-1][1]*=0.8;
				moonpoints[0][0]*=0.5;
				moonpoints[moonpnum-1][0]*=0.5;
				
				moonpoints[2][0] *= 1.4f;
				moonpoints[3][0] *= 1.4f;
				moonexlines = (p2d**)malloc(sizeof(p2d*)*exmoonlinesnum);
				
				
				sunpoints = (p2d*)malloc(sizeof(p2d)*sunpnum);
				
				
				angle = 0;
				term = A2R(30);
				for(i=0;i<sunpnum;i++)
				{
						tmp = sunradius + ((i%2) * (sunradius * 2 / sqrt(3) -sunradius));
						sunpoints[i] = createp2d(cosf(term*i) * tmp, sinf(term*i) * tmp);
				}
				
				sunexlines = (p2d**)malloc(sizeof(p2d*)*sunexlinesnum);
		}
#endif
#ifdef renderGlow
		{
				glowingline = (p3d*)malloc(sizeof(p3d)*4);
				
				
				glowingline[0] = createp3d(30, -2, -0);
				glowingline[1] = createp3d(30, 2, -0);
				glowingline[2] = createp3d(-30, 2, -0);
				glowingline[3] = createp3d(-30, -2, -0);
				shaderV = makeVertexShader("vert.vert",NULL);
				shaderF = makeFragmentShader("frag.frag",NULL);
				
				shaderprogram = glCreateProgram();
				glAttachShader(shaderprogram,shaderV);
				glAttachShader(shaderprogram,shaderF);
				glLinkProgram(shaderprogram);
				
				lv3start = glGetUniformLocation( shaderprogram,"start");
				lv3dir = glGetUniformLocation( shaderprogram,"dir");
				lfdist = glGetUniformLocation( shaderprogram,"dist");
				lfmypow = glGetUniformLocation( shaderprogram, "mypow");
		}
#endif
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		char lightvshadername[] = "light.vert";
		char lightfshadername[] = "light.frag";
		shaderLightprogram = programs.makeAndManageProgram(lightvshadername,lightfshadername);
		
		/*
		char lightProgramuniformeyeposname[] = "eyepos";
		lfUNIv3eyepos = glGetUniformLocation (shaderLightprogram, lightProgramuniformeyeposname);
		*/
		//reduced vertex sphere.....
		unsigned int pnum;
		unsigned int inum;
		
		makeSphereObject(sphereSmoothness, sphereradius, &spherevertices, &sphereindices ,&pnum,&inum);
		meshSphere = new myMesh();
		meshSphere->setVAO(spherevertices, pnum, sphereindices, inum);
		meshSphere->setMaterial( spheremat );
		
		
		///plane.....
		
		char checkervshadername[] = "checker.vert";
		char checkerfshadername[] = "checker.frag";
		shaderCheckProgram = programs.makeAndManageProgram(checkervshadername,checkerfshadername);
		
		char texunlitvshadername[] = "texunlit.vert";
		char texunlitfshadername[] = "texunlit.frag";
		shaderTexUnlitProgram = programs.makeAndManageProgram(texunlitvshadername,texunlitfshadername);
		
		//lfUNIitex = glGetUniformLocation(shaderTexUnlitProgram, "tex");
		
		//이 아랫놈은 3.0부터 가능한거
		//glProgramUniform1i((GLuint)shaderTexUnlitProgram, (GLint)lfUNIitex, (GLint)0);
		
		makePlaneObject(planeWidthlen, planeHeightlen, planeWidthSeg, planeHeightSeg, &planevertices, &planeindices, 0, &pnum, &inum);
		
		char planetexturefilename[] = "plane.png";
		meshPlane = new myMesh();
		//meshPlane->setItsShaderProgram(shaderCheckProgram);

		meshPlane->setVAO(planevertices, pnum, planeindices, inum);
		meshPlane->setMaterial( planemat );
		meshPlane->setTexidByPath(planetexturefilename, GL_TEXTURE0);
		
		printOpenGLError();
		
		//fbx.....
		//char fbxtexname[] = "bodydiffuse.png";
		//fbxtexid = makeTexture(fbxtexname,NULL,NULL);
		char bottomfbxfilename[] = "gamev_girl_base.FBX";
		box = getNodeFromFBXpath(bottomfbxfilename);
		box->getChildAt(0)->mesh->setTexidByPath("hairdiffuse.png", GL_TEXTURE2);
		box->getChildAt(1)->mesh->setTexidByPath("facediffuse.png", GL_TEXTURE3);
		box->getChildAt(2)->mesh->setTexidByPath("footdiffuse.png", GL_TEXTURE4);
		box->getChildAt(3)->mesh->setTexidByPath("handdiffuse.png", GL_TEXTURE5);
		box->getChildAt(4)->mesh->setTexidByPath("bodydiffuse.png", GL_TEXTURE6);
		box->getChildAt(5)->mesh->setTexidByPath("bodydiffuse.png", GL_TEXTURE7);
		
		puts("init end");
		
		//glEnable(GL_CULL_FACE);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		clockstart = clock();
}



void terminate()
{
		free(planeindices);
		free(sphereindices);
		

		delete meshPlane;
		delete meshSphere;
		
		
#ifdef renderBezier
		{
				releasep2darr(points, pointnum);
				for(i=0;i<exlinesnum;i++)
				releasep2darr(exlines[i], pointnum);
				free(exlines);
				
				releasep2darr(moonpoints, moonpnum);
				for(i=0;i<exmoonlinesnum;i++)
				releasep2darr(moonexlines[i], moonpnum);
				free(moonexlines);
				
				releasep2darr(sunpoints, sunpnum);
		}
#endif

}


int main(int argc, char **argv)
{

		GLFWwindow *window;
		if(!glfwInit())
		{
				return -1;
		}
		
		window = glfwCreateWindow(winwidth,winheight,"joningunGLSL",NULL,NULL);
		if(!window)
		{
				glfwTerminate();
				return -1;
		}
		
		glfwMakeContextCurrent(window);
		
		initialize();
		while(!glfwWindowShouldClose(window))
		{
				update();
				display();
				drawfinish();
				
				glfwSwapBuffers(window);
				glfwPollEvents();
		}
		terminate();
		glfwTerminate();
		
		
		
		
		return 0;
}