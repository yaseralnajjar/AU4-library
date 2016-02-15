#pragma once

#include <string>
#include <windows.h>
#include <Shlobj.h> //requisite au_FileCopy, FileCreateShortcut
#include <direct.h> //requisite au_FileChangeDir, 
#include <Shlwapi.h>//also "Shlwapi.obj" //requisite au_FileExists,
#include <vector>
using namespace std;

static wstring Utf8ToUtf16(const string & str);
static wstring Utf8ToUtf16(const char* str);

HANDLE au_FileOpen(string FName, int FMode = GENERIC_READ);
int au_FileClose(HANDLE hFile);
int au_FileCopy(string fSource, string fDest, int flag = 0);
int au_FileChangeDir(string NewPath);
int au_FileCreateNTFSLink(string fSource, string fDest, int flag = 0);
int au_FileCreateShortCut(string fSource, string fDest, string workdir = "", string args = "",
							string desc = "", string icon = "", string hotkey = "", int IcnNum = 0, int state = 0);
int au_FileDelete(string fileName);
int au_FileExists(string fileName);

struct retFileFindStruct
{	HANDLE hSearch;
	wstring hFileName;
};
retFileFindStruct au_FileFindFirstFile(string fileName);
wstring au_FileFindNextFile(retFileFindStruct &retValues, int flag = 0);
BOOL au__FindClose(HANDLE hSearch); //FileClose instead

void filesTest();