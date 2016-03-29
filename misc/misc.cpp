#include "stdafx.h"

/******************************************************/

unsigned long _stringwidth(const char* s, LPABC abc)
{
	int i;
	unsigned long ret=0;

	for(i=0;i<strlen(s);++i)
	{
		ret+=abc[s[i]].abcA+abc[s[i]].abcB+abc[s[i]].abcC;
	}

	return ret;
}

void _fillstring(std::string & dest, const std::string & src, char c)
{
	for (int i = 0; i != src.size(); ++i) dest += c;
}

void _sleep(unsigned long ms)
{
	control_update();
	gfx_update();
	Sleep(ms);
}

/******************************************************/
