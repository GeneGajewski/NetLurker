//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit2.h"
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
	bool retval;
	_di_IXMLNode node, child;
	nl.Error = "";

	DMod->RESTRequest1->Execute();

	if (RESTResponse1->StatusCode == 200) {
		String name, value;

		XMLDocument1->LoadFromXML(DMod->RESTResponse1->Content);
		//		XMLDocument1->SaveToFile("XMLDOC.txt");

		XMLDocument1->Active = true;
		node = XMLDocument1->DocumentElement;

		// parse all document child nodes
		for (int x = 0; x < node->ChildNodes->Count; x++) {

			child = node->ChildNodes->Nodes[x];
			name = child->NodeName;

			if (name == "Header")
				retval = ReadHeader(child, nl.header);
			else if (name == "Error") {
				nl.Error = GetValue(child);
				retval = false;
			} else if (name == "ResponseCode")
				nl.ResponseCode = GetValue(child);
			else if (name == "ServerList")
				retval = ReadServerList(child, nl.serverlist);
			else if (name == "CheckinList")
				retval = ReadCheckinList(child, nl.checkinlist);
		}
		return retval;
	}
	return false;
}

//
// parse a 'header' node
//
bool TDMod::ReadHeader(_di_IXMLNode& head, NLHeader& header)
{
	String name;
	_di_IXMLNode child;

	for (int x = 0; x < head->ChildNodes->Count; x++) {
		child = head->ChildNodes->Nodes[x];
		name = child->NodeName;
		if (name == "CreationDateUTC")
			header.CreationDateUTC = GetValue(child);
		else if (name == "Copyright")
			header.Copyright = GetValue(child);
		else if (name == "APIVersion")
			header.APIVersion = GetValue(child);
		else if (name == "TimeZone")
			header.TimeZone = GetValue(child);
	}
	return true;
}

//
// parse a 'ServerList' node
//
bool TDMod::ReadServerList(_di_IXMLNode& head, ServerList& list)
{
	String name;
	_di_IXMLNode child;

	list.Servers.clear();
	list.NetServer.clear();

	for (int x = 0; x < head->ChildNodes->Count; x++) {

		child = head->ChildNodes->Nodes[x];
		name = child->NodeName;

		if (name == "ResponseCode")
			list.ResponseCode = GetValue(child);
		else if (name == "Server") {
			ReadServer(child, list);
		}
	}
	return true;
}

//
// parse a 'Server' node
//

bool TDMod::ReadServer(_di_IXMLNode& head, ServerList& list)
{
	String name;
	_di_IXMLNode child;
	NLServer server;

	for (int x = 0; x < head->ChildNodes->Count; x++) {

		child = head->ChildNodes->Nodes[x];
		name = child->NodeName;

		if (name == "ServerName")
			server.ServerName = GetValue(child);
		else if (name == "ServerActiveNetCount")
			server.ServerActiveNetCount = GetValue(child);
		else if (name == "Net") {
			if (!ReadNet(child, server))
				return false;
		}
	}

	// add nets to NetServer map
	auto iter = server.Nets.begin();

	while (iter != server.Nets.end()) {
		list.NetServer[iter->first] = server.ServerName;
		iter++;
	}

	// insert server
	list.Servers.insert(list.Servers.end(), { server.ServerName, server });

	return true;
}

//
// parse a 'Net' node
//

bool TDMod::ReadNet(_di_IXMLNode& head, NLServer& server)
{
	String name;
	_di_IXMLNode child;
	NLNet net;

	for (int x = 0; x < head->ChildNodes->Count; x++) {

		child = head->ChildNodes->Nodes[x];
		name = child->NodeName;

		if (name == "NetName")
			net.NetName = GetValue(child);
		else if (name == "AltNetName")
			net.AltNetName = GetValue(child);
		else if (name == "Frequency")
			net.Frequency = GetValue(child);
		else if (name == "Logger")
			net.Logger = GetValue(child);
		else if (name == "NetControl")
			net.NetControl = GetValue(child);
		else if (name == "Date")
			net.Date = GetValue(child);
		else if (name == "Mode")
			net.Mode = GetValue(child);
		else if (name == "Band")
			net.Band = GetValue(child);
		else if (name == "SubscriberCount")
			net.SubscriberCount = GetValue(child);
		else if (name == "NetID")
			net.NetID = GetValue(child);
		else if (name == "AIM")
			net.AIM = GetValue(child);
		else if (name == "UpdateInterval")
			net.UpdateInterval = GetValue(child);
		else if (name == "srcIP")
			net.srcIP = child->NodeValue;
		else if (name == "LastActivity")
			net.LastActivity = GetValue(child);
		else if (name == "InactivityTimer")
			net.InactivityTimer = GetValue(child);
		else if (name == "MiscNetParameter")
			net.MiscNetParameters = GetValue(child);
		else if (name == "ClosedAt")
			net.ClosedAt = child->NodeValue;
		else if (name == "Asassinated")
			net.Asassinated = GetValue(child);
	}
	server.Nets.insert(server.Nets.end(), { net.NetName, net });
	return true;
}

