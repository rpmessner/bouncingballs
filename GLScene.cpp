#include "declarations.h"

void myinit(void) 
{
	/*Initialize and compute the vectors for the spiral*/
	spir[0][0]=0.0;
	spir[0][1]=-1.0;
	spir[1][0]=-sin(30.0*M_PI/180);
	spir[1][1]=-cos(30.0*M_PI/180);
	spir[2][0]=-cos(30.0*M_PI/180);
	spir[2][1]=-sin(30.0*M_PI/180);
	spir[3][0]=-1.0;
	spir[3][1]=0.0;
	spir[4][0]=-cos(30.0*M_PI/180);
	spir[4][1]=sin(30.0*M_PI/180);
	spir[5][0]=-sin(30.0*M_PI/180);
	spir[5][1]=cos(30.0*M_PI/180);
	spir[6][0]=0.0;
	spir[6][1]=1.0;
	spir[7][0]=sin(30.0*M_PI/180);
	spir[7][1]=cos(30.0*M_PI/180);
	spir[8][0]=cos(30.0*M_PI/180);
	spir[8][1]=sin(30.0*M_PI/180);
	spir[9][0]=1.0;
	spir[9][1]=0;
	spir[10][0]=cos(30.0*M_PI/180);
	spir[10][1]=-sin(30.0*M_PI/180);
	spir[11][0]=sin(30.0*M_PI/180);
	spir[11][1]=-cos(30.0*M_PI/180);

/*create all needed spheres and initialize them with default values*/
	initializeSpheres();

/*Initialize the random number creator*/
	time_t t;
    time(&t);
    srand((unsigned int)t);  //initialize with date and time

/* attributes */

    glClearColor(0.0, 0.0, 0.0, 1.0); /* black background */

/* Lighting */
	glLoadIdentity();

	GLfloat light_ambient[] = {0.0,0.0,0.0,1.0};
	GLfloat light_diffuse[] = {1.0,1.0,1.0,1.0};
	//GLfloat light_specular[] = {1.0,1.0,1.0,1.0};
	GLfloat lmodel_ambient[] = {0.5,0.5,0.5,1.0};
	GLfloat mat_amb_diff[] = { 0.5, 0.5, 0.5, 1.0 };
	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	//GLfloat mat_shininess[] = { 40.0 };

	GLfloat light_position0[] = {1.0, 1.0, 0.0, 0.0 }; 

	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//Enable Lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void displayScene (struct winampVisModule *this_mod)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /*clear the window*/
	
	/*Draw circle surface*/
	if(SURFACE) {

		glColor3d(0.86,0.86,0.86);

		glBegin(GL_POLYGON);
			double rad=groundRad;
			double steps=1;
			for(double x=rad; x>=-rad; x-=steps) {
				double z=sqrt(((double)rad*(double)rad)-((double)x*(double)x));
				glVertex3d(x,0.0,z);
			}
			for(x=-rad; x<=rad; x+=steps) {
				double z=sqrt(((double)rad*(double)rad)-((double)x*(double)x));
				glVertex3d(x,0.0,-z);
			}
		glEnd();

	}

	/*Draw spheres and belonging cones to bump them*/
	for(int o=0; o<NUMSPHERES; o++) {
		glPushMatrix(); //store the origin
		
		//get the next sphere from the ordered list by index i
		int i=order[o][0];

		//Draw cones
		if(CONES) {

			glColor3d(0.9,0.9,0.9);
			glTranslatef(sphere[i].x,0.0,sphere[i].z);
			glRotatef(-90.0,1.0,0.0,0.0);
			
			//The cone is always 1/10 high as the height of its sphere
			if(sphere[i].firstUp) {
				glutSolidCone(sphere[i].r/1.5,
					         sphere[i].height/10, 
							 20.0+sphere[i].height/10,
							 10.0+sphere[i].height/8);
			}
			//if ball is bouncing draw the cone pretty small
			else {
				glutSolidCone(sphere[i].r/1.5,
					         0.1, 
					         20.0+(int)((1-NUMSPHERES/MAXSPHERES)*(sphere[i].r/10)),
							 3.0);
			}
				
			glPopMatrix();
			glPushMatrix();
		}

//-------->>>>>	
		//Sound input goes here for sphere with size sphere[i].r
		//Input ranges from 0-MAXSTRENGTH, so now from 0-100

		//change everything in the function to return an input the value
		//the function delivers you the index of the sphere which is i out of NUMSPHERE Spheres, so try to
		//keep it dynamic by base your return value on i and on NUMSPHERE, so if NUMSPHERE changes during
		//execution of the program your function is possible to always return values for each sphere
		double input=computeSoundInput(i);
		
		//this function bumps the ball up and is called each time, it actually just bumps the ball up,
		//when it is on the ground and so just disregard the input if not.
		bump(&sphere[i], input);
//-------->>>>>
		
		//Draw the sphere
		glColor3fv(sphere[i].color);
		glTranslatef(sphere[i].x,sphere[i].r+sphere[i].height,sphere[i].z);
		glutSolidSphere(sphere[i].r,
			           20.0+(int)((1-NUMSPHERES/MAXSPHERES)*(sphere[i].r/10)),
					   50.0+(int)((1-NUMSPHERES/MAXSPHERES)*(sphere[i].r))); 

		glPopMatrix(); //restore the origin
	}
	
/*Text output*/
	setOrthographicProjection(); //switch to orthographic projection to easier print out text
	glPushMatrix();
	glLoadIdentity();

	//Data ouput of current settings
	if(OUTPUT) {
		char temp[50];
		glColor3f(1.0,1.0,1.0);
		
		//Number of Balls
		sprintf(temp,"         Balls: %.d", NUMSPHERES);
		printOut(10,10,temp);
		//Airtime
		sprintf(temp,"  max. Airtime: %.1f", NUMSEC);
		printOut(10,25,temp);
		//Speedfactor
		sprintf(temp,"   Speedfactor: %.1f", SPEEDFACTOR);
		printOut(10,40,temp);
		//Cones on/off?
		sprintf(temp,"         Cones: %s", (CONES)?"on":"off");
		printOut(10,55,temp);
		//Surface on/off?
		sprintf(temp,"       Surface: %s", (SURFACE)?"on":"off");
		printOut(10,70,temp);
		//Bouncing balls?
		sprintf(temp,"Bouncing Balls: %s", (BOUNCE)?"on":"off");
		printOut(10,85,temp);
		//Color change?
		sprintf(temp,"  Change color: %s", (COLORCHANGE)?"on":"off");
		printOut(10,100,temp);
		//Index color of balls
		printOut(10,115,"   Start color: ");
		char * color;
		if(COLOR==0) color="white";
		if(COLOR==1) {color="red"; glColor3f(1.0,0.0,0.0);}
		if(COLOR==2) {color="green"; glColor3f(0.0,1.0,0.0);}
		if(COLOR==3) {color="blue";  glColor3f(0.0,0.0,1.0);}
		if(COLOR==4) {color="yellow";  glColor3f(1.0,1.0,0.0);}
		if(COLOR==5) {color="magenta";  glColor3f(1.0,0.0,1.0);}
		if(COLOR==6) {color="cyan";  glColor3f(0.0,1.0,1.0);}
		sprintf(temp,"%s", color);
		printOut(138,115,temp);
		
		//help message
		if(!HELP) {
			glColor3f(1.0,1.0,0.0);
			printOut(width-20*8,10,"Press \'h\' for help!");	
		}
	}

	//help instructions
	if(HELP) {
		glColor3f(1.0,1.0,0.0);
		int x=200;
		int y=0;

		printOut(x,y+10,"  h = switch help on/off");
		printOut(x,y+25,"  o = switch output of the current settings on/off");

		printOut(x,y+55,"-/+ = decrease/increase number of balls");
		printOut(x,y+70,"u/i = decrease/increase the maximum airtime of the balls");
		printOut(x,y+85,"j/k = decrease/increase the speed factor of the balls");
        printOut(x,y+100,"n/m = decrease/increase the start color of the balls");
		printOut(x,y+130,"  c = display cones?");
		printOut(x,y+145,"  s = display white circle surface?");
		printOut(x,y+160,"  b = allow balls to bounce on the ground?");
		printOut(x,y+175,"  l = allow balls to change color?");

		printOut(x,y+205,"  r = reset all settings to default value");

		printOut(x,y+235,"written by Ryan Messner and Simon Beisel");
	}

	glPopMatrix();
	resetPerspectiveProjection(); //reset to old view

	glFlush(); /* clear buffers */
	//glutSwapBuffers(); /*swap the buffers*/
}

