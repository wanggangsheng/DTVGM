//---------------------------------------------------------------------------

#ifndef MPSAmapH
#define MPSAmapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <TeEngine.hpp>
#include <TeeProcs.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>


//---------------------------------------------------------------------------
class TfrmMPSAmap : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TChart *Chart1;
        TLineSeries *Series01;
        TLineSeries *Series02;
        TChart *Chart2;
        TLineSeries *Series11;
        TLineSeries *Series12;
        TChart *Chart3;
        TLineSeries *Series21;
        TLineSeries *Series22;
        TChart *Chart4;
        TLineSeries *Series31;
        TLineSeries *Series32;
        TChart *Chart5;
        TLineSeries *Series41;
        TLineSeries *Series42;
        TChart *Chart6;
        TLineSeries *Series51;
        TLineSeries *Series52;
        TTabSheet *TabSheet2;
        TChart *Chart7;
        TLineSeries *LineSeries1;
        TLineSeries *LineSeries2;
        TChart *Chart8;
        TLineSeries *LineSeries3;
        TLineSeries *LineSeries4;
        TChart *Chart9;
        TLineSeries *LineSeries5;
        TLineSeries *LineSeries6;
        TChart *Chart10;
        TLineSeries *LineSeries7;
        TLineSeries *LineSeries8;
        TChart *Chart11;
        TLineSeries *LineSeries9;
        TLineSeries *LineSeries10;
        TChart *Chart12;
        TLineSeries *LineSeries11;
        TLineSeries *LineSeries12;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *ImportFile1;
        TMenuItem *Exit1;
        TMenuItem *MPSA1;
        TMenuItem *EVobs1;
        TMenuItem *EVMID1;
        TMenuItem *HMLEOBS1;
        TMenuItem *HMLEMID1;
        TMenuItem *GLUE1;
        TMenuItem *N2;
        TMenuItem *Help1;
        TMenuItem *Tools1;
        TMenuItem *ChangeAxisX1;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TChart *Chart13;
        TPointSeries *Series1;
        TChart *Chart14;
        TPointSeries *Series2;
        TChart *Chart15;
        TPointSeries *Series3;
        TChart *Chart16;
        TPointSeries *Series4;
        TChart *Chart17;
        TPointSeries *Series5;
        TChart *Chart18;
        TPointSeries *Series6;
        TChart *Chart19;
        TPointSeries *PointSeries1;
        TChart *Chart20;
        TPointSeries *PointSeries2;
        TChart *Chart21;
        TPointSeries *PointSeries3;
        TChart *Chart22;
        TPointSeries *PointSeries4;
        TChart *Chart23;
        TPointSeries *PointSeries5;
        TChart *Chart24;
        TPointSeries *PointSeries6;
        TMenuItem *NSEC2;
        TProgressBar *ProgressBar1;
        TProgressBar *ProgressBar2;
        TTabSheet *TabSheet5;
        TChart *Chart25;
        TLineSeries *LineSeries13;
        TBarSeries *BarSeries0;
        TChart *Chart26;
        TBarSeries *BarSeries1;
        TLineSeries *LineSeries14;
        TChart *Chart27;
        TBarSeries *BarSeries2;
        TLineSeries *LineSeries15;
        TChart *Chart28;
        TBarSeries *BarSeries3;
        TLineSeries *LineSeries16;
        TChart *Chart29;
        TBarSeries *BarSeries4;
        TLineSeries *LineSeries17;
        TChart *Chart30;
        TBarSeries *BarSeries5;
        TLineSeries *LineSeries18;
        TMenuItem *SYN1;
        TMenuItem *UNI1;
        TMenuItem *NEC1;
        TMenuItem *POW1;
        TMenuItem *EXP1;

        void __fastcall BtnAxisXClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall ChangeAxisX1Click(TObject *Sender);
        void __fastcall ImportFile1Click(TObject *Sender);
        void __fastcall EVobs1Click(TObject *Sender);
        void __fastcall EVMID1Click(TObject *Sender);
        void __fastcall HMLEOBS1Click(TObject *Sender);
        void __fastcall HMLEMID1Click(TObject *Sender);
        void __fastcall NSEC2Click(TObject *Sender);
        void __fastcall SYN1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall UNI1Click(TObject *Sender);
        void __fastcall NEC1Click(TObject *Sender);
        void __fastcall POW1Click(TObject *Sender);
        void __fastcall EXP1Click(TObject *Sender);
private:	// User declarations
        int nrun,nopt;
        double **para;
        double **VOBF;
public:		// User declarations
        __fastcall TfrmMPSAmap(TComponent* Owner);
        int TfrmMPSAmap::ImportMPSAfile(char *cFile,AnsiString strBegin);
        void TfrmMPSAmap::MPSAplot(int iVOBF,int nrun,int nopt,double **para,double **VOBF);
        void __fastcall TfrmMPSAmap::ChangeAxisX();
        void TfrmMPSAmap::GLUEplot(int iTF,int nrun,double **VOBF);


        TChart **wgsChart;
        TLineSeries **wgsSeries;
        TPointSeries **wgsPSeries;
        TBarSeries **wgsBSeries;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMPSAmap *frmMPSAmap;
//---------------------------------------------------------------------------
#endif
