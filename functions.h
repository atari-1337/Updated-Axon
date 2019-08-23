#include "globals.h"
#include "Bridge.h"
#include "r_lua.h"
using namespace std;

using Bridge::RLS;
using Bridge::LS;

DWORD ScriptContext;
DWORD ScriptContextVFTable = x(0x1E20FF8);

int getRawMetaTable(lua_State* L) {
	Bridge::unwrap(L, RLS, 1);

	if (r_lua_getmetatable(RLS, -1) == 0) {
		lua_pushnil(L);
		return 0;
	}
	Bridge::wrap(RLS, L, -1);

	return 1;
}

static int UserDataGC(lua_State* Thread) {
	void* UD = lua_touserdata(Thread, 1);
	if (RLS) {

		r_lua_rawgeti(RLS, LUA_REGISTRYINDEX, (int)UD);
		if (r_lua_type(RLS, -1) <= R_LUA_TNIL) {
			lua_pushnil(Thread);
			lua_rawseti(Thread, LUA_REGISTRYINDEX, (int)UD);
		}
	}
	return 0;
}


std::string replace(std::string subject, const std::string& search, const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;

}
static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int stringLength = sizeof(alphanum) - 1;

char genRandom()
{
	return alphanum[rand() % stringLength];
}


string ExecuteStr;
void Execute() {
	std::string Script = ExecuteStr;
	Script = "spawn(function() script = Instance.new('LocalScript')\r\n" + Script + "\r\nend)";

	if (Script.find(".Name = \"Dex\"") == std::string::npos) {
		Script = replace(Script, "game:GetObjects", "GetObjects");
		Script = replace(Script, "game:HttpGet", "httpget");
	}
	else {

	}
	if (luaL_loadstring(LS, Script.c_str())) {
		return;
	}
	else {
		lua_pcall(LS, 0, 0, 0);
    }
}

#define CRASH EXCEPTION_EXECUTE_HANDLER

void SecureRun() {
	__try { Execute(); }
	__except (CRASH) {}
}
void Execute_Script(std::string Script) {
	ExecuteStr = Script;
	SecureRun();
}



int epic_loadstring(lua_State* LS) {
	if (lua_gettop(LS) == 0) {
		luaL_error(LS, "'loadstring' needs 1 string argument.");
		return 0;
	}
	Execute_Script(std::string(lua_tostring(LS, -1)));
	return 0;
}

int CrashRoblox(lua_State* L) {
	LPCSTR name = lua_tostring(L, -1);
	MessageBoxA(NULL, name, "ExOn", MB_TOPMOST);
	exit(10);
	return 0;
}
int Bruh(lua_State* L)
{
	Execute_Script("loadstring(game:HttpGet(('https://pastebin.com/raw/eFVTz1iX'),true))()");
	return 0;
}
int Dex(lua_State* L)
{
	Execute_Script("loadstring(game:GetObjects('rbxassetid://2180084478')[1].Source)()");
	return 0;
}
int ExonBox(lua_State* L) {
	LPCSTR name = lua_tostring(L, -1);
	MessageBoxA(NULL, name, "ExOn", MB_TOPMOST);
	return 0;
}

DWORD WINAPI input(PVOID lvpParameter)
{
	string WholeScript = "";
	HANDLE hPipe;
	char buffer[999999];
	DWORD dwRead;
	hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\ax0n"),
		PIPE_ACCESS_DUPLEX | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
		PIPE_WAIT,
		1,
		999999,
		999999,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	while (hPipe != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipe, NULL) != FALSE)
		{
			while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				buffer[dwRead] = '\0';
				try {
					try {
						WholeScript = WholeScript + buffer;
					}
					catch (...) {
					}
				}
				catch (std::exception e) {

				}
				catch (...) {

				}
			}
			Execute_Script(WholeScript);
			WholeScript = "";
		}
		DisconnectNamedPipe(hPipe);
	}
}

#include <windows.h>
#include <string>
#include <stdio.h>


void PushGlobal(DWORD rL, lua_State* L, const char* s)
{
	Sleep(10);
	r_lua_getglobal(rL, s);
	Bridge::wrap(rL, L, -1);
	lua_setglobal(L, s);
	r_lua_pop(rL, 1);
}


void CreateConsole(const char* Title) {
	DWORD superskiddoisachink;
	VirtualProtect((PVOID)& FreeConsole, 1, PAGE_EXECUTE_READWRITE, &superskiddoisachink);
	*(BYTE*)(&FreeConsole) = 0xC3;
	AllocConsole();
	SetConsoleTitleA(Title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow();
	::SetWindowPos(ConsoleHandle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(ConsoleHandle, SW_NORMAL);
}
inline std::ostream& white(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, 7 | FOREGROUND_BLUE);
	return s;
}

inline std::ostream& blue(std::ostream& s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, 1 | FOREGROUND_INTENSITY);
	return s;
}


