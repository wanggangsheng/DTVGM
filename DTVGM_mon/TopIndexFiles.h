//----------------------------------------------------------------------------
#ifndef TopIndexFilesH
#define TopIndexFilesH
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
//----------------------------------------------------------------------------
class TDlgTOPIndex : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
        TLabel *Label1;
        TEdit *EdtINPUT;
        TEdit *EdtOutMap;
        TLabel *Label2;
        TEdit *EdtOutDitn;
        TLabel *Label3;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
private:
public:
	virtual __fastcall TDlgTOPIndex(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TDlgTOPIndex *DlgTOPIndex;
//----------------------------------------------------------------------------
#endif    
