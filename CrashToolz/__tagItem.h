#pragma once
#include "EpochID2.h"
#pragma pack(push,1)
struct __declspec(align(1)) _tagGR_ITEMOPTION
{
	char nType;
	__int16 nValue;
};
#pragma pack(pop)
#pragma pack(push,1)
struct __declspec(align(1)) __tagItem
{
	EpochID2 id;
	_tagGR_ITEMOPTION option[14];
	char nInventory;
	char nSlot;
	char nStack;
	char nRank;
	char nEquipLevel;
	__int16 nEquipStrength;
	__int16 nEquipDexterity;
	__int16 nEquipIntelligence;
	void Init()
	{

		void(__thiscall *Init)(void*) = (void(__thiscall*)(void*))0x004160E0;
		return Init(this);
	}
};
#pragma pack(pop)