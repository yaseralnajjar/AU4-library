#pragma once

#include <string>
#include <windows.h>
#include <Shlobj.h> //requisite au_FileCopy, FileCreateShortcut
#include <direct.h> //requisite au_FileChangeDir, 
#include <Shlwapi.h>//also "Shlwapi.obj" //requisite au_FileExists,
#include <vector>
#include "text_encoding_detect.h"
#include "FileConstants.h"
#include <strsafe.h>
using namespace std;

static string Utf16ToUtf8(const wstring & wstr);
static string Utf16ToUtf8(const wchar_t* wstr);
static wstring Utf8ToUtf16(const string & str);
static wstring Utf8ToUtf16(const char* str);
//string ToAscii(const std::wstring& input);
//wstring ToUnicode(const std::string& input);

bool au_DirCreate(const wstring& dirName);


HANDLE au_FileOpen(const wstring& fileName, int fileMode = $FO_READ);
int au_FileClose(HANDLE hFile);
int au_FileCopy(const wstring& fSource, const wstring& fDest, int flag = $FC_NOOVERWRITE);
bool au_FileChangeDir(wstring NewPath);
bool au_FileCreateNTFSLink(wstring fSource, wstring fDest, int flag = $FC_NOOVERWRITE);
bool au_FileCreateShortCut(wstring fSource, wstring fDest, wstring workdir = L"", wstring args = L"",
							wstring desc = L"", wstring icon = L"", wstring hotkey = L"", int IcnNum = 0, int state = 0);
bool au_FileDelete(wstring fileName);
bool au_FileExists(const wstring& fileName);

struct retFileFindStruct
{	HANDLE hSearch;
	wstring hFileName;
};
retFileFindStruct au_FileFindFirstFile(const wstring& fileName);
wstring au_FileFindNextFile(retFileFindStruct& retValues, int flag = 0);
bool au__FindClose(HANDLE hSearch); //FileClose instead

int au_FileFlush(HANDLE fHandle);
string au_FileGetAttrib(const wstring& fileName);
int au_FileGetEncoding(wstring fileName, int flag = 1);
int au_FileGetEncoding(HANDLE hFile, int flag = 1);
wstring au_FileGetLongName(const wstring& fileName, int flag = 0);
LONGLONG au_FileGetPos(HANDLE fHandle);
vector<wstring> au_FileGetShortCut(const wstring& fName);
LONGLONG au_FileGetSize(const wstring& fileName);
wstring au_FileGetShortName(const wstring& fName);
vector<string> au_FileGetTime(const wstring& fileName, int option = 0, int format = 0);
wstring au_FileGetVersion(const wstring& fileName, const string& option = $FV_FILEVERSION);
bool FileMove(const wstring& fSource, const wstring& fDest, int flag = $FC_NOOVERWRITE);

void filesTest();