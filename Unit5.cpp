																						   //---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit5.h"
#include "winversionquery.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource("*.Windows.fmx", _PLAT_MSWINDOWS)

TForm5* Form5;

#define MS_VERSIG 0xFEEF04BD

//---------------------------------------------------------------------------





__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{

	String List[12] = {
		 "Comments", "InternalName", "ProductName", "CompanyName", "LegalCopyright",
	 "ProductVersion", "FileDescription", "LegalTrademarks", "PrivateBuild",
	 "FileVersion", "OriginalFilename",	"SpecialBuild"
	};

	Util::WinVersionQuery *ver = new Util::WinVersionQuery();
	String comment;

	for(int x = 0; x < 12; x++)
		comment = ver->GetStr(List[x]);

	comment = ver->GetStr("Comments");

	if(ver)
		delete ver;

	ver = NULL;

	ver = new Util::WinVersionQuery(L"xcv");

    bool valid = ver->IsValid();

	if(ver)
		delete ver;
}

//-------------------------------------------------------------------/--------

