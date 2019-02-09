// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <stdio.h>
#include "FindPattern.h"
#include <iostream>
#include <detours.h>
#include "__tagItem.h"
#include <thread>
#include <future>
#pragma comment( lib, "detours.lib" )

static HMODULE hModule;
static HANDLE hConsole;
static bool isHackPrepared = false;
#pragma pack(push,1)
struct _tagITEM_STACK_INFO
{
	EpochID2 id;
	char nInventory;
	char nSlot;
	char nStack;
};

struct _tagRequestMAP_PRODUCTION_ACTION_RESULT
{
	char nCount;
	_tagITEM_STACK_INFO ItemInfo[12];

};
#pragma pack(pop)
_tagRequestMAP_PRODUCTION_ACTION_RESULT hackStructPacket = { 0 };

void CreateConsole()
{
	AllocConsole();
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);
	//freopen("CONOUT$", "w", stdout);
	//freopen("CONIN$", "r", stdin);
}
int(__thiscall * CPacket__CopyPacketData)  (void * thisa, unsigned int a2, void *a3);


int(__cdecl * CPacket__SendProductionActionResult)  (void *a1, void *a2);
int __cdecl dCPacket__SendProductionActionResult(void *a1, void *a2)
{
	std::cout << "CPacket__SendProductionActionResult  called. " << std::endl;
	CPacket__CopyPacketData(a1, sizeof(_tagRequestMAP_PRODUCTION_ACTION_RESULT), a2);
	return 0;
	return CPacket__SendProductionActionResult(a1, a2);
}


int(__thiscall * CDisposer__SendPacketMaybe)  (void * tThis, int packetType, void *packetData, int playerType, int playerId);
int __fastcall dCDisposer__SendPacketMaybe(void * This, void* notUsed, int packetType, void *packetData, int playerType, int playerId)
{

	if (isHackPrepared)
	{
		static bool call_once = false;
		if (!call_once)
		{
			CDisposer__SendPacketMaybe(This, 8973, &hackStructPacket, playerType, playerId);
			call_once = true;
			/*
			std::thread{ [&]() {
				std::this_thread::sleep_for(std::chrono::seconds{ 15 });
				std::cout << "Scheduled packet hack sent. " << std::endl;
				CDisposer__SendPacketMaybe(This, 8973, &hackStructPacket, playerType, playerId);
			} }.detach();
			call_once = true;
			*/
		}
	}
	return CDisposer__SendPacketMaybe(This, packetType, packetData, playerType, playerId);
}
void*(__thiscall * CEpochItemManager__CreateAndAddItem)  (void * tThis, __tagItem *itemData, void *pPlayer, int someFlag);
void* __fastcall dCEpochItemManager__CreateAndAddItem(void * This, void* notUsed, __tagItem *itemData, void *pPlayer, int someFlag)
{
	if (!isHackPrepared)
	{
		int itemCount = hackStructPacket.nCount;
		if (itemData->nInventory > 0)
		{

			hackStructPacket.ItemInfo[itemCount].id.nId = itemData->id.nId;
			hackStructPacket.ItemInfo[itemCount].id.nType = itemData->id.nType;
			hackStructPacket.ItemInfo[itemCount].nInventory = itemData->nInventory;
			hackStructPacket.ItemInfo[itemCount].nSlot = itemData->nSlot;
			hackStructPacket.ItemInfo[itemCount].nStack = itemData->nStack;

			std::cout << "ID: " << hackStructPacket.ItemInfo[itemCount].id.nId << " Type: " << hackStructPacket.ItemInfo[itemCount].id.nType << std::endl;

			hackStructPacket.nCount++;

		}
		if (itemCount > 11)
		{
			std::cout << "Hack items prepared" << std::endl;
			isHackPrepared = true;
		}
	}
	return CEpochItemManager__CreateAndAddItem(This, itemData, pPlayer, someFlag);
}


