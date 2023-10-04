// Pool Game.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "stdafx.h"
#include "stdafx.h"
#include<glut.h>
#include<math.h>
#include"simulation.h"
#include "threaded_client.h"

namespace net {
	Client client;
}


int n = 0;    // a counter for the number of  players' balls
bool redTurn = true;  // it is a boolean checking if is the first player's turn
bool gameFinished = false;    // a variable that checks if the game is finished

std::string emptyKey;

player *Players[100];      // a dynamic array of 100 players
int playersCount = -1;     // is a counter for the player class, used to increment every time player is created


//creates 2 new players in the array of Players[100]
//Also gets input the names of players
void Add2Players(void)
{
	if (playersCount <= 100)
	{
		//Create 2 players
		playersCount++;
		Players[playersCount] = new player;
		
		playersCount++;
		Players[playersCount] = new player;
		
		std::cout << "\n------------------ NEW GAME -------------------\n";
		std::cout << "--2 New Players--\n";

		std::cout << "First Player's name: ";
		std::cin >> Players[playersCount - 1]->name;

		std::cout << "\nSecond Player's name: ";
		std::cin >> Players[playersCount]->name;

		std::cout << "\n\nafter game is finshed, press enter to get the results!\n" << "Now press any key, then enter to continue\n";
		std::cin >> emptyKey;
	}

}


//cue variables
float gCueAngle = 0.0;
float gCuePower = 0.25;
bool gCueControl[4] = {false,false,false,false};
float gCueAngleSpeed = 2.0f; //radians per second
float gCuePowerSpeed = 0.25f;
float gCuePowerMax = 0.75;
float gCuePowerMin = 0.1;
float gCueBallFactor = 8.0;
bool gDoCue = true;

//camera variables
vec3 gCamPos(0.0, 3.9, 2.1);
vec3 gCamLookAt(0.0, -0.5, 0.0);
bool gCamRotate = true;
float gCamRotSpeed = 0.2;
float gCamMoveSpeed = 0.5;
bool gCamL = false;
bool gCamR = false;
bool gCamU = false;
bool gCamD = false;
bool gCamZin = false;
bool gCamZout = false;


//rendering options
#define DRAW_SOLID	(0)

