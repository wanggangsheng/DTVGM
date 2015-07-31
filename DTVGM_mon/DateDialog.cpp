//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DateDialog.h"
#include <dateutils.hpp> //wgs
#include "GlobalVariables.h"      //gdateBegin,gdateEnd
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TDateDlg *DateDlg;
//---------------------------------------------------------------------
__fastcall TDateDlg::TDateDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TDateDlg::ComboBox1Change(TObject *Sender)
{
 //  StrToDate("yyyy-m-d")
        int idays;
        ComboBox2->Clear();
        idays = DaysInAMonth(MaskEdit1->Text.ToInt(),ComboBox1->Text.ToInt());

        for(int i=0;i<idays;i++)
                ComboBox2->Items->Add(IntToStr(i + 1));
        ComboBox2->ItemIndex = idays - 1;        
}
//---------------------------------------------------------------------------

void __fastcall TDateDlg::ComboBox3Change(TObject *Sender)
{
//  StrToDate("yyyy-m-d")
        int idays;
        ComboBox4->Clear();
        idays = DaysInAMonth(MaskEdit2->Text.ToInt(),ComboBox3->Text.ToInt());
        for(int i=0;i<idays;i++)
                ComboBox4->Items->Add(IntToStr(i + 1));
        ComboBox4->ItemIndex = idays - 1;
}
//---------------------------------------------------------------------------

void __fastcall TDateDlg::OKBtnClick(TObject *Sender)
{
        //ModalResult = mrOk;
        gdateBegin = StrToDate(MaskEdit1->Text + "-" + ComboBox1->Text + "-" + ComboBox2->Text);
        gdateEnd   = StrToDate(MaskEdit2->Text + "-" + ComboBox3->Text + "-" + ComboBox4->Text);
        if(gdateBegin > gdateEnd)
                {ShowMessage("Begin Date ("+ DateToStr(gdateBegin) + ") > End Date ("+ DateToStr(gdateEnd) + "), Pls Check!");
                 ModalResult = mrCancel;
                 return;
                }

}
//---------------------------------------------------------------------------







void __fastcall TDateDlg::UpDown1Click(TObject *Sender,
      TUDBtnType Button)
{
        ComboBox1Change(Sender);        
}
//---------------------------------------------------------------------------

void __fastcall TDateDlg::UpDown2Click(TObject *Sender,
      TUDBtnType Button)
{
        ComboBox3Change(Sender);        
}
//---------------------------------------------------------------------------


void __fastcall TDateDlg::FormCreate(TObject *Sender)
{
        MaskEdit1->Text = YearOf(Date()) - 1;
        MaskEdit2->Text = YearOf(Date());
        ComboBox1->Clear();
        ComboBox3->Clear();
        ComboBox2->Clear();
        ComboBox4->Clear();
        for(int i=0;i<12;i++)
                {ComboBox1->Items->Add(IntToStr(i + 1));
                 ComboBox3->Items->Add(IntToStr(i + 1));
                }
        for(int i=0;i<31;i++)
                {ComboBox2->Items->Add(IntToStr(i + 1));
                 ComboBox4->Items->Add(IntToStr(i + 1));
                }
        ComboBox1->ItemIndex = 0;   //January
        ComboBox3->ItemIndex = 11;  //December
        ComboBox2->ItemIndex = 0;
        ComboBox4->ItemIndex = 30;
}
//---------------------------------------------------------------------------

