#ifndef __LIQUID1_H_
#define __LIQUID1_H_

#include "TextureLoader.h"
#include "Scenemanager.h"
#include "LoadAlltextures.h"										
#include "cube.h"
#include "Background.h"
#include "vis.h"

class scene1:public Manager
{

	public:
		scene1(double time,float wid,float hei);												
		~scene1();												
		virtual void Draw	(GLuint blend_colour,struct winampVisModule *this_mod);
		virtual void Update	(float beat_help,struct winampVisModule *this_mod,float beat_scaler,bool Tex_on);
		virtual bool Init	(loadall		*textures);
	
	private:
		float		GetTime(void);

		GLuint		m_Texture[5];										
		cube		*liquid_cube;
		int			i,direction;
		int			scene_switcher;
		float		scalefactor;
		float		xrot,t,t2;
		float		beat_responder;
		int			multi_texture;
		DWORD		timeeffect,SceneStart;
		background	*bg;
		float		fadeffect;
		PFNGLCLIENTACTIVETEXTUREARBPROC  glClientActiveTextureARB;
		PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB;
		loadall		*textures_in;
		bool		speedTexure;
		float		lastTime,sceneTime;
		Manager		*liquid_next;
		float		width,height;
		int			fade_def;
};

#endif __LIQUID1_H_
