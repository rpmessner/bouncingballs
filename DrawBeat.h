int DrawBeat(struct winampVisModule *this_mod, float beat_help,GLuint base,int choice,
				 GLuint texture[6],char * title,float title_transformation) 
{
	
	glLoadIdentity();
	glPushMatrix();
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		int  decimal, sign;
		char *beat_text;
		double source = double(beat_help);
		beat_text = _fcvt( source, 0, &decimal, &sign );
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
			glPushMatrix();
				glTranslatef(-40.0f,20.0f,0.0f);
				glColor4ub(255,255,255,200);
				glTranslatef(20.0f,0.0f,-60.0f);
				glPrint(beat_text,base);
			glPopMatrix();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	return TRUE;
}

