object DMod: TDMod
  Height = 480
  Width = 640
  object RESTClient1: TRESTClient
    BaseURL = 'https://www.netlogger.org/api'
    Params = <>
    SynchronizedEvents = False
    Left = 40
    Top = 24
  end
  object RESTRequest1: TRESTRequest
    Client = RESTClient1
    Params = <>
    Response = RESTResponse1
    SynchronizedEvents = False
    Left = 40
    Top = 88
  end
  object RESTResponse1: TRESTResponse
    Left = 40
    Top = 144
  end
  object XMLDocument1: TXMLDocument
    Left = 40
    Top = 208
  end
end
