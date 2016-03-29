#include "stdafx.h"

/********************************************************************/

CNetworkSystem::CNetworkSystem()
{
}

CNetworkSystem::~CNetworkSystem()
{
	Kill();
}

/********************************************************************/

bool CNetworkSystem::Init(void)
{
	WORD version = MAKEWORD(1,1);
	WSADATA wsaData;

	if(WSAStartup(version, &wsaData)==0)
		return true;
	else
		return false;
}

/********************************************************************/

void CNetworkSystem::Kill(void)
{
	WSACleanup();
}

/********************************************************************/