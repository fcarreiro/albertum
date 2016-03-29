#ifndef __CHARACTER_H__
#define __CHARACTER_H__

/************************************************/

#define STATE_DOWN		0
#define STATE_UP		1
#define STATE_LEFT		2
#define STATE_RIGHT		3

/************************************************/

enum {					// OBJECTS
	O_GOLD,
	MAX_OBJECTS
};

enum {					// RACES
	R_HUMAN,
	R_ELF
};

enum {					// GENDERS
	G_MALE,
	G_FEMALE
};

/************************************************/

class CCharacter {
public:
	CCharacter();
	~CCharacter();

	// FUNCTIONS
	bool Create(const char* a_name, unsigned char a_race, unsigned char a_gender);
	void Kill(void);
	void Draw(int sx,int sy);
	void Move(class CMap* a_map, unsigned char direction);

	int GetX(void) {return x;}
	int GetY(void) {return y;}
	unsigned long GetWidth() {return width;}
	unsigned long GetHeight() {return height;}
	void SetX(unsigned long a_x) {x=a_x;}
	void SetY(unsigned long a_y) {y=a_y;}

	void SetSteps(unsigned char a_steps) {steps=a_steps;}
	void SetSize(unsigned long a_width, unsigned long a_height) {width=a_width;height=a_height;}
	void SetWaitTime(unsigned char a_wait) {wait=a_wait;}

private:
	// VARIABLE	
	IDirectDrawSurface7* gfx;
	std::string strSpeech;
	std::string strName;
	unsigned long objects[MAX_OBJECTS];
	unsigned char wait;
	unsigned char steps;
	unsigned long width;
	unsigned long height;
	unsigned char state;
	unsigned long x;
	unsigned long y;
};

/************************************************/

#endif
