object FormQuery: TFormQuery
  Left = 218
  Top = 169
  Width = 561
  Height = 384
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Query Demo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  Visible = True
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 553
    Height = 297
    ActivePage = TabSheet1
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'DataBase'
      object Label1: TLabel
        Left = 8
        Top = 16
        Width = 85
        Height = 13
        Caption = 'Database Aliases:'
      end
      object Label2: TLabel
        Left = 184
        Top = 16
        Width = 35
        Height = 13
        Caption = 'Tables:'
      end
      object Label3: TLabel
        Left = 367
        Top = 16
        Width = 30
        Height = 13
        Caption = 'Fields:'
      end
      object Label4: TLabel
        Left = 8
        Top = 316
        Width = 27
        Height = 13
        Caption = 'Type:'
      end
      object Label5: TLabel
        Left = 171
        Top = 73
        Width = 22
        Height = 13
        Caption = '==>'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label6: TLabel
        Left = 353
        Top = 73
        Width = 22
        Height = 13
        Caption = '==>'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object ListBox1: TListBox
        Left = 8
        Top = 32
        Width = 161
        Height = 105
        ImeName = #32043#20809#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 0
        OnClick = ListBox1Click
      end
      object ListBox2: TListBox
        Left = 192
        Top = 32
        Width = 161
        Height = 105
        ImeName = #32043#20809#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 1
        OnClick = ListBox2Click
      end
      object ListBox3: TListBox
        Left = 374
        Top = 32
        Width = 161
        Height = 105
        ImeName = #32043#20809#25340#38899#36755#20837#27861
        ItemHeight = 13
        TabOrder = 2
        OnClick = ListBox3Click
      end
      object BitBtn1: TBitBtn
        Left = 432
        Top = 244
        Width = 89
        Height = 25
        Caption = '&Run Query'
        Default = True
        ModalResult = 5
        TabOrder = 3
        OnClick = BitBtn1Click
        Glyph.Data = {
          DE010000424DDE01000000000000760000002800000024000000120000000100
          0400000000006801000000000000000000001000000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00344446333334
          44433333FFFF333333FFFF33000033AAA43333332A4333338833F33333883F33
          00003332A46333332A4333333383F33333383F3300003332A2433336A6633333
          33833F333383F33300003333AA463362A433333333383F333833F33300003333
          6AA4462A46333333333833FF833F33330000333332AA22246333333333338333
          33F3333300003333336AAA22646333333333383333F8FF33000033444466AA43
          6A43333338FFF8833F383F330000336AA246A2436A43333338833F833F383F33
          000033336A24AA442A433333333833F33FF83F330000333333A2AA2AA4333333
          333383333333F3330000333333322AAA4333333333333833333F333300003333
          333322A4333333333333338333F333330000333333344A433333333333333338
          3F333333000033333336A24333333333333333833F333333000033333336AA43
          33333333333333833F3333330000333333336663333333333333333888333333
          0000}
        NumGlyphs = 2
      end
      object DBGrid1: TDBGrid
        Left = 8
        Top = 144
        Width = 529
        Height = 97
        Hint = 
          'Operator'#65309'BETWEEN'#26102', FieldValue'#20013#38388#21152'"AND";Operator=IN'#26102',FieldValue'#20013#38388#21152 +
          '","'
        DataSource = DM.DS1
        Enabled = False
        ImeMode = imChinese
        PopupMenu = PopupMenu1
        TabOrder = 4
        TitleFont.Charset = DEFAULT_CHARSET
        TitleFont.Color = clWindowText
        TitleFont.Height = -11
        TitleFont.Name = 'MS Sans Serif'
        TitleFont.Style = []
        OnCellClick = DBGrid1CellClick
        Columns = <
          item
            Expanded = False
            FieldName = 'QueryID'
            Width = 50
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'FieldName'
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'Operator'
            PickList.Strings = (
              'NoCondition'
              '='
              '<>'
              '<'
              '<='
              '>'
              '>='
              'BETWEEN'
              'LIKE X%'
              'NOT LIKE X%'
              'LIKE %X'
              'NOT LIKE %X'
              'LIKE %X%'
              'NOT LIKE %X%'
              'IS NULL'
              'IS NOT NULL'
              'IN'
              'NOT IN')
            Width = 80
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'FieldValue'
            Width = 200
            Visible = True
          end
          item
            Expanded = False
            FieldName = 'Logical'
            PickList.Strings = (
              'AND'
              'OR')
            Visible = True
          end>
      end
      object BitBtn3: TBitBtn
        Left = 40
        Top = 244
        Width = 89
        Height = 25
        Caption = 'Set &Filter'
        TabOrder = 5
        Visible = False
        OnClick = BitBtn3Click
        Kind = bkRetry
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'SQL Edit'
      ImageIndex = 1
      object RichEdit1: TRichEdit
        Left = 0
        Top = 0
        Width = 545
        Height = 233
        Font.Charset = GB2312_CHARSET
        Font.Color = clFuchsia
        Font.Height = -12
        Font.Name = 'Times New Roman'
        Font.Style = []
        ImeName = #32043#20809#25340#38899#36755#20837#27861
        ParentFont = False
        ScrollBars = ssBoth
        TabOrder = 0
      end
      object btnQuery: TBitBtn
        Left = 200
        Top = 240
        Width = 75
        Height = 25
        Caption = 'Excute'
        TabOrder = 1
        OnClick = btnQueryClick
        Glyph.Data = {
          42020000424D4202000000000000420000002800000010000000100000000100
          1000030000000002000000000000000000000000000000000000007C0000E003
          00001F000000FF7FFF7F0000000000000000FF7FFF7FFF7FFF7F000000000000
          0000FF7FFF7FFF7F00004208AD35AD3542080000FF7FFF7F00004208AD35AD35
          42080000FF7F00004208A514B556B556A5144208203D203D4208A514B556B556
          A51442080000203D82494208AD35AD3542088249607E607E82494208AD35AD35
          42088249203D203D607EE22800000000E228607E607E607E607EE22800000000
          E228607E032D0000203D203D203D203D203D203D203D203DC05D607E69666966
          69662C528410801CC67A807EC67A807EC67A807EC67AE05D2025C05D2C52607E
          607E31468410203D4D77C07E4D77C07E4D77C07E4D77C07E2025203D2431203D
          203DA841C228203D4D77C07E4D77C07E4D77C07E4D77C07E2025E710C07CC07C
          375BF73A0000203D4D77C07E4D77C07E4D77C07E4D77C07E2025EF25D3666E72
          FF4FFF4F0000203D4D77C07E4D77C07E4D77C07E4D77C07E2025EF25D34EEF51
          3667F73A0000203D4D77C07E4D77C07E4D77C07E4D77C07E2025EF258F67207F
          6773EF25FF7F801CC67A807EC67A807EC67A807EC67A807E20318725EF25EF25
          EF25E710FF7FFF7F0000000000000000000000000000801C607E801C00000000
          00000000FF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7F0000203D203D203D
          203D801CFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7F
          FF7FFF7FFF7F}
      end
      object BitBtn5: TBitBtn
        Left = 292
        Top = 240
        Width = 75
        Height = 25
        Caption = 'Load File'
        TabOrder = 2
        OnClick = BitBtn5Click
        Glyph.Data = {
          42020000424D4202000000000000420000002800000010000000100000000100
          1000030000000002000000000000000000000000000000000000007C0000E003
          00001F000000FF7FFF7FFF7FFF7FFF7F000000000000000000000000FF7FFF7F
          FF7FFF7FFF7FFF7FFF7FFF7F000000000064607C407C637DE27CC96900000000
          FF7FFF7FFF7FFF7FFF7F00000A5800600074807CA07C647DC77DA97D646A6762
          0000FF7FFF7FFF7F00001050035C0564007C807CC07CA47DCB7DA67E467ECC6E
          8C620000FF7FFF7F00000F541060026C007CA07C237DCA7D877E687E4E7FC77E
          EC6A0000FF7F0000164418400E5C0E6805680170897144768D7A0A7FE87E2B7F
          2377E562000000001E3C1E401C4814400D5C265C00000000CC6E8976647F617F
          A37F8177000000003F2C1F2C3F2C1B48164C0000FF7FFF7F0000A066C06E4077
          60778077000000005F303F283F303F28DB380000FF7FFF7F0000404E8056C05E
          6073A077000000001F31FF341F351F3D1E39194200000000C02EA025203EE045
          A056A05A000000003A3D1F391F3D1F3DDF35FC39F5222C2F001F0013E021401E
          E045604A0000FF7F00007A451F46FF417F3A7E22521B8A2BC017401B2013C025
          202A0000FF7FFF7F0000184ADB3DDF357F3ABF165B37F21BE937C013C01AA00E
          40360000FF7FFF7FFF7F000078425A2E9E1ADE129B33F21FE93B801B00238032
          0000FF7FFF7FFF7FFF7FFF7F000000005A1EBD069C3BF523AA3F272B00000000
          FF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7F000000000000000000000000FF7FFF7F
          FF7FFF7FFF7F}
      end
      object BitBtn6: TBitBtn
        Left = 372
        Top = 240
        Width = 75
        Height = 25
        Caption = 'Save File'
        TabOrder = 3
        OnClick = BitBtn6Click
        Glyph.Data = {
          42020000424D4202000000000000420000002800000010000000100000000100
          1000030000000002000000000000000000000000000000000000007C0000E003
          00001F000000FF7F203920392039203920392039203920392039203920392039
          2039FF7FFF7F8049C618E05D396739673967396739677B6F7B6F7B6FDE7B967F
          C618C028FF7F8049E97EE05D1863396739673967396739677B6F7B6F7B6F967F
          E97EC028FF7F80490D7FE05DD65A18631863186339673967396739677B6F967F
          E97EC028FF7F80490D7FE05DD65AD65AD65A186339673967396739671863967F
          E97EC028FF7F80490D7FE05D524A9452D65AD65A1863396718631863D65A967F
          E97EC028FF7F80490D7FE05D524A524A94529452D65A18631863D65AD65A967F
          E97EC028FF7F8049517FE05DE05DE05DE05DE05DE05DE05DE05DE05DE05D517F
          E97EC028FF7F8049517F517F517F517F517F517F517F517F517F517F517F967F
          E97EC028FF7F8049517FE05DA07EE05DE05DE05DE05DE05DE05DE05DE97E517F
          E97EC028FF7F80490D7FE05DFF7F8049FF7FDE7BDE7BD65AD65ADE7BE05D517F
          0D7FC028FF7F8049517FE05DFF7F8049FF7F7B6F7B6F10421042DE7B8049517F
          0D7FC028FF7F8049E97EE05DFF7F8049FF7F7B6F7B6FAD35524A7B6F8049517F
          0D7FC028FF7F8049517FE05DFF7F8049FF7F9C737B6FC618C618DE7B8049517F
          0D7FC028FF7F8049967FE05D7B6FE05DFF7FFF7FFF7FFF7FFF7FFF7F8049967F
          2039FF7FFF7FFF7F104210421042104210421042104210421042104210421042
          FF7FFF7FFF7F}
      end
      object BitBtn7: TBitBtn
        Left = 464
        Top = 240
        Width = 75
        Height = 25
        Caption = 'Clear'
        TabOrder = 4
        OnClick = BitBtn7Click
        Glyph.Data = {
          42020000424D4202000000000000420000002800000010000000100000000100
          1000030000000002000000000000000000000000000000000000007C0000E003
          00001F000000FF7FFF7FFF7FFF7FFF7FFF7F0000000000000000FF7FFF7FFF7F
          FF7FFF7FFF7FFF7FFF7FFF7FFF7F000008210821CE39CE39082108210000FF7F
          FF7FFF7FFF7FFF7FFF7FFF7F00000821524A524A8C318C31524A524A08210000
          FF7FFF7FFF7FFF7FFF7FFF7F000018638C31A6356C4E524AAD358C3118630000
          FF7FFF7FFF7FFF7FFF7FFF7F8C31C6188C31AD77EF3DE03D6C6FEF3DC6188C31
          FF7FFF7FFF7FFF7FFF7FFF7FC618524AAD77E71C6C4EA656E01CAD77AD56C618
          FF7FFF7FFF7FFF7FFF7F0000524A8C31AD35AD77E73DE73DAD77E03DA635524A
          0000FF7FFF7FFF7FFF7F00008C31AD35EF5EE03DE75EAD568C31E75EA6358C31
          0000FF7FFF7FFF7FFF7FC6188C316C4EA635E01CE01CE71CE01CE73D6C4E8C31
          C618FF7FFF7FFF7FFF7FC618C618E73DE75EE003E003E003602D6C4EE71CC618
          C618FF7FFF7FFF7F0000EF3DE71CE003E003E003E003E75EA6566C6FE71CF75E
          E71C0000FF7FFF7F0000E71CE73DE003E003E003E003E73D2667A635F75EE71C
          00000000FF7FFF7FFF7F0000E003E003E003E003E003602DE07FC618E71C0000
          0000FF7FFF7FFF7FFF7F0000000000008010602D602DE07FE07F0000FF7FFF7F
          FF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7F00000000A635E07FA635FF7FFF7FFF7F
          FF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7FFF7F00000000FF7FFF7FFF7F
          FF7FFF7FFF7F}
      end
    end
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 300
    Width = 433
    Height = 41
    Caption = 'Options:'
    TabOrder = 1
    object CheckBox1: TCheckBox
      Left = 13
      Top = 16
      Width = 137
      Height = 17
      Caption = 'Show File Extensions'
      Enabled = False
      TabOrder = 0
    end
    object CheckBox2: TCheckBox
      Left = 164
      Top = 16
      Width = 225
      Height = 17
      Caption = 'Show System Table for Remote Aliases'
      Enabled = False
      TabOrder = 1
    end
  end
  object BitBtn2: TBitBtn
    Left = 464
    Top = 312
    Width = 73
    Height = 25
    TabOrder = 2
    Kind = bkClose
  end
  object PopupMenu1: TPopupMenu
    Left = 144
    Top = 80
    object Append1: TMenuItem
      Caption = 'Append'
      Hint = 'Append a Record'
      OnClick = Append1Click
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object Delete1: TMenuItem
      Caption = 'Delete'
      Hint = 'Delete a Record'
      OnClick = Delete1Click
    end
    object DeleteAll1: TMenuItem
      Caption = 'Delete All'
      Hint = 'Delete all Records'
      OnClick = DeleteAll1Click
    end
  end
end
