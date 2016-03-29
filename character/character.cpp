#include "stdafx.h"

/***************************************************/

CCharacter::CCharacter()
{
	gfx=NULL;
	memset(objects,0,sizeof(objects));
	wait=0;
	steps=0;
	width=0;
	height=0;
	state=0;
	x=0;
	y=0;
}

CCharacter::~CCharacter()
{
	Kill();
}

/***************************************************/

bool CCharacter::Create(const char* a_name, unsigned char a_race, unsigned char a_gender)
{
	std::string strTemp;

	strName=a_name;

	strTemp="chars\\";

	switch(a_race)
	{
	case R_HUMAN:
		strTemp+="humano";
		break;

	case R_ELF:
		strTemp+="elfo";

	default:
		return false;
	}

	switch(a_gender)
	{
	case G_MALE:
		strTemp+="h";
		break;

	case G_FEMALE:
		strTemp+="m";

	default:
		return false;
	}

	strTemp+=".bmp";

	gfx=DDLoadBitmap(dd,strTemp.c_str(),0,0);

	if(gfx==NULL) return false;

	DDSetColorKey(gfx, 0);

	// initialize atributtes
	state=0;
	wait=1;
	steps=1;
	x=0;
	y=0;

	return true;
}

/***************************************************/

void CCharacter::Kill()
{
	if(gfx!=NULL) gfx->Release();

	gfx=NULL;
	memset(objects,0,sizeof(objects));
	wait=0;
	steps=0;
	width=0;
	height=0;
	state=0;
	x=0;
	y=0;
}

/***************************************************/

void CCharacter::Draw(int sx,int sy)
{
	if(gfx==NULL) return;

	DXDrawTile(state,gfx,sx,sy-height,width,height,true);
}

/***************************************************/

void CCharacter::Move(CMap* a_map, unsigned char direction)
{
	int i;

	if(!a_map) return;
	if(x<32 && direction==STATE_LEFT) return;
	if(y<32 && direction==STATE_UP) return;
	if(x>=(a_map->GetWidth()*32)-32 && direction==STATE_RIGHT) return;
	if(y>=(a_map->GetHeight()*32)-32 && direction==STATE_DOWN) return;

	if(direction==STATE_RIGHT)
	{
		if(a_map->GetTile(1,(x/32)+1,y/32)==-1)
		{
			for(i=0;i<32;++i)
			{
				++x;
				state=(i/steps)+(STATE_RIGHT*steps);
				_sleep(wait);
			}
		}
		state=(STATE_RIGHT*steps);
	}
	else if(direction==STATE_LEFT)
	{
		if(a_map->GetTile(1,(x/32)-1,y/32)==-1)
		{
			for(i=0;i<32;++i)
			{
				--x;
				state=(i/steps)+(STATE_LEFT*steps);
				_sleep(wait);
			}
		}
		state=(STATE_LEFT*steps);
	}
	else if(direction==STATE_UP)
	{
		if(a_map->GetTile(1,x/32,(y/32)-1)==-1)
		{
			for(i=0;i<32;++i)
			{
				--y;
				state=(i/steps)+(STATE_UP*steps);
				_sleep(wait);
			}
		}
		state=(STATE_UP*steps);
	}
	else if(direction==STATE_DOWN)
	{
		if(a_map->GetTile(1,x/32,(y/32)+1)==-1)
		{
			for(i=0;i<32;++i)
			{
				++y;
				state=(i/steps)+(STATE_DOWN*steps);
				_sleep(wait);
			}
		}
		state=(STATE_DOWN*steps);
	}

	/**** has to teleport? ****/
	TILEPROPS tp=a_map->GetProps(x/32,y/32);

	if(tp.flags & FLAG_TELEPORT)
	{
		int lastx=x;
		int lasty=y;
		char destmap[80];

		memset(destmap,0,sizeof(destmap));
		strncpy(destmap,
				tp.string,
				tp.stringlen);
		x=LOWORD(tp.lparam)*32;
		y=HIWORD(tp.lparam)*32;
		state=tp.wparam;

		a_map->Free();
		a_map->Load(destmap);
	}

	_sleep(wait);
}

/***************************************************/