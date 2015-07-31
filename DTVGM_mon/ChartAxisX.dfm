inherited DlgChartEdit: TDlgChartEdit
  Left = 384
  Top = 205
  ClientWidth = 314
  ParentFont = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  inherited Bevel1: TBevel
    Width = 298
    Height = 162
  end
  object Label1: TLabel [1]
    Left = 24
    Top = 18
    Width = 61
    Height = 13
    Caption = 'Select Chart:'
  end
  object Label2: TLabel [2]
    Left = 24
    Top = 46
    Width = 47
    Height = 13
    Caption = 'MinValue:'
  end
  object Label3: TLabel [3]
    Left = 24
    Top = 74
    Width = 47
    Height = 13
    Caption = 'MaxValue'
  end
  object Label4: TLabel [4]
    Left = 24
    Top = 102
    Width = 90
    Height = 13
    Caption = 'Number of Interval:'
  end
  object Label5: TLabel [5]
    Left = 24
    Top = 130
    Width = 56
    Height = 13
    Caption = 'Increament:'
  end
  inherited OKBtn: TButton
    Left = 70
    Width = 76
    Height = 26
    OnClick = OKBtnClick
  end
  inherited CancelBtn: TButton
    Left = 150
    Width = 76
    Height = 26
  end
  object HelpBtn: TButton
    Left = 230
    Top = 180
    Width = 76
    Height = 26
    Caption = '&Help'
    TabOrder = 2
    OnClick = HelpBtnClick
  end
  object EdtMin: TEdit
    Left = 126
    Top = 44
    Width = 120
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ParentFont = False
    TabOrder = 3
    Text = '0'
  end
  object ComboBox1: TComboBox
    Left = 126
    Top = 16
    Width = 120
    Height = 21
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ItemHeight = 13
    TabOrder = 4
    Text = '1'
    OnChange = ComboBox1Change
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8'
      '9'
      '10'
      '11'
      '12'
      '13'
      '14'
      '15'
      '16'
      '17'
      '18'
      '19'
      '20'
      '21'
      '22'
      '23'
      '24')
  end
  object EdtMax: TEdit
    Left = 126
    Top = 72
    Width = 120
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ParentFont = False
    TabOrder = 5
    Text = '1'
  end
  object EdtNum: TEdit
    Left = 126
    Top = 100
    Width = 120
    Height = 21
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    TabOrder = 6
    Text = '5'
  end
  object EdtInc: TEdit
    Left = 126
    Top = 128
    Width = 120
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ParentFont = False
    TabOrder = 7
    Text = '0.2'
  end
  object Button1: TButton
    Left = 250
    Top = 124
    Width = 50
    Height = 25
    Caption = 'Check'
    TabOrder = 8
    OnClick = Button1Click
  end
end
