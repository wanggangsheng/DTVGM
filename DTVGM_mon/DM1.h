//---------------------------------------------------------------------------

#ifndef DM1H
#define DM1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TDM : public TDataModule
{
__published:	// IDE-managed Components
        TADOTable *ADOTable1;
        TADOQuery *ADOQuery1;
        TDataSource *DS1;
        TDataSource *DS2;
        TADOConnection *ADOConnection1;
        TADOTable *ADOTable2;
        TDataSource *DS3;
        TADOTable *ADOTable0;
        TDataSource *DS0;
        TOpenDialog *OpenDialog;
        TSaveDialog *SaveDialog;
        TADOTable *ADOTable3;
        TDataSource *DS4;
private:	// User declarations
public:		// User declarations
        __fastcall TDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDM *DM;
//---------------------------------------------------------------------------
#endif
