#ifndef __MAPENGINE_H__
#define __MAPENGINE_H__

/*******************************************************/

#define FLAG_STRING		0x01
#define FLAG_TELEPORT	0x02

/*******************************************************/

typedef struct {
	unsigned long flags;
	DWORD lparam;
	DWORD wparam;
	char* string;
	int stringlen;
} TILEPROPS;

/*******************************************************/

class CMap {
public:
	CMap();
	~CMap();

	// FUNCTIONS
	bool Load(const char* filename);
	void Render(IDirectDrawSurface7* surface);
	void Free(void);

	const char* GetTileFilename(void) {return tilefn;}
	const char* GetMusicFilename(void) {return bgmusic;}
	int GetLayerCount(void) {return nlayers;}
	int GetWidth(void) {return cx;}
	int GetHeight(void) {return cy;}
	IDirectDrawSurface7* GetSurface(void) {return tilessurface;}
	CCharacter* GetMainCharacter(void) {return mainchar;}
	void SetMainCharacter(CCharacter* c) {mainchar=c;}
	int GetTile(unsigned long layer, unsigned long x, unsigned long y)	{return layers[layer][((y%cy)*cx)+(x%cx)];}
	TILEPROPS GetProps(unsigned long x, unsigned long y) {return props[((y%cy)*cx)+(x%cx)];}

	// VARIABLES
private:
	IDirectDrawSurface7* tilessurface;
	CCharacter* mainchar;
	std::vector<CCharacter*> netchars;

	int** layers;
	TILEPROPS* props;

	int nlayers;
	int cx;
	int cy;

	int tilefnlen;
	char* tilefn;
	int bgmusiclen;
	char* bgmusic;
};

/*******************************************************/

#endif