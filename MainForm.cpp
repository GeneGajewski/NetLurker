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
#include <fmx.h>
#pragma hdrstop

#include <system.IOUtils.hpp>
#include "MainForm.h"
#include <shellapi.h>
#include <System.Win.Registry.hpp>
#include <cstdlib>
#include <vector>

#include "DLForm.h"
#include "DLChoiceForm.h"
#include "AboutForm.h"
#include "SelectForm.h"

#include "WinVersionQuery.h"

#include <System.UITypes.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource("*.Windows.fmx", _PLAT_MSWINDOWS)
#pragma resource("*.Surface.fmx", _PLAT_MSWINDOWS)

TfrmMain* frmMain;

//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner) : TForm(Owner)
{
	// on startup - just load the first active net we encounter
	NetList = new TStringList;
	LoadDefaults();
	DMod->GetNetNames(NetList);
	if (NetList->Count > 0) {
		edNET->Text = NetList->Strings[0];
		GetCheckins();
		Timer1->Enabled = true;
		ShowBusy(false);
	}
}
//---------------------------------------------------------------------------

// show current net's misc toplevel data
void TfrmMain::DisplayNet()
{
	NLNet Net;
	if (!edNET->Text.IsEmpty()) {
		DMod->GetNetData(edNET->Text, Net);
		lbStatus->Text = DMod->ErrorMessage();
	}
	edFreq->Text = Net.Frequency;
	edBand->Text = Net.Band;
	edMode->Text = Net.Mode;
	edNetControl->Text = Net.NetControl;
	edLogger->Text = Net.Logger;
	edSubscribers->Text = Net.SubscriberCount;
}

//---------------------------------------------------------------------------

//
// Get checkins for the current selected net and load them into the grid
//
// If query returns nothing then the net has been closed

bool TfrmMain::GetCheckins()
{
	CheckinList* Clist = NULL;
	bool retval = false;
	ShowBusy(true);

	if (!edNET->Text.IsEmpty()) {
		Clist = DMod->GetCheckins(edNET->Text);
		lbStatus->Text = DMod->ErrorMessage();

		// display the rows
		if (Clist) {
			SetRows(Clist);
			retval = true;
		} else {  // NET not found/closed
			Grid->RowCount = 0;
			edNET->Text = "";
		}
	}
	DisplayNet();
	ShowBusy(false);
	return retval;
}
//---------------------------------------------------------------------------

//
// populate grid from checkinlist
//

void TfrmMain::SetRows(CheckinList* Clist)
{
	int Selected_row = Clist->Pointer.ToIntDef(0) - 1;
	int Row;

	auto SetCol = [this, &Row](TColumn* Column, const String &Value) {
		Grid->Cells[Column->Index][Row] = Value;
	};

	for (Row = 0; Row < Clist->Checkins.size(); Row++) {
		const NLCheckin &Chk = Clist->Checkins[Row];

		// add another row to grid if needed
		if (Grid->RowCount < Row + 1)
			Grid->RowCount++;

		SetCol(scSerialNo, Chk.SerialNo);

		// hilite this grid row if it's the 'pointer' row
		if (Chk.SerialNo.ToIntDef(0) == Selected_row)
			Grid->Selected = Row;

		SetCol(scCallsign, Chk.Callsign);
		SetCol(scState, Chk.State);
		SetCol(scRemarks, Chk.Remarks);
		SetCol(scQSL, Chk.QSLInfo);
		SetCol(scCity, Chk.CityCountry);
		SetCol(scName, Chk.FirstName);
		SetCol(scStatus, Chk.Status);
		SetCol(scCounty, Chk.County);
		SetCol(scGrid, Chk.Grid);
		SetCol(scStreet, Chk.Street);
		SetCol(scZip, Chk.Zip);
		SetCol(scMemberID, Chk.MemberID);
		SetCol(scCountry, Chk.Country);
		SetCol(scDXCC, Chk.DXCC);
		SetCol(scPreferred, Chk.PreferredName);
	}

	Grid->RowCount = Row + 1;
}
//---------------------------------------------------------------------------

// blinky thing
void TfrmMain::ShowBusy(bool State)
{
	if (State) {
		Circle1->Fill->Color = claChartreuse;
		Application->ProcessMessages();
	} else
		Circle1->Fill->Color = 0xFFE0E0E0;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject* Sender)
{
	delete NetList;
	SaveDefaults();
}

