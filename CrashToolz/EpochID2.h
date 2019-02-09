#pragma once
class EpochID2
{
public:
	unsigned __int16 GetUpper()
	{
		return  ((unsigned __int16)(nType >> 16));
	}
	unsigned int nType;
	unsigned int nId;
};