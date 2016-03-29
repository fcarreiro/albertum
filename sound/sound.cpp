#include "stdafx.h"

/**************************************************************/

CSoundSystem::CSoundSystem()
{
}

CSoundSystem::~CSoundSystem()
{
	Kill();
}

/**************************************************************/

bool CSoundSystem::Init(void)
{
	if(!BASS_Init(-1,44100,0,0)) return false;
	if(!BASS_Start()) return false;

	return true;
}

/**************************************************************/

bool CSoundSystem::PlayMusic(const char* a_file)
{
#ifndef DISABLE_SOUND
	BASS_SampleStop(music);
	BASS_SampleFree(music);
	music=BASS_SampleLoad(false,(void*)a_file,0,0,1,BASS_SAMPLE_LOOP);
	if(music)
		music_channel=BASS_SamplePlay(music);
	else
		return false;
#endif

	return true;
}

/**************************************************************/

void CSoundSystem::StopMusic(unsigned long a_interval)
{
	DWORD volume;
	DWORD sleep;

	if(!music_channel) return;

	if(a_interval>0)
	{
		BASS_ChannelGetAttributes(music_channel,NULL,&volume,NULL);
		if(volume<=0) volume=1;
		sleep=a_interval/volume;

		for(DWORD i=volume;i>0;i--)
		{
			BASS_ChannelSetAttributes(music_channel,-1,i,-1);
			Sleep(sleep);
		}
	}

	BASS_SampleStop(music);
}

/**************************************************************/

void CSoundSystem::Kill(void)
{
	BASS_SampleStop(music);
	BASS_SampleFree(music);

	BASS_Stop();
	BASS_Free();
}
