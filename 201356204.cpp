#include <iostream>
#include<stdio.h>
#include <cmath>
#include <GL/glut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

// Function Declarations

void drawScene();
void update(int value);
void drawSquare(float len);
void drawBall(float rad);
void drawTriangle();
void moveStriker();
void initRendering();
void coinWall();
void coinCoin();
void strikerWall();	
void strikerCoin();
void mouseMove(int x, int y);
void mouseButton(int button, int state, int x, int y);
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);

// Global Variables

int isDragging=0;
float outerBoardLength = 6.0f;
float innerBoardLength = 5.3f;
float cornerBallRadius = 0.3f;
float friction = 0.03f;
GLdouble ox=0.0,oy=0.0,oz=0.0;

int countBlackPocketed = 0;
int countWhitePocketed = 0;
int countRedPocketed = 0;
int countTime = 0;
int strikerMove = 0;
int collision_error[10][10];
char outPrint[100];
char colorByPerson = 'p';
int score = 100;
float black = -11.0;
float white = -11.0;
float red = -11.0;


int coinPocketed = 0;
float ball_x = 1.0f;
float ball_y = 0.0f;
float ball_velx = 0.01f;
float ball_vely = 0.02f;
float ball_rad = 0.2f;
float box_len = 4.0f;
float tri_x = 0.0f;
float tri_y = 0.0f;
float theta = 0.0f; 



// Classes

class scircle
{
	public:
		float coinPositionX;
		float coinPositionY;
		float coinVelocityX;
		float coinVelocityY;
		float coinMass;
		float coinRadius;
		scircle()
		{
			coinPositionX = 0.0f;
			coinPositionY = -3.75f;
			coinVelocityY = 0.05f;
			coinVelocityX = 0.05f;
			coinMass = 2.0f;
			coinRadius = 0.25f;
		}
		void strikerDraw()
		{
			glPushMatrix();
			glTranslatef(coinPositionX, coinPositionY,-8.0f);
			glColor3f(1.0f,1.0f,1.0f);
			drawBall(coinRadius);
			glColor3f(1.0f,0.0f,1.0f);
			drawBall(0.18);
			glPopMatrix();

			// Conditions for checking pocketed

			if(sqrt(pow(coinPositionX-5.0,2)+pow(coinPositionY-5.0,2))<0.225)    
			{
				coinVelocityX=0;
				coinVelocityY=0;
				strikerSetScore();
			}
			else if(sqrt(pow(coinPositionX+5.0,2)+pow(coinPositionY-5.0,2))<0.225 )
			{
				coinVelocityX=0;
				coinVelocityY=0;
				strikerSetScore();
			}
			else if(sqrt(pow(coinPositionX-5.0,2)+pow(coinPositionY+5.0,2))<0.225 )
			{
				coinVelocityX=0;
				coinVelocityY=0;
				strikerSetScore();
			}
			else if(sqrt(pow(coinPositionX+5.0,2)+pow(coinPositionY+5.0,2))<0.225 )
			{
				coinVelocityX=0;
				coinVelocityY=0;
				strikerSetScore();
			}
		}
		void strikerSetScore()
		{
			if(score>5)
			{
				score -= 5;
			}
		}
		void coinMoveX(float distance)
		{
			coinPositionX = coinPositionX + distance;
		}
		void coinMoveY(float distance)
		{
			coinPositionY += distance;
		}
};

