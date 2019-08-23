#include "globals.h"

#include "Bridge.h"
#include "functions.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <fstream>
#include <intrin.h>
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#pragma comment(lib, "D3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "wininet.lib")
using namespace std;


DWORD ScriptContextVFT_Addy = x(0x1E20FF8); //--update each week--//
namespace Memory { bool Compare(const char* pData, const char* bMask, const char* szMask) { while (*szMask) { __try { if (*szMask != '?') { if (*pData != *bMask) return 0; } ++szMask, ++pData, ++bMask; } __except (EXCEPTION_EXECUTE_HANDLER) { return 0; } } return 1; } DWORD Scan() { MEMORY_BASIC_INFORMATION MBI = { 0 }; SYSTEM_INFO SI = { 0 }; GetSystemInfo(&SI); DWORD Start = (DWORD)SI.lpMinimumApplicationAddress; DWORD End = (DWORD)SI.lpMaximumApplicationAddress; do { while (VirtualQuery((void*)Start, &MBI, sizeof(MBI))) { if ((MBI.Protect & PAGE_READWRITE) && !(MBI.Protect & PAGE_GUARD) && !(MBI.Protect & 0x90)) { for (DWORD i = (DWORD)(MBI.BaseAddress); i - (DWORD)(MBI.BaseAddress) < MBI.RegionSize; ++i) { if (Compare((const char*)i, (char*)& ScriptContextVFT_Addy, "xxxx")) return i; } } Start += MBI.RegionSize; } } while (Start < End); return 0; } }
namespace ScriptCOontext {
	DWORD hookStateIndex(DWORD hooklocation, int offset) {
		return *(DWORD*)(hooklocation + offset) - (hooklocation + offset); //--update each week--//
	}
	void scriptcontext() {
		DWORD ScriptContext; ScriptContext = Memory::Scan();
		RLS = hookStateIndex(ScriptContext, 164);
	}
}
void main()
{
	CreateConsole("Updated Ax0n");
	std::string xyz = "[Ax0n]: ";
	ScriptCOontext::scriptcontext();
	LS = luaL_newstate();
	seti(RLS, 6);

	Bridge::VehHandlerpush();
	luaL_openlibs(LS);
	luaL_newmetatable(LS, "garbagecollector");
	lua_pushcfunction(LS, UserDataGC);
	lua_setfield(LS, -2, "__gc");
	lua_pushvalue(LS, -1);
	lua_setfield(LS, -2, "__index");
	PushGlobal(RLS, LS, "printidentity");
	PushGlobal(RLS, LS, "game");
	PushGlobal(RLS, LS, "Game");
	PushGlobal(RLS, LS, "workspace");
	PushGlobal(RLS, LS, "Workspace");
	PushGlobal(RLS, LS, "Axes");
	PushGlobal(RLS, LS, "BrickColor");
	PushGlobal(RLS, LS, "CFrame");
	PushGlobal(RLS, LS, "Color3");
	PushGlobal(RLS, LS, "ColorSequence");
	PushGlobal(RLS, LS, "ColorSequenceKeypoint");
	PushGlobal(RLS, LS, "NumberRange");
	PushGlobal(RLS, LS, "NumberSequence");
	PushGlobal(RLS, LS, "NumberSequenceKeypoint");
	PushGlobal(RLS, LS, "PhysicalProperties");
	PushGlobal(RLS, LS, "Ray");
	PushGlobal(RLS, LS, "Rect");
	PushGlobal(RLS, LS, "Region3");
	PushGlobal(RLS, LS, "Region3int16");
	PushGlobal(RLS, LS, "TweenInfo");
	PushGlobal(RLS, LS, "UDim");
	PushGlobal(RLS, LS, "UDim2");
	PushGlobal(RLS, LS, "Vector2");
	PushGlobal(RLS, LS, "Vector2int16");
	PushGlobal(RLS, LS, "Vector3");
	PushGlobal(RLS, LS, "Vector3int16");
	PushGlobal(RLS, LS, "Enum");
	PushGlobal(RLS, LS, "Faces");
	PushGlobal(RLS, LS, "Instance");
	PushGlobal(RLS, LS, "math");
	PushGlobal(RLS, LS, "warn");
	PushGlobal(RLS, LS, "typeof");
	PushGlobal(RLS, LS, "type");
	PushGlobal(RLS, LS, "spawn");
	PushGlobal(RLS, LS, "Spawn");
	PushGlobal(RLS, LS, "print");
	PushGlobal(RLS, LS, "printidentity");
	PushGlobal(RLS, LS, "ypcall");
	PushGlobal(RLS, LS, "Wait");
	PushGlobal(RLS, LS, "wait");
	PushGlobal(RLS, LS, "delay");
	PushGlobal(RLS, LS, "Delay");
	PushGlobal(RLS, LS, "tick");
	PushGlobal(RLS, LS, "LoadLibrary");
	cout << "Completed." << endl;
	cout << blue << xyz << white << "Registering Functions -> ";
	CommunistFunction(LS, "getrawmetatable", getRawMetaTable);
	CommunistFunction(LS, "loadstring", epic_loadstring);
	CommunistFunction(LS, "httpget", r_httpget);
	CommunistFunction(LS, "WrapGlobal", CustomWrap);

	CommunistFunction(LS, "ShowConsole", ConsoleTrue);
	CommunistFunction(LS, "mousemoverel", MouseMove);
	CommunistFunction(LS, "MouseMoveRel", MouseMove);
	CommunistFunction(LS, "mousemoverelative", MouseMove);
	CommunistFunction(LS, "MouseMoveRelative", MouseMove);
	CommunistFunction(LS, "HideConsole", ConsoleFalse);
	CommunistFunction(LS, "Crash", CrashRoblox);
	CommunistFunction(LS, "MessageBox", ExonBox);
	CommunistFunction(LS, "ExonBox", ExonBox);
	CommunistFunction(LS, "Bruh", Bruh);
	CommunistFunction(LS, "Dex", Dex);

	lua_newtable(LS);
	lua_setglobal(LS, "_G");
	MessageBoxA(NULL, "Injected!", "ax0n", MB_TOPMOST);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
	//ImGUI::Begin();
}


BOOL APIENTRY DllMain(HMODULE Module, DWORD Reason, void* Reserved)
{
	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(Module);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)input, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	;default: break;
	}

	return TRUE;
}