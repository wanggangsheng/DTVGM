object frmHydroGraph: TfrmHydroGraph
  Left = 206
  Top = 121
  Width = 755
  Height = 569
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'HydroGraph'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 94
    Top = 236
    Width = 16
    Height = 13
    Caption = 'File'
  end
  object Notebook1: TNotebook
    Left = 0
    Top = 255
    Width = 745
    Height = 249
    TabOrder = 5
    object TPage
      Left = 0
      Top = 0
      Caption = 'Discharge'
      object Chart1: TChart
        Left = 0
        Top = 0
        Width = 713
        Height = 249
        BackWall.Brush.Color = clWhite
        BackWall.Brush.Style = bsClear
        Border.Visible = True
        Legend.Alignment = laBottom
        Title.Text.Strings = (
          #38477#27700#65293#24452#27969#36807#31243#32447)
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Maximum = 297
        LeftAxis.Title.Caption = 'Qobs-Qcal (m3/s)'
        LeftAxis.Title.Font.Charset = ANSI_CHARSET
        LeftAxis.Title.Font.Color = clFuchsia
        LeftAxis.Title.Font.Height = -12
        LeftAxis.Title.Font.Name = 'Times New Roman'
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        RightAxis.Inverted = True
        RightAxis.Maximum = 87
        RightAxis.Title.Angle = 90
        RightAxis.Title.Caption = 'Rain-SoilMoisture (mm)'
        RightAxis.Title.Font.Color = clFuchsia
        View3D = False
        TabOrder = 0
        object Series_qobs: TLineSeries
          Marks.Callout.Brush.Color = clBlack
          Marks.Visible = False
          SeriesColor = clBlack
          Title = 'Series_Qobs'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series_qcal: TLineSeries
          Marks.Callout.Brush.Color = clBlack
          Marks.Visible = False
          Title = 'Series_Qcal'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series_P: TBarSeries
          BarBrush.Color = clWhite
          Marks.Callout.Brush.Color = clBlack
          Marks.Visible = False
          Title = 'Series_Rain'
          VertAxis = aRightAxis
          Gradient.Direction = gdTopBottom
          SideMargins = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
        object Series_sm: TLineSeries
          Marks.Callout.Brush.Color = clBlack
          Marks.Visible = False
          SeriesColor = clBlue
          Title = 'Series_SoilMoisture'
          VertAxis = aRightAxis
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
    object TPage
      Left = 0
      Top = 0
      Caption = 'Runoff'
      object Chart2: TChart
        Left = 0
        Top = 0
        Width = 735
        Height = 249
        BackWall.Brush.Color = clWhite
        Border.Visible = True
        Legend.Alignment = laBottom
        Title.Text.Strings = (
          #22320#34920#24452#27969#65293#22320#19979#24452#27969#65293#24635#24452#27969#65293#33976#25955#21457)
        LeftAxis.Title.Caption = 'Runff (mm)'
        RightAxis.LabelStyle = talValue
        RightAxis.Title.Angle = 90
        RightAxis.Title.Caption = 'ETa (mm)'
        View3D = False
        View3DOptions.Elevation = 315
        View3DOptions.Orthogonal = False
        View3DOptions.Perspective = 0
        View3DOptions.Rotation = 360
        TabOrder = 0
        object Series_Rd: TLineSeries
          Marks.Callout.Brush.Color = clBlack
          Marks.Visible = False
          SeriesColor = clBlue
          Title = 'Series_SurfaceRunoff'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psTriangle
          Pointer.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series_Rs: TLineSeries
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Length = 20
          Marks.Visible = False
          SeriesColor = 4227327
          Title = 'Series_SubSurfaceRunoff'
          Brush.Style = bsHorizontal
          LineBrush = bsHorizontal
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psSmallDot
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series_R: TLineSeries
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Length = 20
          Marks.Visible = False
          Title = 'Series_Runoff'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psCross
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
        object Series_ETa: TLineSeries
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Length = 20
          Marks.Visible = False
          SeriesColor = clTeal
          Title = 'Series_ETa'
          VertAxis = aRightAxis
          LinePen.Style = psDot
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
        end
      end
    end
  end
  object StringGrid1: TStringGrid
    Left = 0
    Top = 0
    Width = 745
    Height = 225
    ColCount = 7
    DefaultColWidth = 80
    DefaultRowHeight = 20
    FixedCols = 6
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 0
  end
  object RadioGroup1: TRadioGroup
    Left = 448
    Top = 226
    Width = 281
    Height = 28
    TabOrder = 1
  end
  object RadioButton1: TRadioButton
    Left = 456
    Top = 235
    Width = 121
    Height = 17
    Caption = #38477#27700#65293#24452#27969#36807#31243#32447
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -12
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 592
    Top = 235
    Width = 113
    Height = 17
    Caption = #20135#27969#20998#37327
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -12
    Font.Name = #23435#20307
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = RadioButton2Click
  end
  object BitBtn1: TBitBtn
    Left = 5
    Top = 229
    Width = 81
    Height = 24
    Caption = 'Load Data'
    TabOrder = 4
    OnClick = BitBtn1Click
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
end
