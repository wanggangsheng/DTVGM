//----------------------------------------------------------------------------
#ifndef workdirH
#define workdirH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <dbcgrids.hpp>
#include <Mask.hpp>
//----------------------------------------------------------------------------
class TWorkDirDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TDBCtrlGrid *DBCtrlGrid1;
        TDBEdit *DBEdit_gridsize;
        TDBEdit *DBEdit_xcorner;
        TDBEdit *DBEdit_ycorner;
        TDBEdit *DBEdit_ncols;
        TDBEdit *DBEdit_nrows;
        TDBEdit *DBEdit_unit;
        TLabel *Label2;
        TLabel *Label3;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TLabel *Label7;
        TDBEdit *DBEdit_dirwork;
        TDBEdit *DBEdit_dirrain;
        TDBEdit *DBEdit_dirresult;
        TDBEdit *DBEdit_dirsm;
        TLabel *Label8;
        TLabel *Label9;
        TLabel *Label10;
        TLabel *Label11;
        TDBNavigator *DBNavigator1;
        TLabel *Label1;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TBitBtn *BitBtn4;
        TDBEdit *DBEdit_gridname;
        void __fastcall FormShow(TObject *Sender);
       // void __fastcall DBComboBox1Change(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall BitBtn4Click(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall CancelBtnClick(TObject *Sender);
private:
public:
	virtual __fastcall TWorkDirDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TWorkDirDlg *WorkDirDlg;
//----------------------------------------------------------------------------
#endif    
