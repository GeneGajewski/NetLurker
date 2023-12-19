/*
 *   Copyright (C) 2023 Gene Gajewski WG5ENE
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//---------------------------------------------------------------------------

#ifndef WinVersionQueryH
#define WinVersionQueryH
#include <System.Classes.hpp>
//---------------------------------------------------------------------------

// a utility class to handle old Windows Version stuff reasonably...

namespace Util {

class WinVersionQuery
{
	private:

	WORD language, codepage;
	DWORD flags;

	char *pBlock;           // parameter block

	static const int maxmodulename = 512;
	wchar_t modulename[maxmodulename]; // name of file module

	String query;

	// translation table entry
	struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
	} *lp;              // will point to an array[??]
	UINT lp_size;       // total size of array in bytes

	const wchar_t *fmt = L"\\StringFileInfo\\%04x%04x\\";
	const DWORD magic_number = 0xFEEF04BD;
	wchar_t *QStr(const String &name);
	bool GetpBlock(void);
	void copy(const wchar_t *mname);
	DWORD GetFlags();

	public:


	WinVersionQuery();
	WinVersionQuery(const wchar_t *module);

	// just avoid copies -> pBlock is alloc'd
	WinVersionQuery(const WinVersionQuery&) = delete;
	WinVersionQuery& operator=(const WinVersionQuery&) = delete;

	~WinVersionQuery();

	bool SetModuleName(const wchar_t *_modulename);
	bool IsValid(void) { return pBlock != NULL; }
	const wchar_t *ModuleName() { return modulename; }
// ==== STRINGS YOU CAN QUERY via GetStr ========================
//
//	 Comments, InternalName, ProductName, CompanyName, LegalCopyright,
//	 ProductVersion, FileDescription, LegalTrademarks, PrivateBuild,
//	 FileVersion, OriginalFilename,	SpecialBuild

	const wchar_t *GetStr(const String &name);    // query string value

	bool SetLCP(unsigned index);    // table entry to use
	unsigned LCPCount();                 // number of table entries
	WORD Language() { return language; }    // current language
	WORD CodePage() { return codepage; }    // current codepage
	const VS_FIXEDFILEINFO *GetFixedInfo();    // get fixed struct
	bool IsPreRelease() { return  flags & VS_FF_PRERELEASE; }
	bool IsPrivateBuild() { return flags & VS_FF_PRIVATEBUILD; }
	bool IsSpecialBuild() { return  flags & VS_FF_SPECIALBUILD; }
	bool IsPatched() { return flags & VS_FF_PATCHED; }
	bool IsInfoInferred() { return flags & VS_FF_INFOINFERRED; }
	bool IsDebug() { return flags & VS_FF_DEBUG; }

};

}//namespace
#endif
