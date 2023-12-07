//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <REST.Client.hpp>
#include <REST.Types.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>

#include <map>
#include <vector>


// simple 1-1 data structures to match NETLOGGER XML nodes

class NLHeader
{
	public:

	String CreationDateUTC;
	String Copyright;
	String APIVersion;
	String TimeZone;
};

class NLError
{
	public:

	String Value;
	String ResponseCode;
};

class NLNet
{
  public:
	String NetName;
	String AltNetName;
	String Frequency;
	String Logger;
	String NetControl;
	String Date;
	String Mode;
	String Band;
	String SubscriberCount;

	// === PAST NETS ONLY =======

	String NetID;
	String AIM;
	String UpdateInterval;
	String srcIP;
	String LastActivity;
	String InactivityTimer;
	String MiscNetParameters;
	String ClosedAt;
	String Asassinated;

	NLNet() : SubscriberCount(0), InactivityTimer(0) {};
};

class NLServer
{
	public:

	String ServerName;
	String ServerActiveNetCount;
	std::map<String, NLNet> Nets;
};


class NLCheckin
{
	public:

	String SerialNo;
	String Callsign;
	String State;
	String Remarks;
	String QSLInfo;
	String CityCountry;
	String FirstName;
	String Status;
	String County;
	String Grid;
	String Street;
	String Zip;
	String MemberID;
	String Country;
	String DXCC;
	String PreferredName;
};


class ServerList
{
  public:
	String ResponseCode;
	std::map<String,String> NetServer;
	std::map<String, NLServer> Servers;
};

class CheckinList
{
  public:
	String ServerName;
	String NetName;
	String ResponseCode;
	String CheckinCount;
	String Pointer;
	std::vector<NLCheckin> Checkins;
};

// root NETLOGGER XML object

class NetLoggerXML
{
	public:

	NLHeader header;
	String Error;
	String ResponseCode;
	ServerList serverlist;
	CheckinList checkinlist;
};

//---------------------------------------------------------------------------

//
// XML i/o and parsing implemented as a TDatamodule
//

class TDMod : public TDataModule
{
  __published: // IDE-managed Components
	TRESTClient* RESTClient1;
	TRESTRequest* RESTRequest1;
	TRESTResponse* RESTResponse1;
	TXMLDocument *XMLDocument1;

  private: // User declarations

  NetLoggerXML nl;

  bool ReadHeader(_di_IXMLNode &head, NLHeader &header);
  bool ReadServerList(_di_IXMLNode &head, ServerList &list);
  bool ReadServer(_di_IXMLNode &head, ServerList &servers);
  bool ReadNet(_di_IXMLNode &head, NLServer &server);
  bool ReadCheckinList(_di_IXMLNode &head, CheckinList &list);
  bool ReadCheckin(_di_IXMLNode &head, CheckinList &list);
  String GetValue(const _di_IXMLNode &node);

  public: // User declarations

	__fastcall TDMod(TComponent* Owner);
	bool DoQuery(NetLoggerXML &nl);
	bool GetNetData(const String &netname, NLNet &net);
	bool GetNetNames(TStringList *list);
	CheckinList *GetCheckins(const String netname);
	const String &ErrorMessage();
	};
//---------------------------------------------------------------------------
extern PACKAGE TDMod* DMod;

//---------------------------------------------------------------------------
#endif

