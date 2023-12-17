/*
 *   Copyright (C) 2023 Gene Gajewski WG5ENE
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
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

#ifndef MainFormH
#define MainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.Header.hpp>
#include <System.Rtti.hpp>
#include <FMX.Edit.hpp>
#include <System.Math.Vectors.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Menus.hpp>
#include <Web.HTTPApp.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Colors.hpp>

#include "DataModule.h"
#include <FMX.ScrollBox.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
  __published: // IDE-managed Components
	TStatusBar *StatusBar1;
	TMainMenu *MainMenu1;
	TMenuItem *mQuit;
	TLayout *Layout1;
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
	TLabel *lbStatus;
	TMenuItem *mnFields;
	TMenuItem *mnQuit;
	TMenuItem *mnHelp;
	TMenuItem *nmUpdates;
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
	TMenuItem *mnAbout;
	TButton *bnSelect;
	TEdit *edNET;
	TMenuItem *nmHelp;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall GridCellDblClick(TColumn * const Column, const int Row);
	void __fastcall mnQuitClick(TObject *Sender);
	void __fastcall mnFieldsClick(TObject *Sender);
	void __fastcall nmUpdatesClick(TObject *Sender);
	void __fastcall mnAboutClick(TObject *Sender);
	void __fastcall bnSelectClick(TObject *Sender);
	void __fastcall nmHelpClick(TObject *Sender);
  private: // User declaration
	bool UpdateNetList = false;
	TStringList *NetList;
	NLNet CurNet;
	bool GetCheckins();
	void SetRows(CheckinList *clist);
	void SetCol(TStringColumn *col, const String &val, int row);
	void DisplayNet();
	void ShowBusy(bool state);
	void LoadDefaults();
	void SaveDefaults();
  public: // User declarations
	__fastcall TfrmMain(TComponent* Owner);
#ifdef _DEBUG
	void Debug(const String &str);
#endif
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmMain* frmMain;
//---------------------------------------------------------------------------
#endif

