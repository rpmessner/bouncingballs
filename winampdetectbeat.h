int winampDetectBeat(struct winampVisModule *this_mod)
{
	int y=0;																// y=0 | 1 right | left channel;
	int last=this_mod->waveformData[y][0];
	int total=0;
	for (int x = 1; x < 576; x ++)
	{
		total += abs(last - this_mod->waveformData[y][x]);
		last = this_mod->waveformData[y][x];
	}
	total /= 576;
	if (total > 127) total = 127;
	return total;
}

int winampDetectBass(struct winampVisModule *this_mod,int xmin,int xmax)
{																	
	int last=this_mod->waveformData[0][0];
	int total=0;
	for (int y=0;y<2;y++)
	{
		for (int x = xmin; x < xmax; x ++)
		{
			total += abs(this_mod->spectrumData[y][x]);
			last = this_mod->waveformData[y][x];
		}
		total /= (xmax-xmin);
	}
	if (total > 100) total = 100;
	return total;
}

int winampDetectMiddle(struct winampVisModule *this_mod)
{	
	int y=0;																// y=0 | 1 right | left channel;
	int last=this_mod->waveformData[y][0];
	int total=0;
	for (int x = 550; x < 555; x ++)
	{
		total += abs(this_mod->waveformData[y][x]);
		last = this_mod->waveformData[y][x];
	}
	total /= 5;
	if (total > 127) total = 127;
	return total;
}

int winampMiddleUp(struct winampVisModule *this_mod,int i,int j)
{	
	int y=0;																// y=0 | 1 right | left channel;
	int last=this_mod->waveformData[y][i];
	int total=0;
	for (int x = 0; x < j; x ++)
	{
		total += abs(this_mod->spectrumData[y][x+i]);
		last = this_mod->waveformData[y][x+i];
	}
	total /= j;
	if (total > 127) total = 127;
	return total;
}

int winampDetectTreble(struct winampVisModule *this_mod)
{	
	int y=0;																// y=0 | 1 right | left channel;
	int last=this_mod->waveformData[y][0];
	int total=0;
	for (int x = 250; x < 560; x ++)
	{
		total += abs(this_mod->spectrumData[y][x]);
		last = this_mod->waveformData[y][x];
	}
	total /= 310;
	if (total > 127) total = 127;
	return total;
}

int winampBeat(struct winampVisModule *this_mod)
{
	int total;
	for (int y = 0; y < 2; y ++)
	{
		int last=this_mod->waveformData[y][0];
		total=0;
		
		//get average waveformData level
		for (int x = 1; x < 576; x ++)
		{
			total+=abs(last-this_mod->waveformData[y][x]);
			last = this_mod->waveformData[y][x];
		}
		total /= 576;
		
		if (total > 100) total = 100;		
	}
	return total;
}