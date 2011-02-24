#ifndef __PARTICLE_H_
#define __PARTICLE_H_
#include "vis.h"

class particle
{

	public:
		
		typedef struct																							// Create A Structure For Particle
		{
			bool	active;																						// Active (Yes/No)
			float	life;																						// Particle Life
			float	fade;																						// Fade Speed
			float	r;																							// Red Value
			float	g;																							// Green Value
			float	b;																							// Blue Value
			float	x;																							// X Position
			float	y;																							// Y Position
			float	z;																							// Z Position
			float	xi;																							// X Direction
			float	yi;																							// Y Direction
			float	zi;																							// Z Direction
			float	xg;																							// X Gravity
			float	yg;																							// Y Gravity
			float	zg;																							// Z Gravity
		}particles;

		particle(float xg,float yg,float zg);												
		~particle();												
		
		void Draw(GLuint blend_colour,float beat_responder,struct winampVisModule *this_mod,float xrot,float xspeed,float yspeed,int col);
		void Restore(float xg,float yg,float zg);
		
		particles	particle_in[500];

		float		slowdown;
		PFNGLCLIENTACTIVETEXTUREARBPROC  glClientActiveTextureARB;
		PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB;

};

#endif __PARTICLE_H_
