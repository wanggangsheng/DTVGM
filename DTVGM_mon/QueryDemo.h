//---------------------------------------------------------------------------

#ifndef QueryDemoH
#define QueryDemoH

//#include "main.h"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormQuery : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TListBox *ListBox1;
        TListBox *ListBox2;
        TListBox *ListBox3;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TBitBtn *BitBtn1;
        TGroupBox *GroupBox1;
        TCheckBox *CheckBox1;
        TCheckBox *CheckBox2;
        TBitBtn *BitBtn2;
        TDBGrid *DBGrid1;
        TPopupMenu *PopupMenu1;
        TMenuItem *Append1;
        TMenuItem *N1;
        TMenuItem *Delete1;
        TMenuItem *DeleteAll1;
        TLabel *Label4;
        TBitBtn *BitBtn3;
        TRichEdit *RichEdit1;
        TLabel *Label5;
        TLabel *Label6;
        TBitBtn *btnQuery;
        TBitBtn *BitBtn5;
        TBitBtn *BitBtn6;
        TBitBtn *BitBtn7;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ListBox1Click(TObject *Sender);
        void __fastcall ListBox2Click(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Table2NewRecord(TDataSet *DataSet);
        void __fastcall DBGrid1CellClick(TColumn *Column);
        void __fastcall Append1Click(TObject *Sender);
        void __fastcall Delete1Click(TObject *Sender);
        void __fastcall DeleteAll1Click(TObject *Sender);
        void __fastcall BitBtn3Click(TObject *Sender);
        void __fastcall ListBox3Click(TObject *Sender);
        void __fastcall BitBtn5Click(TObject *Sender);
        void __fastcall BitBtn6Click(TObject *Sender);
        void __fastcall BitBtn7Click(TObject *Sender);
        void __fastcall btnQueryClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormQuery(TComponent* Owner);
        String gstrAlias;    //global string of Alias
};
//---------------------------------------------------------------------------
extern PACKAGE TFormQuery *FormQuery;

//---------------------------------------------------------------------------
#endif
