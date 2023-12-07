//---------------------------------------------------------------------------

#ifndef WinVersionQueryH
#define WinVersionQueryH
#include <System.Classes.hpp>
//---------------------------------------------------------------------------

// a utility class to handle old Windows Version stuff, which appears to have
// been written by assembly programmers on crack....

namespace Util {


class WinVersionQuery
{
	private:

	WORD language, codepage;

	char *pBlock;           // parameter block

	static const int maxmodulename = 512;
	wchar_t modulename[maxmodulename]; // name of file module

	String query;

	// translation table entry
	struct LANGANDCODEPAGE {
	WORD wLanguage;
	WORD wCodePage;
	} *lp;              // will point to an array[?]
	UINT lp_size;       // total size of array in bytes


	const wchar_t *fmt = L"\\StringFileInfo\\%04x%04x\\";
	const DWORD magic_number = 0xFEEF04BD;
	wchar_t *QStr(const String &member);
	bool GetpBlock(void);
    void copy(const wchar_t *mname);

	public:

// ==== VERSION STRINGS YOU CAN QUERY ========================
//
//	 Comments, InternalName, ProductName, CompanyName, LegalCopyright,
//	 ProductVersion, FileDescription, LegalTrademarks, PrivateBuild,
//	 FileVersion, OriginalFilename,	SpecialBuild

	WinVersionQuery();
	WinVersionQuery(const wchar_t *module);
	~WinVersionQuery();
	bool SetModuleName(const wchar_t *_modulename);
	bool IsValid(void) { return pBlock != NULL; }
	const wchar_t *ModuleName() { return modulename; }
	const wchar_t *GetStr(const String &name);    // query string value
	bool SetLCP(unsigned index);    // table entry to use
	unsigned LCPCount();                 // number of table entries
	WORD Language() { return language; }    // current language
	WORD CodePage() { return codepage; }    // current codepage
	const VS_FIXEDFILEINFO *FixedInfo();    // get fixed struct

};

}//namespace
#endif
