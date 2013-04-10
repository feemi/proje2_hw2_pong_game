//
//  main.cpp
//  Ders 6
//
//  Created by Gurel Erceis on 3/17/13.
//  Copyright (c) 2013 Yogurt3D. All rights reserved.
//
// Thing to install and test before writing any code
// glm
// assimp (http://assimp.sourceforge.net/main_downloads.html)

#include <windows.h>
#include <iostream>
#include <GL/glew.h>
#include "Scene.h"
#include <Glut.h>
#include "ScreenSpaceShaderNode.h"
#include "NormalShaderNode.h"
#include "ColorFillNode.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"


#define WINDOW_TITLE_PREFIX "Ders 7"

#define TICKS_PER_SECOND 200


const int TIMER_MILLISECONDS = 1000 / TICKS_PER_SECOND;


void IdleFunction(void);
void ResizeFunction(int Width, int Height);
void setupScene();

unsigned FrameCount = 0;

int CurrentWidth = 800,
	CurrentHeight = 600,
	WindowHandle = 0, mouseX = 0, mouseY = 0;

SceneNode* pad1;
SceneNode* pad2;
SceneNode* ball;

Camera *camera;

Scene* scene;

float directionX = 1;
float directionY = 0.5;
float ballSpeed = 3.0;
float pad1Speed = 2.0;
float pad2Speed = 2.0;
int previousTime;
int currentTime;
int elapsedTime;
bool isKeyDownW=false;
bool isKeyDownS=false;
bool isKeyDownO=false;
bool isKeyDownL= false;

bool goalState= false;

unsigned long long scorePlayer1=0;
unsigned long long scorePlayer2=0;

void newRound();


void output(float x, float y, float r, float g, float b, std::string stringo)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
 
  int len, i;
  len = stringo.length();
  for (i = 0; i < len; i++) {
	  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stringo[i]);
  }
}
static void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	std::string  s = std::to_string(scorePlayer1);
	std::string s2 = std::to_string(scorePlayer2);
	s+="-";
	s+=s2;
	
	
	output(0,0.9,0,0,0,s);
	scene->draw();

	glutSwapBuffers();
	FrameCount++;
}
static void SpecialKey(int key, int x , int y)
{
	printf("%d", key);

	switch (key)
	{
	case GLUT_KEY_UP:
		isKeyDownO=true;
		break;
	case GLUT_KEY_DOWN:
		isKeyDownL=true;
	}

}
static void SpecialUpKey(int key, int x, int y)
{
	isKeyDownO=false;
	isKeyDownL=false;
}

static void Key(unsigned char key, int x, int y)
{
	printf("%d", key);
	switch (key) {
	case 27: // ESC
		exit(0);
		break;
	case GLUT_KEY_PAGE_DOWN: // ESC
		//pad1->translate(0, -.5, 0);
		isKeyDownO= true;
		break;
	case GLUT_KEY_DOWN: // ESC
		//pad1->translate(0, 0.5, 0);
		isKeyDownL = true;
		break;
	case 'w': // ESC
		//pad2->translate(0, -0.05, 0);
		isKeyDownW= true;

		break;
	case 's': // ESC
		//pad2->translate(0, 0.05,0);
		isKeyDownS=true;
		break;


	}
}
static void KeyUp( unsigned char key, int x, int y){
	isKeyDownW = false;
	isKeyDownS = false;

}
void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
}

static void timerCallback (int value)
{
	if (0 != value) {
		char* TempString = (char*)
			malloc(512 + strlen(WINDOW_TITLE_PREFIX));

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			FrameCount * 4,
			CurrentWidth,
			CurrentHeight
			);

		glutSetWindowTitle(TempString);
		free(TempString);
	}
	FrameCount = 0;
	previousTime = currentTime;

	// Get the current time (in milliseconds) and calculate the elapsed time
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	elapsedTime = currentTime - previousTime;
	glutTimerFunc(TIMER_MILLISECONDS, timerCallback, 1);
}


