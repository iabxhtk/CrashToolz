#pragma once
#include "EpochID2.h"
class CDisposer
{
public:
	static CDisposer * GetInstance()
	{
		return (CDisposer*)*(int*)CDISPOSER_INSTANCE_ADDY;
	}
	int SendPacket(int packetId, void * data)
	{
		EpochID2 * playerId = (EpochID2*)EPOCHID_INSTANCE_ADDY;
		auto SendPacket = (int(__thiscall*)(void*, int packetId, void * data, int playerType, int playerId))CDISPOSER_SENDPACKET_FUNC_ADDY;
		return SendPacket(this, packetId, data, playerId->nType, playerId->nId);

	}
};