#pragma once
#include <stdexcept>
#include <tuple>
#include <typeinfo>
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include "retcheck.h"
extern "C" {
#include "Lua\lua.h"
#include "Lua\lapi.h"
#include "Lua\lualib.h"
#include "Lua\lstate.h"
#include "Lua\lauxlib.h"
#include "Lua\luaconf.h"
#include "Lua\llimits.h"
#include "Lua\lapi.h"
#include "Lua\lfunc.h"
#include "Lua\lobject.h"
}

#define R_LUA_TNONE	-1
#define R_LUA_TNIL 0 
#define R_LUA_TLIGHTUSERDATA 1
#define R_LUA_TNUMBER 2
#define R_LUA_TBOOLEAN 3
#define R_LUA_TSTRING 4
#define R_LUA_TTHREAD 5
#define R_LUA_TFUNCTION 6
#define R_LUA_TTABLE 7
#define R_LUA_TUSERDATA 8
#define R_LUA_TPROTO 9
#define R_LUA_TUPVALUE 10
#define LoadAddress(Address) (Address - 0x400000 + (DWORD)GetModuleHandle(NULL))
#define Level(RLS, I) DWORD* Identity  = (DWORD*)(RLS - 40); *Identity ^= (I ^ (unsigned __int8)*Identity) & 0x1F;

union r_lua_Value {
	int b;
	double n;
	void* p;
	void* gc;
};

struct r_lua_TValue {
	r_lua_Value value;
	int tt;
};



std::vector<DWORD> Breakpoints;
#define RBX_TOP 20
#define RBX_BASE 8



typedef void(__stdcall * rgetfield)(DWORD rL, int idx, const char* k);
rgetfield r_lua_getfield = (rgetfield)unprotect(LoadAddress(0x847360));//Done

typedef char* (__fastcall* rtolstring)(DWORD rL, int idx, size_t* size);
rtolstring r_lua_tolstring = (rtolstring)(unprotect(LoadAddress(0x849db0)));//Done

typedef void(__cdecl* rsettop)(DWORD rL, int idx);
rsettop r_lua_settop = (rsettop)(unprotect(LoadAddress(0x849b00)));//Done

typedef bool(__cdecl* toboolean)(DWORD rL, bool idx);
toboolean r_lua_toboolean = (toboolean)(LoadAddress(0x849ca0));//Done

typedef void(__stdcall* pushvalue)(DWORD rL, DWORD idx);
pushvalue r_lua_pushvalue = (pushvalue)(unprotect(LoadAddress(0x848a60)));//Done

typedef double(__cdecl* pushnumber)(DWORD rL, double idx);
pushnumber r_lua_pushnumber = (pushnumber)(unprotect(LoadAddress(0x848870)));//Done

typedef void(__fastcall* rpushstring)(DWORD rL, const char*);
rpushstring r_lua_pushstring = (rpushstring)(LoadAddress(0x848900));//Done

auto r_luad_pcall = (int(__cdecl*)(DWORD, int, int, int))unprotect(LoadAddress(0x848320));

typedef void(__cdecl* pushnil)(DWORD);
pushnil r_lua_pushnil = (pushnil)(unprotect(LoadAddress(0x848800)));//Done

typedef DWORD(__cdecl* next2)(DWORD rL, int idx);
next2 r_lua_next = (next2)(unprotect(LoadAddress(0x848160)));//Done

typedef bool(__cdecl* rboolean)(unsigned int, bool);
rboolean r_lua_pushboolean = (rboolean)(unprotect(LoadAddress(0x848400)));

double r_lua_tonumber(DWORD rL, int idx)
{
	return ((double(__cdecl*)(DWORD, int, int))LoadAddress(0x849f20))(rL, idx, 0);
}

typedef void(__cdecl* rpushcclosure)(DWORD rL, int fn, int non);
rpushcclosure r_lua_pushcclosure = (rpushcclosure)(unprotect(LoadAddress(0x848480)));

typedef void(__cdecl* rcreatetable)(DWORD rL, int num, int fix);
rcreatetable r_lua_createtable = (rcreatetable)(unprotect(LoadAddress(0x846ff0)));

typedef DWORD(__cdecl* rnewthread)(DWORD);
rnewthread r_lua_newthread = (rnewthread)unprotect(LoadAddress(0x847fc0));

void* r_lua_newuserdata(DWORD rL, size_t size)
{
	return ((void* (__cdecl*)(DWORD rL, size_t size, int))unprotect(LoadAddress(0x8480b0)))(rL, size, 0);
}

typedef void(__cdecl* rrawgeti)(DWORD, DWORD, DWORD);
rrawgeti r_lua_rawgeti = (rrawgeti)unprotect(LoadAddress(0x848dc0));

typedef void* (__stdcall* rgetmetatable)(DWORD rL, int idx);
rgetmetatable r_lua_getmetatable = (rgetmetatable)(unprotect(LoadAddress(0x847720)));

