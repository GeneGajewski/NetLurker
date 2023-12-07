//---------------------------------------------------------------------------
#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"

#include <shellapi.h>

#include <System.Win.Registry.hpp>
#include <cstdlib>
#include <vector>

#include "Unit2.h"
#include "unit5.h"

using namespace std;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource("*.Windows.fmx", _PLAT_MSWINDOWS)
#pragma resource("*.Surface.fmx", _PLAT_MSWINDOWS)

TForm1* Form1;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    NetList = new TStringList;
//    LoadDefaults();
//    GetActiveNets();
}
//---------------------------------------------------------------------------

//
// Query for the current list of active nets amd load them into
// the combobox selector,

bool TForm1::GetActiveNets()
{
    ShowBusy(true);
    String last_selected;
    bool retval;

    NetList->Clear();

    // remember previous combobox selection, if any
    if (ComboBox1->Selected != NULL)
        last_selected = ComboBox1->Selected->Text;

    retval = DMod->GetNetNames(NetList);
    Label1->Text = DMod->ErrorMessage();

    ComboBox1->Items->Clear();
    ComboBox1->Items->AddStrings(NetList);

    // keep same selection as before if possible
    if (NetList->Count > 0) {
        int index;
        if (last_selected.IsEmpty())
            index = 0;
        else
            index = ComboBox1->Items->IndexOf(last_selected);

        ComboBox1->ItemIndex = index;
	}

	ShowBusy(false);
	return retval;
}

void TForm1::DisplayNet()
{
	NLNet net;
	bool retval;

	if (ComboBox1->Selected != NULL) {
		DMod->GetNetData(ComboBox1->Selected->Text, net);
		Label1->Text = DMod->ErrorMessage();
	}

	edFreq->Text = net.Frequency;
	edBand->Text = net.Band;
	edMode->Text = net.Mode;
	edNetControl->Text = net.NetControl;
	edLogger->Text = net.Logger;
	edSubscribers->Text = net.SubscriberCount;
}

//---------------------------------------------------------------------------

//
// Get checkins for the currently selected net and load them into the grid
//
// If query returns nothing then the net has been closed -> clear grid and
// remove the selected net from the combobox's item list.

bool TForm1::GetCheckins()
{
	ShowBusy(true);
	CheckinList* ptr = NULL;
	bool retval = false;

	if (ComboBox1->Selected != NULL) {
		ptr = DMod->GetCheckins(ComboBox1->Selected->Text);
		Label1->Text = DMod->ErrorMessage();

		// display the rows
		if (ptr) {
			SetRows(ptr);
            DisplayNet();
			retval = true;
		} else // net not found
		{
			Grid->RowCount = 0;
			TStrings* strs = ComboBox1->Items;
			strs->Delete(strs->IndexOf(ComboBox1->Selected->Text));
		}
	}
	ShowBusy(false);
	return retval;
}
//---------------------------------------------------------------------------

//
// populate grid from checkinlist
//

void TForm1::SetRows(CheckinList* clist)
{
	int selected_row = clist->Pointer.ToIntDef(0) - 1;
	int row;

	auto setcol = [this, &row](TColumn* c, const String& s) {
		Grid->Cells[c->Index][row] = s;
	};


	for (row = 0; row < clist->Checkins.size(); row++) {

		const NLCheckin& ck = clist->Checkins[row];

		// add another row to grid if needed
		if (Grid->RowCount < row + 1)
			Grid->RowCount++;

		setcol(scSerialNo, ck.SerialNo);

		// hilite this grid row if it's the 'pointer' row
		if (ck.SerialNo.ToIntDef(0) == selected_row)
			Grid->Selected = row;

		setcol(scCallsign, ck.Callsign);
		setcol(scState, ck.State);
		setcol(scRemarks, ck.Remarks);
		setcol(scQSL, ck.QSLInfo);
		setcol(scCity, ck.CityCountry);
		setcol(scName, ck.FirstName);
		setcol(scStatus, ck.Status);
		setcol(scCounty, ck.County);
		setcol(scGrid, ck.Grid);
		setcol(scStreet, ck.Street);
		setcol(scZip, ck.Zip);
		setcol(scMemberID, ck.MemberID);
		setcol(scCountry, ck.Country);
		setcol(scDXCC, ck.DXCC);
		setcol(scPreferred, ck.PreferredName);
	}

	// delete any extra allocated rows
	if (Grid->RowCount > row + 1)
		Grid->RowCount = row + 1;
}
//---------------------------------------------------------------------------

