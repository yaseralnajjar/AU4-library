#include "files.h"

string Utf16ToUtf8(const wstring & wstr)
{
	//credits: Mr.Exodia, original from : http://www.nubaria.com/en/blog/?p=289
	string convertedString;
	int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
	if (requiredSize > 0)
	{
		std::vector<char> buffer(requiredSize);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
		convertedString.assign(buffer.begin(), buffer.end() - 1);
	}
	return convertedString;
}

string Utf16ToUtf8(const wchar_t* wstr)
{
	//credits: Mr.Exodia, original from : http://www.nubaria.com/en/blog/?p=289
	return Utf16ToUtf8(wstr ? wstring(wstr) : wstring());
}

wstring Utf8ToUtf16(const string& str)
{
	//credits: Mr.Exodia, original from : http://www.nubaria.com/en/blog/?p=289
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
	//credits: Mr.Exodia, original from : http://www.nubaria.com/en/blog/?p=289
	return Utf8ToUtf16(str ? string(str) : string());
}

//string ToAscii(const std::wstring& input) {
//	//credits: atom0s
//    std::string str(input.begin(), input.end());
//    return str;
//}
//wstring ToUnicode(const std::string& input) {
//    std::wstring str(input.begin(), input.end());
//    return str;
//}

bool au_DirCreate(const wstring& dirName) {
	if (SHCreateDirectoryExW(NULL, dirName.c_str(), NULL)) {
		return TRUE;
	}

	return FALSE; // failed

	//if (CreateDirectoryW(dirName.c_str(), NULL)) {
	//	return TRUE;
	//}
	//else { // maybe we need to create directories recursively
	//	wstring dirNamePre = dirName;
	//	if (dirNamePre.at(dirNamePre.length() - 1) == L'\\') 
	//		dirNamePre = dirNamePre.substr(0, dirNamePre.length() - 1); // remove last slash '\'
	//	dirNamePre = dirNamePre.substr(0, dirNamePre.rfind('\\'));
	//	if (!PathFileExistsW(dirNamePre.c_str())) {
	//		size_t limit = count(dirNamePre.begin(), dirNamePre.end(), '\\');
	//		unsigned foldersNeeded = 2;
	//		for (foldersNeeded; foldersNeeded <= limit; foldersNeeded++) { // counting needed folders
	//			dirNamePre = dirNamePre.substr(0, dirNamePre.rfind('\\'));
	//			if (PathFileExistsW(dirNamePre.c_str())) break;
	//		}
	//		wstring tempString = dirName;
	//		tempString = tempString.substr(dirNamePre.length());
	//		for (unsigned s = 0; s < foldersNeeded; s++) { // creating folders
	//			dirNamePre = dirNamePre + tempString.substr(0, tempString.find(L'\\', 1));
	//			if (!CreateDirectoryW(dirNamePre.c_str(), NULL)) {
	//				return FALSE; // one failed ==> all failed
	//			}
	//			tempString = tempString.substr(tempString.find(L'\\', 1));
	//		}

	//		return TRUE; // it worked !
	//	}
	//}

	//return FALSE; // failed
}

