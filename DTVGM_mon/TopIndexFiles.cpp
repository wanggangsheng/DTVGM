//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TopIndexFiles.h"
#include "DirectoryDialog.h"//wgs

//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TDlgTOPIndex *DlgTOPIndex;
//---------------------------------------------------------------------
__fastcall TDlgTOPIndex::TDlgTOPIndex(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TDlgTOPIndex::BitBtn1Click(TObject *Sender)
{
        if(DirectoryDlg->ShowModal()==mrOk)
                {EdtINPUT->Text = DirectoryDlg->Label_dir->Caption + "\\"  + DirectoryDlg->EditFile->Text;}
}
//---------------------------------------------------------------------------

void __fastcall TDlgTOPIndex::BitBtn2Click(TObject *Sender)
{
        if(DirectoryDlg->ShowModal()==mrOk)
                {EdtOutMap->Text = DirectoryDlg->Label_dir->Caption + "\\"  + DirectoryDlg->EditFile->Text;}
}
//---------------------------------------------------------------------------

void __fastcall TDlgTOPIndex::BitBtn3Click(TObject *Sender)
{
        if(DirectoryDlg->ShowModal()==mrOk)
                {EdtOutDitn->Text = DirectoryDlg->Label_dir->Caption + "\\"  + DirectoryDlg->EditFile->Text;}
}
//---------------------------------------------------------------------------

