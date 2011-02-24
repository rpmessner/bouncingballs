#ifndef __LIQMANAGER_H
#define __LIQMANAGER_H

#include "headers.h"
#include "Misc.h"
#include "Scenemanager.h"
#include "LoadAlltextures.h"
#include "Texture.h"
#include "Liquid1.h"
/*#include "Liquid2.h"
#include "Liquid3.h"
#include "Liquid6.h"
#include "Liquid7.h"
#include "Liquid8.h"
#include "Liquid9.h"
#include "Liquid10.h"
#include "Liquid11.h"
#include "Liquid12.h"
#include "Liquid13.h"
#include "Liquid14.h"
#include "Liquid15.h"
#include "Liquid16.h"
#include "Liquid17.h"
#include "Liquid18.h"
#include "Liquid19.h"
#include "Liquid20.h"
#include "Liquid21.h"
#include "Liquid22.h"
#include "Liquid23.h"
#include "Liquid24.h"
#include "Liquid25.h"
#include "Liquid26.h"
#include "Liquid27.h"
#include "Liquid28.h"
#include "Liquid29.h"
#include "Liquid30.h"
#include "Liquid31.h"
#include "Liquid34.h"*/
#include "vis.h"

class Liquidmanager
{
	public:
		Liquidmanager(int wid,int hei,struct winampVisModule *this_mod);
		~Liquidmanager();

		void			Init();
		void			Update(float beat_help,struct winampVisModule *this_mod,float beat_scaler,bool Tex_on);
		void			Draw(GLuint blend_colour,struct winampVisModule *this_mod);
		void			holdBeat(float beat_help);
		loadall			*textures;
		void			nextScene();
		void			prevScene();
		void			holdScene();
		void			adveScene(bool status);

	protected:
		bool			lockscene;
		void			initScenes();
		Manager			*m_pCurrentScene;

		int				width,height;

		scene1			*liquid1;
		/*scene2			*liquid2;
		scene3			*liquid3;
		scene6			*liquid6;
		scene7			*liquid7;
		scene8			*liquid8;
		scene9			*liquid9;
		scene10			*liquid10;
		scene11			*liquid11;
		scene12			*liquid12;
		scene13			*liquid13;
		scene14			*liquid14;
		scene15			*liquid15;
		scene16			*liquid16;
		scene17			*liquid17;
		scene18			*liquid18;
		scene19			*liquid19;
		scene20			*liquid20;
		scene21			*liquid21;
		scene22			*liquid22;
		scene23			*liquid23;
		scene24			*liquid24;
		scene25			*liquid25;
		scene26			*liquid26;
		scene27			*liquid27;
		scene28			*liquid28;
		scene29			*liquid29;
		scene30			*liquid30;
		scene31			*liquid31;
		scene34			*liquid34;*/
		Manager			*liquid_next;
};

#endif __LIQMANAGER_H
