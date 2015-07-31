object DM: TDM
  OldCreateOrder = False
  Left = 325
  Top = 220
  Height = 269
  Width = 426
  object ADOTable1: TADOTable
    Connection = ADOConnection1
    Left = 96
    Top = 64
  end
  object ADOQuery1: TADOQuery
    Connection = ADOConnection1
    Parameters = <>
    Left = 160
    Top = 64
  end
  object DS1: TDataSource
    DataSet = ADOTable1
    Left = 96
    Top = 136
  end
  object DS2: TDataSource
    DataSet = ADOQuery1
    Left = 160
    Top = 136
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source=DB\DH' +
      'M_CBH.mdb;Mode=Share Deny None;Extended Properties="";Persist Se' +
      'curity Info=False;Jet OLEDB:System database="";Jet OLEDB:Registr' +
      'y Path="";Jet OLEDB:Database Password="";Jet OLEDB:Engine Type=5' +
      ';Jet OLEDB:Database Locking Mode=1;Jet OLEDB:Global Partial Bulk' +
      ' Ops=2;Jet OLEDB:Global Bulk Transactions=1;Jet OLEDB:New Databa' +
      'se Password="";Jet OLEDB:Create System Database=False;Jet OLEDB:' +
      'Encrypt Database=False;Jet OLEDB:Don'#39't Copy Locale on Compact=Fa' +
      'lse;Jet OLEDB:Compact Without Replica Repair=False;Jet OLEDB:SFP' +
      '=False'
    LoginPrompt = False
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 176
  end
  object ADOTable2: TADOTable
    Connection = ADOConnection1
    CursorType = ctStatic
    Left = 224
    Top = 64
  end
  object DS3: TDataSource
    DataSet = ADOTable2
    Left = 224
    Top = 136
  end
  object ADOTable0: TADOTable
    Connection = ADOConnection1
    CursorType = ctStatic
    TableName = 'dem'
    Left = 32
    Top = 64
  end
  object DS0: TDataSource
    DataSet = ADOTable0
    Left = 32
    Top = 136
  end
  object OpenDialog: TOpenDialog
    Left = 352
    Top = 8
  end
  object SaveDialog: TSaveDialog
    Left = 352
    Top = 56
  end
  object ADOTable3: TADOTable
    Connection = ADOConnection1
    Left = 288
    Top = 64
  end
  object DS4: TDataSource
    DataSet = ADOTable3
    Left = 288
    Top = 136
  end
end
