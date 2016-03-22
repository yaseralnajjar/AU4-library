#include "files.h"


/*wstring Utf8ToUtf16(const string& str)
{
	//credits: Mr.Exodia
	wstring convertedString;
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
	if(requiredSize > 0)
	{
		vector<wchar_t> buffer(requiredSize);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}
	return convertedString;
}

wstring Utf8ToUtf16(const char* str)
{
	return Utf8ToUtf16(str ? string(str) : string());
}*/

string ToAscii(const std::wstring& input){
	//credits: atom0s
    std::string str(input.begin(), input.end());
    return str;
}

wstring ToUnicode(const std::string& input){
    std::wstring str(input.begin(), input.end());
    return str;
}

HANDLE au_FileOpen(const wstring& fileName, int FMode){
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
		default: FMode = GENERIC_READ;
	}


	HANDLE hFile = CreateFileW(fileName.c_str(),
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

int au_FileCopy(const wstring& fSource, const wstring& fDest, int flag){
	//$FC_CREATEPATH (8) = Create destination directory structure if it doesn't exist (See Remarks).
	if (flag == 8){
		const SECURITY_ATTRIBUTES *psa = NULL;
		SHCreateDirectoryExW(NULL, wstring(fDest.substr(0, fDest.rfind(L"\\"))).c_str(), psa);
	}
	int value = CopyFileW(fSource.c_str(),
				fDest.c_str(),
				flag == 0 ? TRUE : FALSE);			//$FC_NOOVERWRITE (0) = (default) do not overwrite existing files
													//$FC_OVERWRITE (1) = overwrite existing files					
	return value;
}

bool au_FileChangeDir(wstring NewPath){
	int value = _wchdir(NewPath.c_str());
	return value == 0 ? TRUE : FALSE;
}

bool au_FileCreateNTFSLink(wstring fSource, wstring fDest, int flag){
	if (flag == 1){
		DeleteFileW(fDest.c_str());
	}
	int value = CreateHardLinkW(fDest.c_str(), fSource.c_str(), NULL);
	return value == 0 ? FALSE : TRUE;
}

bool au_FileCreateShortCut(wstring fSource, wstring fDest, wstring workdir, wstring args, wstring desc, wstring icon, wstring hotkey, int IcnNum, int state){
	//Credits: Deluge, cplusplus.com
	CoInitialize(NULL);
	IShellLinkW* pShellLink = NULL;
	HRESULT hres;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);
	if (SUCCEEDED(hres)){
		pShellLink->SetPath(fSource.c_str());
		pShellLink->SetWorkingDirectory(workdir.c_str());
		pShellLink->SetArguments(args.c_str());
		pShellLink->SetDescription(desc.c_str());
		pShellLink->SetIconLocation(icon.c_str(), IcnNum); // to-check
		//pShellLink->SetHotkey(0x0000); // to-do
		pShellLink->SetShowCmd(state);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres)){
			hres = pPersistFile->Save(fDest.c_str(), TRUE);
			pPersistFile->Release();
		}
		else{
			//Error 2
			return 0;
		}
		pShellLink->Release();
	}
	else{
		//Error 1
		return 0;
	}
	return 1;
}

bool au_FileDelete(wstring fileName){
	int value = 0;
	wstring getLastPart = fileName.substr(fileName.rfind(L"\\")+1, fileName.length());
	if (!PathIsDirectoryW(fileName.c_str())){ //Check if fileName is a folder.
		if ((getLastPart.find(L"*") == -1) && (getLastPart.find(L"?") == -1)){ //Parse last part of path to get wildcards if exists.
			value = DeleteFileW(fileName.c_str());
			return value == 0 ? FALSE : TRUE;
		}
	}
	else{
		fileName.append(L"\\*.*");
	}
	//delete using wildcards
	wstring getPath = fileName.substr(0, fileName.rfind(L"\\")+1);
	retFileFindStruct fileSearch = au_FileFindFirstFile(fileName);
	while (1){
		wstring SearchResult = au_FileFindNextFile(fileSearch);
		if (GetLastError() == ERROR_NO_MORE_FILES || SearchResult == L"") break;
		wstring lastPath = getPath + SearchResult;
		if (!PathIsDirectoryW(lastPath.c_str())){
			if (!DeleteFileW(lastPath.c_str())) return FALSE;
		}
	}
	FindClose(fileSearch.hSearch);
	return TRUE;
}

