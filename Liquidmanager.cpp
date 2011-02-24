#include "HelperFuncs.h"
#include "TextGeneration.h"
#include "MeshGeneration.h"
#include "Liquidmanager.h"
#include "LiquidTime.h"
#include "3ds.h"

#define FILE_NAME  "Liquid\\face.3ds"


liqTime			*g_pFrameTime;	
TextGeneration	*Generated_textures;
MeshGeneration	*Generated_Meshes;
CLoad3DS		g_Load3ds;
t3DModel		liquidmodel;
								
Liquidmanager::Liquidmanager(int wid,int hei,struct winampVisModule *this_mod)
{
	Generated_textures	= new TextGeneration();
	Generated_Meshes	= new MeshGeneration();	

	char *ini_file=new char[70];
	char *help=new char[70];
	char *p;
	GetModuleFileName(this_mod->hDllInstance,ini_file,MAX_PATH);
	p=ini_file+strlen(ini_file);
	while (p >= ini_file && *p != '\\') p--;
	if (++p >= ini_file) 
		*p = 0;
	strcpy(help,ini_file);
	strcat(help,FILE_NAME);

	g_Load3ds.Import3DS(&liquidmodel, help);
	
	width				= wid;
	height				= hei;

	textures			= new loadall(this_mod);
	
	liquid1				= new scene1(40.0f,(float)width,(float)height);
	/*liquid2				= new scene2(40.0f,4,(float)width,(float)height,0);
	liquid3				= new scene3(40.0f,Generated_textures,(float)width,(float)height);
	liquid6				= new scene6(40.0f,(float)width,(float)height);
	liquid7				= new scene7(40.0f,(float)width,(float)height);
	liquid8				= new scene8(40.0f,(float)width,(float)height);
	liquid9				= new scene9(40.0f,(float)width,(float)height);
	liquid10			= new scene10(40.0f,(float)width,(float)height);
	liquid11			= new scene11(40.0f,(float)width,(float)height);
	liquid12			= new scene12(40.0f,(float)width,(float)height);
	liquid13			= new scene13(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid14			= new scene14(40.0f,(float)width,(float)height);
	liquid15			= new scene15(40.0f,(float)width,(float)height);
	liquid16			= new scene16(40.0f,0,(float)width,(float)height);
	liquid17			= new scene17(40.0f,(float)width,(float)height);
	liquid18			= new scene18(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid19			= new scene19(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid20			= new scene20(40.0f,(float)width,(float)height);
	liquid21			= new scene21(40.0f,(float)width,(float)height);
	liquid22			= new scene22(40.0f,(float)width,(float)height);
	liquid23			= new scene23(40.0f,(float)width,(float)height);
	liquid24			= new scene24(40.0f,(float)width,(float)height);
	liquid25			= new scene25(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid26			= new scene26(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid27			= new scene27(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid28			= new scene28(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid29			= new scene29(40.0f,Generated_textures,Generated_Meshes,(float)width,(float)height);
	liquid30			= new scene30(40.0f,(float)width,(float)height);
	liquid31			= new scene31(40.0f,Generated_textures,Generated_Meshes,&liquidmodel,(float)width,(float)height);
	//liquid34			= new scene34(40.0f,(float)width,(float)height);*/
	g_pFrameTime		= new liqTime();
	liquid1->Init(textures);

	lockscene		= FALSE;
}

void Liquidmanager::Init()
{
	initScenes();
}

void Liquidmanager::initScenes()
{	
	liquid1->add_Next_Prev(liquid1,liquid1);
	/*liquid2->add_Next_Prev(liquid3,liquid1);
	liquid3->add_Next_Prev(liquid6,liquid2);
	liquid6->add_Next_Prev(liquid7,liquid3);
	liquid7->add_Next_Prev(liquid8,liquid6);
	liquid8->add_Next_Prev(liquid9,liquid7);
	liquid9->add_Next_Prev(liquid10,liquid8);
	liquid10->add_Next_Prev(liquid11,liquid9);
	liquid11->add_Next_Prev(liquid12,liquid10);
	liquid12->add_Next_Prev(liquid13,liquid11);
	liquid13->add_Next_Prev(liquid14,liquid12);
	liquid14->add_Next_Prev(liquid15,liquid13);
	liquid15->add_Next_Prev(liquid16,liquid14);
	liquid16->add_Next_Prev(liquid17,liquid15);
	liquid17->add_Next_Prev(liquid18,liquid16);
	liquid18->add_Next_Prev(liquid19,liquid17);
	liquid19->add_Next_Prev(liquid20,liquid18);
	liquid20->add_Next_Prev(liquid21,liquid19);
	liquid21->add_Next_Prev(liquid22,liquid20);
	liquid22->add_Next_Prev(liquid23,liquid21);
	liquid23->add_Next_Prev(liquid24,liquid22);
	liquid24->add_Next_Prev(liquid25,liquid23);
	liquid25->add_Next_Prev(liquid26,liquid24);
	liquid26->add_Next_Prev(liquid27,liquid25);
	liquid27->add_Next_Prev(liquid28,liquid26);
	liquid28->add_Next_Prev(liquid29,liquid27);
	liquid29->add_Next_Prev(liquid30,liquid28);
	liquid30->add_Next_Prev(liquid31,liquid29);
	liquid31->add_Next_Prev(liquid1,liquid30);
	//liquid34->add_Next_Prev(liquid1,liquid31);*/
	
	m_pCurrentScene = liquid1;
	m_pCurrentScene->start();
}

void Liquidmanager::Update(float beat_help,struct winampVisModule *this_mod,float beat_scaler,bool Tex_on)
{
	g_pFrameTime->update();
	if(m_pCurrentScene != NULL)
	{
		if(m_pCurrentScene->isComplete())
		{
			nextScene();
		}

		if(m_pCurrentScene != NULL)
			m_pCurrentScene->Update(beat_help,this_mod,beat_scaler,Tex_on);
	}
}

void Liquidmanager::nextScene()
{
	if (lockscene==FALSE)
	{
		m_pCurrentScene = m_pCurrentScene->getNext();
	}
	else
	{
		m_pCurrentScene = m_pCurrentScene;
	}

	if(m_pCurrentScene == NULL)
		setExit();
	else
	{
		m_pCurrentScene->Init(textures);
		m_pCurrentScene->start();		
	}
}

void Liquidmanager::prevScene()
{
	if (lockscene==FALSE)
	{
		m_pCurrentScene = m_pCurrentScene->getPrev();
	}
	else
	{
		m_pCurrentScene = m_pCurrentScene;
	}

	if(m_pCurrentScene == NULL)
		setExit();
	else
	{
		m_pCurrentScene->Init(textures);
		m_pCurrentScene->start();		
	}
}

void Liquidmanager::holdScene()
{	
	lockscene		= !lockscene;
}

void Liquidmanager::holdBeat(float beat_help)
{
}


void Liquidmanager::Draw(GLuint blend_colour,struct winampVisModule *this_mod)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(m_pCurrentScene != NULL)
		m_pCurrentScene->Draw(blend_colour,this_mod);
}

Liquidmanager::~Liquidmanager()
{
	if(liquid1)
		delete liquid1;
	//if(liquid2)
	//	delete liquid2;
	//if(liquid3)
	//	delete liquid3;
}
