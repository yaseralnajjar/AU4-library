#pragma once

#include <string>
#include <windows.h>
#include <Shlobj.h> //requisite au_FileCopy, FileCreateShortcut
#include <direct.h> //requisite au_FileChangeDir, 
#include <Shlwapi.h>//also "Shlwapi.obj" //requisite au_FileExists,
#include <vector>
#include "text_encoding_detect.h"
using namespace std;

//static wstring Utf8ToUtf16(const string & str);
//static wstring Utf8ToUtf16(const char* str);
string ToAscii(const std::wstring& input);
wstring ToUnicode(const std::string& input);

HANDLE au_FileOpen(wstring FName, int FMode = GENERIC_READ);
int au_FileClose(HANDLE hFile);
int au_FileCopy(wstring fSource, wstring fDest, int flag = 0);
bool au_FileChangeDir(wstring NewPath);
bool au_FileCreateNTFSLink(wstring fSource, wstring fDest, int flag = 0);
bool au_FileCreateShortCut(wstring fSource, wstring fDest, wstring workdir = L"", wstring args = L"",
							wstring desc = L"", wstring icon = L"", wstring hotkey = L"", int IcnNum = 0, int state = 0);
bool au_FileDelete(wstring fileName);
bool au_FileExists(wstring fileName);

struct retFileFindStruct
{	HANDLE hSearch;
	wstring hFileName;
};
retFileFindStruct au_FileFindFirstFile(wstring fileName);
wstring au_FileFindNextFile(retFileFindStruct &retValues, int flag = 0);
bool au__FindClose(HANDLE hSearch); //FileClose instead

int au_FileFlush(HANDLE fHandle);
string au_FileGetAttrib(wstring fileName);
int au_FileGetEncoding(wstring fileName, int flag = 1);
int au_FileGetEncoding(HANDLE hFile, int flag = 1);
wstring au_FileGetLongName(wstring fileName, int flag = 0);

void filesTest();