#pragma comment(lib,"ws2_32.lib")

HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename);
SOCKET connectToServer(char* szServerName, WORD portNum);
int getHeaderLength(char* content);
char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut);


int r_httpget(lua_State* L)
{
	if (!lua_isstring(L, 1))
		return 1;
	const int bufLen = 1024;
	const char* szUrl = lua_tostring(L, 1); 
	long fileSize;
	char* memBuffer, * headerBuffer;
	FILE* fp;
	memBuffer = headerBuffer = NULL;
	if (WSAStartup(0x101, &wsaData) != 0)
		return -1;
	memBuffer = readUrl2((char*)szUrl, fileSize, &headerBuffer);
	lua_pushstring(L, memBuffer);
	if (fileSize != 0)
	{
		fp = fopen("downloaded.file", "wb");
		fwrite(memBuffer, 1, fileSize, fp);
		fclose(fp);
		delete(memBuffer);
		delete(headerBuffer);
	}
	WSACleanup();
	return 1;
}

void mParseUrl(char* mUrl, string& serverName, string& filepath, string& filename)
{
	string::size_type n;
	string url = mUrl;

	if (url.substr(0, 7) == "http://")
		url.erase(0, 7);

	if (url.substr(0, 8) == "https://")
		url.erase(0, 8);

	n = url.find('/');
	if (n != string::npos)
	{
		serverName = url.substr(0, n);
		filepath = url.substr(n);
		n = filepath.rfind('/');
		filename = filepath.substr(n + 1);
	}

	else
	{
		serverName = url;
		filepath = "/";
		filename = "";
	}
}

SOCKET connectToServer(char* szServerName, WORD portNum)
{
	struct hostent* hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)& addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*) & server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}

int getHeaderLength(char* content)
{
	const char* srchStr1 = "\r\n\r\n", * srchStr2 = "\n\r\n\r";
	char* findPos;
	int ofset = -1;

	findPos = strstr(content, srchStr1);
	if (findPos != NULL)
	{
		ofset = findPos - content;
		ofset += strlen(srchStr1);
	}

	else
	{
		findPos = strstr(content, srchStr2);
		if (findPos != NULL)
		{
			ofset = findPos - content;
			ofset += strlen(srchStr2);
		}
	}
	return ofset;
}

char* readUrl2(char* szUrl, long& bytesReturnedOut, char** headerOut)
{
	const int bufSize = 512;
	char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
	char* tmpResult = NULL, * result;
	SOCKET conn;
	string server, filepath, filename;
	long totalBytesRead, thisReadSize, headerLen;
	mParseUrl(szUrl, server, filepath, filename);
	conn = connectToServer((char*)server.c_str(), 80);
	sprintf(tmpBuffer, "GET %s HTTP/1.0", filepath.c_str());
	strcpy(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tmpBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(conn, sendBuffer, strlen(sendBuffer), 0);
	totalBytesRead = 0;
	while (1)
	{
		memset(readBuffer, 0, bufSize);
		thisReadSize = recv(conn, readBuffer, bufSize, 0);

		if (thisReadSize <= 0)
			break;

		tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

		memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	headerLen = getHeaderLength(tmpResult);
	long contenLen = totalBytesRead - headerLen;
	result = new char[contenLen + 1];
	memcpy(result, tmpResult + headerLen, contenLen);
	result[contenLen] = 0x0;
	char* myTmp;

	myTmp = new char[headerLen + 1];
	strncpy(myTmp, tmpResult, headerLen);
	myTmp[headerLen] = NULL;
	delete(tmpResult);
	*headerOut = myTmp;

	bytesReturnedOut = contenLen;
	closesocket(conn);
	return(result);
}

int ConsolePrint(lua_State* thread) {
	std::string msg = lua_tostring(thread, -1);
	cout << blue << "[ax0n]: " << white << msg << endl;
	return 1;
}

int CustomWrap(lua_State* thread) {
	std::string globalName = lua_tostring(thread, -1);
	PushGlobal(RLS, LS, globalName.c_str());
	Execute_Script("game.StarterGui:SetCore('SendNotification', {Duration = 10; Title='mmm yee ax0n'; Text='" + globalName + "'})");
	return 1;
}
int MouseMove(lua_State* thread) {
	int x = lua_tonumber(thread, -1);
	int y = lua_tonumber(thread, -2);
	POINT p;
	if (GetCursorPos(&p))
	{
		SetCursorPos(p.x + x, p.y + y);
	}

	return 1;
}
int ConsoleFalse(lua_State* L) {
	::ShowWindow(GetConsoleWindow(), SW_HIDE);
	return 1;
}

int ConsoleTrue(lua_State* L) {
	::ShowWindow(GetConsoleWindow(), SW_SHOW);
	return 1;
}