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
#include <system.IOUtils.hpp>
#pragma hdrstop

#include "DLForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TfrmDL* frmDL;

//---------------------------------------------------------------------------
__fastcall TfrmDL::TfrmDL(TComponent* Owner) : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool TfrmDL::GetFile(String &url, String &filename)
{
	using System::Ioutils::TPath;

	try {
		String basefile = TPath::GetFileName(url);
		filename =
			TPath::Combine(TPath::GetDownloadsPath(), basefile);
		Caption = "Downloading: " + basefile;

		_di_IHTTPResponse LResponse = FClient->Head(url);
		__int64 LSize = LResponse->ContentLength;

		ProgressBar1->Max = LSize;
		ProgressBar1->Min = 0;
		ProgressBar1->Value = 0;

		// Create the file that is going to be downloaded
		FDownloadStream = new TFileStream(filename, fmCreate);
		FDownloadStream->Position = 0;

		FGlobalStart = TThread::GetTickCount();

		// synchronous http download
		FHTTPResponse = FClient->Get(url, FDownloadStream);

		__int64 fsize = FDownloadStream->Size;
		FDownloadStream->Free();

		if(FHTTPResponse->StatusCode == 200)
		{
			if(fsize == LSize)
				return true;
		}
	}
	catch(...)
	{
	  // exceptions
	}

	// failed

	if(FileExists(filename))
		DeleteFile(filename);
	filename = "";
	return  false;
}
//---------------------------------------------------------------------------
void TfrmDL::UpdateProgress(int AValue, float ASpeed, bool &Abort)
{
	ProgressBar1->Value = AValue;
	String s;
	s.sprintf(L"%0.2f MB/s", ASpeed / 1000000.0);
	Label1->Text = s;
	Application->ProcessMessages();
}
//---------------------------------------------------------------------------
void __fastcall TfrmDL::ReceiveDataEvent(TObject* const Sender,
	__int64 AContentLength, __int64 AReadCount, bool &Abort)
{
	if (Abort == false) {
		UInt32 LTime = TThread::GetTickCount() - FGlobalStart;
		float LSpeed = (AReadCount * 1000) / LTime;

		bool LAbort = Abort;
		System::Classes::TThread::Synchronize(
			NULL, SyncLambda<int, float, bool &>(
					  &this->UpdateProgress, AReadCount, LSpeed, LAbort));
		Abort = LAbort;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmDL::FormCreate(TObject* Sender)
{
	FClient = THTTPClient::Create();
	FClient->OnReceiveData = ReceiveDataEvent;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDL::FormDestroy(TObject* Sender)
{
	FClient->Free();
}
//---------------------------------------------------------------------------

