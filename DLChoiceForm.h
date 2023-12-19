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

#ifndef DLChoiceFormH
#define DLChoiceFormH
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
//---------------------------------------------------------------------------
class TfrmDLChoice : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *bnOK;
	TButton *bnCancel;
	TMemo *Memo1;

private:	// User declarations
public:		// User declarations

	__fastcall TfrmDLChoice(TComponent* Owner);
    void SetVersion(String &ver);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDLChoice *frmDLChoice;
//---------------------------------------------------------------------------
#endif
