//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "workdir.h"
#include "DM1.h"
#include "DirectoryDialog.h"
#include "GlobalVariables.h"  //gstrDir
//---------------------------------------------------------------------
#pragma resource "*.dfm"
TWorkDirDlg *WorkDirDlg;
//---------------------------------------------------------------------
__fastcall TWorkDirDlg::TWorkDirDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------



void __fastcall TWorkDirDlg::FormShow(TObject *Sender)
{

        DM->ADOTable1->Active = false;
        DM->ADOTable1->TableName = "WorkDir";
        DM->ADOTable1->Active = true;
        DBCtrlGrid1->DataSource = DM->DS1;
        DBEdit_gridname->DataField = "gridname";
        DBEdit_gridsize->DataField = "gridsize";
        DBEdit_unit->DataField = "unit";
        DBEdit_xcorner->DataField = "xcorner";
        DBEdit_ycorner->DataField = "ycorner";
        DBEdit_ncols->DataField = "ncols";
        DBEdit_nrows->DataField = "nrows";
        DBEdit_dirwork->DataField = "dirwork";
        DBEdit_dirrain->DataField = "dirrain";
        DBEdit_dirresult->DataField = "dirresult";
        DBEdit_dirsm->DataField = "dirsm";

        /*
        DBComboBox1->Items->Clear();
        for(int i=0;i<DM->ADOTable1->RecordCount;i++)
        {
                DBComboBox1->Items->Add(DBComboBox1->Field->AsString);
                DM->ADOTable1->MoveBy(1);
        }  */
        DM->ADOTable1->MoveBy(-1*DM->ADOTable1->RecordCount);
        DBNavigator1->BtnClick(nbEdit);

}
//---------------------------------------------------------------------------
/*
void __fastcall TWorkDirDlg::DBComboBox1Change(TObject *Sender)
{
        String strAlias,strTable,strField,strFilter;

        strAlias = DM->ADOConnection1->DefaultDatabase;
        strTable = "workdir";
        strField = "gridname";
        strFilter = strField + "= '" + DBComboBox1->Text + "'";
        Screen->Cursor=crHourGlass;
        if(DM->ADOTable1->Active) DM->ADOTable1->Close();
        DM->ADOTable1->Filter = strFilter;
        DM->ADOTable1->Filtered = true;
        DM->ADOTable1->Active=true;
        DBNavigator1->BtnClick(nbEdit);
        Screen->Cursor=crDefault;
}  */
//---------------------------------------------------------------------------


void __fastcall TWorkDirDlg::BitBtn1Click(TObject *Sender)
{
         //DBNavigator1->BtnClick(nbEdit);
         if(DirectoryDlg->ShowModal()==mrOk)
                {DBEdit_dirwork->Text = gstrDir;}
}
//---------------------------------------------------------------------------


void __fastcall TWorkDirDlg::BitBtn2Click(TObject *Sender)
{
         //DBNavigator1->BtnClick(nbEdit);
         if(DirectoryDlg->ShowModal()==mrOk)
                {DBEdit_dirrain->Text = gstrDir;}
}
//---------------------------------------------------------------------------

void __fastcall TWorkDirDlg::BitBtn3Click(TObject *Sender)
{
        //DBNavigator1->BtnClick(nbEdit);
        if(DirectoryDlg->ShowModal()==mrOk)
                {DBEdit_dirresult->Text = gstrDir;}
}
//---------------------------------------------------------------------------

void __fastcall TWorkDirDlg::BitBtn4Click(TObject *Sender)
{
         //DBNavigator1->BtnClick(nbEdit);
         if(DirectoryDlg->ShowModal()==mrOk)
                {DBEdit_dirsm->Text = gstrDir;}
}
//---------------------------------------------------------------------------

void __fastcall TWorkDirDlg::OKBtnClick(TObject *Sender)
{
        //DBNavigator1->BtnClick(nbPost);
        gstrDir = DBEdit_gridname->Text;  //记录选择的水文单元划分方案
        //DBEdit_gridname->DataSource->DataSet->Active = false;
}
//---------------------------------------------------------------------------




void __fastcall TWorkDirDlg::CancelBtnClick(TObject *Sender)
{
        //DBEdit_gridname->DataSource->DataSet->Active = false;
}
//---------------------------------------------------------------------------



