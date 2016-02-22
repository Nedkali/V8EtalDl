//////////////////////////////////////////////////////////////////////
// D2Helpers.cpp
//////////////////////////////////////////////////////////////////////
#include "D2Helpers.h"
#include "D2Pointers.h"



void LoadMPQ(const char* mpq)
{
	D2WIN_InitMPQ("D2Win.DLL", mpq, NULL, 0, 0);
	*p_BNCLIENT_XPacKey = *p_BNCLIENT_ClassicKey = *p_BNCLIENT_KeyOwner = NULL;
	BNCLIENT_DecodeAndLoadKeys();
}

