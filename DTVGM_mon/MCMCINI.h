//---------------------------------------------------------------------------

#ifndef MCMCINIH
#define MCMCINIH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "DateDialog.h"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TDlgMCMCINI : public TDateDlg
{
__published:	// IDE-managed Components
        TRadioGroup *RadioGroup2;
        TGroupBox *GroupBox1;
        TCheckBox *CBox_g1;
        TCheckBox *CBox_g2;
        TCheckBox *CBox_Kr;
        TCheckBox *CBox_KAW;
        TCheckBox *CBox_AW0;
        TLabel *Label6;
        TMaskEdit *MaskEdit_Sigma;
        TLabel *Label7;
        TMaskEdit *MaskEdit_Samples;
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall RadioGroup2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TDlgMCMCINI(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDlgMCMCINI *DlgMCMCINI;
//---------------------------------------------------------------------------
#endif
