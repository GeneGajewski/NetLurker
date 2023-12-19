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

#pragma hdrstop

#include "DataModule.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "FMX.Controls.TControl"
#pragma resource "*.dfm"
TDMod* DMod;
//---------------------------------------------------------------------------
__fastcall TDMod::TDMod(TComponent* Owner)
	: TDataModule(Owner)
{
}

//
// perform an XML REST query and parse the results into the netlogger structure
//
bool TDMod::DoQuery(NetLoggerXML& nl)
{
	bool Retval = false;
	_di_IXMLNode Node, Child;
	nl.Error = "";

	RESTRequest1->Execute();

	if (RESTResponse1->StatusCode == 200) {
		String Name;

		XMLDocument1->LoadFromXML(DMod->RESTResponse1->Content);
		XMLDocument1->Active = true;
		Node = XMLDocument1->DocumentElement;

		// parse all document child nodes
		for (int x = 0; x < Node->ChildNodes->Count; x++) {
			Child = Node->ChildNodes->Nodes[x];
			Name = Child->NodeName;

			if (Name == "Header")
				Retval = ReadHeader(Child, nl.header);
			else if (Name == "Error") {
				nl.Error = GetValue(Child);
				Retval = false;
			} else if (Name == "ResponseCode")
				nl.ResponseCode = GetValue(Child);
			else if (Name == "ServerList")
				Retval = ReadServerList(Child, nl.serverlist);
			else if (Name == "CheckinList")
				Retval = ReadCheckinList(Child, nl.checkinlist);
		}
		return Retval;
	}
	return   false;
}

//
// parse a 'header' node
//
bool TDMod::ReadHeader(_di_IXMLNode& Head, NLHeader& Header)
{
	String Name;
	_di_IXMLNode Child;

	for (int x = 0; x < Head->ChildNodes->Count; x++) {
		Child = Head->ChildNodes->Nodes[x];
		Name = Child->NodeName;
		if (Name == "CreationDateUTC")
			Header.CreationDateUTC = GetValue(Child);
		else if (Name == "Copyright")
			Header.Copyright = GetValue(Child);
		else if (Name == "APIVersion")
			Header.APIVersion = GetValue(Child);
		else if (Name == "TimeZone")
			Header.TimeZone = GetValue(Child);
	}
	return true;
}

//
// parse a 'ServerList' node
//
bool TDMod::ReadServerList(_di_IXMLNode& Head, ServerList& List)
{
	String Name;
	_di_IXMLNode Child;

	List.Servers.clear(); List.NetServerMap.clear();

	for (int x = 0; x < Head->ChildNodes->Count; x++) {

		Child = Head->ChildNodes->Nodes[x];
		Name = Child->NodeName;

		if (Name == "ResponseCode")
			List.ResponseCode = GetValue(Child);
		else if (Name == "Server") {
			ReadServer(Child, List);
		}
	}
	return true;
}

//
// parse a 'Server' node
//

bool TDMod::ReadServer(_di_IXMLNode& Head, ServerList& List)
{
	String Name;
	_di_IXMLNode Child;
	NLServer Server;

	for (int x = 0; x < Head->ChildNodes->Count; x++) {

		Child = Head->ChildNodes->Nodes[x];
		Name = Child->NodeName;

		if (Name == "ServerName")
			Server.ServerName = GetValue(Child);
		else if (Name == "ServerActiveNetCount")
			Server.ServerActiveNetCount = GetValue(Child);
		else if (Name == "Net") {
			if (!ReadNet(Child, Server))
				return false;
		}
	}

	// add nets to NetServerMap map
	for(auto Item : Server.Nets)
		List.NetServerMap[Item.first] = Server.ServerName;

	// add server to Serrverlist	
	List.Servers.insert(List.Servers.end(), { Server.ServerName, Server });
	return true;
}

//
// parse a 'Net' node
//

bool TDMod::ReadNet(_di_IXMLNode& Head, NLServer& Server)
{
	String Name;
	_di_IXMLNode Child;
	NLNet Net;

	for (int x = 0; x < Head->ChildNodes->Count; x++) {

		Child = Head->ChildNodes->Nodes[x];
		Name = Child->NodeName;

		if (Name == "NetName")
			Net.NetName = GetValue(Child);
		else if (Name == "AltNetName")
			Net.AltNetName = GetValue(Child);
		else if (Name == "Frequency")
			Net.Frequency = GetValue(Child);
		else if (Name == "Logger")
			Net.Logger = GetValue(Child);
		else if (Name == "NetControl")
			Net.NetControl = GetValue(Child);
		else if (Name == "Date")
			Net.Date = GetValue(Child);
		else if (Name == "Mode")
			Net.Mode = GetValue(Child);
		else if (Name == "Band")
			Net.Band = GetValue(Child);
		else if (Name == "SubscriberCount")
			Net.SubscriberCount = GetValue(Child);
		else if (Name == "NetID")
			Net.NetID = GetValue(Child);
		else if (Name == "AIM")
			Net.AIM = GetValue(Child);
		else if (Name == "UpdateInterval")
			Net.UpdateInterval = GetValue(Child);
		else if (Name == "srcIP")
			Net.srcIP = GetValue(Child);
		else if (Name == "LastActivity")
			Net.LastActivity = GetValue(Child);
		else if (Name == "InactivityTimer")
			Net.InactivityTimer = GetValue(Child);
		else if (Name == "MiscNetParameter")
			Net.MiscNetParameters = GetValue(Child);
		else if (Name == "ClosedAt")
			Net.ClosedAt = GetValue(Child);
		else if (Name == "Asassinated")
			Net.Asassinated = GetValue(Child);
	}
	Server.Nets.insert(Server.Nets.end(), { Net.NetName, Net });
	return true;
}

