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

#include <fmx.h>
#pragma hdrstop

#include "AboutForm.h"
#include "WinVersionQuery.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource ("*.Windows.fmx", _PLAT_MSWINDOWS)

TfrmAbout *frmAbout;
//---------------------------------------------------------------------------
__fastcall TfrmAbout::TfrmAbout(TComponent* Owner)
	: TForm(Owner)
{
	Util::WinVersionQuery vers;
	lbVersion->Text = vers.GetStr("ProductVersion");

	auto addstr = [this](const String &str) {
		lbVersion->Text += " " + str;
	};

	if(vers.IsDebug()) addstr("DEBUG");
	if(vers.IsPreRelease()) addstr("PreRelease");
	if(vers.IsSpecialBuild()) addstr("SpecialBuild");
	if(vers.IsPrivateBuild()) addstr("PrivateBuild");

    lbComment->Text = vers.GetStr("Comments");
}
//---------------------------------------------------------------------------

