#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include "headers.h"
#include "vis.h"
#include "winampdetectbeat.h"
//#include "structures.h"
//#include "beat_return.h"

const FPS_TIMER = 1;
const FPS_INTERVAL = 2000;
double input;
														
HGLRC	hRC=NULL;																					// Der OpenGL Rendering Context
HDC		hDC=NULL;																					// Geschützter GDI Device Context
HWND	hWnd=NULL;																					// Verweist später auf den Windows Handle
HINSTANCE hInstance;
HWND	hMainWnd;																					// main window handle
HDC		memDC;																						// memory device context
HBITMAP	memBM,																						// memory bitmap (for memDC)
		oldBM;																						// old bitmap (from memDC)																									// Die Instanz der Anwendung
				
char	szAppName[] = "Bouncing Balls";																		// Our window class, etc
char	*tmp=new char[70]; 
char	*title=new char[70];
char	*old_title=new char[70];
char	** title_winamp;
								
int		width=640 ,config_x = 1600 ,height=480 ,config_y	=1200 ;
int		key=1;		
int		current_window_size=4;											
int		now_playing,pos;
float	old_beat,beat_help						=0.0f;
bool	keys[256];																				// Vektor (Array) der den Status einzelner Tasten enthält (gedrückt/nicht gedrückt)
bool	done=FALSE;
bool	new_title=TRUE;
bool	fullscreen=TRUE;																			// Läuft das Programm im Vollbildmodus oder nicht?
bool	change_scene=TRUE;
bool	active=TRUE;

