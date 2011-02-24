#include "headers.h"
#include "Misc.h"
#include "liquid1.h"

scene1::scene1(double time,float wid,float hei):Manager(time)					
{
	xrot						= 0.0f;
	t							= 0.0f;
	liquid_cube					= new cube();	
	direction					= -1;
	beat_responder				= 0.0f;
	bg							= new background();
	timeeffect					= 0;
	multi_texture				=0;
	speedTexure					= FALSE;

	glClientActiveTextureARB	= NULL;
	glActiveTextureARB			= NULL;

	glActiveTextureARB			= ( PFNGLCLIENTACTIVETEXTUREARBPROC )	wglGetProcAddress  ( "glActiveTextureARB" );	
	glClientActiveTextureARB	= ( PFNGLACTIVETEXTUREARBPROC )			wglGetProcAddress  ( "glClientActiveTextureARB" );
	t2							= 0.0f;
	sceneTime					= 0.0f;
	lastTime					= 0.0f;
	width						= wid;
	height						= hei;
	fade_def					= 0;

}

scene1::~scene1()					
{
	if (liquid_cube)
		delete liquid_cube;
}


void scene1::Draw(GLuint blend_colour,struct winampVisModule *this_mod)
{
	glDisable(GL_FOG);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_COLOR_MATERIAL);									
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);												
	glDisable(GL_LIGHT0);                             
	glDisable(GL_LIGHTING);
	glDisable ( GL_DEPTH_TEST );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);															
	
	glEnable(GL_BLEND);

	for (int i=0;i<7;i++)
	{
		glPushMatrix();
			glTranslatef(0.0f,0.0f,-20.0f+(10-i)*(float)pow(beat_responder/50,2));
			glPushMatrix();
				liquid_cube->Update(20*i+blend_colour,beat_responder/10,timeeffect);
				liquid_cube->Draw(1.0f-beat_responder/120.0f,0.1f,m_Texture,beat_responder*i/100.0f+0.1f,xrot);
			glPopMatrix();
		glPopMatrix();
	}
	
}

void scene1::Update(float beat_help,struct winampVisModule *this_mod,float beat_scaler,bool Tex_on)
{							
	glLoadIdentity ();	

	speedTexure = Tex_on;
	if (fade_def==0)
		fadeffect+=0.1f;
	if (fade_def==1)
		fadeffect-=0.1f;

	timeeffect=(GetTickCount()-SceneStart)/1000;
	beat_responder=beat_help*beat_scaler;
	xrot+=direction*((beat_responder/30.0f)*(beat_responder/30.0f))+1.0f;
	t+=(beat_responder/20.0f)*0.01f*(float)cos(timeeffect/300.0f*(beat_responder))*direction;
	if (t>1.5*beat_responder/16)	
		direction=-1;
	if (t<-1.5*beat_responder/16)
		direction=1;
	float currTime = GetTime();
	float deltaTime = currTime - lastTime;
	lastTime = currTime;	
	float speed = 1;	
	sceneTime += deltaTime * speed;
}

bool scene1::Init(loadall		*textures)
{
	textures_in=textures;
	liquid_cube->Init();
	glClientActiveTextureARB ( GL_TEXTURE0_ARB        );    
	glEnableClientState      ( GL_TEXTURE_COORD_ARRAY );
   
	glClientActiveTextureARB ( GL_TEXTURE1_ARB        );    
	glEnableClientState       ( GL_TEXTURE_COORD_ARRAY );
	SceneStart		= GetTickCount();
	fadeffect		= 0;

	multi_texture=rand()%19;
	
	switch (multi_texture)
	{
		case 0:
			m_Texture[0]=textures->Bind(22);
			m_Texture[1]=textures->Bind(1);
		break;
		case 1:
			m_Texture[0]=textures->Bind(1);
			m_Texture[1]=textures->Bind(0);
		break;
		case 2:
			m_Texture[0]=textures->Bind(0);
			m_Texture[1]=textures->Bind(2);
		break;
		case 3:
			m_Texture[0]=textures->Bind(1);
			m_Texture[1]=textures->Bind(4);
		break;
		case 4:
			m_Texture[0]=textures->Bind(3);
			m_Texture[1]=textures->Bind(21);
		break;
		case 5:
			m_Texture[0]=textures->Bind(21);
			m_Texture[1]=textures->Bind(3);
		break;
		case 6:
			m_Texture[0]=textures->Bind(10);
			m_Texture[1]=textures->Bind(7);
		break;
		case 7:
			m_Texture[0]=textures->Bind(11);
			m_Texture[1]=textures->Bind(3);
		break;
		case 8:
			m_Texture[0]=textures->Bind(13);
			m_Texture[1]=textures->Bind(21);
		break;
		case 9:
			m_Texture[0]=textures->Bind(12);
			m_Texture[1]=textures->Bind(4);
		break;
		case 10:
			m_Texture[0]=textures->Bind(14);
			m_Texture[1]=textures->Bind(7);
		break;
		case 11:
			m_Texture[0]=textures->Bind(15);
			m_Texture[1]=textures->Bind(3);
		break;
		case 12:
			m_Texture[0]=textures->Bind(16);
			m_Texture[1]=textures->Bind(21);
		break;
		case 13:
			m_Texture[0]=textures->Bind(17);
			m_Texture[1]=textures->Bind(4);
		break;
		case 14:
			m_Texture[0]=textures->Bind(18);
			m_Texture[1]=textures->Bind(2);
		break;
		case 15:
			m_Texture[0]=textures->Bind(19);
			m_Texture[1]=textures->Bind(4);
		break;
		case 16:
			m_Texture[0]=textures->Bind(20);
			m_Texture[1]=textures->Bind(17);
		break;
		case 17:
			m_Texture[0]=textures->Bind(21);
			m_Texture[1]=textures->Bind(15);
		break;
		case 18:
			m_Texture[0]=textures->Bind(1);
			m_Texture[1]=textures->Bind(22);
		break;
	}
	/*multi_texture=rand()%18;
	if (multi_texture==19)
		multi_texture=0;*/
	return true;
}

float scene1::GetTime(void)
{
	static bool init = false;
	static bool hires = false;
	static __int64 freq = 1;
	if(!init)
	{
		hires = !QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
		if(!hires)
			freq = 1000;
		init = true;
	}

	__int64 now;

	if(hires)
		QueryPerformanceCounter((LARGE_INTEGER *)&now);
	else
		now = GetTickCount();

	return (float)((double)now / (double)freq);

}

