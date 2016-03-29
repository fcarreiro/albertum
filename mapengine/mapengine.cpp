#include "stdafx.h"

/**********************************************************************/

CMap::CMap()
{
	nlayers=0;
	cx=0;
	cy=0;
	tilefnlen=0;
	tilefn=NULL;
	tilessurface=NULL;
	layers=NULL;
}

CMap::~CMap()
{
	Free();
}

/**********************************************************************/

void CMap::Free()
{
	int i;

	nlayers=0;
	cx=0;
	cy=0;
	tilefnlen=0;
	bgmusiclen=0;

	// clears chars vector
	for(i=0;i<netchars.size();++i)
	{
		netchars.at(i)->Kill();
		delete netchars.at(i);
	}
	netchars.clear();

	delete[] tilefn;
	tilefn=NULL;

	delete[] bgmusic;
	bgmusic=NULL;

	if(tilessurface!=NULL) tilessurface->Release();
	tilessurface=NULL;

	for(i=0;i<nlayers;++i)
	{
		delete[] layers[i];
	}

	delete[] layers;
	layers=NULL;

	for(i=0;i<cx*cy;++i)
	{
		if(props[i].flags & FLAG_STRING)
		{
			delete[] props[i].string;
		}
	}

	delete[] props;
	props=NULL;
}

/**********************************************************************/

bool CMap::Load(const char* filename)
{
	char tmpstr[250];
	int i,l;
	FILE *f;

	/**** open file ****/
	memset(tmpstr,0,sizeof(tmpstr));
	strcpy(tmpstr,"maps\\");
	strcat(tmpstr,filename);
	f=fopen(tmpstr,"rb");
	if(f==NULL) return false;

	/**** read tiles filename ****/
	fread(&tilefnlen,1,4,f);
	if(tilefnlen>0)
	{
		tilefn=new char[tilefnlen+1];
		fread(tilefn,tilefnlen,1,f);
	}

	/**** read bgmusic ****/
	fread(&bgmusiclen,1,4,f);
	if(bgmusiclen>0)
	{
		bgmusic=new char[bgmusiclen+1];
		fread(bgmusic,bgmusiclen,1,f);
	}

	/**** read header's leftovers ****/
	fread(&cx,1,4,f);
	fread(&cy,1,4,f);
	fread(&nlayers,1,4,f);

	/**** create structure ****/
	props=new TILEPROPS[cx*cy];
	layers=new int*[nlayers];
	for(l=0;l<nlayers;++l)
	{
		layers[l]=new int[cx*cy];
	}

	/**** read tiles ****/
	for(l=0;l<nlayers;++l)
	{
		for(i=0;i<cx*cy;++i)
		{
			fread(&layers[l][i],1,4,f);
		}
	}

	/**** read property map ****/
	for(i=0;i<cx*cy;++i)
	{
		fread(&props[i].flags,1,4,f);

		/**** has a string? ****/
		if(props[i].flags & FLAG_STRING)
		{
			fread(&props[i].stringlen,4,1,f);
			props[i].string=new char[props[i].stringlen+1];
			fread(props[i].string,props[i].stringlen,1,f);
		}
		/**** has a teleport? ****/
		if(props[i].flags & FLAG_TELEPORT)
		{
			fread(&props[i].lparam,4,1,f);
			fread(&props[i].wparam,4,1,f);
		}

	}

	/**** close file ****/
	fclose(f);

	/**** load tiles ****/
	memset(tmpstr,0,sizeof(tmpstr));
	strcpy(tmpstr,"tiles\\");
	strncat(tmpstr,tilefn,tilefnlen);
	tilessurface=DDLoadBitmap(dd,tmpstr,0,0);
	if(tilessurface==NULL) return false;

	/**** play music ****/
	if(bgmusiclen>0)
	{
		memset(tmpstr,0,sizeof(tmpstr));
		strcpy(tmpstr,"music\\");
		strncat(tmpstr,bgmusic,bgmusiclen);
//		SFXStopMusic(333);
//		SFXPlayMusic(tmpstr);
	}

	return true;
}

/****************************************************************/

void CMap::Render(IDirectDrawSurface7* surface)
{
	int x;
	int y;
	int l;

	int vx;
	int vy;
	int offset;

	const int sx=320-mainchar->GetX();
	const int sy=240-mainchar->GetY()-32;

	for(l=0;l<nlayers;++l)
	{
		for(y=0;y<cy;++y)
		{
			for(x=0;x<cx;++x)
			{
				vx=x*32+sx;
				vy=y*32+sy;
				offset=y*cx+x;

				if( layers[l][offset] != -1 &&
					vx > -32 &&
					vx < 640 &&
					vy > -32 &&
					vy < 480)
				{
					DXDrawTile( layers[l][offset],
								tilessurface,
								vx,
								vy,
								32,
								32,
								false);
				}
			}
		}
	}

//	for(l=0;l<netchars.size();++l)
//	{
//		netchars.at(l)->Draw(sx+netchars.at(l)->GetX(),sy+netchars.at(l)->GetY());
//	}

	mainchar->Draw(320,240);
}

/****************************************************************/