void TForm1::ShowBusy(bool state)
{
	if (state) {
		Circle1->Fill->Color = claChartreuse;
		Application->ProcessMessages();
	} else
        Circle1->Fill->Color = 0xFFE0E0E0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormDestroy(TObject* Sender)
{
    delete NetList;
    SaveDefaults();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject* Sender)
{
    static int tcount = 0;

    tcount++;

    if (tcount == 4) {
        UpdateNetList = true;
        tcount = 0;
    }

    Timer1->Enabled = false;
    bool retval = GetCheckins();
    Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject* Sender)
{
    ShowBusy(true);
    GetCheckins();
}
//---------------------------------------------------------------------------

//
// open QRZ.com with the selected row's callsign (if any)
//

void __fastcall TForm1::GridCellDblClick(TColumn* const Column, const int Row)
{
    String callsign, cmd;

    callsign = Grid->Cells[scCallsign->Index][Row];

    if (callsign.IsEmpty())
        return;

    cmd = "https://qrz.com/db/" + callsign;

    ShellExecute(NULL, L"open", cmd.c_str(), NULL, NULL, SW_SHOW);
}
//---------------------------------------------------------------------------

//
// Refresh list of nets if needed
//
void __fastcall TForm1::ComboBox1Popup(TObject* Sender)
{
    if (UpdateNetList) {
        UpdateNetList = false;
        GetActiveNets();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::mnQuitClick(TObject* Sender)
{
    Close();
}
//---------------------------------------------------------------------------

//
// save each grid column's visibility, width, and index to registry
//

const char* rootkey = "Software\\WG5ENE\\NetLurker\\Fields";

void TForm1::SaveDefaults()
{
    TRegistry* reg = 0;
    try {

        reg = new TRegistry(KEY_WRITE);
        reg->RootKey = HKEY_CURRENT_USER;
        reg->OpenKey(rootkey, true);

        for (int x = 0; x < Grid->ColumnCount; x++) {
            TColumn* col = Grid->Columns[x];
            reg->OpenKey(col->Header, true);

            reg->WriteBool("Visible", col->Visible);
            reg->WriteInteger("Width", col->Width);
            reg->WriteInteger("Index", col->Index);

            reg->CloseKey();
            reg->OpenKey(rootkey, false);
        }
        reg->CloseKey();
    } catch (ERegistryException&) {
    }
    if (reg != NULL)
        delete reg;
}

//
// restore grid columns order and widths from registry and
// build 'fields' menu
//

void TForm1::LoadDefaults()
{
	TRegistry* reg = 0;
	map<int, TColumn*> colmap;
	TMenuItem* field_menu = 0;

	try {

		// locate '&Fields" in main menu
		for (int x = 0; x < MainMenu1->ItemsCount; x++) {
			TMenuItem* i;
			i = (TMenuItem*)MainMenu1->Items[x];
			if (i->Text == "&Fields") {
				field_menu = i;
				break;
			}
		}

		reg = new TRegistry(KEY_READ);
		reg->RootKey = HKEY_CURRENT_USER;
		reg->OpenKeyReadOnly(rootkey);

		// read column data.
		// save an index/column map for later use in a a *second* pass

		for (int x = 0; x < Grid->ColumnCount; x++) {

			TColumn* col = Grid->Columns[x];
			TMenuItem* mi = new TMenuItem(this);

			mi->AutoCheck = true;
			mi->Text = col->Header;

			if (!reg->KeyExists(col->Header)) // unlikely, but
				continue;

			reg->OpenKeyReadOnly(col->Header);

			col->Visible = reg->ReadInteger("Visible");
			mi->IsChecked = col->IsVisible;
			mi->OnClick = mnFieldsClick;

			if (field_menu != NULL)
				field_menu->AddObject(mi);

			col->Width = reg->ReadInteger("Width");
			int index = reg->ReadInteger("Index");

			colmap[index] = col;

			reg->CloseKey();
			reg->OpenKeyReadOnly(rootkey);
		}
		reg->CloseKey();

		// now we can finally set column's index ordinally
		for (auto z : colmap)
			z.second->Index = z.first;

	} catch (ERegistryException&) {
	}
	if (reg != NULL)
		delete reg;
}

void TForm1::Debug(const String& str)
{
	OutputDebugString(str.c_str());
}

void __fastcall TForm1::mnFieldsClick(TObject* Sender)
{
	TColumn* col;
	TMenuItem* mi = (TMenuItem*)Sender;

	for (int x = 0; x < Grid->ColumnCount; x++)
		if (mi->Text == Grid->Columns[x]->Header) {
            Grid->Columns[x]->Visible = mi->IsChecked;
            break;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MenuItem3Click(TObject *Sender)
{
	Form5->Show();
}
//---------------------------------------------------------------------------