class circle
{
	public:
		char color;
		float coinPositionX;
		float coinPositionY;
		float coinVelocityX;
		float coinVelocityY;
		float coinMass;
		float coinRadius;
		int coinPocketed;
		circle()
		{	
			coinPositionX=0.0f;
			coinPositionY=0.0f;
			coinVelocityX=0.0f;
			coinVelocityY=0.0f;
			coinMass=0.0f;
			coinRadius=0.0f;
			coinPocketed=0;
		}
		void coinSetScore() 				 
		{
			if(coinPocketed==0)
			{
				if(color==colorByPerson)
				{	
					score+=10;
				}
				else
				{
					if(color=='r' &&  countBlackPocketed==3 && colorByPerson == 'b')
					{
						score+=10;
					}
					else
					{
						if(score-10>=0)
						{
							score-=10;
						}
					}
					if(color=='r' &&  countWhitePocketed==3 && colorByPerson == 'w')
					{
						score+=10;
					}
					else
					{
						if(score-10>=0)
						{
							score-=10;
						}
					}
				}
				if(color=='b')
				{
					countBlackPocketed++;
					coinPositionX = black++;
					coinPositionY = -2.0f;
				}
				else if(color=='w')
				{
					countWhitePocketed++;
					coinPositionX = white++;
					coinPositionY = -3.0f;
				}
				else if(color=='r')
				{
					countRedPocketed++;
					coinPositionX = red++;
					coinPositionY = -4.0f;
				}
				coinPocketed=1;
			}
		}
		void coinDraw(int j)			
		{
			coinRadius = 0.25f;
			glPushMatrix();
			glTranslatef(coinPositionX,coinPositionY,-8.0f);
			if(j==8)
			{
				color='r';
				glColor3f(1.0f, 0.0f, 0.0f);
				glBegin(GL_TRIANGLE_FAN);
				for(int i=0 ; i<360 ; i++)
				{
					glColor3f(1.0f, 0.0f, 0.0f);
					glVertex2f(coinRadius * cos(DEG2RAD(i)), coinRadius * sin(DEG2RAD(i)));
				}
			}
			else
			{
				j=j%2;
				if(j==0)
					color='b';
				else
					color='w';
				j*=1.0f;
				glColor3f(j, j, j);
				glBegin(GL_TRIANGLE_FAN);
				for(int i=0 ; i<360 ; i++) {

					glColor3f(j,j, j);
					glVertex2f(coinRadius * cos(DEG2RAD(i)), coinRadius * sin(DEG2RAD(i)));
				}
			}
			glEnd();
			glPopMatrix();

			if(sqrt(pow(coinPositionX-5.0,2)+pow(coinPositionY-5.0,2))<0.225 )  
			{
				coinVelocityX = 0.0f;
				coinVelocityY = 0.0f;
				coinRadius = 0.0f;
				coinSetScore();
				coinPocketed++;
			}
			if(sqrt(pow(coinPositionX+5.0,2)+pow(coinPositionY-5.0,2))<0.225 )
			{
				coinVelocityX = 0.0f;
				coinVelocityY = 0.0f;
				coinRadius = 0.0f;
				coinSetScore();
				coinPocketed++;
			}
			if(sqrt(pow(coinPositionX-5.0,2)+pow(coinPositionY+5.0,2))<0.225 )
			{
				coinVelocityX = 0.0f;
				coinVelocityY = 0.0f;
				coinRadius = 0.0f;
				coinSetScore();
				coinPocketed++;
			}
			if(sqrt(pow(coinPositionX+5.0,2)+pow(coinPositionY+5.0,2))<0.225 )
			{
				coinVelocityX = 0.0f;
				coinVelocityY = 0.0f;
				coinRadius = 0.0f;
				coinSetScore();
				coinPocketed++;
			}
		}
		void coinMoveX(float distance)
		{
			coinPositionX = coinPositionX + distance;
		}
		void coinMoveY(float distance)
		{
			coinPositionY += distance;
		}
};

// Initializing

scircle striker;
circle coins[9];