auto r_lua_touserdata = (int(__cdecl*)(DWORD, int))LoadAddress(0x84a1b0);

typedef DWORD(__cdecl* rtype)(DWORD, int);
rtype r_lua_type = (rtype)(LoadAddress(0x84a250));

typedef void* (__cdecl* rsettable)(DWORD rL, int);
rsettable r_lua_settable = (rsettable)(unprotect(LoadAddress(0x849a50)));

typedef void(__cdecl* rpushlight)(DWORD, void*);
rpushlight r_lua_pushlightuserdata = (rpushlight)(unprotect(LoadAddress(0x8486e0)));

typedef int(__cdecl* gettop)(DWORD);
gettop r_lua_gettop = (gettop)(LoadAddress(0x847990));

#define r_lua_pop(rL,n)		r_lua_settop(rL, -(n)-1)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, -10002, (s))
#define r_lua_createnewtable(rL) r_lua_createtable(rL, 0, 0)
#define r_lua_tostring(rL,i)	r_lua_tolstring(rL, (i), NULL)
#define r_lua_pushcfunction(rL, fn)		 (r_lua_pushcclosure(rL,fn,0))
#define r_lua_isnil(L,n)		(r_lua_type(L, (n)) == R_LUA_TNIL)
#define r_lua_getglobal(rL,s)	r_lua_getfield(rL, LUA_GLOBALSINDEX, (s))










void fakeChain(DWORD* chain)
{
	chain[1] = 0x1555555;
	if ((((DWORD*)chain[0])[1]) != NULL) {
		((DWORD*)chain[0])[1] = 0x1555555;
	}
}
void restoreChain(DWORD* chain, DWORD unk, DWORD nextUnk)
{
	chain[1] = unk;
	if ((((DWORD*)chain[0])[1]) != NULL) {
		((DWORD*)chain[0])[1] = nextUnk;
	}
}


typedef int(__cdecl* rtointeger)(int, int, int);
rtointeger r_lua_tointeger = (rtointeger)(LoadAddress(0x849cf0));

typedef void(__cdecl* rrawseti)(DWORD, DWORD, DWORD);
rrawseti r_lua_rawseti = (rrawseti)unprotect(LoadAddress(0x848f80));

typedef int(__cdecl* robjlen)(int, int);
robjlen r_lua_objlen = (robjlen)unprotect(LoadAddress(0x848210));

#define abs_index(L, i)		((i) > 0 || (i) <= -10000 ? (i) : \
					r_lua_gettop(L) + (i) + 1)
#define r_lua_isnil(L,n)		(r_lua_type(L, (n)) == 0)
#define r_LUA_REFNIL      (-1)

int r_luaL_ref(int L, int t) {//just like vanilla lual_ref
	int ref;
	t = abs_index(L, t);
	if (r_lua_isnil(L, -1)) {
		r_lua_pop(L, 1);  /* remove from stack */
		return r_LUA_REFNIL;  /* `nil' has a unique fixed reference */
	}
	r_lua_rawgeti(L, t, 0);  /* get first free element */
	ref = (int)r_lua_tointeger(L, -1, 0);  /* ref = t[FREELIST_REF] */
	r_lua_pop(L, 1);  /* remove it from stack */
	if (ref != 0) {  /* any free element? */
		r_lua_rawgeti(L, t, ref);  /* remove it from list */
		r_lua_rawseti(L, t, 0);  /* (t[FREELIST_REF] = t[ref]) */
	}
	else {  /* no free elements */
		ref = (int)r_lua_objlen(L, t);
		ref++;  /* create new reference */
	}
	r_lua_rawseti(L, t, ref);
	return ref;
}

int r_lua_pcall(DWORD rL, int nargs, int nresults, int errfunc)
{
	DWORD* exceptionChain = (DWORD*)__readfsdword(0);
	DWORD unk = exceptionChain[1];
	((((DWORD*)exceptionChain[0])[1]) != NULL);
	{
		DWORD nextUnk = ((DWORD*)exceptionChain[0])[1];
		fakeChain(exceptionChain);
		int ret = r_luad_pcall(rL, nargs, nresults, errfunc);
		restoreChain(exceptionChain, unk, nextUnk);
		return ret;
	}

	fakeChain(exceptionChain);
	int ret = r_luad_pcall(rL, nargs, nresults, errfunc);
	restoreChain(exceptionChain, unk, 0);
	return ret;
}

void r_lua_setfield(DWORD L, int idx, const char* k) {
	r_lua_pushvalue(L, idx);
	if (r_lua_getmetatable(L, -1)) {
		r_lua_getfield(L, -1, "__newindex");
		r_lua_pushvalue(L, -3);
		r_lua_pushstring(L, k);
		r_lua_pushvalue(L, -6);
		r_lua_pcall(L, 3, 1, 0);
		r_lua_pop(L, 3);
	}
	else {
		r_lua_pop(L, 1);
		r_lua_setfield(L, idx, k);
	}
}