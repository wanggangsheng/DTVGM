//----------------------------------------------------------------------------
#ifndef MainH
#define MainH
//----------------------------------------------------------------------------
#include "ChildWin.h"
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Messages.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <jpeg.hpp>
//----------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *FileNewItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileCloseItem;
	TMenuItem *Window1;
	TMenuItem *Help1;
	TMenuItem *N1;
	TMenuItem *FileExitItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileItem;
	TMenuItem *WindowArrangeItem;
	TMenuItem *HelpAboutItem;
	TOpenDialog *OpenDialog;
	TMenuItem *FileSaveItem;
	TMenuItem *FileSaveAsItem;
	TMenuItem *Edit1;
	TMenuItem *CutItem;
	TMenuItem *CopyItem;
	TMenuItem *PasteItem;
	TMenuItem *WindowMinimizeItem;
        TStatusBar *StatusBar;
        TActionList *ActionList1;
        TEditCut *EditCut1;
        TEditCopy *EditCopy1;
        TEditPaste *EditPaste1;
        TAction *FileNew1;
        TAction *FileSave1;
        TAction *FileExit1;
        TAction *FileOpen1;
        TAction *FileSaveAs1;
        TWindowCascade *WindowCascade1;
        TWindowTileHorizontal *WindowTileHorizontal1;
        TWindowArrange *WindowArrangeAll1;
        TWindowMinimizeAll *WindowMinimizeAll1;
        TAction *HelpAbout1;
        TWindowClose *FileClose1;
        TWindowTileVertical *WindowTileVertical1;
        TMenuItem *WindowTileItem2;
        TToolBar *ToolBar2;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TImageList *ImageList1;
        TMenuItem *DataBase1;
        TMenuItem *CloseAll1;
        TMenuItem *ModelMonth;
        TMenuItem *DTVGM1;
        TMenuItem *GIS1;
        TMenuItem *RunArcView1;
        TMenuItem *Query1;
        TMenuItem *Filter1;
        TSaveDialog *SaveDialog;
        TMenuItem *N2;
        TMenuItem *GaugeRain1;
        TMenuItem *DayToMonth1;
        TMenuItem *GridRain1;
        TMenuItem *Interpolation1;
        TMenuItem *InterpolationGRDS1;
        TMenuItem *InterpolationKriging1;
        TMenuItem *N3;
        TMenuItem *Discharge1;
        TMenuItem *DayToMonth2;
        TMenuItem *DTVGMDay1;
        TImage *Image1;
        TMenuItem *N4;
        TMenuItem *HydroGraph1;
        TMenuItem *Help2;
        TMenuItem *Optimize1;
        TMenuItem *GeneticAlgorithm1;
        TMenuItem *Simplex1;
        TMenuItem *N5;
        TMenuItem *GALUCC1;
        TMenuItem *SimpleLUCC1;
        TMenuItem *SCEUA1;
        TMenuItem *DTSWizard;
        TMenuItem *N6;
        TMenuItem *SubbFlowIn1;
        TMenuItem *Visualize1;
        TMenuItem *DTVGMGIS1;
        TMenuItem *N7;
        TMenuItem *TOPIndex1;
        TMenuItem *ParaSensitivity1;
        TMenuItem *MPSAmap1;
        TMenuItem *Uncertainty1;
        TMenuItem *PUSA1;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *HMCM1;
        TMenuItem *N10;
        TMenuItem *MetropolisMCMC1;
        TMenuItem *UncertaintyofParaandStruct1;
        void __fastcall FileNew1Execute(TObject *Sender);
        void __fastcall FileOpen1Execute(TObject *Sender);
        void __fastcall HelpAbout1Execute(TObject *Sender);
        void __fastcall FileExit1Execute(TObject *Sender);
        void __fastcall CloseAll1Click(TObject *Sender);
        void __fastcall GridRain1Click(TObject *Sender);
        void __fastcall RunArcView1Click(TObject *Sender);
        void __fastcall Query1Click(TObject *Sender);
        void __fastcall RainGauge1Click(TObject *Sender);
        void __fastcall Filter1Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall DTVGM1Click(TObject *Sender);
        void __fastcall DayToMonth1Click(TObject *Sender);
        void __fastcall Interpolation1Click(TObject *Sender);
        void __fastcall DayToMonth2Click(TObject *Sender);
        void __fastcall DTVGMDay1Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall HydroGraph1Click(TObject *Sender);
        void __fastcall GeneticAlgorithm1Click(TObject *Sender);
        void __fastcall Simplex1Click(TObject *Sender);
        void __fastcall Help2Click(TObject *Sender);
        void __fastcall GALUCC1Click(TObject *Sender);
        void __fastcall SimpleLUCC1Click(TObject *Sender);
        void __fastcall DTSWizardClick(TObject *Sender);
        void __fastcall SubbFlowIn1Click(TObject *Sender);
        void __fastcall DTVGMGIS1Click(TObject *Sender);
        void __fastcall SCEUA1Click(TObject *Sender);
        void __fastcall TOPIndex1Click(TObject *Sender);
        void __fastcall ParaSensitivity1Click(TObject *Sender);
        void __fastcall MPSAmap1Click(TObject *Sender);
        void __fastcall PUSA1Click(TObject *Sender);
        void __fastcall HMCM1Click(TObject *Sender);
        void __fastcall MetropolisMCMC1Click(TObject *Sender);
        void __fastcall UncertaintyofParaandStruct1Click(TObject *Sender);

private:
	void __fastcall CreateMDIChild(const String Name);
public:
	virtual __fastcall TMainForm(TComponent *Owner);
        float __fastcall TMainForm::Add(float add_x,float add_y);
       // void __fastcall TMainForm::ImportFile(String TableName,char *txtFileName);
        void de_allocate(long double **data,int m);
        void __fastcall TMainForm::RunQuery(bool blOption,String strAlias,String strTable,String strField,String strQuery);
        void __fastcall TMainForm::SetFilter(bool blOption,String strAlias,String strTable,String strField,String strFilter);

        void __fastcall TMainForm::LinearInterp(struct TFPoint FPoint[]);
        void __fastcall TMainForm::CloseAllMDIChild();

        void __fastcall TMainForm::RunTopIndex();
        
};
//----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
extern void __fastcall ThrowException(TObject *Sender);

//extern PACKAGE AnsiString __fastcall LeftStr(const AnsiString AText, int ACount);
//extern void* fscanfFormat(int nFieldCount,char* DataFormat);
//----------------------------------------------------------------------------
#endif    
