#include "declarations.h"

GLuint			m_texture[];

int				xmin=1,xmax=20;

#ifdef __cplusplus
extern "C" {
#endif
__declspec( dllexport ) winampVisHeader *winampVisGetHeader()
{
	return &hdr;
}
#ifdef __cplusplus
}
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


winampVisModule *getModule(int which)
{
	switch (which)
	{
		case 0: return &mod;
		default:return NULL;
	}
}

char* winampGetTitle(HWND handle)
{ 
	GetWindowText(handle,tmp,70);											
	pos = strcspn( tmp, "Winamp");
	strncpy(title,tmp,strlen(tmp)-9);  
	return title;
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------winamp config function------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

void config(struct winampVisModule *this_mod)
{
	MessageBox(	this_mod->hwndParent,
				"Bouncing Balls Visualizer /n(c) 2004 Ryan Messner and Simon Biesel",
				"Configuration",MB_OK);
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------frame rate function---------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;												
    static float lastTime			= 0.0f;												
	static char strFrameRate[50] = {0};													
	float currentTime = GetTickCount() * 0.001f;				
    ++framesPerSecond;
    if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		sprintf(strFrameRate, "FPS: %d -Bouncing Balls- Ryan Messner & Simon Beisel", int(framesPerSecond));
		SetWindowText(hMainWnd, strFrameRate);
        framesPerSecond = 0;
    }
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------KillGL function-------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

GLvoid KillGLWindow(struct winampVisModule *this_mod)
{
	if (fullscreen)																		// Kontrolle auf Vollbildmodus
	{
		ChangeDisplaySettings(NULL,0);													// Zurück zum Desktop
		ShowCursor(TRUE);																// Der abgeschaltete Mauszeiger wird wieder angezeigt
	}
	if (hRC)																			// Rendering Context (RC) vorhanden?
	{
		if (!wglMakeCurrent(NULL,NULL))													// Kann der DC und RC überhaupt gelöscht werden?
		{
			MessageBox(	NULL,"Entfernen des DC und RC fehlgeschlagen.","Fehler",
						MB_OK | MB_ICONINFORMATION);
		}
   
		if (!wglDeleteContext(hRC))														// Kann der RC gelöscht werden?
		{
			MessageBox(	NULL,"Entfernen des RC fehlgeschlagen.","Fehler...",
						MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;																		// Der RC wird NULL gesetzt, also entfernt
	}
}
/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------Resize function-------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

GLvoid ReSizeGLScene(GLsizei w, GLsizei h) 
{	
	width=w;
	height=h;

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(alpha,(GLfloat)w/(GLfloat)h, 0.0, 100.0); //perspective view
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//to always have the same view on the scene independend of how big the circle surface is
	//and how many spheres are on it I compute the coordinates for y and z dynamically
	double distance=(double)(groundRad*1.02)/(double)(sin((alpha/2)*(M_PI/180)));
	double yz=sqrt((distance*distance)/2);

	//The view is along and on the z axis to the origin
	gluLookAt(0.0, yz, yz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------change resolution-----------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

bool change_resolution(struct winampVisModule *this_mod)
{
	if (fullscreen)																		// Soll im Vollbildmodus gestartet werden
	{
		DWORD dwExStyle2;																	
		DWORD dwStyle2;
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));							// Diese wird geleert
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);								// dmsize soll genauso groß wie die dmScreenSettings sein
		dmScreenSettings.dmPelsWidth = width; 
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = 32; 
		dmScreenSettings.dmFields=DM_BITSPERPEL|
		DM_PELSWIDTH|DM_PELSHEIGHT;	
		RECT WindowRect2;																// Speicher für aktuelle Auflösung
		WindowRect2.left=(long)0;														// Die linke Seite des Rechtecks wirtd auf 0 gesetzt
		WindowRect2.right=(long)width;													// Hier wird die gewünschte Breite des Fensters gespeichert
		WindowRect2.top=(long)0; 														// Die obere Seite wird auch auf 0 gesetzt
		WindowRect2.bottom=(long)height;												// Und hier wird die Höhe abgelegt 
		dwExStyle2=WS_EX_APPWINDOW;														// Fenstereigenschaften
		dwStyle2=WS_POPUP; 
		ShowCursor(FALSE);
		ChangeDisplaySettings(&dmScreenSettings,0);										// Zurück zum Desktop
		ReSizeGLScene(width, height);
		AdjustWindowRectEx(&WindowRect2,dwStyle2,FALSE,dwExStyle2);
		SetForegroundWindow(hMainWnd); 
		SetFocus(hMainWnd); 
	}
																						// Der Mauszeiger wird nicht angezeigt

	else
	{
		DWORD dwExStyle2;																	
		DWORD dwStyle2;
		DEVMODE dmScreenSettings;														// 
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));							// 
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);								// 
		dmScreenSettings.dmPelsWidth = width; 
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = 32; 
		dmScreenSettings.dmFields=DM_BITSPERPEL|
		DM_PELSWIDTH|DM_PELSHEIGHT;
		dwExStyle2=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;									// Das Fenster soll zusätzlich einen 3D Rahmen bekommen
		dwStyle2=WS_OVERLAPPEDWINDOW;													// Ein typisches Windowsfenster mit Minimieren, Maximieren, etc
		ReSizeGLScene(width, height);
		RECT WindowRect2;																// Speicher für aktuelle Auflösung
		WindowRect2.left=(long)0;														// Die linke Seite des Rechtecks wirtd auf 0 gesetzt
		WindowRect2.right=(long)width;													// Hier wird die gewünschte Breite des Fensters gespeichert
		WindowRect2.top=(long)0; 														// Die obere Seite wird auch auf 0 gesetzt
		WindowRect2.bottom=(long)height;												// Und hier wird die Höhe abgelegt 
		dwExStyle2=WS_EX_APPWINDOW;														// Fenstereigenschaften
		dwStyle2=WS_POPUP; 
		SetWindowPos(hMainWnd,HWND_TOP, 0, 0,width,height,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		SetForegroundWindow(hMainWnd); 
		SetFocus(hMainWnd);

	}
	return 0;
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* -----------------------------------------------OPENGL init function-------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

bool InitGL(struct winampVisModule *this_mod)							
{	
	if (width==640)
		current_window_size=0;
	if (width==800)
		current_window_size=1;
	if (width==1024)
		current_window_size=2;
	if (width==1280)
		current_window_size=3;
	if (width==1600)
		current_window_size=4;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);																	// Enables Clearing Of The Depth Buffer
	glShadeModel(GL_SMOOTH);															// Enables Smooth Color Shading
	glMatrixMode(GL_PROJECTION);														// Select The Projection Matrix
	glLoadIdentity();																	// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);					// Calculate The Aspect Ratio Of The Window
	glMatrixMode(GL_MODELVIEW);															// Select The Modelview Matrix
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_TEXTURE_2D);
																					
	return TRUE;																		// Initialization Went OK
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------init function for our window------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

