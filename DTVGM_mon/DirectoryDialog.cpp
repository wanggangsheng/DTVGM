//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DirectoryDialog.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TDirectoryDlg *DirectoryDlg;
String gstrDir; //wgs
//---------------------------------------------------------------------
__fastcall TDirectoryDlg::TDirectoryDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TDirectoryDlg::OKBtnClick(TObject *Sender)
{
        gstrDir = Label_dir->Caption + "\\";
        
}
//---------------------------------------------------------------------------




void __fastcall TDirectoryDlg::DirectoryListBox1Change(TObject *Sender)
{
        Label_dir->Caption = DirectoryListBox1->Directory;        
}
//---------------------------------------------------------------------------