void DoCamera(int ms)
{
	static const vec3 up(0.0,1.0,0.0);

	if(gCamRotate)
	{
		if(gCamL)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localL = up.Cross(camDir);
			vec3 inc = (localL* ((gCamRotSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir + inc;
		}
		if(gCamR)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = up.Cross(camDir);
			vec3 inc = (localR* ((gCamRotSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir - inc;
		}
		if(gCamU)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localUp = localR.Cross(camDir);
			vec3 inc = (localUp* ((gCamMoveSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir + inc;
		}
		if(gCamD)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localUp = localR.Cross(camDir);
			vec3 inc = (localUp* ((gCamMoveSpeed*ms)/1000.0) );
			gCamLookAt = gCamPos + camDir - inc;
		}		
	}
	else
	{
		if(gCamL)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localL = up.Cross(camDir);
			vec3 inc = (localL* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
		if(gCamR)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 inc = (localR* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
		if(gCamU)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localUp = localR.Cross(camDir);
			vec3 inc = (localUp* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
		if(gCamD)
		{
			vec3 camDir = (gCamLookAt - gCamPos).Normalised();
			vec3 localR = camDir.Cross(up);
			vec3 localDown = camDir.Cross(localR);
			vec3 inc = (localDown* ((gCamMoveSpeed*ms)/1000.0) );
			gCamPos += inc;
			gCamLookAt += inc;
		}
	}

	if(gCamZin)
	{
		vec3 camDir = (gCamLookAt - gCamPos).Normalised();
		vec3 inc = (camDir* ((gCamMoveSpeed*ms)/1000.0) );
		gCamPos += inc;
		gCamLookAt += inc;
	}
	if(gCamZout)
	{
		vec3 camDir = (gCamLookAt - gCamPos).Normalised();
		vec3 inc = (camDir* ((gCamMoveSpeed*ms)/1000.0) );
		gCamPos -= inc;
		gCamLookAt -= inc;
	}
}


void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set camera
	glLoadIdentity();
	gluLookAt(gCamPos(0),gCamPos(1),gCamPos(2),gCamLookAt(0),gCamLookAt(1),gCamLookAt(2),0.0f,1.0f,0.0f);

	//draw the  white ball
		glColor3f(1.0,1.0,1.0);
		glPushMatrix();
		glTranslatef(gTable.balls[0].position(0),(BALL_RADIUS/2.0),gTable.balls[0].position(1));
		#if DRAW_SOLID
		glutSolidSphere(gTable.balls[0].radius,32,32);
		#else
		glutWireSphere(gTable.balls[0].radius,12,12);
		#endif
		glPopMatrix();
		glColor3f(1.0,0.0,0.0);
	
	//draw the red balls
	glColor3f(1.0, 0.0, 0.0);
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(gTable.balls1[i].position(0), (BALL_RADIUS / 2.0), gTable.balls1[i].position(1));
		#if DRAW_SOLID
		glutSolidSphere(gTable.balls1[i].radius, 32, 32);
		#else
		glutWireSphere(gTable.balls1[i].radius, 12, 12);
		#endif
		glPopMatrix();
		glColor3f(1.0, 0.0, 0.0);
	}

		//draw the blue balls
		glColor3f(0.0, 0.0, 1.0);
		for (int i = 0; i < 4; i++)
		{
			glPushMatrix();
			glTranslatef(gTable.balls2[i].position(0), (BALL_RADIUS / 2.0), gTable.balls2[i].position(1));
			#if DRAW_SOLID
			glutSolidSphere(gTable.balls2[i].radius, 32, 32);
			#else
			glutWireSphere(gTable.balls2[i].radius, 12, 12);
			#endif
			glPopMatrix();
			glColor3f(0.0, 0.0, 1.0);
		}



	glColor3f(0.0,0.0,1.0);

	//draw the table
	for(int i=0;i<NUM_CUSHIONS;i++)
	{	
		glBegin(GL_LINE_LOOP);
		glVertex3f (gTable.cushions[i].vertices[0](0), 0.0, gTable.cushions[i].vertices[0](1));
		glVertex3f (gTable.cushions[i].vertices[0](0), 0.1, gTable.cushions[i].vertices[0](1));
		glVertex3f (gTable.cushions[i].vertices[1](0), 0.1, gTable.cushions[i].vertices[1](1));
		glVertex3f (gTable.cushions[i].vertices[1](0), 0.0, gTable.cushions[i].vertices[1](1));
		glEnd();
	}

	for(int i=0;i<gTable.parts.num;i++)
	{
		glColor3f(1.0,0.0,0.0);
		glPushMatrix();
		glTranslatef(gTable.parts.particles[i]->position(0),gTable.parts.particles[i]->position(1),gTable.parts.particles[i]->position(2));
		#if DRAW_SOLID
		glutSolidSphere(0.002f,32,32);
		#else
		glutWireSphere(0.002f,12,12);
		#endif
		glPopMatrix();		
	}
	

	//draw the cue
	if(gDoCue)
	{
		glBegin(GL_LINES);
		float cuex = sin(gCueAngle) * gCuePower;
		float cuez = cos(gCueAngle) * gCuePower;
		glColor3f(0.0,0.5,0.5);
		if (redTurn == true && n < 4)
		{
			glVertex3f(gTable.balls1[n].position(0), (BALL_RADIUS / 2.0f), gTable.balls1[n].position(1));
			glVertex3f(gTable.balls1[n].position(0) + cuex, (BALL_RADIUS / 2.0f), gTable.balls1[n].position(1) + cuez);
		}
		else if (redTurn == false && n < 4)
		{
			glVertex3f(gTable.balls2[n].position(0), (BALL_RADIUS / 2.0f), gTable.balls2[n].position(1));
			glVertex3f(gTable.balls2[n].position(0) + cuex, (BALL_RADIUS / 2.0f), gTable.balls2[n].position(1) + cuez);
		}

		glColor3f(1.0,0.0,0.0);
		glEnd();
	}

	//glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void SpecKeyboardFunc(int key, int x, int y) 
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
		{
			gCueControl[0] = true;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			gCueControl[1] = true;
			break;
		}
		case GLUT_KEY_UP:
		{
			gCueControl[2] = true;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			gCueControl[3] = true;
			break;
		}
	}
}

void SpecKeyboardUpFunc(int key, int x, int y) 
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
		{
			gCueControl[0] = false;
			break;
		}
		case GLUT_KEY_RIGHT:
		{
			gCueControl[1] = false;
			break;
		}
		case GLUT_KEY_UP:
		{
			gCueControl[2] = false;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			gCueControl[3] = false;
			break;
		}
	}
}

void KeyboardFunc(unsigned char key, int x, int y) 
{
	switch(key)
	{
	case(13):
		{
			//if gDoCue is false and n = 5 do not apply force to the balls
			if(gDoCue && n < 4)
			{
				vec2 imp(	(-sin(gCueAngle) * gCuePower * gCueBallFactor),
							(-cos(gCueAngle) * gCuePower * gCueBallFactor));
				if (redTurn == true)
				{
					gTable.balls1[n].ApplyImpulse(imp);
					redTurn = false;
				}
				else if (redTurn == false)
				{
					gTable.balls2[n].ApplyImpulse(imp);
					redTurn = true;
					n++;
				}

			}

			//if game is finished, calculate the distance of all balls both of 2 players and print out the results
			//Also send the results to the server
			else if (gameFinished == true)
			{
				for (int i = 0; i < 4; i++)
				{
					Players[playersCount - 1]->score += gTable.JackDistance(gTable.balls1[i]);
					Players[playersCount]->score += gTable.JackDistance(gTable.balls2[i]);
				}

				std::cout << Players[playersCount - 1]->name <<" balls distance from jack is:\n" << Players[playersCount - 1]->score << "\n\n";
				std::cout << Players[playersCount]->name << " balls distance from jack is:\n" << Players[playersCount]->score << "\n\n";

				if (Players[playersCount - 1]->score < Players[playersCount]->score)
					std::cout << Players[playersCount - 1]->name << " wins! \n";
				else
					std::cout << Players[playersCount]->name << " wins! \n";

				for (int i = 0; i <= playersCount; i++)
				{
					Players[i]->ranking = 1;
					for (int j = 0; j <= playersCount; j++)
					{
						if (Players[i]->score > Players[j]->score)
						{
							Players[i]->ranking++;
						}

					}
				}

				for (int i = 0; i <= playersCount; i++)
				{
					std::cout << "Player: " << Players[i]->name << " in Ranking: " << Players[i]->ranking << " with score: " << Players[i]->score << "\n";
					std::string msg = Players[i]->name + "s score : " + std::to_string(Players[i]->score) + "in ranking: " + std::to_string(Players[i]->ranking) + "\n";
					net::client.sendthis(msg.c_str());
				}
				

			}

			break;
		}
	case(27):
		{
			// if Esc key is pressed return all balls to starting position
			//and add 2 new players for the next game
			for(int i=0;i<NUM_BALLS;i++)
			{
				gTable.balls[i].Reset();
			}

			n = 0;
			redTurn = true;
			gameFinished = false;
			Add2Players();
			break;
		}
	case(32):
		{
			gCamRotate = false;
			break;
		}
	case('z'):
		{
			gCamL = true;
			break;
		}
	case('c'):
		{
			gCamR = true;
			break;
		}
	case('s'):
		{
			gCamU = true;
			break;
		}
	case('x'):
		{
			gCamD = true;
			break;
		}
	case('f'):
		{
			gCamZin = true;
			break;
		}
	case('v'):
		{
			gCamZout = true;
			break;
		}
	}

}

void KeyboardUpFunc(unsigned char key, int x, int y) 
{
	switch(key)
	{
	case(32):
		{
			gCamRotate = true;
			break;
		}
	case('z'):
		{
			gCamL = false;
			break;
		}
	case('c'):
		{
			gCamR = false;
			break;
		}
	case('s'):
		{
			gCamU = false;
			break;
		}
	case('x'):
		{
			gCamD = false;
			break;
		}
	case('f'):
		{
			gCamZin = false;
			break;
		}
	case('v'):
		{
			gCamZout = false;
			break;
		}
	}
}

void ChangeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0) h = 1;
	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,0.2,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0,0.7,2.1, 0.0,0.0,0.0, 0.0f,1.0f,0.0f);
	gluLookAt(gCamPos(0),gCamPos(1),gCamPos(2),gCamLookAt(0),gCamLookAt(1),gCamLookAt(2),0.0f,1.0f,0.0f);
}

