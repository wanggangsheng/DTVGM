inherited DlgMCMCINI: TDlgMCMCINI
  Left = 383
  Top = 187
  Caption = 'MCMCINI'
  ClientHeight = 324
  ClientWidth = 290
  PixelsPerInch = 96
  TextHeight = 13
  inherited Bevel1: TBevel
    Left = 19
    Top = 184
  end
  inherited Label3: TLabel
    Left = 105
    Top = 192
  end
  inherited Label4: TLabel
    Left = 153
    Top = 192
  end
  inherited Label5: TLabel
    Left = 208
    Top = 192
  end
  inherited Label1: TLabel
    Left = 33
    Top = 211
  end
  inherited Label2: TLabel
    Left = 33
    Top = 240
  end
  object Label6: TLabel [6]
    Left = 168
    Top = 152
    Width = 38
    Height = 13
    Caption = 'Sigma ='
  end
  object Label7: TLabel [7]
    Left = 16
    Top = 152
    Width = 52
    Height = 13
    Caption = 'Samples = '
  end
  object RadioGroup2: TRadioGroup [8]
    Left = 144
    Top = 8
    Width = 137
    Height = 137
    Caption = 'Proposal Distribution'
    ItemIndex = 0
    Items.Strings = (
      'Symmetric Uniform'
      'Asymmetric Uniform'
      'Symmetric Normal'
      'Asymmetric Normal')
    TabOrder = 10
    OnClick = RadioGroup2Click
  end
  inherited OKBtn: TButton
    Left = 66
    Top = 292
  end
  inherited CancelBtn: TButton
    Left = 162
    Top = 292
  end
  inherited MaskEdit1: TMaskEdit
    Left = 102
    Top = 208
  end
  inherited UpDown1: TUpDown
    Left = 136
    Top = 208
  end
  inherited ComboBox1: TComboBox
    Left = 153
    Top = 208
  end
  inherited ComboBox2: TComboBox
    Left = 202
    Top = 208
  end
  inherited MaskEdit2: TMaskEdit
    Left = 102
    Top = 238
  end
  inherited UpDown2: TUpDown
    Left = 136
    Top = 238
  end
  inherited ComboBox3: TComboBox
    Left = 153
    Top = 238
  end
  inherited ComboBox4: TComboBox
    Left = 202
    Top = 238
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 129
    Height = 137
    Caption = 'Parameter Selection'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    TabOrder = 11
    object CBox_g1: TCheckBox
      Left = 24
      Top = 18
      Width = 97
      Height = 17
      Caption = 'g1'
      Checked = True
      State = cbChecked
      TabOrder = 0
    end
    object CBox_g2: TCheckBox
      Left = 24
      Top = 42
      Width = 97
      Height = 17
      Caption = 'g2'
      TabOrder = 1
    end
    object CBox_Kr: TCheckBox
      Left = 24
      Top = 66
      Width = 97
      Height = 17
      Caption = 'Kr'
      Checked = True
      State = cbChecked
      TabOrder = 2
    end
    object CBox_KAW: TCheckBox
      Left = 24
      Top = 90
      Width = 97
      Height = 17
      Caption = 'KAW'
      Checked = True
      State = cbChecked
      TabOrder = 3
    end
    object CBox_AW0: TCheckBox
      Left = 24
      Top = 114
      Width = 97
      Height = 17
      Caption = 'AW0'
      TabOrder = 4
    end
  end
  object MaskEdit_Sigma: TMaskEdit
    Left = 212
    Top = 150
    Width = 63
    Height = 21
    BiDiMode = bdLeftToRight
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    ParentBiDiMode = False
    TabOrder = 12
    Text = '0.05'
  end
  object MaskEdit_Samples: TMaskEdit
    Left = 76
    Top = 150
    Width = 60
    Height = 21
    BiDiMode = bdLeftToRight
    EditMask = '######;1;_'
    ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899
    MaxLength = 6
    ParentBiDiMode = False
    TabOrder = 13
    Text = '100   '
  end
end
