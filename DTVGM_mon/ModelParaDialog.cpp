//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <StrUtils.hpp>  //leftstr()
//#include <Classes.hpp>

#include "ModelParaDialog.h"
#include "DM1.h"
#include "GlobalVariables.h"     // gstrBasinSelect
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TModelParaDlg *ModelParaDlg;
//---------------------------------------------------------------------
__fastcall TModelParaDlg::TModelParaDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------

void __fastcall TModelParaDlg::FormShow(TObject *Sender)
{
        ShowTable_ModelParameters();
}
//---------------------------------------------------------------------------



void __fastcall TModelParaDlg::Button1Click(TObject *Sender)
{
        if(DM->ADOTable0->Filtered) DM->ADOTable0->Filtered = false;
        DM->ADOTable0->Active = false;
        DM->ADOTable0->TableName = "DTVGMlog";
        DM->ADOTable0->Active = true;
        DM->ADOTable0->Sort = "RunStart ASC";
}
//---------------------------------------------------------------------------

void __fastcall TModelParaDlg::DBGrid1DblClick(TObject *Sender)
{
        String strTableName,strParaEn;
        double AW0,AWC,g1,g2,Kr,KAW,PDc,PDc_basic,KET;
        strTableName = DM->ADOTable0->TableName;
        if(strTableName == "DTVGMlog")
        {
                AW0 = DM->ADOTable0->FieldValues["AW0"];
                AWC = DM->ADOTable0->FieldValues["AWC"];
                g1 = DM->ADOTable0->FieldValues["g1"];
                g2 = DM->ADOTable0->FieldValues["g2"];
                Kr = DM->ADOTable0->FieldValues["Kr"];
                KAW = DM->ADOTable0->FieldValues["KAW"];
                PDc = DM->ADOTable0->FieldValues["PDc"];
                PDc_basic = DM->ADOTable0->FieldValues["PDc_basic"];
                KET = DM->ADOTable0->FieldValues["KET"];

                ShowTable_ModelParameters();

                for(int i=0;i<DM->ADOTable0->RecordCount;i++)
                {DM->ADOTable0->Edit();
                 strParaEn = DM->ADOTable0->FieldValues["ParaEn"];
                 if(strParaEn == "aAW0")
                        DM->ADOTable0->FieldValues["ParaValue"] = AW0;
                 else if (strParaEn == "bAWC")
                        DM->ADOTable0->FieldValues["ParaValue"] = AWC;
                 else if (strParaEn == "ag1")
                        DM->ADOTable0->FieldValues["ParaValue"] = g1;
                 else if (strParaEn == "ag2")
                        DM->ADOTable0->FieldValues["ParaValue"] = g2;
                 else if (strParaEn == "aKr")
                        DM->ADOTable0->FieldValues["ParaValue"] = Kr;
                 else if (strParaEn == "bPDc")
                        DM->ADOTable0->FieldValues["ParaValue"] = PDc;
                 else if(strParaEn == "bPDc_basic")
                        DM->ADOTable0->FieldValues["ParaValue"] = PDc_basic;
                 else if(strParaEn == "bKET")
                        DM->ADOTable0->FieldValues["ParaValue"] = KET;
                 else if(strParaEn == "aKAW")
                        DM->ADOTable0->FieldValues["ParaValue"] = KAW;

                 DM->ADOTable0->Next();
                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TModelParaDlg::ShowTable_ModelParameters()
{
        String strQuery,strStation,strFilter;
        TStrings *StringList = new TStringList();

        DM->ADOTable0->Active = false;
        DM->ADOTable0->TableName = "ModelParameters";
        strFilter = "Scale = 'Month'";
        DM->ADOTable0->Filter = strFilter;
        DM->ADOTable0->Filtered = true;
        DM->ADOTable0->Active = true;

        DBGrid1->Columns->Items[2]->Font->Color = clFuchsia;
        DBGrid1->Columns->Items[5]->Font->Color = clBlue;
        DBGrid1->Columns->Items[2]->Font->Style = TFontStyles()<< fsBold << fsUnderline;
        //DBCtrlGrid1->DataSource = DM->DS1;

        strQuery = "SELECT station,stationchina FROM HydroMeteoStation WHERE class = 'outlet'";
        ComboBox1->Items->Clear();
        DM->ADOQuery1->Active = false;
        DM->ADOQuery1->SQL->Clear();
        DM->ADOQuery1->SQL->Add(strQuery);
        DM->ADOQuery1->Prepared;  //大大加快查询速度
        DM->ADOQuery1->Active = true;
        DM->ADOQuery1->First();
        try
        {
        for(int i=0;i<DM->ADOQuery1->RecordCount;i++)
        {
                strStation = String(DM->ADOQuery1->FieldValues["station"])
                             + ": " + String(DM->ADOQuery1->FieldValues["stationchina"]);
                StringList->Add(strStation);
                DM->ADOQuery1->Next();
        }
        DM->ADOQuery1->Close();

        ComboBox1->Items->Assign(StringList);
        ComboBox1->ItemIndex = 0;
        } //try
        __finally 
        {
                delete StringList;
        }
        gstrBasinSelect = LeftStr(String(ComboBox1->Items->Strings[ComboBox1->ItemIndex]),4);
        RegionCCFilter(gstrBasinSelect);
        ////ShowMessage(gstrBasinSelect);
}





void __fastcall TModelParaDlg::ComboBox1Change(TObject *Sender)
{
        gstrBasinSelect = LeftStr(String(ComboBox1->Items->Strings[ComboBox1->ItemIndex]),4);
        RegionCCFilter(gstrBasinSelect);

        ////ShowMessage(gstrBasinSelect);
}
//---------------------------------------------------------------------------

void __fastcall TModelParaDlg::RegionCCFilter(String Outlet)
{
        TClass ClassRef;
        TADOTable *tADOTable;
        DBGrid2->DataSource->DataSet->Active = false;
        ClassRef = DBGrid2->DataSource->DataSet->ClassType();
        if(String(ClassRef->ClassName()) != "TADOTable")
        {
                ShowMessage("The DataSet is not a TADOTable");
                return;
        }
        else
                tADOTable = DM->ADOTable2;

        tADOTable->TableName = "RegionCC";
        tADOTable->Filter = "Outlet = '" + Outlet + "'";
        tADOTable->Filtered = true;
        tADOTable->Active=true;
        //tADOTable->Edit();
}



void __fastcall TModelParaDlg::OKBtnClick(TObject *Sender)
{
        DM->ADOTable2->Close();
        blVar = CheckBox1->Checked;
        ////DM->ADOTable2->TableName = "DTVGMlog";
        ////DM->ADOTable2->Active = true;
}
//---------------------------------------------------------------------------




void __fastcall TModelParaDlg::Button2Click(TObject *Sender)
{
        ModalResult = mrNone;
        this->Close();
}
//---------------------------------------------------------------------------

