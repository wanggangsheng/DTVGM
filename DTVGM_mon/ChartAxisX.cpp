//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChartAxisX.h"
#include "MPSAmap.h"
//---------------------------------------------------------------------
#pragma link "okcancl1"
#pragma resource "*.dfm"
TDlgChartEdit *DlgChartEdit;
//--------------------------------------------------------------------- 
__fastcall TDlgChartEdit::TDlgChartEdit(TComponent* AOwner)
	: TOKBottomDlg(AOwner)
{
}
//--------------------------------------------------------------------- 
void __fastcall TDlgChartEdit::HelpBtnClick(TObject *Sender)
{
	Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------



int TDlgChartEdit::EditCheck()
{
        double xmax,xmin,xinc;
        int xnum;
        
        xmax = EdtMax->Text.ToDouble();
        xmin = EdtMin->Text.ToDouble();
        xnum = EdtNum->Text.ToInt();

        if(xnum < 0)
        {
                ShowMessage("Number of Intervals must > 0.");
                return 0;
        }
        else if(xmax < xmin)
        {
                ShowMessage("Maximum must > Minimum.");
                return 0;
        }
        else
        {
                xinc = (xmax - xmin)/xnum;
                EdtInc->Text = FloatToStrF(xinc,ffFixed,4,2);
                return 1;
        }
}//end EditCheck
void __fastcall TDlgChartEdit::Button1Click(TObject *Sender)
{
        EditCheck();
}
//---------------------------------------------------------------------------

void __fastcall TDlgChartEdit::OKBtnClick(TObject *Sender)
{
        int iCheck;
        iCheck = EditCheck();
        if(iCheck == 0)
        {
                ModalResult = mrCancel;
                return;
        }
}
//---------------------------------------------------------------------------


void __fastcall TDlgChartEdit::FormCreate(TObject *Sender)
{
        ComboBox1->Items->Clear();
        for(int i=0;i<30;i++)
                ComboBox1->Items->Add(String(i + 1));
}
//---------------------------------------------------------------------------

void __fastcall TDlgChartEdit::ComboBox1Change(TObject *Sender)
{
        int i,k;
        int iChart;
        double xmax,xmin,xinc;
        int xnum;
        TChart *Charti;

        iChart = ComboBox1->Text.ToInt();

        Charti = frmMPSAmap->wgsChart[iChart - 1];
        xmax = Charti->BottomAxis->Maximum;
        xmin = Charti->BottomAxis->Minimum;
        xinc = Charti->BottomAxis->Increment;
        
        EdtMax->Text = FloatToStrF(xmax,ffFixed,10,2);
        EdtMin->Text = FloatToStrF(xmin,ffFixed,10,2);
        EdtInc->Text = FloatToStrF(xinc,ffFixed,10,2);
                
}
//---------------------------------------------------------------------------



