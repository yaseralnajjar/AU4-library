#pragma once

/*//#INDEX#                                                                                                                        
//Title .........: File_Constants
//AutoIt Version : 3.3.14.1
//Language ......: English
//Description ...: Constants to be included in an AutoIt v3 script when using File functions.
//Author(s) .....: Valik, Gary Frost, ...
//                                                                                                                               */

#define $FC_NOOVERWRITE   0 //Do not overwrite existing files (default)
#define $FC_OVERWRITE   1 //Overwrite existing files
#define $FC_CREATEPATH   8 //Create destination directory structure if it doesn't exist

//Indicates file date and time options
#define $FT_MODIFIED   0 //Date and time file was last modified (default)
#define $FT_CREATED   1 //Date and time file was created
#define $FT_ACCESSED   2 //Date and time file was last accessed

//FileGetTime Constants
#define $FT_ARRAY   0
#define $FT_STRING   1

//FileSelectFolder Constants
#define $FSF_CREATEBUTTON   1
#define $FSF_NEWDIALOG   2
#define $FSF_EDITCONTROL   4

//FileSetTime, FileSetAttrib
#define $FT_NONRECURSIVE   0
#define $FT_RECURSIVE   1

//Indicates the mode to open a file
#define $FO_READ   0 //Read mode
#define $FO_APPEND   1 //Write mode (append)
#define $FO_OVERWRITE   2 //Write mode (erase previous contents)
#define $FO_CREATEPATH   8 //Create directory structure if it doesn't exist
#define $FO_BINARY   16 //Read/Write mode binary
#define $FO_UNICODE   32 //Write mode Unicode UTF16-LE
#define $FO_UTF16_LE   32 //Write mode Unicode UTF16-LE
#define $FO_UTF16_BE   64 //Write mode Unicode UTF16-BE
#define $FO_UTF8   128 //Read/Write mode UTF8 with BOM
#define $FO_UTF8_NOBOM   256 //Read/Write mode UTF8 with no BOM
#define $FO_ANSI   512 //Read/Write mode ANSI
#define $FO_UTF16_LE_NOBOM   1024 //Write mode Unicode UTF16-LE with no BOM
#define $FO_UTF16_BE_NOBOM   2048 //Write mode Unicode UTF16-BE with no BOM
#define $FO_UTF8_FULL   16384 //Use full file UTF8 detection if no BOM present
#define $FO_FULLFILE_DETECT   16384 //Use full file UTF8 detection if no BOM present

//Indicates file read options
#define $EOF   -1 //End-of-file reached

//Indicates file open and save dialog options
#define $FD_FILEMUSTEXIST   1 //File must exist
#define $FD_PATHMUSTEXIST   2 //Path must exist
#define $FD_MULTISELECT   4 //Allow multi-select
#define $FD_PROMPTCREATENEW   8 //Prompt to create new file
#define $FD_PROMPTOVERWRITE   16 //Prompt to overWrite file

#define $CREATE_NEW   1
#define $CREATE_ALWAYS   2
#define $OPEN_EXISTING   3
#define $OPEN_ALWAYS   4
#define $TRUNCATE_EXISTING   5

#define $INVALID_SET_FILE_POINTER   -1

//Indicates starting point for the file pointer move operations
#define $FILE_BEGIN   0
#define $FILE_CURRENT   1
#define $FILE_END   2

#define $FILE_ATTRIBUTE_READONLY   0x00000001
#define $FILE_ATTRIBUTE_HIDDEN   0x00000002
#define $FILE_ATTRIBUTE_SYSTEM   0x00000004
#define $FILE_ATTRIBUTE_DIRECTORY   0x00000010
#define $FILE_ATTRIBUTE_ARCHIVE   0x00000020
#define $FILE_ATTRIBUTE_DEVICE   0x00000040
#define $FILE_ATTRIBUTE_NORMAL   0x00000080
#define $FILE_ATTRIBUTE_TEMPORARY   0x00000100
#define $FILE_ATTRIBUTE_SPARSE_FILE   0x00000200
#define $FILE_ATTRIBUTE_REPARSE_POINT   0x00000400
#define $FILE_ATTRIBUTE_COMPRESSED   0x00000800
#define $FILE_ATTRIBUTE_OFFLINE   0x00001000
#define $FILE_ATTRIBUTE_NOT_CONTENT_INDEXED   0x00002000
#define $FILE_ATTRIBUTE_ENCRYPTED   0x00004000

#define $FILE_SHARE_READ   0x00000001
#define $FILE_SHARE_WRITE   0x00000002
#define $FILE_SHARE_DELETE   0x00000004
#define $FILE_SHARE_READWRITE   ($FILE_SHARE_READ | $FILE_SHARE_WRITE)
#define $FILE_SHARE_ANY   ($FILE_SHARE_READ | $FILE_SHARE_WRITE | $FILE_SHARE_DELETE)

#define $GENERIC_ALL   0x10000000
#define $GENERIC_EXECUTE   0x20000000
#define $GENERIC_WRITE   0x40000000
#define $GENERIC_READ   0x80000000
#define $GENERIC_READWRITE   ($GENERIC_READ | $GENERIC_WRITE)

//FileGetEncoding Constants
#define $FILE_ENCODING_UTF16LE   32

#define $FE_ENTIRE_UTF8   1
#define $FE_PARTIALFIRST_UTF8   2

//FileGetLongName and FileGetShortName
#define $FN_FULLPATH   0
#define $FN_RELATIVEPATH   1

//FileGetVersion Constants _WinAPI_VerQueryValue, _WinAPI_VerQueryValueEx
#define $FV_COMMENTS   "Comments"
#define $FV_COMPANYNAME   "CompanyName"
#define $FV_FILEDESCRIPTION   "FileDescription"
#define $FV_FILEVERSION   "FileVersion"
#define $FV_INTERNALNAME   "InternalName"
#define $FV_LEGALCOPYRIGHT   "LegalCopyright"
#define $FV_LEGALTRADEMARKS   "LegalTrademarks"
#define $FV_ORIGINALFILENAME   "OriginalFilename"
#define $FV_PRODUCTNAME   "ProductName"
#define $FV_PRODUCTVERSION   "ProductVersion"
#define $FV_PRIVATEBUILD   "PrivateBuild"
#define $FV_SPECIALBUILD   "SpecialBuild"

//Indicates _FileReadToArray modes
#define $FRTA_NOCOUNT   0
#define $FRTA_COUNT   1
#define $FRTA_INTARRAYS   2
#define $FRTA_ENTIRESPLIT   4

//Indicates _FileListToArray modes
#define $FLTA_FILESFOLDERS   0
#define $FLTA_FILES   1
#define $FLTA_FOLDERS   2

//Indicates _FileListToArrayRec modes
#define $FLTAR_FILESFOLDERS   0
#define $FLTAR_FILES   1
#define $FLTAR_FOLDERS   2
#define $FLTAR_NOHIDDEN   4
#define $FLTAR_NOSYSTEM   8
#define $FLTAR_NOLINK   16
#define $FLTAR_NORECUR   0
#define $FLTAR_RECUR   1
#define $FLTAR_NOSORT   0
#define $FLTAR_SORT   1
#define $FLTAR_FASTSORT   2
#define $FLTAR_NOPATH   0
#define $FLTAR_RELPATH   1
#define $FLTAR_FULLPATH   2
//                                                                                                                               
