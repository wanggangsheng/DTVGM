object DateDlg: TDateDlg
  Left = 400
  Top = 227
  BorderStyle = bsDialog
  Caption = 'DateDialog'
  ClientHeight = 150
  ClientWidth = 266
  Color = clBtnFace
  ParentFont = True
  FormStyle = fsStayOnTop
  Icon.Data = {
    0000010001002020100000000000E80200001600000028000000200000004000
    0000010004000000000080020000000000000000000000000000000000000000
    0000000080000080000000808000800000008000800080800000C0C0C0008080
    80000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000000
    0000000000000000000000000000008888888888888888888880000000000800
    8888888888888888888000000000088800000000000000000000000000000888
    07F7F7700000000000000000000008807F7F7770FFFFFFFFFFFFFFFFFFF00880
    F7F77707FFFFFFFFFFFFFFFFFF7008807F7F770FFFFFFFFFFFFFFFFFFF000880
    F7F7707FFFFFFFFFFFFFFFFFF00008807F7770FFFF7CCCCFFFF7CCFF70000880
    F77707FF7CCCCCCCFF7CCCFF000008807F770FFFCCCCFCCCFFCCCFF700000880
    F770FFFFCCFF7CCFF7CC7FF0000008887707FFFFFFFFCC7FFCCCFF7000000807
    770FFFFFFFCCCCFF7CC7FF0000000807707FFFFCCCCC7FFFCCCFF70000000807
    70FFFFFFFCCCFFF7CC7FF0000000080707FFCCCF7CCCFCFCCCFF700000000807
    0FF7CCFFCCCFCCCCCFFF0000000008070FF7CCCCCCFFCCCC7FFF000000000800
    7FFFCCCC7FFFFCCCFFF7000000000800FFFFFFFFFFFFFFFFFFF0000000000800
    FFFFFFFFFFFFFFFFFF70000000001007FFFFFFFFFFFFFFFFFF00000000001119
    9999999999944499990000000000111999999999999944999900000000001119
    9444494494444499990000000000111994444444444444999000000000001119
    9444444444444449900000000000111994444944944494499000000000001119
    9999999999999999900000000000011999999999999999999000000000008000
    00FF0000007F0000007F0000003F000000000000000000000000000000010000
    00030000000300000007000000070000000F0000000F0000001F0000001F0000
    003F0000003F0000007F0000007F0000007F000000FF000000FF000001FF0000
    01FF000001FF000001FF000003FF000003FF000003FF000003FF800007FF}
  OldCreateOrder = True
  Position = poScreenCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 249
    Height = 97
    Shape = bsFrame
  end
  object Label3: TLabel
    Left = 94
    Top = 16
    Width = 34
    Height = 13
    Caption = 'YEAR'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label4: TLabel
    Left = 142
    Top = 16
    Width = 46
    Height = 13
    Caption = 'MONTH'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 197
    Top = 16
    Width = 26
    Height = 13
    Caption = 'DAY'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label1: TLabel
    Left = 22
    Top = 35
    Width = 68
    Height = 13
    Caption = 'Begin Date:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 22
    Top = 64
    Width = 58
    Height = 13
    Caption = 'End Date:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object OKBtn: TButton
    Left = 55
    Top = 116
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKBtnClick
  end
  object CancelBtn: TButton
    Left = 151
    Top = 116
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object MaskEdit1: TMaskEdit
    Left = 91
    Top = 32
    Width = 33
    Height = 21
    EditMask = '0000;1;_'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    MaxLength = 4
    ParentFont = False
    TabOrder = 2
    Text = '1967'
  end
  object UpDown1: TUpDown
    Left = 125
    Top = 32
    Width = 15
    Height = 21
    Associate = MaskEdit1
    Min = 1900
    Max = 9999
    Position = 1967
    TabOrder = 3
    Thousands = False
    Wrap = False
    OnClick = UpDown1Click
  end
  object ComboBox1: TComboBox
    Left = 142
    Top = 32
    Width = 50
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ItemHeight = 13
    ParentFont = False
    TabOrder = 4
    OnChange = ComboBox1Change
    Items.Strings = (
      '01'
      '02'
      '03'
      '04'
      '05'
      '06'
      '07'
      '08'
      '09'
      '10'
      '11'
      '12')
  end
  object ComboBox2: TComboBox
    Left = 191
    Top = 32
    Width = 50
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ItemHeight = 13
    ParentFont = False
    TabOrder = 5
  end
  object MaskEdit2: TMaskEdit
    Left = 91
    Top = 62
    Width = 33
    Height = 21
    EditMask = '0000;1;_'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    MaxLength = 4
    ParentFont = False
    TabOrder = 6
    Text = '1972'
  end
  object UpDown2: TUpDown
    Left = 125
    Top = 62
    Width = 15
    Height = 21
    Associate = MaskEdit2
    Min = 1900
    Max = 9999
    Position = 1972
    TabOrder = 7
    Thousands = False
    Wrap = False
    OnClick = UpDown2Click
  end
  object ComboBox3: TComboBox
    Left = 142
    Top = 62
    Width = 50
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ItemHeight = 13
    ParentFont = False
    TabOrder = 8
    OnChange = ComboBox3Change
    Items.Strings = (
      '01'
      '02'
      '03'
      '04'
      '05'
      '06'
      '07'
      '08'
      '09'
      '10'
      '11'
      '12')
  end
  object ComboBox4: TComboBox
    Left = 191
    Top = 62
    Width = 50
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ImeName = #32043#20809#25340#38899#36755#20837#27861
    ItemHeight = 13
    ParentFont = False
    TabOrder = 9
    Items.Strings = (
      '01'
      '02'
      '03'
      '04'
      '05'
      '06'
      '07'
      '08'
      '09'
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
      '24'
      '25'
      '26'
      '27'
      '28'
      '29'
      '30'
      '31')
  end
end
