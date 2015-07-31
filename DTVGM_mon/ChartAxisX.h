//----------------------------------------------------------------------------
#ifndef ChartAxisXH
#define ChartAxisXH
//----------------------------------------------------------------------------
#include <OKCANCL1.h>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include "okcancl1.h"
//----------------------------------------------------------------------------
class TDlgChartEdit : public TOKBottomDlg
{
__published:
	TButton *HelpBtn;
        TLabel *Label1;
        TEdit *EdtMin;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TComboBox *ComboBox1;
        TEdit *EdtMax;
        TEdit *EdtNum;
        TEdit *EdtInc;
        TButton *Button1;
	void __fastcall HelpBtnClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
private:
public:
	virtual __fastcall TDlgChartEdit(TComponent* AOwner);
        int TDlgChartEdit::EditCheck();
};
//----------------------------------------------------------------------------
extern PACKAGE TDlgChartEdit *DlgChartEdit;
//----------------------------------------------------------------------------
#endif    
