//---------------------------------------------------------------------------

#ifndef HydroGraphH
#define HydroGraphH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <Buttons.hpp>
#include <Series.hpp>
//---------------------------------------------------------------------------
class TfrmHydroGraph : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *StringGrid1;
        TRadioGroup *RadioGroup1;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TBitBtn *BitBtn1;
        TLabel *Label1;
        TNotebook *Notebook1;
        TChart *Chart2;
        TChart *Chart1;
        TLineSeries *Series_qobs;
        TLineSeries *Series_qcal;
        TBarSeries *Series_P;
        TLineSeries *Series_sm;
        TLineSeries *Series_ETa;
        TLineSeries *Series_Rd;
        TLineSeries *Series_Rs;
        TLineSeries *Series_R;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TfrmHydroGraph(TComponent* Owner);
        void __fastcall TfrmHydroGraph::HydroGraph();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHydroGraph *frmHydroGraph;
extern int __fastcall ImportFile(TStringGrid *StringGrid,char *txtFileName);
//---------------------------------------------------------------------------
#endif