void initializeSpheres() {
	groundRad=0;
    int i;
	for(i=0; i<MAXSPHERES; i++) order[i][1]=65000;

	for(i=0; i<NUMSPHERES; i++) {
		sphere[i].r=i+1;
		sphere[i].x=(i+1)*((NUMSPHERES/6)+3)*spir[i%12][0]+5; //position it along a spiral
		sphere[i].z=(i+1)*((NUMSPHERES/6)+3)*spir[i%12][1]+5; //position it along a spiral
		//only change color if it had no color so far
		if (!(sphere[i].color[0]>0.0)||!(sphere[i].color[1]>0.0)||!(sphere[i].color[2]>0.0)) {
			changeColor(&sphere[i], COLOR);
		} 
		sphere[i].up=true;
		sphere[i].firstUp=false;
		sphere[i].inAir=false;
		sphere[i].SEC=0;
		sphere[i].height=0;
		if (i==NUMSPHERES-1) {
			groundRad=sqrt((double)((sphere[i].x)*(sphere[i].x))+(double)((sphere[i].z)*(sphere[i].z)))+(double)(i*1.2);
		}
		doSort(i,sphere[i].z);
	}

	//to always have the same view on the scene independend of how big the circle surface is
	//and how many spheres are on it I compute the coordinates for y and z dynamically
	double distance=(double)(groundRad*1.02)/(double)(sin((alpha/2)*(M_PI/180)));
	double yz=sqrt((distance*distance)/2);
	
	//The view is along and on the z axis to the origin
	glLoadIdentity();
	gluLookAt(0.0, yz, yz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//-------->>>>>	
//That is your function, implement everything here and return a double value for the 
//strength of the tune in the specific frequency range for sphere i
double computeSoundInput(int i) {
	//at the moment it is just a random number	
	return (double)((rand() % 101)+0);
}
//-------->>>>>

//is used to print out several text at the position x,y
void printOut(int x, int y, char * text) {
	glRasterPos2i(x,y);
	int length=strlen(text);
	for (int i = 0; i < length; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
	}
}

//Most important function of the whole program, it gets a strength variable as an input and based
//on it bumps a ball in the air if it is laying still on the ground and is not bouncing or in the air
void bump(struct SPHERE *sp, double strength) {

	//if ball not in Air compute it current new airtime SEC based on the strength value
	if(!sp->inAir&&strength>0) {
		startTimer(sp); //start the timer
		//this is the main calculation, SEC is always higher for smaller balls than for
		//bigger balls with the same strength input, that is what the second term of the
		//function is for, the last term is the max air time and is calculated in
		sp->SEC=strength/MAXSTRENGTH*(1-(sp->r/(NUMSPHERES*2)))*NUMSEC;
		
		//change color of the ball slightly
		if(COLORCHANGE) {
			int choice=(int) (rand() % 3);
			int plusminus=(int) (rand() % 2);
			float change=((float)(rand() % 10)+5)/100.0;
			if (plusminus==1) change=-change;
			sp->color[choice]+=(sp->color[choice]+change<1.0&&sp->color[choice]+change>0.0)?change:-change;
		}

		sp->firstUp=true;
		sp->inAir=true;
	}
	
	//stop the timer and compute seconds till the start of the timer
	stopTimer(sp);
	double seconds=timerSeconds(sp);
	
	//compute the speed based on the speedfactor and the number of spheres
	double speed=NUMSPHERES*SPEEDFACTOR;

	//let the ball fly up and down
	if(sp->SEC>0.05)
	{
		if(!BOUNCE) sp->inAir=true;
		
		//up-direction
		if(sp->up==true) {
			double max=4.905*sp->SEC*sp->SEC;
			sp->height=(max-4.905*(sp->SEC-seconds)*(sp->SEC-seconds))*speed;
		}
		//down-direction
		else {
			double max=4.905*sp->SEC*sp->SEC;
			sp->height=(max-4.905*seconds*seconds)*speed;
		}
		
		//if ball reaches the floor or is at the top of its flight turn the direction and
		//probably decrease the height for let the ball bounce
		if(seconds>sp->SEC) {
				sp->up=(sp->up==true)?false:true;
				startTimer(sp);
				if(sp->SEC>0&&sp->up==true) {
					sp->firstUp=false;
					sp->SEC-=(sp->SEC*4/9);

					if(!BOUNCE) sp->inAir=false;
				}
					
		}
	}
	//finished with bouncing or at the ground again and accepting input again
	else {
		sp->inAir=false;
	}
}

/*function to change color of one sphere*/
void changeColor(struct SPHERE *sp, int c) {
	double red=0.0;
	double green=0.0;
	double blue=0.0;
	
	//0 = white
	//1 = red
	//2 = green
	//3 = blue
	//4 = yellow
	//5 = magenta
	//6 = cyan
	if(c==1||c==4||c==5||c==0) red=1.0; //red
	if(c==2||c==4||c==6||c==0) green=1.0; //green
	if(c==3||c==5||c==6||c==0) blue=1.0; //blue

	sp->color[0]=red; //SET COLOR
	sp->color[1]=green;
	sp->color[2]=blue;
}

/*Sort the z value from one sphere with index no, into an array*/
void doSort(int no, double z) {
	int place=0;
	for(int i=0; i<NUMSPHERES; i++) {
		if(z<order[i][1]) {
			place=i;
			break;
		}
	}
	for(i=NUMSPHERES-1; i>place; i--) {
		order[i][0]=order[i-1][0];
		order[i][1]=order[i-1][1];
	}
	order[place][0]=no;
	order[place][1]=z;
}

/*switch to an orthographic projection which makes it easier to output text*/
void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, width, 0, height);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -height, 0);
	glMatrixMode(GL_MODELVIEW);
}

/*switch back to the perspective projection and reset the matrix*/
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

/*start the timer for a sphere*/
void startTimer(struct SPHERE *sp) {
	::QueryPerformanceCounter(&sp->startTime_);
}

/*stop the timer for a sphere*/
void stopTimer(struct SPHERE *sp) {
	::QueryPerformanceCounter(&sp->stopTime_);
}

/*get the seconds between start and stop of the timer for a sphere*/
double timerSeconds(struct SPHERE *sp) {
	if (!::QueryPerformanceFrequency(&sp->frequency_)) 
	throw "Error with QueryPerformanceFrequency";
	return (double)(sp->stopTime_.QuadPart - sp->startTime_.QuadPart) / (double) sp->frequency_.QuadPart;
}