int init(struct winampVisModule *this_mod)
{
	//config_read(this_mod);

	/* ----------------------------------------------------Register our window class-----------------------------------*/
	/* ----------------------------------------------------------------------------------------------------------------*/

	GLuint PixelFormat;																		// Speichert das Pixelformat
	WNDCLASS wc;																			// wc wird eine Instanz der Fensterklasse
	DWORD dwExStyle;																		// weitere Informationen
	DWORD dwStyle;																			// Fensterinformationen
	RECT WindowRect;																		// Speicher für aktuelle Auflösung
	WindowRect.left=(long)0;																// Die linke Seite des Rechtecks wirtd auf 0 gesetzt
	WindowRect.right=(long)width;															// Hier wird die gewünschte Breite des Fensters gespeichert
	WindowRect.top=(long)0; 																// Die obere Seite wird auch auf 0 gesetzt
	WindowRect.bottom=(long)height;															// Und hier wird die Höhe abgelegt 
	hInstance = this_mod->hDllInstance; 
	memset(&wc,0,sizeof(wc));
	wc.lpfnWndProc = (WNDPROC) WndProc;
	wc.hInstance = this_mod->hDllInstance;													// hInstance of DLL
	wc.lpszClassName = szAppName;															// our window class name
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);												// Lädt einen Cursor
	wc.lpszMenuName = NULL;																	// Auch ein Menü wird nicht benötigt.
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", 
	//	"Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	//{
		fullscreen=FALSE;																	// Windowed Mode
	//}

	if (!RegisterClass(&wc)) 
	{
		MessageBox(	NULL,"Can't register window class.","ERROR",
					MB_OK|MB_ICONEXCLAMATION);
		return 1;
	}
	if (fullscreen)																			// Soll im Vollbildmodus gestartet werden
	{
		DEVMODE dmScreenSettings;															// Instanz von DEVMODE wird erzeugt
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));								// Diese wird geleert
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);									// dmsize soll genauso groß wie die dmScreenSettings sein
		dmScreenSettings.dmPelsWidth = width; 
		dmScreenSettings.dmPelsHeight = height;
		dmScreenSettings.dmBitsPerPel = 32; 
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;											
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)
								!=DISP_CHANGE_SUCCESSFUL)    
		{
			if (MessageBox(NULL,"Fullscreen device not available , Do you want to start in a windowed mode?",
				"Problem",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE; 
			}

			else
			{
				return FALSE;
			}
		}
	}
	if (fullscreen) 
	{
		dwExStyle=WS_EX_APPWINDOW;															// Fenstereigenschaften
		dwStyle=WS_POPUP; 
		ShowCursor(FALSE);																	// Der Mauszeiger wird nicht angezeigt
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;										// Das Fenster soll zusätzlich einen 3D Rahmen bekommen
		dwStyle=WS_OVERLAPPEDWINDOW;														// Ein typisches Windowsfenster mit Minimieren, Maximieren, etc
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);								// Fenster wird angepasst
	hMainWnd = CreateWindowEx(dwExStyle,													// these exstyles put a nice small frame, but also a button in the taskbar
		szAppName,																			// our window class name
		this_mod->description,																// use description for a window title
		WS_CLIPSIBLINGS |																	// Wird von OpenGL benötigt
		WS_CLIPCHILDREN |																	// Wird auch von OpenGL benötigt
		dwStyle,
		0,0,																				// screen position (read from config)
		WindowRect.right-WindowRect.left,													// Hier werden die ermittelten Werte für die Breite eingesetzt
		WindowRect.bottom-WindowRect.top,													// und hier für die Länge
		this_mod->hwndParent,																// parent window (winamp main window)
		NULL,																				// no menu
		this_mod->hDllInstance,																// hInstance of DLL
		0);																					// no window creation data
	if (!hMainWnd) 
	{
		MessageBox(this_mod->hwndParent,"Error creating window","ERROR",MB_OK);
		return 1;
	}
	static PIXELFORMATDESCRIPTOR pfd=														// pdf ist jetzt ein PIXELFORMATDESCRIPTOR
	{
		sizeof(PIXELFORMATDESCRIPTOR), 
		1,																					// Versionsnummer
		PFD_DRAW_TO_WINDOW |																// Das Format muss in Fenster sichtbar sein können
		PFD_SUPPORT_OPENGL |																// OpenGL muss unterstützt werden
		PFD_DOUBLEBUFFER,																	// Double Buffering muss unterstützt werden
		PFD_TYPE_RGBA,																		// Das RGBA (Rot,Grün,Blau,Alpha(Transparenz)) muss unterstützt werden
		32,																					// Die Farbtiefe, die schon übergeben wurde, wird hier benötigt
		0, 0, 0, 0, 0, 0,																	// wird nicht benötigt
		0,																					// kein Alpha Buffer
		0,																					// Shift Bit ignoriert
		0,																					// kein Accumulation Buffer
		0, 0, 0, 0,																			// nicht benötigt
		32,																					// 16Bit Z-Buffer (Depth Buffer)
		0,																					// kein Stencil Buffer
		0,																					// kein Auxiliary Buffer
		PFD_MAIN_PLANE,																		// Die Hauptebene auf die später gezeichnet wird
		0,																					// unwichtig
		0, 0, 0																				// keine Ebenenmasken benötigt
	};
	if (!(hDC=GetDC(hMainWnd)))																// Versuch, den DC zu bekommen
	{
		KillGLWindow(this_mod);																// Alles rückgängig machen

		MessageBox(	NULL,"NO dc available.","ERROR",
					MB_OK|MB_ICONEXCLAMATION);
		return FALSE;																		// FALSE zurückgeben, beenden
	}
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))											// Kann Windows ein passendes finden? 
	{ 
		KillGLWindow(this_mod);																// Alles zurücksetzen
		MessageBox(	NULL,"Can't find pixelformat.","ERROR",
					MB_OK|MB_ICONEXCLAMATION);
		return FALSE;																		// FALSE zurück und Ende.
	}
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
    {
		KillGLWindow(this_mod);																// Leider nicht, Fehlerpopup und raus
		MessageBox(	NULL,"Can't use pixelformat.",
					"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;																		// FALSE zurück und raus
	}
	if (!(hRC=wglCreateContext(hDC)))														// Versuch den RC zu bekommen
    {
		KillGLWindow(this_mod);																// Alles rückgängig machen
		MessageBox(	NULL,"Can't get rendering context.","ERROR",
					MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	if(!wglMakeCurrent(hDC,hRC))															// Versuch den RC zu aktivieren
	{
		KillGLWindow(this_mod);																// hat nicht geklappt, also alles zurück
		MessageBox(	NULL,"Can't activate rendering context.","ERROR",
					MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	ShowWindow(hMainWnd,SW_SHOW);															// Fenster anzeigen
	SetForegroundWindow(hMainWnd);															// Priorität des Programms wird erhöht
	SetFocus(hMainWnd);																		// Tastatureingaben werden jetzt an das Programm geleitet
	ReSizeGLScene(width, height);															// Die Perspektive wird aktiviert 
	
	SetTimer(hMainWnd , FPS_TIMER, FPS_INTERVAL, NULL);

	myinit();
	initializeSpheres();

	return 0;								
}

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
	
	for (int i = 0; i <= 103; i++)
	{
		cosTable[i] = cos((double)(M_PI*2*i/100.0));
		sinTable[i] = sin((double)(M_PI*2*i/100.0));
	}
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
	GLfloat light_specular[] = {1.0,1.0,1.0,1.0};
	GLfloat lmodel_ambient[] = {0.5,0.5,0.5,1.0};
	GLfloat mat_amb_diff[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 40.0 };

	GLfloat light_position0[] = {1.0, 1.0, 0.0, 0.0 }; 

	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//Enable Lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}
/* --------------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------mainrenderfunction-----------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

int render(struct winampVisModule *this_mod)
{	
	displayScene(this_mod);
	SwapBuffers(hDC);																				// Die Puffer werden getauscht
	CalculateFrameRate();
	if (done==TRUE)
		return 1;
	else	
		return 0;			
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------quitfunction-----------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

void quit(struct winampVisModule *this_mod)
{
	SelectObject(memDC,oldBM);																		// delete our doublebuffer
	DeleteObject(memDC);
	DeleteObject(memBM);	
	DestroyWindow(hMainWnd);																		// delete our window
	UnregisterClass(szAppName,this_mod->hDllInstance);												// unregister window class
	KillGLWindow(this_mod);	
}

void quit3(struct winampVisModule *this_mod)
{
	if (current_window_size==0)
	{
		width=640;
		height=480;
	}
	if (current_window_size==1)
	{
		width=800;
		height=600;
	}
	if (current_window_size==2)
	{
		width=1024;
		height=768;
	}
	if (current_window_size==3)
	{
		width=1280;
		height=1024;
	}
	if (current_window_size==4)
	{
		width=1600;
		height=1200;
	}

	//config_write(this_mod);																			// write configuration
	change_resolution(this_mod);
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ---------------------------------------------------------keysfunction-----------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:		return 0;
		case WM_ERASEBKGND: return 0;
		case WM_PAINT:
			{ 
				PAINTSTRUCT ps;
				RECT r;
				HDC hdc = BeginPaint(hwnd,&ps);
				GetClientRect(hwnd,&r);
				BitBlt(hdc,0,0,r.right,r.bottom,memDC,0,0,SRCCOPY);
				EndPaint(hwnd,&ps);
			}
		return 0;
		case WM_CLOSE:																				// Did We Receive A Close Message?
		{
			PostQuitMessage(0);																		// Send A Quit Message
			return 0;																				// Jump Back
		}
		case WM_DESTROY: PostQuitMessage(0); return 0;
		{																							// get this_mod from our window's user data
			winampVisModule *this_mod = (winampVisModule *) GetWindowLong(hwnd,GWL_USERDATA);
			PostMessage(this_mod->hwndParent,message,wParam,lParam);
		}
		return 0;
		case WM_MOVE:
			{	
				RECT r;
				GetWindowRect(hMainWnd,&r);
				config_x = 1024;
				config_y = 768;
			}
		return 0;
		case WM_SIZE:																				// Resize The OpenGL Window
		{
			width=LOWORD(lParam);
			height=HIWORD(lParam);
			ReSizeGLScene(width,height);															// LoWord=Width, HiWord=Height
			return 0;																				// Jump Back
		}
		case WM_SYSCOMMAND:																			// Intercept System Commands
		{
			switch (wParam)																			// Check System Calls
			{
				case SC_SCREENSAVE:																	// Screensaver Trying To Start?
				case SC_MONITORPOWER:																// Monitor Trying To Enter Powersave?
				return 0;																			// Prevent From Happening
			}
			break;																					// Exit
		}
		case WM_KEYDOWN:																			// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;																	// If So, Mark It As TRUE
			keyboard();
			return 0;																				// Jump Back
		}
		case WM_KEYUP:																				// Has A Key Been Released?
		{
			keys[wParam] = FALSE;																	// If So, Mark It As FALSE
			return 0;																				// Jump Back
		}
		case WM_MOUSEMOVE:
		{
			return 0;
		}
		case WM_LBUTTONUP :
		{
			return 0;
		}
		case WM_RBUTTONDOWN :
		{
			return 0;	
		}
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------------winamp plugin module--------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

void config_getinifn(struct winampVisModule *this_mod, char *ini_file)
{	
	char *p;
	GetModuleFileName(this_mod->hDllInstance,ini_file,MAX_PATH);
	p=ini_file+strlen(ini_file);
	while (p >= ini_file && *p != '\\') p--;
	if (++p >= ini_file) *p = 0;
	strcat(ini_file,"plugin.ini");
	delete p;
}

void config_read(struct winampVisModule *this_mod)
{
	char ini_file[MAX_PATH];
	config_getinifn(this_mod,ini_file);
	width = GetPrivateProfileInt(this_mod->description,"width",width,ini_file);
	height = GetPrivateProfileInt(this_mod->description,"height",height,ini_file);
	if ((width!=640) && (width!=800) && (width!=1024) && (width!=1280) && (width!=1600))
	{
		width=640;
		height=480;
	}
}

void config_write(struct winampVisModule *this_mod)
{
	char string[32];
	char ini_file[MAX_PATH];

	config_getinifn(this_mod,ini_file);

	wsprintf(string,"%d",width);
	WritePrivateProfileString(this_mod->description,"width",string,ini_file);
	wsprintf(string,"%d",height);
	WritePrivateProfileString(this_mod->description,"height",string,ini_file);
}
/* --------------------------------------------------------------------------------------------------------------------*/
/* ----------------------------------------------winmain function------------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

int WINAPI WinMain(	HINSTANCE	hInstance,															// Instance
					HINSTANCE	hPrevInstance,														// Previous Instance
					LPSTR		lpCmdLine,															// Command Line Parameters
					int			nCmdShow)															// Window Show State
{
	MSG		msg;																					// Windows Message Structure
	config(&mod);
	init(&mod);
	while(!done)																					// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))													// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)																// Have We Received A Quit Message?
			{
				done=TRUE;																			// If So done=TRUE
				if (done==TRUE) quit(&mod);
			}
			else																					// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);																// Translate The Message
				DispatchMessage(&msg);																// Dispatch The Message
			}
		}
		else																						// If There Are No Messages
		{	
			int ret=render(&mod);
			if (ret==1)
				quit(&mod);
		}
	}
	return 1;																			// Exit The Program
}



void setLighting (GLfloat r,
				  GLfloat g,
				  GLfloat b)
{
	GLfloat light_ambient[] = {0.0,0.0,0.0,1.0};
	GLfloat light_diffuse[] = {r,g,b,1.0};
	GLfloat light_specular[] = {1.0,1.0,1.0,1.0};
	GLfloat lmodel_ambient[] = {0.5,0.5,0.5,1.0};
	GLfloat mat_amb_diff[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 40.0 };

	GLfloat light_position0[] = {1.0, 1.0, 0.0, 0.0 }; 

	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

/* --------------------------------------------------------------------------------------------------------------------*/
/* --------------------------------------------- Rendering Functions ------------------------------------------------------*/
/* --------------------------------------------------------------------------------------------------------------------*/

/*Draw the circles which reflects the beat*/
void drawWaveformCircles(struct winampVisModule *this_mod, int numSamples)
{
	double istep = groundRad/numSamples;
	double jstep = 0.05;
	double beatFactor = (((double)winampDetectBass(this_mod, 0,100)/100.0)*0.2)+0.8;
	double color = (double)winampDetectBass(this_mod, 0, 100)/100.0 + .01;	
	for (double i = groundRad; i > 0; i -= istep)
	{
		
		
		glColor3d(color,1,color);	
		glLineWidth(2.0);
		glBegin(GL_LINE_LOOP);
		double steps=1;
		for(double x = 1; x > 0; x -= .02) 
		{
			
			glVertex3d(beatFactor*i*my_cos(x), 
				       0.0, 
					   beatFactor*i*my_sin(x) );
		}
		glEnd();
	}
}

void displayScene (struct winampVisModule *this_mod)
{
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

	GLfloat lcolor = (GLfloat)winampDetectBass(this_mod,0,100)/100.0;

	rotateFactor += rotateRate;
	if (rotateFactor >= 1.0) rotateFactor = 0.0;
	
	/*Draw circle surface*/
	if(SURFACE) {
		glColor3d(0.86,0.86,0.86);
		
		glBegin(GL_POLYGON);
		glVertex3d(0.0,0.0,0.0);
		for(double x = 1.02; x >=0; x -= .02) 
		{
			glVertex3d(groundRad*my_cos(x), 
				       0.0, 
					   groundRad*my_sin(x) );
		}
		glEnd();
	}
	if (WAVEFORMCIRCLES)
	{
		drawWaveformCircles(this_mod, NUM_CIRCLES);
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
		//Sound input for the balls goes here for sphere with size sphere[i].r
		//Input ranges from 0-MAXSTRENGTH, so now from 0-100		
		
		//the input is only computed if the ball is not in the air
		if (!sphere[i].inAir) 
			input=computeSoundInput(i, NUMSPHERES, this_mod, BT_BEAT);
	
		//this function bumps the ball up and is called each time, it actually just bumps the ball up,
		//when it is on the ground and so just disregard the input if not.
		bump(&sphere[i], input);
		
//-------->>>>>

		//Draw the sphere
		if (SPHERES)
		{
			glColor3fv(sphere[i].color);
			glTranslatef(sphere[i].x,sphere[i].r+sphere[i].height,sphere[i].z);
			if(SQUASH) {
                double squash = (sphere[i].height/100.0)*0.6 + 0.5;
				glScaled(1.0, squash,1.0);
			}
			glutSolidSphere(sphere[i].r,
			           20.0+(int)((1-NUMSPHERES/MAXSPHERES)*(sphere[i].r/10)),
					   50.0+(int)((1-NUMSPHERES/MAXSPHERES)*(sphere[i].r))); 
		}

		glPopMatrix(); //restore the origin

		
	}
	
/*Text output*/
	setOrthographicProjection(); //switch to orthographic projection to easier print out text
	glPushMatrix();
	glLoadIdentity();

	//Data ouput of current settings
	if(OUTPUT) {
		char temp[50];
		glColor4f(1.0,1.0,1.0, 1.0);
		
		//Number of Balls
		sprintf(temp,"       # Balls: %.d", NUMSPHERES);
		printOut(10,10,temp);
		//Balls on/off?
		sprintf(temp,"         Balls: %s", (SPHERES)?"on":"off");
		printOut(10,25,temp);
		//Airtime
		sprintf(temp,"  max. Airtime: %.1f", NUMSEC);
		printOut(10,40,temp);
		//Speedfactor
		sprintf(temp,"   Speedfactor: %.1f", SPEEDFACTOR);
		printOut(10,55,temp);
		//Cones on/off?
		sprintf(temp,"         Cones: %s", (CONES)?"on":"off");
		printOut(10,70,temp);
		//Surface on/off?
		sprintf(temp,"       Surface: %s", (SURFACE)?"on":"off");
		printOut(10,85,temp);
		//Bouncing balls?
		sprintf(temp,"Bouncing Balls: %s", (BOUNCE)?"on":"off");
		printOut(10,100,temp);
		//Squash balls?
		sprintf(temp,"  Squash Balls: %s", (SQUASH)?"on":"off");
		printOut(10,115,temp);
		//Beat Circles?
		sprintf(temp,"  Beat Circles: %s", (WAVEFORMCIRCLES)?"on":"off");
		printOut(10, 130, temp);
		//Color change?
		sprintf(temp,"  Change color: %s", (COLORCHANGE)?"on":"off");
		printOut(10,145,temp);
		//Index color of balls
		printOut(10,160,"   Start color: ");
		char * color;
		if(COLOR==0) color="white";
		if(COLOR==1) {color="red"; glColor3f(1.0,0.0,0.0);}
		if(COLOR==2) {color="green"; glColor3f(0.0,1.0,0.0);}
		if(COLOR==3) {color="blue";  glColor3f(0.0,0.0,1.0);}
		if(COLOR==4) {color="yellow";  glColor3f(1.0,1.0,0.0);}
		if(COLOR==5) {color="magenta";  glColor3f(1.0,0.0,1.0);}
		if(COLOR==6) {color="cyan";  glColor3f(0.0,1.0,1.0);}
		sprintf(temp,"%s", color);
		printOut(138,160,temp);
		glColor3f(1.0, 1.0, 1.0);
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

		printOut(x,y+55,"z/x = decrease/increase number of balls");
		printOut(x,y+70,"u/i = decrease/increase the maximum airtime of the balls");
		printOut(x,y+85,"j/k = decrease/increase the speed factor of the balls");
        printOut(x,y+100,"n/m = decrease/increase the start color of the balls");
		printOut(x,y+130,"  c = display cones?");
		printOut(x,y+145,"  s = display white circle surface?");
		printOut(x,y+160,"  w = display beat circles?");
		printOut(x,y+175,"  a = display balls?");
		printOut(x,y+190,"  b = allow balls to bounce on the ground?");
		printOut(x,y+205,"  t = allow balls to be squashed?");
		printOut(x,y+220,"  l = allow balls to change color?");

		printOut(x,y+250,"  r = reset all settings to default value");

		printOut(x,y+280,"by Ryan Messner and Simon Beisel");
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
//compute the sound input for each single sphere depending on their size
double computeSoundInput(int i, int total, struct winampVisModule *this_mod, int BounceType)
{
	int range = (int) (576/NUMSPHERES);
	int place=(int)(((NUMSPHERES-(i+1))/NUMSPHERES)*576);
	place=(place<0)?0:place;
	double tempstrength=0;
	double level=0;
	double strength;

	switch (BounceType)
	{
	case BT_WAVEFORM:
		for(int a=0; a<range; a++) {
			level=((this_mod->spectrumData[0][place+a] + this_mod->spectrumData[1][place+a]) /2);
			tempstrength+=level;
		}

		strength=((tempstrength/range)/255)*MAXSTRENGTH;
		strength=(strength+(i/(NUMSPHERES*2)))*1.8;
		
		strength=(strength>MAXSTRENGTH)?MAXSTRENGTH:strength;
		return strength;
	case BT_BEAT:
		return ((double)winampBeat(this_mod)/100)*MAXSTRENGTH;
	case BT_BASS:
	default:
		return (rand()%100)+1;
	}
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
	if(!sp->inAir&&strength>MAXSTRENGTH/10) {
		startTimer(sp); //start the timer
		//this is the main calculation, SEC is always higher for smaller balls than for
		//bigger balls with the same strength input, that is what the second term of the
		//function is for, the last term is the max air time and is calculated in
		sp->SEC=strength/MAXSTRENGTH*(1-(sp->r/(NUMSPHERES*2)))*NUMSEC;
		
		//change color of the ball slightly
		if(COLORCHANGE) {
			int choice=(int) (rand() % 3);
			int plusminus=(int) (rand() % 2);
			float change=(strength/MAXSTRENGTH)/10+0.02;
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

void keyboard ()
{
	if(keys['R']) { //reset to default settings
		CONES = DEFAULTCONES;
		SURFACE = DEFAULTSURFACE;
		NUMSPHERES = DEFAULTSPHERES;
		SPEEDFACTOR = DEFAULTSPEEDFACTOR;
		NUMSEC = DEFAULTSEC;
		COLORCHANGE = DEFAULTCOLORCHANGE;
		COLOR = DEFAULTCOLOR;
		WAVEFORMCIRCLES = DEFAULTWAVEFORMCIRCLES;
		SPHERES = SPHERESONOFF;
		SQUASH = DEFAULTSQUASH;

		for(int i=0; i<MAXSPHERES; i++) {
			changeColor(&sphere[i], COLOR);
		}

		initializeSpheres();
	}
	if(keys['X']&&NUMSPHERES<MAXSPHERES) { //increase number of spheres
		NUMSPHERES++;
		initializeSpheres();
	}
	if((keys['Z']||keys['Y'])&&NUMSPHERES>MINSPHERES) { //decrease number of spheres
		NUMSPHERES--;
		changeColor(&sphere[NUMSPHERES], COLOR); //RESET COLOR
		initializeSpheres();
	}
	if(keys['I']&&NUMSEC<MAXSEC) { //increase Airtime 
		NUMSEC+=0.1;
	}
	if(keys['U']&&NUMSEC>MINSEC) { //decrease Airtime 
		NUMSEC-=0.1;
	}
	if(keys['K']&&SPEEDFACTOR<MAXSPEEDFACTOR) { //increase Speedfactor
		SPEEDFACTOR+=0.1;
	}
	if(keys['J']&&SPEEDFACTOR>MINSPEEDFACTOR) { //decrease Speedfactor
		SPEEDFACTOR-=0.1;
	}
	if(keys['S']) { //switch surface on/off
		SURFACE=(SURFACE==true)?false:true;
	}
	if(keys['C']) { //switch cones on/off
		CONES=(CONES==true)?false:true;
	}
	if(keys['B']) { //switch bouncing balls on/off
		BOUNCE=(BOUNCE==true)?false:true;
	}
	if(keys['O']) { //switch output of data on/off
		OUTPUT=(OUTPUT==true)?false:true;
	}
	
	if(keys['H']) { //switch help on/off
		if(HELP==false) {
			surfacetemp=SURFACE;
			SURFACE=false;
			circlestemp=WAVEFORMCIRCLES;
			WAVEFORMCIRCLES=false;
		}
		if(HELP==true) {
			SURFACE=(SURFACE==true)?true:surfacetemp;
			WAVEFORMCIRCLES=(WAVEFORMCIRCLES==true)?true:circlestemp;
		}
		HELP=(HELP==true)?false:true;
	}
	if(keys['L']) { //switch colorchange on/off
		COLORCHANGE=(COLORCHANGE==true)?false:true;
		for(int i=0; i<NUMSPHERES; i++) {
			changeColor(&sphere[i], COLOR);
		}
	}
	if(keys['M']&&COLOR<MAXCOLOR) { //switch through colors
		COLOR++;
		for(int i=0; i<NUMSPHERES; i++) {
			changeColor(&sphere[i], COLOR);
		}
	}
	if(keys['N']&&COLOR>MINCOLOR) { //switch through colors
		COLOR--;
		for(int i=0; i<NUMSPHERES; i++) {
			changeColor(&sphere[i], COLOR);
		}
	}
	if(keys['W']) { //switch waveformcircles on/off
		WAVEFORMCIRCLES = (WAVEFORMCIRCLES == true)?false:true;
	}
	if(keys['A']) { //switch balls on/off
		SPHERES = (SPHERES == true)?false:true;
	}
	if(keys['T']) { //switch balls on/off
		SQUASH = (SQUASH == true)?false:true;
	}
}