HANDLE au_FileOpen(const wstring& fileName, int fileMode) {
	switch (fileMode) {
		case $FO_READ: fileMode = GENERIC_READ; break;
		case $FO_APPEND: fileMode = FILE_APPEND_DATA; break;
		case $FO_OVERWRITE:	fileMode = GENERIC_WRITE; break;
		//case $FO_CREATEPATH : fileMode = 
		//case $FO_BINARY : fileMode = GENERIC_WRITE;		/*$FO_BINARY (16) = Force binary mode (See Remarks).*/
		//case $FO_UNICODE: fileMode = GENERIC_WRITE;		/*$FO_UNICODE or $FO_UTF16_LE (32) = Use Unicode UTF16 Little Endian reading and writing mode.*/
		//case $FO_UTF16_BE: fileMode = GENERIC_WRITE;		/*$FO_UTF16_BE (64) = Use Unicode UTF16 Big Endian reading and writing mode.*/
		//case $FO_UTF8: fileMode = GENERIC_WRITE;		/*$FO_UTF8 (128) = Use Unicode UTF8 (with BOM) reading and writing mode.*/
		//case $FO_UTF8_NOBOM: fileMode = GENERIC_WRITE;		/*$FO_UTF8_NOBOM (256) = Use Unicode UTF8 (without BOM) reading and writing mode.*/
		//case $FO_ANSI: fileMode = GENERIC_WRITE;		/*$FO_ANSI (512) = Use ANSI reading and writing mode.*/
		//case $FO_UTF16_LE_NOBOM: fileMode = GENERIC_WRITE;		/*$FO_UTF16_LE_NOBOM (1024) = Use Unicode UTF16 Little Endian (without BOM) reading and writing mode.*/
		//case $FO_UTF16_BE_NOBOM: fileMode = GENERIC_WRITE;		/*$FO_UTF16_BE_NOBOM (2048) = Use Unicode UTF16 Big Endian (without BOM) reading and writing mode.*/
		//case $FO_FULLFILE_DETECT :	fileMode = GENERIC_WRITE;		//$FO_FULLFILE_DETECT (16384) = When opening for reading and no BOM is present, use the entire file to determine if it is UTF8 or UTF16. If this is not used then only the initial part of the file (up to 64KB) is checked for performance reasons.
		default: fileMode = GENERIC_READ;
	}


	HANDLE hFile = CreateFileW(fileName.c_str(),
								fileMode,
								0,
								NULL,
								fileMode == 8 ? OPEN_ALWAYS : OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	return hFile;
}

int au_FileClose(HANDLE hFile) {
	return CloseHandle(hFile);
}

int au_FileCopy(const wstring& fSource, const wstring& fDest, int flag){
	//$FC_CREATEPATH (8) = Create destination directory structure if it doesn't exist (See Remarks).
	if (flag == $FC_CREATEPATH) {
		const SECURITY_ATTRIBUTES *psa = NULL;
		SHCreateDirectoryExW(NULL, wstring(fDest.substr(0, fDest.rfind(L"\\"))).c_str(), psa);
	}
	int value = CopyFileW(fSource.c_str(),
				fDest.c_str(),
				flag == $FC_NOOVERWRITE ? TRUE : FALSE);	//$FC_NOOVERWRITE (0) = (default) do not overwrite existing files
														//$FC_OVERWRITE (1) = overwrite existing files					
	return value;
}

bool au_FileChangeDir(wstring NewPath) {
	int value = _wchdir(NewPath.c_str());

	return value == 0 ? TRUE : FALSE; // 0 = failed
}

bool au_FileCreateNTFSLink(wstring fSource, wstring fDest, int flag) {
	if (flag == $FC_OVERWRITE) {
		DeleteFileW(fDest.c_str());
	}
	int value = CreateHardLinkW(fDest.c_str(), fSource.c_str(), NULL);

	return value == 0 ? FALSE : TRUE; // 0 = failed
}

bool au_FileCreateShortCut(wstring fSource, wstring fDest, wstring workdir, wstring args, wstring desc, wstring icon, wstring hotkey, int IcnNum, int state){
	//Credits: Deluge, cplusplus.com
	CoInitialize(NULL);
	IShellLinkW* pShellLink = NULL;
	HRESULT hres;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);
	if (SUCCEEDED(hres)) {
		pShellLink->SetPath(fSource.c_str());
		pShellLink->SetWorkingDirectory(workdir.c_str());
		pShellLink->SetArguments(args.c_str());
		pShellLink->SetDescription(desc.c_str());
		pShellLink->SetIconLocation(icon.c_str(), IcnNum); // to-check
		//pShellLink->SetHotkey(0x0000); // to-do
		pShellLink->SetShowCmd(state);

		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres)) {
			hres = pPersistFile->Save(fDest.c_str(), TRUE);
			pPersistFile->Release();
		}
		else {
			//Error 2
			return 0;
		}
		pShellLink->Release();
	}
	else {
		//Error 1
		return 0;
	}

	return 1;
}