//
// parse a "CheckinList" node
//

bool TDMod::ReadCheckinList(_di_IXMLNode& Head, CheckinList& List)
{
	String Name;
	_di_IXMLNode Child;

	List.Checkins.clear();

	for (int x = 0; x < Head->ChildNodes->Count; x++) {
		Child = Head->ChildNodes->Nodes[x];
		Name = Child->NodeName;
		if (Name == "ServerName")
			List.ServerName = GetValue(Child);
		else if (Name == "NetName")
			List.NetName = GetValue(Child);
		else if (Name == "ResponseCode")
			List.ResponseCode = GetValue(Child);
		else if (Name == "CheckinCount")
			List.CheckinCount = GetValue(Child);
		else if (Name == "Pointer")
			List.Pointer = GetValue(Child);
		else if (Name == "Checkin") {
			if (!ReadCheckin(Child, List))
				return false;
		}
	}
	return true;
}

//
// Read a 'Checkin' node
//
bool TDMod::ReadCheckin(_di_IXMLNode& Head, CheckinList& List)
{
	String Name;
	_di_IXMLNode Child;
	NLCheckin Chk;

	for (int x = 0; x < Head->ChildNodes->Count; x++) {
		Child = Head->ChildNodes->Nodes[x];
		Name = Child->NodeName;

		if (Name == "SerialNo")
			Chk.SerialNo = GetValue(Child);
		else if (Name == "Callsign")
			Chk.Callsign = GetValue(Child);
		else if (Name == "State")
			Chk.State = GetValue(Child);
		else if (Name == "Remarks")
			Chk.Remarks = GetValue(Child);
		else if (Name == "QSLInfo")
			Chk.QSLInfo = GetValue(Child);
		else if (Name == "CityCountry")
			Chk.CityCountry = GetValue(Child);
		else if (Name == "FirstName")
			Chk.FirstName = GetValue(Child);
		else if (Name == "Status")
			Chk.Status = GetValue(Child);
		else if (Name == "County")
			Chk.County = GetValue(Child);
		else if (Name == "Grid")
			Chk.Grid = GetValue(Child);
		else if (Name == "Street")
			Chk.Street = GetValue(Child);
		else if (Name == "Zip")
			Chk.Zip = GetValue(Child);
		else if (Name == "MemberID")
			Chk.MemberID = GetValue(Child);
		else if (Name == "Country")
			Chk.Country = GetValue(Child);
		else if (Name == "DXCC")
			Chk.DXCC = GetValue(Child);
		else if (Name == "PreferredName")
			Chk.PreferredName = GetValue(Child);
	}
	List.Checkins.insert(List.Checkins.end(), Chk);
	return true;
}

//
// return an empty string whenever node contents
// are NULL (empty)
//

String TDMod::GetValue(const _di_IXMLNode& Node)
{
	String retval;
	try {

		if (Node->IsTextElement)
			retval = Node->Text;
		else
			retval = ""; // empty node ...

	} catch (...) // just in case...
	{
		retval = "";
	}
	return retval;
}

//
// Obtain network info given name of network
//

bool TDMod::GetNetData(const String& Name, NLNet& Net)
{
	Net = NL.serverlist.Servers[NL.serverlist.NetServerMap[Name]].Nets[Name];
	return true;
}


//
// Obtain a TStringlist of active nets
//

bool TDMod::GetNetNames(TStringList* List)
{
	bool Retval;
	List->Clear();

	NL.serverlist.Servers.clear();	NL.serverlist.NetServerMap.clear();

	RESTRequest1->Resource = "GetActiveNets.php";
	RESTRequest1->Params->Clear();
	Retval = DoQuery(NL);

	for( auto Item : NL.serverlist.NetServerMap)
		List->Add(Item.first);
		
	return Retval;
}

//
// return a CheckinList * for the given net
//

CheckinList* TDMod::GetCheckins(const String Netname)
{
	bool Retval = false;
	String Servername = NL.serverlist.NetServerMap[Netname];

	// XML query
	RESTRequest1->Resource = "GetCheckins.php";
	RESTRequest1->Params->Clear();
	RESTRequest1->Params->AddItem("ServerName", Servername, pkGETorPOST);
	RESTRequest1->Params->AddItem("NetName", Netname, pkGETorPOST);
	Retval = DoQuery(NL);

	if (Retval)
		return &NL.checkinlist;
		
	return NULL;
}

//
// Get the Error string for last XML query, if any
//
const String& TDMod::ErrorMessage()
{
	return NL.Error;
}
//---------------------------------------------------------------------------