/* --------------------------------------------------------------------------------------------------------------------*/
/* -----------------------------------------------------winamp functions-----------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/
																									// configuration declarations
void	config_read(struct winampVisModule *this_mod);												// reads the configuration
void	config_write(struct winampVisModule *this_mod);												// writes the configuration
void	config_getinifn(struct winampVisModule *this_mod, char *ini_file);							// makes the .ini file filename																			
		winampVisModule *getModule(int which);														// returns a winampVisModule when requested. Used in hdr, below
void	config(struct winampVisModule *this_mod);													// configuration dialog
int		init(struct winampVisModule *this_mod);														// initialization for module
int		render(struct winampVisModule *this_mod);													// rendering for winamp module
void	quit(struct winampVisModule *this_mod);														// deinitialization for module
void	quit3(struct winampVisModule *this_mod);													// deinitialization for module and restart


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

/* --------------------------------------------------------------------------------------------------------------------*/
/* -------------Module header, includes version, description, and address of the module retriever function-------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

winampVisHeader hdr = { VIS_HDRVER, "Bouncing Balls Visualisation plugin for winamp", getModule };

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------module OpenGL---------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

winampVisModule mod =
{
	"Bouncing Balls Visualizer",
	NULL,																							// hwndParent
	NULL,																							// hDllInstance
	0,																								// sRate
	0,																								// nCh
	1,																								// latencyMS
	1,																								// delayMS
	2,																								// spectrumNch
	2,																								// waveformNch
	{ 0, },																							// spectrumData
	{ 0, },																							// waveformData
	config,
	init,
	render, 
	quit
};

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------Scene Variables---------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

#define M_PI	3.14159265358979323846

//Maximum input number for each sphere ranges from 0 to MAXSTRENGTH
#define MAXSTRENGTH 100

//Define the values for the number of spheres
#define MAXSPHERES 100
#define MINSPHERES 7
#define DEFAULTSPHERES 19

//Define the values for the max air-time in one direction
//so 1 sec means actually max 2 sec a ball can be in the air
#define MAXSEC 3.0
#define MINSEC 0.5
#define DEFAULTSEC 1.0

//Define the speedfactor which is multiplied whith the actual height of each ball,
//when he is in the air, so a higher speedfactor with would let a ball jump higher in
//the same amount of time than with a lower speedfactor
#define MAXSPEEDFACTOR 5.0
#define MINSPEEDFACTOR 0.5
#define DEFAULTSPEEDFACTOR 3.5

//define the number of different colors with which the balls can be initialized
#define MINCOLOR 0
#define MAXCOLOR 6
#define DEFAULTCOLOR 1

//show cones or not?
#define DEFAULTCONES false
//show the circle surface or not?
#define DEFAULTSURFACE true
//let the ball change the color when they are bumped up?
#define DEFAULTCOLORCHANGE true
//let ball first bounce a little up and down after they accept the next input?
#define DEFAULTBOUNCE false
//Output the current settings for all necessary user-variables on the screen?
#define DEFAULTOUTPUT true
//print out a short help on the screen?
#define DEFAULTHELP false
//output the waveform circles?
#define DEFAULTWAVEFORMCIRCLES false
//output the spheres?
#define SPHERESONOFF true
//squash the balls?
#define DEFAULTSQUASH false

#define DEFAULTSAMPLES 20
#define BT_WAVEFORM 0
#define BT_BEAT 1
#define BT_BASS 2
#define MAXPILLARHEIGHT 50
//set the decision variables to their default values

#define my_cos(n) cosTable[(int)(n*100)]
#define my_sin(n) sinTable[(int)(n*100)]


int NUMSPHERES = DEFAULTSPHERES;
int COLOR = DEFAULTCOLOR;
int NUM_CIRCLES = DEFAULTSAMPLES;
bool CONES = DEFAULTCONES;
bool SURFACE = DEFAULTSURFACE;
bool WAVEFORMCIRCLES = DEFAULTWAVEFORMCIRCLES;
bool COLORCHANGE = DEFAULTCOLORCHANGE;
bool BOUNCE = DEFAULTBOUNCE;
bool OUTPUT = DEFAULTOUTPUT;
bool HELP = DEFAULTHELP;
bool SPHERES = SPHERESONOFF;
bool SQUASH = DEFAULTSQUASH;
double sinTable[104];
double cosTable[104];
double SPEEDFACTOR = DEFAULTSPEEDFACTOR;
double NUMSEC = DEFAULTSEC;
double rotateFactor = 0;
double rotateRate = .05;

bool surfacetemp;
bool circlestemp;

//function definition for later use
void bump(struct SPHERE *sp, double strength);
void startTimer(struct SPHERE *sp);
void stopTimer(struct SPHERE *sp);
void doSort(int no, double z);
void initializeSpheres();
void changeColor(struct SPHERE *sp, int c);
void printOut(int x, int y, char * text);
void setOrthographicProjection();
void resetPerspectiveProjection();
double computeSoundInput(int index, int total, struct winampVisModule *this_mod, int BounceType);
double timerSeconds(struct SPHERE *sp);
void displayScene(struct winampVisModule *this_mod);
void myinit(void);
void keyboard(void);
void drawPillars (int i, int spherenum, bool orient, struct winampVisModule* this_mod);
void drawSquare (double a[3], double b[3], double c[3], double d[3]);
//Datastructure to safe the state of each single sphere
struct SPHERE {
	//radius
	double r;
	//x,z - coordinates
	double x;
	double z;
	GLfloat color[3];
	//is the sphere flying upwards at the moment?
	bool up;
	//is it his first way up after being bumped up?
	bool firstUp;
	//is he in air now or waiting for an input?
	bool inAir;
	//store the current air-time for each sphere
	double SEC;
	//contains the current height of the ball
	double height;
	//needed to store start and stop times of each ball, to
	//compute its height
	LARGE_INTEGER frequency_;
	LARGE_INTEGER startTime_;
	LARGE_INTEGER stopTime_;
};



/*Define global variables*/
//First define an array of Spheres
SPHERE *sphere = new SPHERE[MAXSPHERES];

//will later store the radius of the ground circle
double groundRad=0;
//this will determine the number of circles drawn in waveformcircle mode
int wfci_samples = 10;
//this will determine whether the color and alpha shift with the beat
bool wfcb_color = true;
//used to store the vectors for creating the spiral shape
double spir[12][2];
//view angle for the perspective view
double alpha=45.0;
//is used to order the Spheres by their z-values to print them out in the right
//order so that no unwanted overlap can happen
int order[MAXSPHERES][2];

#endif //DECLARATIONS_H