bool au_FileExists(const wstring& fileName){
	int returnValue = PathFileExistsW(fileName.c_str());
	return returnValue == 0 ? FALSE : TRUE;
}

retFileFindStruct au_FileFindFirstFile(const wstring& fileName){
	retFileFindStruct retValues;
	WIN32_FIND_DATAW FindFileData;
	retValues.hSearch = FindFirstFileW(fileName.c_str(), &FindFileData);
	retValues.hFileName = FindFileData.cFileName;
	if (retValues.hFileName == L"."){ // if fileName=="*.*"
		retValues.hFileName = L"";
		au_FileFindNextFile(retValues);
	}
	return retValues;
}

wstring au_FileFindNextFile(retFileFindStruct& retValues, int flag){
	if (retValues.hFileName != L""){
		wstring tempString = retValues.hFileName;
		retValues.hFileName = L"";
		return tempString;
	}
	WIN32_FIND_DATAW FindFileData;
	if (FindNextFileW(retValues.hSearch, &FindFileData)){
		return FindFileData.cFileName;
	}

	return L"";
}

bool au__FindClose(HANDLE hSearch){
	int returnValue = FindClose(hSearch);
	return returnValue == 0 ? FALSE : TRUE;
}

int au_FileFlush(HANDLE fHandle){
	return FlushFileBuffers(fHandle); 
}

string au_FileGetAttrib(const wstring& fileName){
	int fileAttributes = GetFileAttributesW(fileName.c_str());
	string returnValue = "";
	if (fileAttributes & FILE_ATTRIBUTE_READONLY) returnValue.append("R"); // "R" = READONLY
	if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE) returnValue.append("A"); // "A" = ARCHIVE
	if (fileAttributes & FILE_ATTRIBUTE_SYSTEM) returnValue.append("S"); // "S" = SYSTEM
	if (fileAttributes & FILE_ATTRIBUTE_HIDDEN) returnValue.append("H"); // "H" = HIDDEN
	if (fileAttributes & FILE_ATTRIBUTE_NORMAL) returnValue.append("N"); // "N" = NORMAL
	if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) returnValue.append("D"); // "D" = DIRECTORY
	if (fileAttributes & FILE_ATTRIBUTE_OFFLINE) returnValue.append("O"); // "O" = OFFLINE
	if (fileAttributes & FILE_ATTRIBUTE_COMPRESSED) returnValue.append("C"); // "C" = COMPRESSED (NTFS compression, not ZIP compression)
	if (fileAttributes & FILE_ATTRIBUTE_TEMPORARY) returnValue.append("T"); // "T" = TEMPORARY
	if (fileAttributes & FILE_ATTRIBUTE_ENCRYPTED) returnValue.append("X"); //"X" = EFS ENCRYPTION
	return returnValue;
}

int au_FileGetEncoding(wstring fileName, int flag){ 
	
	return 0;
}

wstring au_FileGetLongName(const wstring& fileName, int flag){ 
	//updated by Mr.Exodia
	auto bufferLength = GetLongPathNameW(fileName.c_str(), nullptr, 0);
	std::vector<wchar_t> longPath(bufferLength);
	GetLongPathNameW(fileName.c_str(), longPath.data(), bufferLength);

	return wstring(longPath.data());

	/*wchar_t retVal[MAX_PATH] = L"";
	GetLongPathNameW(fileName.c_str(), retVal, MAX_PATH);
	wstring returnValue(retVal);
	return returnValue;*/
}

LONGLONG au_FileGetPos(HANDLE fHandle){
	LARGE_INTEGER fSize;
	if (GetFileSizeEx(fHandle, &fSize)){
		//credits: NULL, cplusplus.com
		if (fSize.QuadPart > 4194304){
			LARGE_INTEGER ret;

			LARGE_INTEGER pos;
			pos.QuadPart=0;

			SetFilePointerEx(fHandle,pos,&ret,FILE_CURRENT);

			return ret.QuadPart;
		} 
		else if (fSize.QuadPart > 0){
			return SetFilePointer(fHandle,0,0,FILE_CURRENT);
		}
		return 0; //failed
	}
	else{
		return 0;
	}
}

