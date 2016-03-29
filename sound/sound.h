#ifndef __SOUND_H__
#define __SOUND_H__

/***************************************************************/

class CSoundSystem {
public:
	CSoundSystem();
	~CSoundSystem();

	bool Init(void);
	void Kill(void);
	bool PlayMusic(const char* a_file);
	void StopMusic(unsigned long a_interval);

private:
	HSAMPLE music;
	HCHANNEL music_channel;
};

/***************************************************************/

#endif