//
// parse a "CheckinList" node
//

bool TDMod::ReadCheckinList(_di_IXMLNode& head, CheckinList& list)
{
	String name;
	_di_IXMLNode child;

	list.Checkins.clear();

	for (int x = 0; x < head->ChildNodes->Count; x++) {
		child = head->ChildNodes->Nodes[x];
		name = child->NodeName;
		if (name == "ServerName")
			list.ServerName = GetValue(child);
		else if (name == "NetName")
			list.NetName = GetValue(child);
		else if (name == "ResponseCode")
			list.ResponseCode = GetValue(child);
		else if (name == "CheckinCount")
			list.CheckinCount = GetValue(child);
		else if (name == "Pointer")
			list.Pointer = GetValue(child);
		else if (name == "Checkin") {
			if (!ReadCheckin(child, list))
				return false;
		}
	}
	return true;
}

//
// Read a 'Checkin' node
//
bool TDMod::ReadCheckin(_di_IXMLNode& head, CheckinList& list)
{
	String name;
	_di_IXMLNode child;
	NLCheckin ck;

	for (int x = 0; x < head->ChildNodes->Count; x++) {
		child = head->ChildNodes->Nodes[x];
		name = child->NodeName;

		if (name == "SerialNo")
			ck.SerialNo = GetValue(child);
		else if (name == "Callsign")
			ck.Callsign = GetValue(child);
		else if (name == "State")
			ck.State = GetValue(child);
		else if (name == "Remarks")
			ck.Remarks = GetValue(child);
		else if (name == "QSLInfo")
			ck.QSLInfo = GetValue(child);
		else if (name == "CityCountry")
			ck.CityCountry = GetValue(child);
		else if (name == "FirstName")
			ck.FirstName = GetValue(child);
		else if (name == "Status")
			ck.Status = GetValue(child);
		else if (name == "County")
			ck.County = GetValue(child);
		else if (name == "Grid")
			ck.Grid = GetValue(child);
		else if (name == "Street")
			ck.Street = GetValue(child);
		else if (name == "Zip")
			ck.Zip = GetValue(child);
		else if (name == "MemberID")
			ck.MemberID = GetValue(child);
		else if (name == "Country")
			ck.Country = GetValue(child);
		else if (name == "DXCC")
			ck.DXCC = GetValue(child);
		else if (name == "PreferredName")
			ck.PreferredName = GetValue(child);
	}
	list.Checkins.insert(list.Checkins.end(), ck);
	return true;
}

//
// return an empty string whenever node contents
// are NULL (empty)
//

String TDMod::GetValue(const _di_IXMLNode& node)
{
	String retval;
	try {

		if (node->IsTextElement)
			retval = node->Text;
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

bool TDMod::GetNetData(const String& name, NLNet& net)
{
	net = nl.serverlist.Servers[nl.serverlist.NetServer[name]].Nets[name];
	return true;
}


//
// Obtain a TStringlist of active nets
//

bool TDMod::GetNetNames(TStringList* list)
{
	bool retval;
	list->Clear();

	nl.serverlist.Servers.clear();
	nl.serverlist.NetServer.clear();

	RESTRequest1->Resource = "GetActiveNets.php";
	RESTRequest1->Params->Clear();
	retval = DoQuery(nl);

	auto iter = nl.serverlist.NetServer.begin();
	while (iter != nl.serverlist.NetServer.end()) {
		list->Add(iter->first);
		iter++;
	}
	return retval;
}

//
// return a CheckinList * for the given net
//

CheckinList* TDMod::GetCheckins(const String netname)
{
	bool retval = false;

	String servername = nl.serverlist.Servers[nl.serverlist.NetServer[netname]].ServerName;

	// XML query
	DMod->RESTRequest1->Resource = "GetCheckins.php";
	DMod->RESTRequest1->Params->Clear();
	DMod->RESTRequest1->Params->AddItem("ServerName", servername, pkGETorPOST);
	DMod->RESTRequest1->Params->AddItem("NetName", netname, pkGETorPOST);
	retval = DMod->DoQuery(nl);

	if (retval)
		return &nl.checkinlist;
	return NULL;
}

//
// Get the Error string for last XML query, if any
//
const String& TDMod::ErrorMessage()
{
	return nl.Error;
}
//---------------------------------------------------------------------------

