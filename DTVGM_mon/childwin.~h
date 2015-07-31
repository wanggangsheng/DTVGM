//----------------------------------------------------------------------------
#ifndef ChildWinH
#define ChildWinH
//-------------------------

//----------------------------------------------------------------------------
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <StdCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ADODB.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <jpeg.hpp>


//----------------------------------------------------------------------------
class TMDIChild : public TForm
{
__published:
        TDBGrid *DBGrid1;
        TDBNavigator *DBNavigator1;
        TPopupMenu *PopupMenu1;
        TMenuItem *Insert1;
        TMenuItem *Append1;
        TMenuItem *N1;
        TMenuItem *Delete1;
        TMenuItem *DeleteAll1;
        TMenuItem *N2;
        TMenuItem *Import1;
        TMenuItem *Export1;
        TProgressBar *ProgressBar1;
        TEdit *Edit1;
        TMenuItem *EmptyTable1;
        TADOTable *ADOTable0;
        TADOQuery *ADOQuery1;
        TDataSource *DataSource1;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);

        void __fastcall Insert1Click(TObject *Sender);
        void __fastcall Append1Click(TObject *Sender);
        void __fastcall Delete1Click(TObject *Sender);
        void __fastcall DeleteAll1Click(TObject *Sender);
        void __fastcall Import1Click(TObject *Sender);
        void __fastcall Export1Click(TObject *Sender);
        
        void __fastcall EmptyTable1Click(TObject *Sender);

private:
        
public:
	virtual __fastcall TMDIChild(TComponent *Owner);
        void __fastcall TMDIChild::ImportFile(char *txtFileName);
        void __fastcall TMDIChild::ExportFile(TDataSet *DataSet,char *txtFileName);
        void* TMDIChild::fscanfFormat(int nFieldCount,char* DataFormat);

//        void TMDIChild::display(char **,int m,int n);
//        void TMDIChild::de_allocate(char **,int m);

        void __fastcall TMDIChild::ControlResize();
        int __fastcall TMDIChild::DataQuery(bool blOption,String strAlias,String strTable,String strField,String strQuery);
        int __fastcall TMDIChild::SetFilter(bool blOption,String strAlias,String strTable,String strField,String strFilter);
        void __fastcall TMDIChild::ReadArrayofBagrovN(double ArrayofBagrovN[]); //

        void __fastcall TMDIChild::RunDTVGM_Month(int Func); //0-DTVGM,1-GA,2-SCEUA,3-MPSA
        void __fastcall TMDIChild::GaugeRain_DayToMonth();
        void __fastcall TMDIChild::Discharge_DayToMonth();

        void __fastcall TMDIChild::GridRain_IDW();

        void __fastcall TMDIChild::RunDTVGM_Day();

        void __fastcall TMDIChild::Optimize_GA();

        void __fastcall TMDIChild::Run_LandFit(int Func);

        void __fastcall TMDIChild::GA_LUCC();

        void __fastcall TMDIChild::Run_LandFit_Simple();

        void __fastcall TMDIChild::SubbFlowIn();
//==============================================================================
//        void __fastcall LinearInterp(struct TFPoint FPoint[]);
//==============================================================================
        String strQuoteA;

};

extern double __fastcall DistanceBetween(struct TFPoint FPoint1,struct TFPoint FPoint2,char Algorithm);
extern void __fastcall RainIDW(int ngauge,int ngrid,double gridsize,struct TFPoint FPointGauge[],struct TFPoint FPointGrid[],double gaugerain[],double gridrain[]);  //距离倒数平方查补雨量
extern TDate __fastcall DateConsistOf(int iyear,int imonth,int iday);

extern void __fastcall DTVGMlog(TADOTable *ADOTable,String SaveFile,double WaterBalance,double NashSutcliffeEC,
                                  double AW0,double AWC,double g1,double g2,double Kr,
                                  double PDc,double PDc_basic,double KAW,double KET,TDate DayStart,TDate DayEnd,
                                  TDateTime RunStart,TDateTime RunEnd,String SpendTime);

extern void __fastcall EvaluatePopulation(int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
                              String strDirRain,String strPrefix_rainfile,
                              int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
                              int *gridSubID,double *gridArea,double *gridRCC,
                              double *Array_ETp,double *RunoffObs,
                              double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
                              double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
                              double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC);

extern double __fastcall LandFit(int ngrid,int nLand,double ITER[],int LandID[],int *Landnum,
                                 double FitForest[],double FitGrass[],double FitCrop[],double FitUrban[],
                                 int Land1[],int Land2[],int Land2_cal[]);
                                 
extern double __fastcall LandFit_Simple(int MaxStep,int ngrid,int nLand,double ITER[],int LandID[],int *Landnum,int *Landnum_cal,
                                 double FitForest[],double FitGrass[],double FitCrop[],double FitUrban[],double *FitNoUse,
                                 int Land1[],int Land2[],int Land2_cal[]);

extern void __fastcall EvaluatePopulation_LUCC(int ngrid,int nLand,double ITER[],int LandID[],int *Landnum,
                                 double FitForest[],double FitGrass[],double FitCrop[],double FitUrban[],
                                 int Land1[],int Land2[],int Land2_cal[]);

extern void __fastcall sortASC(int nArray,double *Array,int *Arrryi);


//extern PACKAGE AnsiString __fastcall LeftStr(const AnsiString AText, int ACount);
//extern PACKAGE bool __fastcall AnsiStartsStr(const AnsiString ASubText, const AnsiString AText);
//==============================================================================
//==============================================================================
/*
void __fastcall SaveGridData(bool blSave, char *txtFileName,
                            int ngrid, int *gridID, double *gridP, double *gridETa, double *gridsm,
                            double *gridWU,double *gridR, double *gridRS, double *gridRSS, double *gridRG);

AnsiString __fastcall IntToStrF(int iNum, int iLength);//string5(int iNum);
extern double __fastcall FuncETatoETp(double PtoETp,double BagrovN);
extern int __fastcall wgsMonthsBetween(TDate dateBegin,TDate dateEnd);
extern double __fastcall NashSutcliffeEC(int DataSize,double DataObs[],double DataCal[]);
extern double __fastcall ETatoETp(double PtoETp,double BagrovN,int nBagrovN,double *ArrayofBagrovN);

extern double __fastcall DTVGM_Month(int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
                              String strDirRain,String strPrefix_rainfile,
                              int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
                              int *gridSubID,double *gridArea,double *gridRCC,
                              double *Array_ETp,double *RunoffObs,
                              double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
                              double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
                              double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC);

*/
//==============================================================================
//==============================================================================
//----------------------------------------------------------------------------
#endif
