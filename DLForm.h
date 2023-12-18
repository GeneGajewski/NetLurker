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

#ifndef DLFormH
#define DLFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Net.HttpClient.hpp>

// This ugly class is used to stash parameters for a callback __closure
// that will be called by TThreadProcedure::Invoke() interface to execute in the
// applications main thread.
//---------------------------------------------------------------------------
template<typename P0, typename P1, typename P2>
struct TMySyncProcedure3 : public TCppInterfacedObject<TThreadProcedure>
{
	typedef void(__closure* TMyFunction)(P0, P1, P2);
	TMySyncProcedure3(TMyFunction AFunction, P0 AP0, P1 AP1, P2 AP2) :
		FFunction(AFunction), FParam0(AP0), FParam1(AP1), FParam2(AP2)
	{
	}
	void __fastcall Invoke(void)
	{
		FFunction(FParam0, FParam1, FParam2);
	}
	P0 FParam0;
	P1 FParam1;
	P2 FParam2;
	TMyFunction FFunction;
};
template<typename P0, typename P1, typename P2>
_di_TThreadProcedure SyncLambda(
	typename TMySyncProcedure3<P0, P1, P2>::TMyFunction AClosure, P0 AP0,
	P1 AP1, P2 AP2)
{
	return new TMySyncProcedure3<P0, P1, P2>(AClosure, AP0, AP1, AP2);
}

//---------------------------------------------------------------------------
// show file download progress - Call GetFile to download a file from an url
// filename will contain the name of the file downloaded and will be located in
// the system Downloads directory. On error - filename will be empty.

class TfrmDL : public TForm
{
  __published: // IDE-managed Components
	TProgressBar* ProgressBar1;
	TLabel* Label1;
	void __fastcall FormCreate(TObject* Sender);
	void __fastcall FormDestroy(TObject* Sender);
  private: // User declarations
	String Url, Filename;
	THTTPClient* FClient;
	UInt32 FGlobalStart;
	_di_IHTTPResponse FHTTPResponse;
	void UpdateProgress(int AValue, float ASpeed, bool &Abort);
	void __fastcall ReceiveDataEvent(TObject* const Sender,
		__int64 AContentLength, __int64 AReadCount, bool &Abort);
	TStream* FDownloadStream;
  public: // User declarations
	__fastcall TfrmDL(TComponent* Owner);
	bool GetFile(String &url, String &filename);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDL* frmDL;
//---------------------------------------------------------------------------
#endif