bool au_FileDelete(wstring fileName) {
	int value = 0;
	wstring getLastPart = fileName.substr(fileName.rfind(L"\\")+1, fileName.length());
	if (!PathIsDirectoryW(fileName.c_str())){ // Check if fileName is a folder.
		if ((getLastPart.find(L"*") == -1) && (getLastPart.find(L"?") == -1)){ // Parse last part of path to get wildcards if exists.
			value = DeleteFileW(fileName.c_str());
			return value == 0 ? FALSE : TRUE; // 0 = failed
		}
	}
	else{
		fileName.append(L"\\*.*");
	}
	//delete using wildcards
	wstring getPath = fileName.substr(0, fileName.rfind(L"\\")+1);
	retFileFindStruct fileSearch = au_FileFindFirstFile(fileName);
	while (1) {
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

bool au_FileExists(const wstring& fileName) {
	int returnValue = PathFileExistsW(fileName.c_str());

	return returnValue == 0 ? FALSE : TRUE; // 0 = failed
}

retFileFindStruct au_FileFindFirstFile(const wstring& fileName) {
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

wstring au_FileFindNextFile(retFileFindStruct& retValues, int flag) {
	if (retValues.hFileName != L"") {
		wstring tempString = retValues.hFileName;
		retValues.hFileName = L"";
		return tempString;
	}
	WIN32_FIND_DATAW FindFileData;
	if (FindNextFileW(retValues.hSearch, &FindFileData)) {
		return FindFileData.cFileName;
	}

	return L"";
}

bool au__FindClose(HANDLE hSearch) {
	int returnValue = FindClose(hSearch);

	return returnValue == 0 ? FALSE : TRUE;
}

int au_FileFlush(HANDLE fHandle) {
	return FlushFileBuffers(fHandle);
}

string au_FileGetAttrib(const wstring& fileName) {
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

wstring au_FileGetLongName(const wstring& fileName, int flag) { 
	//updated by Mr.Exodia
	auto bufferLength = GetLongPathNameW(fileName.c_str(), nullptr, 0);
	vector<wchar_t> longPath(bufferLength);
	if (GetLongPathNameW(fileName.c_str(), longPath.data(), bufferLength)) {
		return wstring(longPath.data());
	}

	return 0; // failed

	/*wchar_t retVal[MAX_PATH] = L"";
	GetLongPathNameW(fileName.c_str(), retVal, MAX_PATH);
	wstring returnValue(retVal);
	return returnValue;*/
}

LONGLONG au_FileGetPos(HANDLE fHandle) {
	LARGE_INTEGER fSize;
	if (GetFileSizeEx(fHandle, &fSize)) {
		//credits: NULL, cplusplus.com
		if (fSize.QuadPart > 4194304) {
			LARGE_INTEGER ret;

			LARGE_INTEGER pos;
			pos.QuadPart=0;

			SetFilePointerEx(fHandle,pos,&ret,FILE_CURRENT);

			return ret.QuadPart;
		} 
		else if (fSize.QuadPart > 0) {
			return SetFilePointer(fHandle,0,0,FILE_CURRENT);
		}
		return 0; //failed
	}
	else {
		return 0;
	}
}

LONGLONG au_FileGetSize(const wstring& fileName) {
	HANDLE fHandle = au_FileOpen(fileName, 0);
	if (fHandle) {
		LARGE_INTEGER returnValue;
		if (GetFileSizeEx(fHandle, &returnValue)) {
			return returnValue.QuadPart;
		}
		au_FileClose(fHandle);
	}
	
	return 0; // failed
}

vector<wstring> au_FileGetShortCut(const wstring& fileName) {
	vector<wstring> returnValue;
	CoInitialize(NULL);
	IShellLinkW* pShellLink = NULL;
	HRESULT hres;
	WIN32_FIND_DATA wfd;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_ALL, IID_IShellLink, (void**)&pShellLink);
	if (SUCCEEDED(hres)) {
		IPersistFile *pPersistFile;
		hres = pShellLink->QueryInterface(IID_IPersistFile, (void**)&pPersistFile);

		if (SUCCEEDED(hres)) {
			hres = pPersistFile->Load(fileName.c_str(), STGM_READ);
			if (SUCCEEDED(hres)) {
				//hres = pShellLink->Resolve(hwnd, 0);
				if (SUCCEEDED(hres)) {
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

wstring au_FileGetShortName(const wstring& fileName) {
	auto bufferLength = GetShortPathNameW(fileName.c_str(), nullptr, 0);
	vector<wchar_t> shortPath(bufferLength);
	if (GetShortPathNameW(fileName.c_str(), shortPath.data(), bufferLength)){
		return shortPath.data();
	}

	return 0;
}

vector<string> au_FileGetTime(const wstring& fileName, int option, int format) {
	vector<string> ReturnFileTime;
	HANDLE hFile = au_FileOpen(fileName, $FO_READ);
	if (hFile) {
		FILETIME fTimeStruct;
		int SuccessFileTime;
		switch (option) {
		case $FT_MODIFIED:
			SuccessFileTime = GetFileTime(hFile, NULL, NULL, &fTimeStruct); // $FT_MODIFIED(0) = Last modified(default)
			break;
		case $FT_CREATED:
			SuccessFileTime = GetFileTime(hFile, &fTimeStruct, NULL, NULL); // $FT_CREATED(1) = Created
			break;
		case $FT_ACCESSED:
			SuccessFileTime = GetFileTime(hFile, NULL, &fTimeStruct, NULL); // $FT_ACCESSED(2) = Last accessed
			break;
		}

		if (SuccessFileTime != 0) {
			SYSTEMTIME fSystemTimeStruct;
			BOOL SuccessSystemTime = FileTimeToSystemTime(&fTimeStruct, &fSystemTimeStruct);
			if (SuccessSystemTime){
				if (format == $FT_ARRAY) { // $FT_ARRAY (0) = return an array (default)
					ReturnFileTime.push_back(to_string(fSystemTimeStruct.wYear));
					ReturnFileTime.push_back(to_string(fSystemTimeStruct.wMonth));
					ReturnFileTime.push_back(to_string(fSystemTimeStruct.wDay));
					ReturnFileTime.push_back(to_string(fSystemTimeStruct.wHour));
					ReturnFileTime.push_back(to_string(fSystemTimeStruct.wMinute));
					ReturnFileTime.push_back(to_string(fSystemTimeStruct.wSecond));
				}
				else if (format == $FT_STRING) { // $FT_STRING (1) = return a string YYYYMMDDHHMMSS
					char x[15] = { 0 };
					//"%4d %02d %02d %02d %02d %02d"
					sprintf_s(x, "%4d%02d%02d%02d%02d%02d",	
						fSystemTimeStruct.wYear,
						fSystemTimeStruct.wMonth,
						fSystemTimeStruct.wDay,
						fSystemTimeStruct.wHour,
						fSystemTimeStruct.wMinute,
						fSystemTimeStruct.wSecond
						);

					ReturnFileTime.push_back(x);

				}
			}
		}
		au_FileClose(hFile);
	}

	return ReturnFileTime; // failed
}

wstring au_FileGetVersion(const wstring& fileName, const string& option) {
	DWORD dwHandle, bufferSize = GetFileVersionInfoSizeW(fileName.c_str(), &dwHandle);
	if (bufferSize) {
		vector<wchar_t> buffer(bufferSize);
		if (GetFileVersionInfoW(fileName.c_str(), dwHandle, bufferSize, buffer.data())) {
			WORD* langInfo = NULL;
			UINT cbLang;
			int success = VerQueryValueW(buffer.data(), L"\\VarFileInfo\\Translation", (LPVOID *)&langInfo, &cbLang);

			if (success) {
				LPVOID stringBuffer = NULL;
				bufferSize = 0;
				wchar_t anotherOpt[128];
				StringCbPrintfW(anotherOpt, 128, L"\\StringFileInfo\\%04x%04x\\%s", 
								langInfo[0], langInfo[1], &wstring(Utf8ToUtf16(option))[0] );
				success = VerQueryValueW(buffer.data(), anotherOpt, &stringBuffer, (UINT *)&bufferSize);
				if (success) {
					return wstring((LPTSTR) stringBuffer);
				}
				else {
					return wstring(L""); // Nothing there
				}
			}
		}
	}

	return L"0.0.0.0"; // failed
}

bool FileMove(const wstring& fSource, const wstring& fDest, int flag) {

	return FALSE; // failed
}

void filesTest() {
	int a = au_DirCreate(L"C:\\test\\dir1\\dir2\\dir3\\");

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
	//vector<wstring> fLink = au_FileGetShortCut(L"C:\\test\\tlink.lnk");
	//wstring fShortName = au_FileGetShortName(L"C:\\test\\New folder\\go.txt");
	//vector<string> fTime = au_FileGetTime(L"C:\\test\\go.txt", 2, 1);
	//wstring fVersion = au_FileGetVersion(L"C:\\test\\VirtualBox.exe", $FV_PRODUCTNAME);

}