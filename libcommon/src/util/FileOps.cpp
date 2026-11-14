////////////////////////////////////////////////////////////////////////////////
//	Copyright 2015 by Joseph Forgione
//	This file is part of VCC (Virtual Color Computer).
//	
//	VCC (Virtual Color Computer) is free software: you can redistribute itand/or
//	modify it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or (at your
//	option) any later version.
//	
//	VCC (Virtual Color Computer) is distributed in the hope that it will be
//	useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
//	Public License for more details.
//	
//	You should have received a copy of the GNU General Public License along with
//	VCC (Virtual Color Computer). If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////

// TODO: FileOps should be depreciated and functions moved to fileutil.cpp

#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <filesystem>
#include <string>
#include <vcc/util/FileOps.h>

void ValidatePath(char *Path)
{
	char FullExePath[MAX_PATH]="";
	char TempPath[MAX_PATH]="";

	GetModuleFileName(nullptr,FullExePath,MAX_PATH);
	PathRemoveFileSpec(FullExePath);	//Get path to executable
	strcpy(TempPath,Path);			
	PathRemoveFileSpec(TempPath);		//Get path to Incomming file
	if (!strcmp(TempPath,FullExePath))	// If they match remove the Path
		PathStripPath(Path);
	return;
}

int CheckPath(char *Path)	// Return 1 on error
{
	if (!Path || (strlen(Path) == 0))
		return 1;

	// Relative application resources are resolved only beside VCC.exe.
	// Never accept a same-named file from the process working directory.
	std::filesystem::path resolved(Path);
	if (!resolved.is_absolute()) {
		char exePath[MAX_PATH] = {};
		const DWORD length = GetModuleFileNameA(nullptr, exePath, MAX_PATH);
		if (length == 0 || length >= MAX_PATH)
			return 1;
		resolved = std::filesystem::path(exePath).parent_path() / resolved;
	}
	resolved = resolved.lexically_normal();

	const std::string fullPath = resolved.string();
	if (fullPath.size() >= MAX_PATH)
		return 1;

	HANDLE file = CreateFileA(fullPath.c_str(), 0, FILE_SHARE_READ, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (file == INVALID_HANDLE_VALUE)
		return 1;

	CloseHandle(file);
	strcpy_s(Path, MAX_PATH, fullPath.c_str());
	return 0;
}

// These are here to remove dependance on shlwapi.dll. ASCII only
void PathStripPath ( char *TextBuffer)
{
	char TempBuffer[MAX_PATH] = "";
	short Index = (short)strlen(TextBuffer);

	if ((Index > MAX_PATH) || (Index==0))	//Test for overflow
		return;

	for (; Index >= 0; Index--)
		if (TextBuffer[Index] == '\\' || TextBuffer[Index] == '/')
			break;
	
	if (Index < 0)	//delimiter not found
		return;
	strcpy(TempBuffer, &TextBuffer[Index + 1]);
	strcpy(TextBuffer, TempBuffer);
}

BOOL PathRemoveFileSpec(char *Path)
{
	size_t Index=strlen(Path),Lenth=Index;
	if ( (Index==0) || (Index > MAX_PATH))
		return false;
	
	while ( (Index>0) && (Path[Index] != '\\') )
		Index--;
	while ( (Index>0) && (Path[Index] == '\\') )
		Index--;
	if (Index==0)
		return false;
	Path[Index+2]=0;
	return( !(strlen(Path) == Lenth));
}		

BOOL PathRemoveExtension(char *Path)
{
	size_t Index=strlen(Path),Lenth=Index;
	if ( (Index==0) || (Index > MAX_PATH))
		return false;
	
	while ( (Index>0) && (Path[Index--] != '.') );
	Path[Index+1]=0;
	return( !(strlen(Path) == Lenth));
}

char* PathFindExtension(char *Path)
{
	size_t Index=strlen(Path);
	if ( (Index==0) || (Index > MAX_PATH))
		return(&Path[strlen(Path)+1]);
	while ( (Index>0) && (Path[Index--] != '.') );
	return(&Path[Index+1]);
}

BOOL FilePrintf(HANDLE hFile, const char* fmt, ...)
{
	DWORD dummy;
	va_list args;
	char msg[512];
	va_start(args, fmt);
	vsnprintf(msg, 512, fmt, args);
	va_end(args);
	return WriteFile(hFile,msg,strlen(msg),&dummy,nullptr);
}

