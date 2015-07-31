//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DM1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDM *DM;
//---------------------------------------------------------------------------
__fastcall TDM::TDM(TComponent* Owner)
        : TDataModule(Owner)
{
        //ADOTable1->ConnectionString = ADOConnection1->ConnectionString;
        //ADOTable2->ConnectionString = ADOConnection1->ConnectionString;
        //ADOQuery1->ConnectionString = ADOConnection1->ConnectionString;
}
//---------------------------------------------------------------------------