void InitializationThreadFunc()
{
	//Module mod("DarknessClient.exe");
	Module mod;

	void * sendFuncAddyPtr = mod.FindPattern<void*>("68 ? ? ? ? E8 ? ? ? ? 8B 8C 24 ? ? ? ? 33 C0 5E E8 ? ? ? ? 81 C4 ? ? ? ? C3");
	if (!sendFuncAddyPtr)
	{
		std::cout << "CDisposer__SendPacketMaybe function address not found";
		return;
	}
	int relativeCall = *(int*)(((char*)sendFuncAddyPtr) + 6);

	void * sendFuncAddy = ((char*)sendFuncAddyPtr + 10) + relativeCall;
	CDisposer__SendPacketMaybe = (int(__thiscall*)(void * tThis, int packetType, void *packetData, int playerType, int playerId))(sendFuncAddy);
	std::cout << "Found CDisposer__SendPacketMaybe address: 0x" << std::hex << (int)sendFuncAddy << std::dec << std::endl;


	void * onGetCharItemAddy = mod.FindPattern<void*>("55 8B EC 83 E4 F8 81 EC ? ? ? ? A1 ? ? ? ? 53 8B 5D 08 56 57 8B 7D 0C 57 53 89 84 24 ? ? ? ? E8 ? ? ? ? 8B F0 85 F6 0F 84 ? ? ? ? 8A 43 32 84 C0 75 57 8A 4D 10 84 C9 74 50");
	if (!onGetCharItemAddy)
	{
		std::cout << "CEpochItemManager__CreateAndAddItem function address not found";
		return;
	}
	CEpochItemManager__CreateAndAddItem = (void*(__thiscall *)  (void * tThis, __tagItem *itemData, void *pPlayer, int someFlag))(onGetCharItemAddy);
	std::cout << "Found CEpochItemManager__CreateAndAddItem address: 0x" << std::hex << (int)onGetCharItemAddy << std::dec << std::endl;


	void * copyPacketDataAddy = mod.FindPattern<void*>("8B C1 8B 50 0C 53 8B 5C 24 08 56 8B 70 08 8D 0C 1A 3B CE 7F 2A 8B 74 24 10 57 8B 78 04 03 FA 8B CB 8B D1 C1 E9 02");
	if (!copyPacketDataAddy)
	{
		std::cout << "CPacket__CopyPacketData function address not found";
		return;
	}
	CPacket__CopyPacketData = (int(__thiscall *)  (void * thisa, unsigned int a2, void *a3))(copyPacketDataAddy);
	std::cout << "Found CPacket__CopyPacketData address: 0x" << std::hex << (int)copyPacketDataAddy << std::dec << std::endl;


	void * sendProdAddy = mod.FindPattern<void*>("8B 44 24 08 8B 4C 24 04 50 6A 6F E8 ? ? ? ? 33 C0 C3");
	if (!sendProdAddy)
	{
		std::cout << "CPacket__SendProductionActionResult function address not found";
		return;
	}
	CPacket__SendProductionActionResult = (int(__cdecl *)  (void *a1, void *a2))(sendProdAddy);
	std::cout << "Found CPacket__SendProductionActionResult address: 0x" << std::hex << (int)sendProdAddy << std::dec << std::endl;

	DisableThreadLibraryCalls(hModule);
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&(PVOID&)CPacket__SendProductionActionResult, dCPacket__SendProductionActionResult);
	DetourAttach(&(PVOID&)CEpochItemManager__CreateAndAddItem, dCEpochItemManager__CreateAndAddItem);
	DetourAttach(&(PVOID&)CDisposer__SendPacketMaybe, dCDisposer__SendPacketMaybe);
	DetourTransactionCommit();

}
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
	//
	// We don't need to get notified in thread attach- or detachments
	// 
	CreateConsole();
	DisableThreadLibraryCalls(static_cast<HMODULE>(hInstance));
	hModule = hInstance;
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InitializationThreadFunc, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		break;
	default:
		break;
	}

	return TRUE;
}