void IdleFunction(void)
{
	glm::mat4 ballMatrix = ball->getModelMatrix();
	float x = ball->getPosition().x;
	float y = ball->getPosition().y;

	
	if( y > 4.0 )
	{
		directionY *= -1;
	}
	if( y < -4.0 )
	{
		directionY *= -1;
	}

	if( fabs(5.3-x)< 0.1 && directionX>0)
	{
		if(pad2->getPosition().y+1.1 > y && pad2->getPosition().y-1.1 < y ) 
		{
			
			directionX *= -1;
		}
		else
		{
			goalState=true;
			scorePlayer1+=1;
		}
	}
	
	if( fabs(5.3+x)<0.1 && directionX<0 )
	{
		if(pad1->getPosition().y+1.1 > y && pad1->getPosition().y-1.1 < y ) 
		{

			directionX *= -1;

		}
		else
		{
			goalState=true;
			scorePlayer2+=1;
		}
	}

	// pad1->rotate(1, 0, 1, 0);
	if(isKeyDownW)
	{

		if(pad2->getPosition().y+1.1<4.2)
		{
			pad2Speed= 1;
		}
		else
		{
			pad2Speed= 0;
		}
	}
	else if (isKeyDownS)
	{
		
		if(pad2->getPosition().y-1.1>-4.2)
		{	
			pad2Speed= -1;
		}
		else
		{
			pad2Speed=0;
		}
	}
	else
	{
		pad2Speed = 0;
	}
	if(isKeyDownO)
	{

		if(pad1->getPosition().y+1.1<4.2)
		{
			pad1Speed= 1;
		}
		else
		{
			pad1Speed= 0;
		}
	}
	else if (isKeyDownL)
	{
		
		if(pad1->getPosition().y-1.1>-4.2)
		{	
			pad1Speed= -1;
		}
		else
		{
			pad1Speed=0;
		}
	}
	else
	{
		pad1Speed = 0;
	}
	if (ballSpeed< 10)
	{
		ballSpeed+=0.0001;
	}
	ball->translate(directionX*ballSpeed*(elapsedTime / 1000.0f), directionY*ballSpeed*(elapsedTime/1000.0f),0);
	pad2->translate(0,pad2Speed*(elapsedTime / 1000.0f),0);
	pad1->translate(0,pad1Speed*(elapsedTime/1000.0f),0);
	newRound();
	
	glutPostRedisplay();
}



void setupScene(){
	scene = new Scene();

	pad1 = new ScreenSpaceShaderNode("capsule.obj");

	pad2 = new ScreenSpaceShaderNode("capsule.obj");

	ball = new ColorFillNode("capsule.obj");



	pad1->scale(0.05,1,0.1);
	pad2->scale(0.05,1,0.1);
	ball->scale(0.1,0.1,0.1);

	pad1->translate(-108.f, 0.f, 0.f);
	pad2->translate( 108.f, 0.f, 0.f);

	scene->addNode(pad1);
	scene->addNode(pad2);
	scene->addNode(ball);

	camera = new Camera();

	camera->translate(0.f, 0.f, 10.f);

	



	scene->setCamera(camera);
}

void newRound()
{
	if(goalState==true)
	{
		if(scorePlayer1==10 || scorePlayer2==10)
		{
			scorePlayer1=0;
			scorePlayer2=0;
			
			
		}
		else
		ballSpeed= 3.0;
		ball->identity();
		ball->scale(0.1,0.1,0.1);
		goalState=false;
	}

}

int main (int argc, char ** argv)
{

	GLenum type;

	glutInit(&argc, argv);
	glutInitWindowSize(CurrentWidth,CurrentHeight);
	type = GLUT_RGB;
	type |= GLUT_DOUBLE;
	type |= GLUT_DEPTH;
	type |= GLUT_MULTISAMPLE;
	glutInitDisplayMode(type);
	glutCreateWindow("");

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glClearColor(0.7, 0.70 ,0.7 , 1.0);	
	// Z-Buffer i aciyoruz
	glEnable(GL_DEPTH_TEST);


	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE); 

	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	printf("GLSL Version: %s\n",glslVersion);

	const GLubyte* glVersion = glGetString(GL_VERSION);

	printf("GL Version: %s\n",glVersion);

	setupScene();
	
	timerCallback(0);
	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(Draw);
	glutSetKeyRepeat(false);
	glutKeyboardFunc(Key);
	glutKeyboardUpFunc(KeyUp);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialUpKey);
	glutIdleFunc(IdleFunction);
	glutMainLoop();

	return 0;
}


