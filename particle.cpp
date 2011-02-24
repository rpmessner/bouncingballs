#include "headers.h"
#include "Misc.h"
#include "particle.h"

particle::particle(float xg,float yg,float zg)					
{
	GLfloat colors[12][3]=																		
	{
		{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
		{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
		{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
	};

	const int MAX_PARTICLES=100;
	for (int loop=0;loop<MAX_PARTICLES;loop++)												
	{
		particle_in[loop].active=true;														
		particle_in[loop].life=1.0f;														
		particle_in[loop].fade=float(rand()%100)/1000.0f+0.003f;							
		particle_in[loop].r=colors[loop*(12/MAX_PARTICLES)][0];							
		particle_in[loop].g=colors[loop*(12/MAX_PARTICLES)][1];							
		particle_in[loop].b=colors[loop*(12/MAX_PARTICLES)][2];							
		particle_in[loop].xi=float((rand()%50)-26.0f)*10.0f;								
		particle_in[loop].yi=float((rand()%50)-25.0f)*10.0f;								
		particle_in[loop].zi=float((rand()%50)-25.0f)*10.0f;								
		particle_in[loop].xg=xg;															
		particle_in[loop].yg=yg;															
		particle_in[loop].zg=zg;
		particle_in[loop].x=0.0f;
		particle_in[loop].y=0.0f;
		particle_in[loop].z=0.0f;
	}
	slowdown=0.0f;

	glClientActiveTextureARB	= NULL;
	glActiveTextureARB			= NULL;

	glActiveTextureARB = ( PFNGLCLIENTACTIVETEXTUREARBPROC )wglGetProcAddress  ( "glActiveTextureARB" );	
	glClientActiveTextureARB = ( PFNGLACTIVETEXTUREARBPROC )wglGetProcAddress  ( "glClientActiveTextureARB" );
}

particle::~particle()					
{
}

void particle::Restore(float xg,float yg,float zg)
{
	GLfloat colors[12][3]=																		
	{
		{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
		{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
		{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
	};

	const int MAX_PARTICLES=20;
	for (int loop=0;loop<MAX_PARTICLES;loop++)												
	{
		particle_in[loop].active=true;														
		particle_in[loop].life=5.0f;														
		particle_in[loop].fade=float(rand()%100)/1000.0f+0.003f;							
		particle_in[loop].r=colors[loop*(12/MAX_PARTICLES)][0];							
		particle_in[loop].g=colors[loop*(12/MAX_PARTICLES)][1];							
		particle_in[loop].b=colors[loop*(12/MAX_PARTICLES)][2];							
		particle_in[loop].xi=float((rand()%50)-26.0f)*10.0f;								
		particle_in[loop].yi=float((rand()%50)-25.0f)*10.0f;								
		particle_in[loop].zi=float((rand()%50)-25.0f)*10.0f;								
		particle_in[loop].xg=xg;															
		particle_in[loop].yg=yg;															
		particle_in[loop].zg=zg;
		particle_in[loop].x=0.0f;
		particle_in[loop].y=0.0f;
		particle_in[loop].z=0.0f;
	}
	slowdown=0.0f;
}

void particle::Draw(GLuint blend_colour,float beat_responder,struct winampVisModule *this_mod,float xrot,float xspeed,float yspeed,int col)
{
	GLfloat colors[12][3]=																		
	{
		{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
		{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
		{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
	};

	GLfloat	tex_coords[]=
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	glClientActiveTextureARB ( GL_TEXTURE0_ARB             ); 
	glTexCoordPointer        ( 2, GL_FLOAT, 0, &tex_coords ); 
   
	glClientActiveTextureARB ( GL_TEXTURE1_ARB             );  
	glTexCoordPointer        ( 2, GL_FLOAT, 0, &tex_coords );

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	if (beat_responder<30)
		slowdown+=(float)pow((beat_responder/32),2);
	else
		slowdown-=(float)pow((beat_responder/64),2);
	int	MAX_PARTICLES = 500;
	GLfloat	zoom	  = -12.0f;
	for (int loop=0;loop<MAX_PARTICLES;loop++)															
	{
		if (particle_in[loop].active)																		
		{
			float x=particle_in[loop].x;																	
			float y=particle_in[loop].y;																	
			float z=particle_in[loop].z+zoom;
			
			GLfloat colours[]=
			{
				particle_in[loop].r,particle_in[loop].g,particle_in[loop].b,particle_in[loop].life,
				particle_in[loop].r,particle_in[loop].g,particle_in[loop].b,particle_in[loop].life,
				particle_in[loop].r,particle_in[loop].g,particle_in[loop].b,particle_in[loop].life,
				particle_in[loop].r,particle_in[loop].g,particle_in[loop].b,particle_in[loop].life
			};
			
			GLfloat vertices[]=
			{
				x+0.5f,y+0.5f,z,
				x-0.5f,y+0.5f,z,
				x-0.5f,y-0.5f,z,
				x+0.5f,y-0.5f,z
			};

			GLuint		indices[]=
			{
				0,1,2,3
			};

			particle_in[loop].x+=particle_in[loop].xi/(slowdown*1000);										
			particle_in[loop].y+=particle_in[loop].yi/(slowdown*1000);										
			particle_in[loop].z+=particle_in[loop].zi/(slowdown*1000);										

			int waveform2=this_mod->waveformData[1][int(loop)]/127;
			//float waveform2=(32767.0f-pcm[int(loop)])*SPECHEIGHT/105536.0f;

			particle_in[loop].xi+=particle_in[loop].xg+waveform2/5;														
			particle_in[loop].yi+=particle_in[loop].yg+waveform2/100;														
			particle_in[loop].zi+=particle_in[loop].zg*waveform2/10;														
			particle_in[loop].life-=particle_in[loop].fade;													
			particle_in[loop].xg+=(float)cos(rand()%180)*waveform2/(10*60);												
			particle_in[loop].yg-=((float)cos(rand()%180)/10)*xrot/60;												
			particle_in[loop].zg-=(float)cos(rand()%180)*waveform2/127;												

			if (particle_in[loop].life<0.0f)																
			{
				particle_in[loop].life=1.0f;																
				particle_in[loop].fade=float(rand()%100)/1000.0f+0.003f;									
				particle_in[loop].x=0.0f;																	
				particle_in[loop].y=0.0f;																	
				particle_in[loop].z=0.0f;																	
				particle_in[loop].xi=xspeed+float((rand()%60)-32.0f);										
				particle_in[loop].yi=yspeed+float((rand()%60)-30.0f);										
				particle_in[loop].zi=float((rand()%60)-30.0f);												
				particle_in[loop].r=colors[col][0];														
				particle_in[loop].g=colors[col][1];														
				particle_in[loop].b=colors[col][2];														
			}
			glVertexPointer ( 3, GL_FLOAT, 0, &vertices );
			glColorPointer  ( 4, GL_FLOAT, 0, &colours );
			glDrawElements  ( GL_QUADS, 4 , GL_UNSIGNED_INT, &indices );
		}
    }
}



