//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.Header.hpp>
#include <System.Rtti.hpp>
#include <FMX.Edit.hpp>
#include <FMX.EditBox.hpp>
#include <FMX.SpinBox.hpp>
#include <FMX.WebBrowser.hpp>
#include <FMX.Controls3D.hpp>
#include <FMX.Objects3D.hpp>
#include <System.Math.Vectors.hpp>
#include <FMX.MaterialSources.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Menus.hpp>
#include <Web.HTTPApp.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Colors.hpp>
#include<map>
#include "unit2.h"


typedef std::map<String, int> col_map;

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
  __published: // IDE-managed Components
	TStatusBar *StatusBar1;
	TMainMenu *MainMenu1;
	TMenuItem *mQuit;
	TLayout *Layout1;
	TComboBox *ComboBox1;
	TCircle *Circle1;
	TTimer *Timer1;
	TLayout *Layout2;
	TStringGrid *Grid;
	TStringColumn *scSerialNo;
	TStringColumn *scCallsign;
	TStringColumn *scState;
	TStringColumn *scQSL;
	TStringColumn *scCity;
	TStringColumn *scName;
	TStringColumn *scStatus;
	TStringColumn *scCounty;
	TStringColumn *scGrid;
	TStringColumn *scStreet;
	TStringColumn *scZip;
	TStringColumn *scMemberID;
	TStringColumn *scCountry;
	TStringColumn *scDXCC;
	TStringColumn *scPreferred;
	TStringColumn *scRemarks;
	TLabel *Label1;
	TMenuItem *mnFields;
	TMenuItem *mnQuit;
	TMenuItem *MenuItem2;
	TMenuItem *MenuItem3;
	TMenuItem *MenuItem4;
	TEdit *edFreq;
	TEdit *edBand;
	TEdit *edMode;
	TEdit *edNetControl;
	TEdit *edLogger;
	TEdit *edSubscribers;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall GridCellDblClick(TColumn * const Column, const int Row);
	void __fastcall ComboBox1Popup(TObject *Sender);
	void __fastcall mnQuitClick(TObject *Sender);
	void __fastcall mnFieldsClick(TObject *Sender);
	void __fastcall MenuItem3Click(TObject *Sender);
  private: // User declaration
	bool UpdateNetList = false;
	TStringList *NetList;
	NLNet CurNet;
	bool GetActiveNets();
	bool GetCheckins();
	void SetRows(CheckinList *clist);
	void SetCol(TStringColumn *col, const String &val, int row);
	void DisplayNet();
	void ShowBusy(bool state);
	void LoadDefaults();
	void SaveDefaults();
  public: // User declarations
	__fastcall TForm1(TComponent* Owner);
    void Debug(const String &str);

};



//---------------------------------------------------------------------------
extern PACKAGE TForm1* Form1;
//---------------------------------------------------------------------------
#endif