//---------------------------------------------------------------------------

// poll for checkins data every 30 secs
void __fastcall TfrmMain::Timer1Timer(TObject* Sender)
{
	Timer1->Enabled = false;
	if (!edNET->Text.IsEmpty())
		GetCheckins();
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

//
// open QRZ.com with the selected row's callsign (if any)
//
void __fastcall TfrmMain::GridCellDblClick(TColumn* const Column, const int Row)
{
	String Callsign, Cmd;
	Callsign = Grid->Cells[scCallsign->Index][Row];

	if (Callsign.IsEmpty())
		return;

	Cmd = "https://qrz.com/db/" + Callsign;
	ShellExecuteW(NULL, L"open", Cmd.c_str(), NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnQuitClick(TObject* Sender)
{
	Close();
}
//---------------------------------------------------------------------------

//
// save each grid column's visibility, width, and index to the Registry
//
static const char* fieldkey = "Software\\WG5ENE\\NetLurker\\Fields";
static const char* pgmkey = "Software\\WG5ENE\\NetLurker";

void TfrmMain::SaveDefaults()
{
	TRegistry* Reg = 0;
	try {
		Reg = new TRegistry(KEY_WRITE);
		Reg->RootKey = HKEY_CURRENT_USER;
		Reg->OpenKey(fieldkey, true);

		for (int x = 0; x < Grid->ColumnCount; x++) {
			TColumn* col = Grid->Columns[x];
			Reg->OpenKey(fieldkey, false);
			Reg->OpenKey(col->Header, true);
			Reg->WriteBool("Visible", col->Visible);
			Reg->WriteInteger("Width", col->Width);
			Reg->WriteInteger("Index", col->Index);
			Reg->CloseKey();
		}
		// save window state
		Reg->OpenKey(pgmkey, true);
		Reg->WriteBool("Maximized", WindowState == TWindowState::wsMaximized);
		Reg->WriteInteger("Top", Top);
		Reg->WriteInteger("Left", Left);
		Reg->WriteInteger("Width", Width);
		Reg->WriteInteger("Height", Height);
		Reg->CloseKey();
	} catch (ERegistryException &) {
	}
	if (Reg)
		delete Reg;
}

//
// restore saved grid columns order and widths from registry and
// build 'fields' menu also
//
void TfrmMain::LoadDefaults()
{
	TRegistry* Reg = NULL;
	std::map<int, TColumn*> ColumnMap;
	try {
		Reg = new TRegistry(KEY_READ);
		Reg->RootKey = HKEY_CURRENT_USER;
		Reg->OpenKeyReadOnly(fieldkey);

		// read column data.
		// save an index/column map for later use in a *second* pass

		for (int x = 0; x < Grid->ColumnCount; x++) {
			Reg->OpenKeyReadOnly(fieldkey);
			TColumn* Column = Grid->Columns[x];
			TMenuItem* MenuItem = new TMenuItem(this);

			MenuItem->AutoCheck = true;
			MenuItem->OnClick = mnFieldsClick;
			MenuItem->Text = Column->Header;

			if (!Reg->KeyExists(Column->Header)) // unlikely, but
				continue;

			Reg->OpenKeyReadOnly(Column->Header);
			Column->Visible = Reg->ReadInteger("Visible");
			MenuItem->IsChecked = Column->IsVisible;
			// add child item to fields menu
			mnFields->AddObject(MenuItem);

			Column->Width = Reg->ReadInteger("Width");
			int Index = Reg->ReadInteger("Index");
			ColumnMap[Index] = Column;
			Reg->CloseKey();
		}

		// get window state
		Reg->OpenKeyReadOnly(pgmkey);
		if (Reg->ReadBool("Maximized"))
			WindowState = TWindowState::wsMaximized;
		Left = Reg->ReadInteger("Left");
		Top = Reg->ReadInteger("Top");
		Height = Reg->ReadInteger("Height");
		Width = Reg->ReadInteger("Width");
		Reg->CloseKey();

		// we must set columns' index in *ordinal* fashion because of the
		// 'swapping' method Delphi uses to reorder TColumns
		for (auto Item : ColumnMap)
			Item.second->Index = Item.first;

	} catch (ERegistryException &except) {
	}
	if (Reg)
		delete Reg;
}

#ifdef _DEBUG
void TfrmMain::Debug(const String &str)
{
	OutputDebugStringW(str.c_str());
}
#endif

// toggle a column's visibility
void __fastcall TfrmMain::mnFieldsClick(TObject* Sender)
{
	TMenuItem *MenuItem = reinterpret_cast<TMenuItem *>(Sender);
	for (int x = 0; x < Grid->ColumnCount; x++)
		if (MenuItem->Text == Grid->Columns[x]->Header) {
			Grid->Columns[x]->Visible = MenuItem->IsChecked;
			break;
		}
}
//---------------------------------------------------------------------------

// query the application's web update server
void __fastcall TfrmMain::nmUpdatesClick(TObject* Sender)
{
	Timer1->Enabled = false;
	TRESTClient* Client = NULL;
	TRESTRequest* Request = NULL;
	TRESTResponse* Response = NULL;
	TXMLDocument* Document = NULL;

	try {
		Util::WinVersionQuery WinVer;
		Client = new TRESTClient(this);
		Client->BaseURL = "https://gajewski.net";
		Response = new TRESTResponse(this);
		Request = new TRESTRequest(this);
		Request->Client = Client;
		Request->Resource = "NetLurker-getDLUrl.php";
		Request->Response = Response;
		Request->Params->AddItem(
			L"version", WinVer.GetStr(L"ProductVersion"), pkGETorPOST);
		Document = new TXMLDocument(this);

		Request->Execute();

		if (Response->StatusCode == 200) {
			_di_IXMLNode Node;

			Document->LoadFromXML(Response->Content);
			Document->Active = true;
			Node = Document->DocumentElement;

			// Hat tip to the Notepad++ updater here
			if (Node->NodeName == "GUP") {
				String Answer, Location, Version, Filename, Info;

				Answer = Node->ChildValues["NeedToBeUpdated"];
				if (Answer == "yes") {
					Location = Node->ChildValues["Location"];
					Version = Node->ChildValues["Version"];
					Info = Node->ChildValues["Info"];

					frmDLChoice->Memo1->Text = Info;
					frmDLChoice->Label1->Text = "An updated version ";
					frmDLChoice->Label1->Text += Version;
					frmDLChoice->Label1->Text +=
						" is available.\nDownload and install this update?";
					frmDLChoice->ShowModal();

					if (frmDLChoice->ModalResult == mrOk) {
						frmDL->Show();
						frmDL->GetFile(Location, Filename);
						frmDL->Close();
						if (!Filename.IsEmpty()) {
							ShowMessage(
								"Download complete. Application will close and update will proceed.");
							ShellExecuteW(NULL, L"open", Filename.c_str(), NULL,
								NULL, SW_SHOW);
							Close();
						}
					}
				} else
					ShowMessage("You have the latest version.");
			}
		}
	} catch (EXMLDocError &error) {
	}
	if (Response)
		delete Response;
	if (Request)
		delete Request;
	if (Client)
		delete Client;
	if (Document)
		delete Document;
	Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::mnAboutClick(TObject* Sender)
{
	frmAbout->ShowModal();
}
//---------------------------------------------------------------------------
// select the net to display
void __fastcall TfrmMain::bnSelectClick(TObject* Sender)
{
	NetList->Clear();
	DMod->GetNetNames(NetList);
	frmSelect->ListBox1->Items = NetList;

	if (NetList->Count > 0)
		frmSelect->ListBox1->ItemIndex =
			edNET->Text.IsEmpty() ? 0 : NetList->IndexOf(edNET->Text);

	frmSelect->ShowModal();

	if (frmSelect->ModalResult == mrOk) {
		TListBoxItem* Item = frmSelect->ListBox1->Selected;

		if (Item == NULL) // nothing chosen?
			return;

		if (Item->Text != edNET->Text) {
			edNET->Text = Item->Text;
			Timer1->Enabled = false;
			GetCheckins();
			Timer1->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------

// use a PDF for help file
void __fastcall TfrmMain::nmHelpClick(TObject *Sender)
{
	using System::Ioutils::TPath;
	Util::WinVersionQuery Version;

	// pdf help will be in same dir as executable
	String Path = TPath::GetDirectoryName(Version.ModuleName());
	String Filename = TPath::Combine(Path,"NetLurker.pdf");
	ShellExecuteW(NULL, L"open", Filename.c_str(), NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------