class stick
{
	public:
		float stickPositionX;
		float stickPositionY;
		float stickLength;
		float stickAngle;
		stick()
		{
			stickAngle=90;
			stickLength=3.0f;
			stickPositionX=0.0f;
			stickPositionY=-3.75f;
		}
		void draw()
		{
			stickPositionX = striker.coinPositionX;
			stickPositionY = striker.coinPositionY;
			glPushMatrix();
			glTranslatef(stickPositionX,stickPositionY,-8.0f);
			glColor3f(0.0f, 0.0f, 1.0f);
			glBegin(GL_LINES);
			glVertex2f(0.0f,0.0f);
			glVertex2f(stickLength*cos(DEG2RAD(stickAngle)),stickLength*sin(DEG2RAD(stickAngle)));
			glEnd();
			glPopMatrix();
		}
		void stickChangeX(float change)
		{
			stickPositionX += change;
		}
		void stickChangeY(float change)
		{
			stickPositionY += change;
		}
		void stickChangeLength(float change)
		{
			stickLength += change;
		}
		void stickChangeAngle(float change)
		{
			stickAngle += change;
		}

};

// Initializing

stick directionstick;

class powerBar
{
	public:
		float barWidth;
		float barHeight;
		powerBar()
		{
			barWidth = 1.0f;
			barHeight = -5.0f;
		}
		void draw()
		{
			glPushMatrix();
			glTranslatef(5.0f,2.0f, -8.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3f(0.0f,0.0f,0.0f);
			glBegin(GL_QUADS);
			glVertex2f(2.0f,-5.0f);
			glVertex2f(3.0f,-5.0f);
			glVertex2f(3.0f,-1.0f);
			glVertex2f(2.0f,-1.0f);
			glEnd();
			glPopMatrix();

			glColor3f((1.0f*244)/255,(1.0f*164)/255 , (1.0f*96)/255);
			glPushMatrix();
			glTranslatef(5.0f,2.0f, -8.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glColor3f((1.0f*244)/255,(1.0f*164)/255 , (1.0f*96)/255);
			glBegin(GL_QUADS);
			glVertex2f(3.0f,barHeight);
			glVertex2f(2.0f,barHeight);
			glVertex2f(2.0f,-5.0f);
			glVertex2f(3.0f,-5.0f);
			glEnd();
			glPopMatrix();
		}
		void changeLength(float change) 
		{
			if(barHeight+change <= -1 && barHeight+change >=-5)
			{
				barHeight+=change;
			}
		}
};

// Initializing

powerBar powerbar;


void initRendering() 
{
	glEnable(GL_DEPTH_TEST);        
	glEnable(GL_COLOR_MATERIAL);    
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   
}

// Display string inside the window

void renderBitmapString(float x,float y,float z,void *font,char *string) 
{
	char *c;
	glRasterPos3f(x,y,z);
	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(font, *c);
	}
}

void drawSquare(float len) 
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();		
}

void drawBall(float rad) 
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
	}
	glEnd();
}

void drawCircle(float radius)
{
	int i;
	int lineAmount = 100; 
	float twicePi = 2.0f * PI;
	glBegin(GL_LINE_LOOP);
	for(i = 0; i <= lineAmount;i++) 
	{
		glVertex2f(radius * cos(i * twicePi / lineAmount),radius* sin(i * twicePi / lineAmount));
	}
	glEnd();
} 

void drawTriangle() 
{
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.25f, 0.0f);
	glVertex3f(-0.25f, -0.25f, 0.0f);
	glVertex3f(0.25f, -0.25f, 0.0f);
	glEnd();
}

void drawTriangleWithLessDimension() 
{
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.15f, 0.15f, 0.0f);
	glVertex3f(-0.15f, -0.15f, 0.0f);
	glVertex3f(0.15f, -0.15f, 0.0f);
	glEnd();
}

void drawHollowSquare(float len) 
{
	glBegin(GL_LINE_LOOP);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
}

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w * 2 / 3;
	int windowHeight = h * 2 / 3;
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);
	glutCreateWindow("Assignment 01 - Graphics - 201356204");
	initRendering();
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			collision_error[i][j]=0;
		}
	}
	for(int i=0;i<8;i++)
	{
		coins[i].coinMoveX(1.0*cos(DEG2RAD(i*45)));
		coins[i].coinMoveY(1.0*sin(DEG2RAD(i*45)));
	}
	coins[8].coinMoveX(0.0f);
	coins[8].coinMoveY(0.0f);
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(mouseButton); 
	glutMotionFunc(mouseMove); 
	glutReshapeFunc(handleResize);
	glutTimerFunc(5, update, 0);
	glutMainLoop();
	return 0;
}

void drawScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -8.0f);

	// Draw Outer Square Carrom Board

	glColor3f(0.0,0.0,0.0);
	drawSquare(outerBoardLength);

	// Draw Inner Square Carrom Board

	glColor3f((1.0f*240)/255,(1.0f*168)/255 ,(1.0f*96)/255);
	drawSquare(innerBoardLength);

	// Draw Triangles to cover the gap between corner holes and outer carrom board

	glPushMatrix();
	glTranslatef(-5.2f,-5.1f,-8.0f);
	glRotatef(15,0,0,1);
	glColor3f(0.0f,0.0f,.0f);
	drawTriangle();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.12f,5.12f,-8.0f);
	glRotatef(135,0,0,1);
	glColor3f(0.0f,0.0f,.0f);
	drawTriangle();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.2f,-5.1f,-8.0f);
	glRotatef(15,0,0,1);
	glColor3f(0.0f,0.0f,.0f);
	drawTriangle();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.2f,5.3f,-8.0f);
	glRotatef(30,0,0,1);
	glColor3f(0.0f,0.0f,.0f);
	drawTriangle();
	glPopMatrix();

	// Draw Holes at the Corners

	glPushMatrix();
	glTranslatef(-5.0f,5.0f,-8.0f);
	glColor3f((1.0f*255)/255,(1.0f*255)/255 , (1.0f*0)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0f,5.0f,-8.0f);
	glColor3f((1.0f*255)/255,(1.0f*255)/255 , (1.0f*0)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0f,-5.0f,-8.0f);
	glColor3f((1.0f*255)/255,(1.0f*255)/255 , (1.0f*0)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0f,-5.0f,-8.0f);
	glColor3f((1.0f*255)/255,(1.0f*255)/255 , (1.0f*0)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	// Draw center circles of the carrom board

	glColor3f(0.0f,0.0f,0.0f);
	drawCircle(0.8);
	drawCircle(0.75);
	drawCircle(0.7);

	// Draw inner square lines 

	drawHollowSquare(4.0);
	drawHollowSquare(3.5);

	// Draw triangles to eliminate gap between red corners and lines

	glPushMatrix();
	glTranslatef(-3.87f,-3.86f,-8.0f);
	glRotatef(0,0,0,1);
	glColor3f((1.0f*240)/255,(1.0f*168)/255 ,(1.0f*96)/255);
	drawTriangleWithLessDimension();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.87f,3.86f,-8.0f);
	glRotatef(180,0,0,1);
	glColor3f((1.0f*240)/255,(1.0f*168)/255 ,(1.0f*96)/255);
	drawTriangleWithLessDimension();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.87f,-3.86f,-8.0f);
	glRotatef(90,0,0,1);
	glColor3f((1.0f*240)/255,(1.0f*168)/255 ,(1.0f*96)/255);
	drawTriangleWithLessDimension();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.87f,3.86f,-8.0f);
	glRotatef(270,0,0,1);
	glColor3f((1.0f*240)/255,(1.0f*168)/255 ,(1.0f*96)/255);
	drawTriangleWithLessDimension();
	glPopMatrix();

	// Draw balls for corner shots in carrom board

	glPushMatrix();
	glTranslatef(-3.70f,3.70f,-8.0f);
	glColor3f((1.0f*0)/255,(1.0f*51)/255 ,(1.0f*51)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.70f,-3.70f,-8.0f);
	glColor3f((1.0f*0)/255,(1.0f*51)/255 ,(1.0f*51)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.70f,-3.70f,-8.0f);
	glColor3f((1.0f*0)/255,(1.0f*51)/255 ,(1.0f*51)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.70f,3.70f,-8.0f);
	glColor3f((1.0f*0)/255,(1.0f*51)/255 ,(1.0f*51)/255);
	drawBall(cornerBallRadius);
	glPopMatrix();

	// Draw striker

	striker.strikerDraw();

	// Draw stick

	directionstick.draw();

	// Draw Other Coins

	for(int i=0;i<9;i++)
	{
		coins[i].coinDraw(i);	
	}

	// Draw the power bar

	powerbar.draw();

	// Title

	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	sprintf(outPrint,"%s","Carrom Board - Single Player\n");
	renderBitmapString(-11.0f,5.5f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
	glPopMatrix();

	// Draw the score 

	if(score>0)
	{
		sprintf(outPrint,"%s","Score:\n");
		renderBitmapString(7.5f,5.0f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
		sprintf(outPrint,"%d",score);
		renderBitmapString(8.7f,5.0f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
		if(countBlackPocketed==3)
		{
			sprintf(outPrint,"%s","You Win!!!!!\n");
			renderBitmapString(-11.0f,2.5f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
		}
	}
	else
	{
		sprintf(outPrint,"%s","You Lose!!!!!\n");
		renderBitmapString(-11.0f,2.5f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
	}

	// Draw coins pocketed

	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	sprintf(outPrint,"%s","Coins Pocketed:\n");
	renderBitmapString(7.0f,3.0f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
	sprintf(outPrint,"%d",coinPocketed);
	renderBitmapString(9.7f,3.0f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
	glPopMatrix();
	glPopMatrix();

	// Draw the title of power bar

	glPushMatrix();
	glTranslatef(0.0f,0.0f,-8.0f);
	glColor3f(0.0f,0.0f,0.0f);
	sprintf(outPrint,"%s","Power Bar\n");
	renderBitmapString(6.9f,1.5f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
	glPopMatrix();

	// Draw the choosing

	if(colorByPerson == 'p')
	{
		glPushMatrix();
		glTranslatef(0.0f,0.0f,-8.0f);
		glColor3f(0.0f,0.0f,0.0f);
		sprintf(outPrint,"%s","Choose color : Press 'w' for White coins or 'b' for Black coins\n");
		renderBitmapString(-5.0f,-6.5f,-8.0f,GLUT_BITMAP_HELVETICA_18,outPrint);
		glPopMatrix();
	}

	glPopMatrix();
	glutSwapBuffers();

}


// Function to handle all calculations in the scene
// updated evry 10 milliseconds
void update(int value) 
{
	strikerWall();
	coinWall();
	strikerCoin();
	coinCoin();
	if(countTime==100)
	{
		if(score>0)
		{
			score--;
		}
		countTime=0;
	}
	if(strikerMove == 1)
	{
		moveStriker();
	}
	for (int i = 0; i < 9; ++i)
	{
		coins[i].coinMoveX(coins[i].coinVelocityX);
		coins[i].coinMoveY(coins[i].coinVelocityY);
		coins[i].coinVelocityX-=coins[i].coinVelocityX*friction;
		coins[i].coinVelocityY-=coins[i].coinVelocityY*friction;
		coins[i].coinDraw(i);
	}
	if(abs(striker.coinVelocityX)<=0.0001 && abs(striker.coinVelocityY)<=0.0001 && strikerMove == 1)
	{
		striker.coinMoveX(-striker.coinPositionX);
		striker.coinMoveY(-striker.coinPositionY-3.7);
		striker.strikerDraw();
		directionstick.stickAngle = 90;
		directionstick.stickLength = 3.0f;
		directionstick.stickPositionX = 0.0f;
		directionstick.stickPositionY = -3.75f;
		strikerMove=0; 
	}
	countTime++;
	glutTimerFunc(10, update, 0);
}

void drawBox(float len) {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex2f(-len / 2, -len / 2);
	glVertex2f(len / 2, -len / 2);
	glVertex2f(len / 2, len / 2);
	glVertex2f(-len / 2, len / 2);
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Checking all types of collisions

void strikerWall()	
{
	if(striker.coinPositionX + striker.coinRadius > innerBoardLength)
	{
		if(striker.coinVelocityX>0)
		{
			striker.coinVelocityX *= -1;
		}
	}
	if(striker.coinPositionX - striker.coinRadius < -innerBoardLength)
	{
		if(striker.coinVelocityX<0)
		{
			striker.coinVelocityX *= -1;
		}
	}
	if(striker.coinPositionY + striker.coinRadius > innerBoardLength)
	{
		if(striker.coinVelocityY>0)
		{
			striker.coinVelocityY *= -1;
		}
	}
	if(striker.coinPositionY - striker.coinRadius < -innerBoardLength)
	{
		if(striker.coinVelocityY<0)
		{
			striker.coinVelocityY *= -1;
		}
	}
}

void coinWall()
{
	for(int i=0;i<9;i++)
	{
		if(coins[i].coinPositionX + coins[i].coinRadius > innerBoardLength)
		{
			if(coins[i].coinVelocityX>0)
			{
				coins[i].coinVelocityX *= -1;
			}
		}
		else if(coins[i].coinPositionX -coins[i].coinRadius <-innerBoardLength)
		{
			if(coins[i].coinVelocityX<0)
			{
				coins[i].coinVelocityX *= -1;
			}
		}
		else if(coins[i].coinPositionY + coins[i].coinRadius > innerBoardLength)
		{
			if(coins[i].coinVelocityY>0)
			{
				coins[i].coinVelocityY *= -1;
			}
		}
		else if(coins[i].coinPositionY - coins[i].coinRadius < -innerBoardLength)
		{
			if(coins[i].coinVelocityY<0)
			{
				coins[i].coinVelocityY *= -1;
			}
		}
	}
}

void strikerCoin()
{
	float cosangle1;
	float sinangle1;
	float angle1;
	float v1;
	float v2;
	float temp;
	float slope;
	float ul1;
	float ul2;
	for(int i=0;i<9;i++)
	{
		temp=sqrt(pow((striker.coinPositionX-coins[i].coinPositionX),2)+pow((striker.coinPositionY-coins[i].coinPositionY),2));
		if((temp-striker.coinRadius-coins[i].coinRadius<=0.01))
		{
			if(collision_error[9][i]==0)
			{
				collision_error[9][i]=1;
				if(abs(coins[i].coinPositionX-striker.coinPositionX)<=0.0000001)
				{
					if(coins[i].coinPositionY-striker.coinPositionY>=0.00001)
					{
						slope=1.0/0.0;
					}
					else
					{
						slope=-1.0/0.0;
					}
				}
				else
				{
					slope=(coins[i].coinPositionY-striker.coinPositionY)/(coins[i].coinPositionX-striker.coinPositionX);
				}
				if(slope>=0)
				{
					angle1=atan(slope)*180/PI;  //angle of line of contact with x axis
				}
				else   
				{
					angle1=atan(slope)*180/PI; //angle of line of contact with x axis
				}
				// Equations for momentum conservations and finally setting the vx and vy of both  collided coins
				cosangle1=cos(DEG2RAD(angle1));
				sinangle1=sin(DEG2RAD(angle1));
				ul1=striker.coinVelocityX*cosangle1+striker.coinVelocityY*sinangle1;
				ul2=coins[i].coinVelocityX*cosangle1+coins[i].coinVelocityY*sinangle1;
				v1= ul1/3 + (2*ul2)/3 ;
				v2=(4*ul1)/3 - (ul2)/3 ;
				striker.coinVelocityX=v1*cosangle1-(-striker.coinVelocityX*sinangle1+striker.coinVelocityY*cosangle1)*sinangle1;
				striker.coinVelocityY=v1*sinangle1+(-striker.coinVelocityX*sinangle1+striker.coinVelocityY*cosangle1)*cosangle1;
				coins[i].coinVelocityX=v2*cosangle1-(-coins[i].coinVelocityX*sinangle1+coins[i].coinVelocityY*cosangle1)*sinangle1;
				coins[i].coinVelocityY=v2*sinangle1+(-coins[i].coinVelocityX*sinangle1+coins[i].coinVelocityY*cosangle1)*cosangle1;
			}
		}
		else
		{
			collision_error[9][i]=0;
		}
	}
}

void coinCoin()	 
{
	float angle1;
	float v1;
	float v2;
	float d;
	float slope;
	float ul1;
	float ul2;
	float cosangle1;
	float sinangle1;
	for(int j=0;j<9;j++)
	{
		for(int i=0;i<9;i++)
		{
			d=sqrt(pow((coins[j].coinPositionX-coins[i].coinPositionX),2)+pow((coins[j].coinPositionY-coins[i].coinPositionY),2));
			if(d-coins[j].coinRadius-coins[i].coinRadius<=0.01)
			{
				if(collision_error[j][i]==0 && collision_error[i][j]==0)
				{
					collision_error[j][i]=1;
					collision_error[i][j]=1;
					if(abs(coins[j].coinPositionX-coins[i].coinPositionX)<=0.0000001)
					{
						if(coins[j].coinPositionY-coins[i].coinPositionY>=0.00001)
							slope=1.0/0.0;
						else
							slope=-1.0/0.0;
					}
					else
					{
						slope=(coins[j].coinPositionY-coins[i].coinPositionY)/(coins[j].coinPositionX-coins[i].coinPositionX);
					}
					if(slope>=0)
					{
						angle1=atan(slope)*180/PI;	//angle of line of contact with xaxis 
					}
					else   
					{
						angle1=atan(slope)*180/PI; //angle of line of contact with xaxis 
					}
					cosangle1=cos(DEG2RAD(angle1));
					sinangle1=sin(DEG2RAD(angle1));
					ul1=coins[j].coinVelocityX*cosangle1+coins[j].coinVelocityY*sinangle1;
					ul2=coins[i].coinVelocityX*cosangle1+coins[i].coinVelocityY*sinangle1;
					v1= ul2 ;
					v2= ul1 ;
					coins[j].coinVelocityX=v1*cosangle1-(-coins[j].coinVelocityX*sinangle1+coins[j].coinVelocityY*cosangle1)*sinangle1;
					coins[j].coinVelocityY=v1*sinangle1+(-coins[j].coinVelocityX*sinangle1+coins[j].coinVelocityY*cosangle1)*cosangle1;
					coins[i].coinVelocityX=v2*cosangle1-(-coins[i].coinVelocityX*sinangle1+coins[i].coinVelocityY*cosangle1)*sinangle1;
					coins[i].coinVelocityY=v2*sinangle1+(-coins[i].coinVelocityX*sinangle1+coins[i].coinVelocityY*cosangle1)*cosangle1;
				}
			}
			else
			{
				collision_error[j][i]=0;
				collision_error[i][j]=0;
			}
		}
	}
}




void moveStriker()				 
{
	striker.coinMoveX(striker.coinVelocityX);
	striker.coinMoveY(striker.coinVelocityY);
	striker.coinVelocityX -= striker.coinVelocityX*friction;
	striker.coinVelocityY -= striker.coinVelocityY*friction;
}




// Function called when the window is resized

void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);	glLoadIdentity();
}

void handleKeypress2(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		float temp=sqrt(pow((striker.coinPositionX+4.05),2)+pow((striker.coinPositionY+4.05),2));
		if(abs(temp-striker.coinRadius-0.2)>0.05)
		{
			striker.coinMoveX(-0.1);
		}
	}
	if (key == GLUT_KEY_RIGHT)
	{
		float temp=sqrt(pow((striker.coinPositionX-4.05),2)+pow((striker.coinPositionY+4.05),2));
		if(abs(temp-striker.coinRadius-0.2)>0.05)
		{		
			striker.coinMoveX(+0.1);
		}
	}
	if (key == GLUT_KEY_UP)
	{
		powerbar.changeLength(+0.1);
	}
	if (key == GLUT_KEY_DOWN)
	{
		powerbar.changeLength(-0.1);
	}
}

void handleKeypress1(unsigned char key, int x, int y)
{
	if (key == 27) 
	{
		exit(0);
	}
	if(key==65 || key==97)
	{
		directionstick.stickChangeAngle(5.0);
	}
	if(key==66 || key==98)
	{
		colorByPerson='b';
	}
	if(key==119 || key==87)
	{
		colorByPerson='w';
	}
	if(key==67 || key==99)  
	{
		directionstick.stickChangeAngle(-5.0);
	}
	if(key==32)	
	{
		directionstick.stickChangeLength(-directionstick.stickLength);
		float temp = powerbar.barHeight+5.0;
		float factor=temp/4;
		striker.coinVelocityX=factor*cos(DEG2RAD(directionstick.stickAngle));
		striker.coinVelocityY=factor*sin(DEG2RAD(directionstick.stickAngle));
		strikerMove=1;
	}
}

void mouseMove(int x, int y)
{
	if (isDragging) 
	{
		GLint viewport[4];
		GLdouble modelview[16],projection[16];
		GLfloat wx=x,wy,wz;
		glGetIntegerv(GL_VIEWPORT,viewport);
		y=viewport[3]-y;
		wy=y;
		glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
		glGetDoublev(GL_PROJECTION_MATRIX,projection);
		glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
		gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
		oy*=160;
		ox*=160;
		if((ox<3.75) && (ox>-3.75))
		{
			striker.coinPositionX = ox;
		}
	}
}


void mouseButton(int button, int state, int x, int y)  // handling mouse button clicks
{
	float slope;
	GLint viewport[4];
	GLdouble modelview[16],projection[16];
	GLfloat wx=x,wy,wz;
	glGetIntegerv(GL_VIEWPORT,viewport);
	y=viewport[3]-y;
	wy=y;
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
	gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);

	oy*=160;
	ox*=160;

	if (button == GLUT_RIGHT_BUTTON) 
	{												
		if (state == GLUT_DOWN) 
		{ 
			float d=sqrt(pow((striker.coinPositionX-ox),2)+pow((striker.coinPositionY-oy),2));
			if(d < striker.coinRadius)
			{
				isDragging = 1; 
			}
		}
		else  {
			isDragging = 0;
		}
	}
	if(button == GLUT_LEFT_BUTTON)       // setting the direction of the direction stick
	{
		if(state==GLUT_DOWN)
		{
			if(fabs(ox-striker.coinPositionX)>0.00001f)
			{
				slope=(oy-striker.coinPositionY)/(ox-striker.coinPositionX);
				if(slope>=0)
					directionstick.stickAngle = atan(slope)*180/PI;
				else
					directionstick.stickAngle = 180 + atan(slope)*180/PI;
			}
			else
			{
				if(oy-striker.coinPositionY>0.00001)
					directionstick.stickAngle=90;
				else
					directionstick.stickAngle=180;
			}
		}
		if(state==GLUT_UP)
		{
			directionstick.stickLength = 0.0f;
			float dist=sqrt(pow((ox-striker.coinPositionX),2)+pow(oy-striker.coinPositionY,2));
			dist/=10;
			striker.coinVelocityX=dist*cos(DEG2RAD(directionstick.stickAngle));
			striker.coinVelocityY=dist*sin(DEG2RAD(directionstick.stickAngle));
			directionstick.stickLength = 0.0f;
			strikerMove=1;
		}
	}
}