void InitLights(void)
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat light_ambient[] = { 2.0, 2.0, 2.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
}

void UpdateScene(int ms) 
{
	//if all balls are static then give true for the cue
	if (gTable.AnyBallsMoving() == false)
	{
		gDoCue = true;
	}
	else gDoCue = false;

	if(gDoCue)
	{
		if(gCueControl[0]) gCueAngle -= ((gCueAngleSpeed * ms)/1000);
		if(gCueControl[1]) gCueAngle += ((gCueAngleSpeed * ms)/1000);
		if (gCueAngle <0.0) gCueAngle += TWO_PI;
		if (gCueAngle >TWO_PI) gCueAngle -= TWO_PI;

		if(gCueControl[2]) gCuePower += ((gCuePowerSpeed * ms)/1000);
		if(gCueControl[3]) gCuePower -= ((gCuePowerSpeed * ms)/1000);
		if(gCuePower > gCuePowerMax) gCuePower = gCuePowerMax;
		if(gCuePower < gCuePowerMin) gCuePower = gCuePowerMin;
	}

	DoCamera(ms);

	gTable.Update(ms);
	
	if (n > 3)
	{
		gameFinished = true;
	}


	glutTimerFunc(SIM_UPDATE_MS, UpdateScene, SIM_UPDATE_MS);
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	if (argc < 3) {
		printf("Usage: %s hotname port\n", argv[0]);
		return 0;
	}

	printf("%s %s %s\n", argv[0], argv[1], argv[2]);
	//start and connect the client to the server
	net::client.start(argv[1], argv[2]);
	net::client.interact();

	//choose randomly between of 5 cushions
	srand((unsigned)time(0));
	int Courses = rand() % 5 + 1;
	switch (Courses)
	{
	case(1):
	{
		gTable.SetupCushions();
		break;
	}
	case(2):
	{
		gTable.SetupCushions2();
		break;
	}
	case(3):
	{
		gTable.SetupCushions3();
		break;
	}
	case(4):
	{
		gTable.SetupCushions4();
		break;
	}
	case(5):
	{
		gTable.SetupCushions5();
		break;
	}

	}

	gTable.balls[0].radius = BALL_RADIUS / 2.0f;

	//function is called once here, for the game to be started with 2 players
	Add2Players();

	glutInit(&argc, ((char **)argv));
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE| GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1000,700);
	//glutFullScreen();
	glutCreateWindow("Boules Game");
	#if DRAW_SOLID
	InitLights();
	#endif
	glutDisplayFunc(RenderScene);
	glutTimerFunc(SIM_UPDATE_MS, UpdateScene, SIM_UPDATE_MS);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(RenderScene);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(KeyboardFunc);
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutSpecialFunc(SpecKeyboardFunc);
	glutSpecialUpFunc(SpecKeyboardUpFunc);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	
}