LONGLONG au_FileGetSize(const wstring& fileName){
	HANDLE fHandle = au_FileOpen(fileName, 0);
	LARGE_INTEGER returnValue;
	if (GetFileSizeEx(fHandle, &returnValue)){
		return returnValue.QuadPart;
	}
	else{
		return 0;
	}
}

vector<wstring> au_FileGetShortCut(const wstring& fName){
	vector<wstring> returnValue;
	CoInitialize(NULL);
	IShellLinkW* pShellLink = NULL;
	HRESULT hres;
	WIN32_FIND_DATA wfd;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);
	if (SUCCEEDED(hres)){
		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres)){
			hres = pPersistFile->Load(fName.c_str(), STGM_READ);
			if (SUCCEEDED(hres)){
				//hres = pShellLink->Resolve(hwnd, 0);
				if (SUCCEEDED(hres)){
					vector<wchar_t> containVal(MAX_PATH);
					hres = pShellLink->GetPath(containVal.data(), MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH);
					if (SUCCEEDED(hres)) { //works once = works for others
						returnValue.push_back(containVal.data());
						containVal.clear();
						pShellLink->GetWorkingDirectory(containVal.data(), MAX_PATH); //get working dir
						returnValue.push_back(containVal.data());
						containVal.clear();
						pShellLink->GetArguments(containVal.data(), INFOTIPSIZE); //get args
						returnValue.push_back(containVal.data());
						containVal.clear();
						pShellLink->GetDescription(containVal.data(), INFOTIPSIZE); //get description
						returnValue.push_back(containVal.data());
						containVal.clear();
						int iconID = 0;
						pShellLink->GetIconLocation(containVal.data(), MAX_PATH, &iconID); //get icon file name & ID
						returnValue.push_back(containVal.data());
						returnValue.push_back(to_wstring(iconID));
						containVal.clear();
						//pShellLink->GetShowCmd(containVal.data(), MAX_PATH); //get shortcut state to-do
						returnValue.push_back(containVal.data());
					}
				}
			}
			pPersistFile->Release();
		}
		pShellLink->Release();
	}

	return returnValue;
}

void filesTest(){
	//HANDLE hFile = au_FileOpen(L"Hello1.txt", 2);
	//int a = au_FileClose(hFile);
	//int a = au_FileCopy(L"Hello.txt", L"hi.txt", 1);
	//int a = au_FileChangeDir(L"z:\\");
	//int a = au_FileCreateNTFSLink(L"C:\\hi.txt", L"C:\\FLink.txt", 1);
	//int a = au_FileCreateShortCut(L"C:\\Hello.txt", L"C:\\test.lnk", L"C:\\", L"a", L"Description test", L"C:\\windows\\system32\\shell.dll", L"", 0, 0);
	//int a = au_FileExists(L"C:\\test\\Hello.txt");
	//int a = au_FileDelete(L"C:\\test\\hello.txt");
	//int a = au_FileDelete(L"C:\\test");
	//int a = au_FileDelete(L"C:\\test\\go.tx?");
	
	//FileFind:
	/*retFileFindStruct fileSearch = au_FileFindFirstFile(L"C:\\test\\*.t*");
	while (1)
	{
	wstring SearchResult = au_FileFindNextFile(fileSearch);
	if (GetLastError() == ERROR_NO_MORE_FILES || SearchResult == L"") break;
	}
	FindClose(fileSearch.hSearch);*/
	
	//string attribTest = au_FileGetAttrib(L"C:\\test\\go.txt");
	//wstring GetLongPath = au_FileGetLongName(L"C:\\PROGRA~1");
	//int fSize = au_FileGetSize(L"C:\\test\\hell2o.txt");
	vector<wstring> fLink = au_FileGetShortCut(L"C:\\test\\tlink.lnk");
}