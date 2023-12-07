//---------------------------------------------------------------------------

#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.Types.hpp>
#include <System.Rtti.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *Grid;
	TStringColumn *scSerialNo;
	TStringColumn *scCallsign;
	TStringColumn *scState;
	TStringColumn *scRemarks;
	TStringColumn *scQSLInfo;
	TStringColumn *scCity;
	TStringColumn *scFirstName;
	TStringColumn *scStatus;
	TStringColumn *scCounty;
	TStringColumn *scGrid;
	TStringColumn *scStreet;
	TStringColumn *scZip;
	TStringColumn *scMemberID;
	TStringColumn *scCountry;
	TStringColumn *scDXCC;
	TStringColumn *scPreferredName;
private:	// User declarations
public:		// User declarations
	__fastcall TForm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
