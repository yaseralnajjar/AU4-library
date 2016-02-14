#include "files.h"

HANDLE au_FileOpen(string FName, int FMode){
	switch (FMode){
		case 0: FMode = GENERIC_READ; break;
		case 1: FMode = FILE_APPEND_DATA; break;
		case 2:	FMode = GENERIC_WRITE; break;
		//case 8: FMode = 
		//case 16: FMode = GENERIC_WRITE;		/*$FO_BINARY (16) = Force binary mode (See Remarks).*/
		//case 32: FMode = GENERIC_WRITE;		/*$FO_UNICODE or $FO_UTF16_LE (32) = Use Unicode UTF16 Little Endian reading and writing mode.*/
		//case 64: FMode = GENERIC_WRITE;		/*$FO_UTF16_BE (64) = Use Unicode UTF16 Big Endian reading and writing mode.*/
		//case 128: FMode = GENERIC_WRITE;		/*$FO_UTF8 (128) = Use Unicode UTF8 (with BOM) reading and writing mode.*/
		//case 256: FMode = GENERIC_WRITE;		/*$FO_UTF8_NOBOM (256) = Use Unicode UTF8 (without BOM) reading and writing mode.*/
		//case 512: FMode = GENERIC_WRITE;		/*$FO_ANSI (512) = Use ANSI reading and writing mode.*/
		//case 1024: FMode = GENERIC_WRITE;		/*$FO_UTF16_LE_NOBOM (1024) = Use Unicode UTF16 Little Endian (without BOM) reading and writing mode.*/
		//case 2048: FMode = GENERIC_WRITE;		/*$FO_UTF16_BE_NOBOM (2048) = Use Unicode UTF16 Big Endian (without BOM) reading and writing mode.*/
		//case 16384:	FMode = GENERIC_WRITE;		//$FO_FULLFILE_DETECT (16384) = When opening for reading and no BOM is present, use the entire file to determine if it is UTF8 or UTF16. If this is not used then only the initial part of the file (up to 64KB) is checked for performance reasons.
	}


	HANDLE hFile = CreateFile(FName.c_str(),
								FMode,
								0,
								NULL,
								FMode == 8 ? OPEN_ALWAYS : OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	return hFile;
}

int au_FileClose(HANDLE hFile){
	return CloseHandle(hFile);
}

int au_FileCopy(string fSource, string fDest, int flag){
	//$FC_CREATEPATH (8) = Create destination directory structure if it doesn't exist (See Remarks).
	if (flag == 8){
		const SECURITY_ATTRIBUTES *psa = NULL;
		SHCreateDirectoryEx(NULL, string(fDest.substr(0, fDest.rfind("\\"))).c_str(), psa);
	}
	int value = CopyFile(fSource.c_str(),
				fDest.c_str(),
				flag == 0 ? TRUE : FALSE);			//$FC_NOOVERWRITE (0) = (default) do not overwrite existing files
													//$FC_OVERWRITE (1) = overwrite existing files					
	return value;
}

int au_FileChangeDir(string NewPath){
	int value = _chdir(NewPath.c_str());
	return value == 0 ? TRUE : FALSE;
}

int au_FileCreateNTFSLink(string fSource, string fDest, int flag){
	if (flag == 1){
		DeleteFile(fDest.c_str());
	}
	int value =  CreateHardLink(fDest.c_str(), 
					fSource.c_str(),
					NULL);
	return value == 0 ? FALSE : TRUE;
}

int au_FileCreateShortCut(string fSource, string fDest, string workdir, string args, string desc, string icon, string hotkey, int IcnNum, int state){
	//Credits: Deluge cplusplus.com
	CoInitialize(NULL);
	IShellLink* pShellLink = NULL;
	HRESULT hres;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL,
		IID_IShellLink, (void**)&pShellLink);
	if (SUCCEEDED(hres))
	{
		pShellLink->SetPath(fSource.c_str());
		pShellLink->SetWorkingDirectory(workdir.c_str());
		pShellLink->SetArguments(args.c_str());
		pShellLink->SetDescription(desc.c_str());
		pShellLink->SetIconLocation(icon.c_str(), IcnNum); // to-check
		//pShellLink->SetHotkey(0x0000); // to-do
		pShellLink->SetShowCmd(state);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres))
		{
			wstring fDestW(fDest.length(), L' ');
			copy(fDest.begin(), fDest.end(), fDestW.begin());
			hres = pPersistFile->Save(fDestW.c_str(), TRUE);
			pPersistFile->Release();
		}
		else
		{
			//Error 2
			return 0;
		}
		pShellLink->Release();
	}
	else
	{
		//Error 1
		return 0;
	}
	return 1;
}

int au_FileDelete(string fileName){
	int value = DeleteFile(fileName.c_str());
	return value == 0 ? FALSE : TRUE;
}

int au_FileExists(string fileName){
	return PathFileExists(fileName.c_str());
}

retFileFindStruct au_FileFindFirstFile(string fileName){
	retFileFindStruct retValues;
	WIN32_FIND_DATA FindFileData;
	retValues.hSearch = FindFirstFile(fileName.c_str(), &FindFileData);
	retValues.hFileName = FindFileData.cFileName;
	return retValues;
}

string au_FileFindNextFile(retFileFindStruct &retValues, int flag){
	if (retValues.hFileName != ""){
		string tempString = retValues.hFileName;
		retValues.hFileName = "";
		return tempString;
	}
	WIN32_FIND_DATA FindFileData;
	if (FindNextFile(retValues.hSearch, &FindFileData)){
		return FindFileData.cFileName;
	}
	return "";
}

BOOL au__FindClose(HANDLE hSearch){
	return FindClose(hSearch);
}


void filesTest(){
	//HANDLE hFile = au_FileOpen("Hello1.txt", 2);
	//int a = au_FileClose(hFile);
	//int a = au_FileCopy("Hello.txt", "hi.txt", 1);
	//int a = au_FileChangeDir("z:\\");
	//int a = au_FileCreateNTFSLink("C:\\hi.txt", "C:\\FLink.txt", 1);
	//int a = au_FileCreateShortCut("C:\\Hello.txt", "C:\\test.lnk", "C:\\", "a", "Description test", "C:\\windows\\system32\\shell.dll", "", 0, 0);
	//int a = au_FileExists("C:\\Hello.txt");
	//int a = au_FileDelete("C:\\Hello.txt");
	
	//FileFind:
	/*retFileFindStruct fileSearch = au_FileFindFirstFile("C:\\test\\*.txt");
	while (1)
	{
		string SearchResult = au_FileFindNextFile(fileSearch);
		if (GetLastError() == ERROR_NO_MORE_FILES) break;
		MessageBox(NULL, SearchResult.c_str(), "", MB_ICONINFORMATION);
	}
	FindClose(fileSearch.hSearch);*/
	
}