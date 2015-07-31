//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
//#include <iostream.h> //max()

#include "Main.h"
#include "About.h"
//#include "GlobalVariables.h"  //wgs

#include "QueryDemo.h"
#include "DateDialog.h"
#include "DM1.h"
#include "workdir.h"
#include "ModelParaDialog.h"
#include "HydroGraph.h"

#include "TopIndexFiles.h"//wgs ,2004-10-28

#include "wgsTOPIndex.h" //wgs,2004-10-28

#include "MPSAmap.h"
#include "MCMCINI.h"

//wgs2003-6-8
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;
TDate gdateBegin,gdateEnd;//defined in GlobalVariables.h
String gstrBasinSelect;   //the Basin selected to simulate, Global Variant, defined in GlobalVariables.h
bool blVar;  // defined in GlobalVariables.h
//---------------------------------------------------------------------------

__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CreateMDIChild(String Name)
{
	TMDIChild *Child;

	//--- create a new MDI child window ----
	Child = new TMDIChild(Application);
	Child->Caption = Name;

        //Child->DBGrid1->DataSource = DM->DS0;
        //DM->ADOTable0->Active = false;
        Child->PopupMenu1->AutoPopup = false;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::FileNew1Execute(TObject *Sender)
{
	//CreateMDIChild("NONAME" + IntToStr(MDIChildCount + 1));
        CreateMDIChild("Modeling");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileOpen1Execute(TObject *Sender)
{
	if (OpenDialog->Execute())
		CreateMDIChild(OpenDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HelpAbout1Execute(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileExit1Execute(TObject *Sender)
{
int i;
for(i = MDIChildCount-1; i >= 0; i--)
    MDIChildren[i]->Close();
Close();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseAll1Click(TObject *Sender)
{
        CloseAllMDIChild();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CloseAllMDIChild()
{
        int i;
        for(i = MDIChildCount-1; i >= 0; i--)
                MDIChildren[i]->Close();

}
//---------------------------------------------------------------------------

float __fastcall TMainForm::Add(float add_x,float add_y)
{
return add_x + add_y;
}


void de_allocate(long double **data,int m)
{   for (int i = 0; i < m;  i++)
       delete[] data[i];                 // STEP 1: DELETE THE COLUMNS

   delete[] data;                        // STEP 2: DELETE THE ROWS

}





void __fastcall TMainForm::GridRain1Click(TObject *Sender)
{
        CreateMDIChild("dbo.GridRain");
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::RunArcView1Click(TObject *Sender)
{
        AnsiString arcdir = "C:\\ESRI\\AV_GIS30\\ARCVIEW\\BIN32\\";
        AnsiString asFileName = FileSearch("arcview.exe", GetCurrentDir() + AnsiString(";") + arcdir);
        switch(MessageDlg("运行ArcView吗?",mtConfirmation,TMsgDlgButtons() <<mbNo<< mbYes,0))
        {case mrYes:
                if (asFileName.IsEmpty())
                        ShowMessage("没有找到ArcView运行程序!");
                else
                        WinExec(asFileName.c_str(),SW_SHOWDEFAULT);//执行应用程序//
                break;
         case mrNo:break;}
  
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Query1Click(TObject *Sender)
{
        //CloseAllMDIChild();
        if(FormQuery) delete FormQuery;
        FormQuery = new TFormQuery(Application);
        FormQuery->Caption = "Query Demo";
        FormQuery->BitBtn1->Caption = "Run Query";
        //FormQuery->BitBtn3->Enabled = false;
        //FormQuery->BitBtn1->Enabled = true;
        FormQuery->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RunQuery(bool blOption,String strAlias,String strTable,String strField,String strQuery)
{ //imark=0:正常查询；1:查询表BagrovN，并读取字段BagrovN的值
//ShowMessage(strQuery);

TMDIChild *Child;
Child = new TMDIChild(Application);  	//--- create a new MDI child window ----
((TMDIChild*)ActiveMDIChild)->DataQuery(blOption,strAlias,strTable,strField,strQuery);
Child->Show();
}//TMainForm::RunQuery

//-----------------------------------------------------------

void __fastcall TMainForm::RainGauge1Click(TObject *Sender)
{
        CreateMDIChild("dbo.raingauge");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SetFilter(bool blOption,String strAlias,String strTable,String strField,String strFilter)
{
//ShowMessage("Filter:" + strFilter);
TObject *Sender;
//CloseAll1Click(Sender); //close all MdiChild Windows
TMDIChild *Child;
Child = new TMDIChild(Application);
((TMDIChild*)ActiveMDIChild)->SetFilter(blOption,strAlias,strTable,strField,strFilter);
Child->Show();
}//end SetFilter

void __fastcall TMainForm::Filter1Click(TObject *Sender)
{
        //CloseAllMDIChild();
        if(FormQuery) delete FormQuery;
        FormQuery = new TFormQuery(Application);
        FormQuery->Caption = "Filter Demo";
        FormQuery->BitBtn1->Caption = "Set Filter";
        FormQuery->btnQuery->Enabled = false;
        //FormQuery->BitBtn1->Enabled = false;
        //FormQuery->BitBtn3->Enabled = true;
        FormQuery->Show();
}
//---------------------------------------------------------------------------
void __fastcall ThrowException(TObject *Sender)

{
  try
  {
    throw Exception(" An error has occurred");
  }
  catch(Exception &E)
  {
    ShowMessage(AnsiString(E.ClassName())+ E.Message);
  }
}
//-----------------------------------------------------


//--------------------------------------------------------
void __fastcall TMainForm::LinearInterp(struct TFPoint FPoint[])
{//根据FPoint[1]、FPoint[2]以及FPoint[0].x, 计算FPoint[0].y
        if((FPoint[2].x - FPoint[1].x)!=0)
                FPoint[0].y = FPoint[1].y +
                              (FPoint[0].x - FPoint[1].x)*
                              (FPoint[2].y - FPoint[1].y)/
                              (FPoint[2].x - FPoint[1].x);
        else
                FPoint[0].y = 0.5*(FPoint[2].y + FPoint[1].y);

}  //TMainForm::LinearInterp

void __fastcall TMainForm::FormShow(TObject *Sender)
{
        //CreateMDIChild("Modeling");
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DTVGM1Click(TObject *Sender)
{
        CloseAllMDIChild(); //close all MdiChild Windows
        if(WorkDirDlg->ShowModal()==mrOk)
        {
                if(ModelParaDlg->ShowModal()==mrOk)
                {
                        if(DateDlg->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("Modeling");
                        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Month(0);}
                }
        }

}
//---------------------------------------------------------------------------



void __fastcall TMainForm::DayToMonth1Click(TObject *Sender)
{
        if(DateDlg->ShowModal()==mrOk)
                {if(MDIChildCount<1) CreateMDIChild("Modeling");
                ((TMDIChild*)ActiveMDIChild)->GaugeRain_DayToMonth();}
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::Interpolation1Click(TObject *Sender)
{
        if(WorkDirDlg->ShowModal()==mrOk)
        {
                if(DateDlg->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("Modeling");
                        ((TMDIChild*)ActiveMDIChild)->GridRain_IDW();}

        }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::DayToMonth2Click(TObject *Sender)
{
        if(DateDlg->ShowModal()==mrOk)
                {if(MDIChildCount<1) CreateMDIChild("Modeling");
                ((TMDIChild*)ActiveMDIChild)->Discharge_DayToMonth();}        
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::DTVGMDay1Click(TObject *Sender)
{
        if(MDIChildCount<1) CreateMDIChild("Modeling");
        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Day();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::FormResize(TObject *Sender)
{
        Image1->Left = this->ClientRect.Left;
        Image1->Top = this->ClientRect.Top;
        Image1->Width = this->ClientWidth;
        Image1->Height = this->ClientHeight - 20;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HydroGraph1Click(TObject *Sender)
{
        if(frmHydroGraph) delete frmHydroGraph;
        frmHydroGraph = new TfrmHydroGraph(Application);
        frmHydroGraph->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GeneticAlgorithm1Click(TObject *Sender)
{
        CloseAllMDIChild(); //close all MdiChild Windows
        if(WorkDirDlg->ShowModal()==mrOk)
        {
                if(ModelParaDlg->ShowModal()==mrOk)
                {
                        if(DateDlg->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("Genetic Algorithm");
                        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Month(1);}
                }
        } 
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Simplex1Click(TObject *Sender)
{
        ((TMDIChild*)ActiveMDIChild)->Optimize_GA();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::Help2Click(TObject *Sender)
{
        Application->HelpFile = ".\\TEXTFILE\\DDAF.HLP";  // E:\\DTVGM_CB
        Application->HelpJump("TApplication_HelpJump");

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::GALUCC1Click(TObject *Sender)
{
        //((TMDIChild*)ActiveMDIChild)->GA_LUCC();
        if(MDIChildCount<1) CreateMDIChild("GA_LUCC");
        ((TMDIChild*)ActiveMDIChild)->Run_LandFit(1);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SimpleLUCC1Click(TObject *Sender)
{
        if(MDIChildCount<1) CreateMDIChild("Simple_LUCC");
        ((TMDIChild*)ActiveMDIChild)->Run_LandFit_Simple();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::DTSWizardClick(TObject *Sender)
{ //调用SQLServer的DTSWizard
 AnsiString arcdir = "D:\\DTVGM_CB\\DB\\DTSWiz\\";//"C:\\MSSQL7\\Binn\\";
        AnsiString asFileName = FileSearch("dtswiz.exe", GetCurrentDir() + AnsiString(";") + arcdir);
        switch(MessageDlg("运行DTSWizard吗?",mtConfirmation,TMsgDlgButtons() <<mbNo<< mbYes,0))
        {case mrYes:
                if (asFileName.IsEmpty())
                        ShowMessage("没有找到DTSWizard运行程序!");
                else
                        WinExec(asFileName.c_str(),SW_SHOWDEFAULT);//执行应用程序//
                break;
         case mrNo:break;}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SubbFlowIn1Click(TObject *Sender)
{
        if(MDIChildCount<1) CreateMDIChild("SubbFlowIn");
                ((TMDIChild*)ActiveMDIChild)->SubbFlowIn();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DTVGMGIS1Click(TObject *Sender)
{
        AnsiString arcdir = "E:\\mydtvgm\\";
        AnsiString asFileName = FileSearch("ydtvgm.exe", GetCurrentDir() + AnsiString(";") + arcdir);
        switch(MessageDlg("运行DTVGMGIS吗?",mtConfirmation,TMsgDlgButtons() <<mbNo<< mbYes,0))
        {case mrYes:
                if (asFileName.IsEmpty())
                        ShowMessage("没有找到DTVGMGIS运行程序!");
                else
                        WinExec(asFileName.c_str(),SW_SHOWDEFAULT);//执行应用程序//
                break;
         case mrNo:break;}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SCEUA1Click(TObject *Sender)
{
        CloseAllMDIChild(); //close all MdiChild Windows
        if(WorkDirDlg->ShowModal()==mrOk)
        {
                if(ModelParaDlg->ShowModal()==mrOk)
                {
                        if(DateDlg->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("SCEUA");
                        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Month(2);}
                }
        }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::TOPIndex1Click(TObject *Sender)
{
        RunTopIndex(); //TOPIndex.h
}
//---------------------------------------------------------------------------
//==============================================================================
void __fastcall TMainForm::RunTopIndex()
{
      AnsiString asInput,asMap,asDitn;
      FILE *finput,*fmap,*fditn;
      char *cFile;

      if(DlgTOPIndex->ShowModal()==mrOk)
      {
        asInput = DlgTOPIndex->EdtINPUT->Text; //"D:\\DTVGM_bai\\TopIndex\\indem.asc";
        asMap = DlgTOPIndex->EdtOutMap->Text; //"D:\\DTVGM_bai\\TopIndex\\map.asc";
        asDitn = DlgTOPIndex->EdtOutDitn->Text; //"D:\\DTVGM_bai\\TopIndex\\Ditn.txt";
      }
      else
        return;
//*  Read in and open data files
        cFile = asInput.c_str();
        if(FileExists(cFile))
        {
                if((finput=fopen(cFile,"r"))==NULL)
                {
                        fprintf(stderr,"Cannot open input file.\n");
                        return;
                }
        }
        else
        {
                ShowMessage(AnsiString("Couldnot find the file:")+cFile+".");
                return;
        }

        cFile = asMap.c_str();
        if((fmap=fopen(cFile,"w+"))==NULL)
        {
                fprintf(stderr,"Cannot open input file.\n");
                return;
        }

        cFile = asDitn.c_str();
        if((fditn=fopen(cFile,"w+"))==NULL)
        {
                fprintf(stderr,"Cannot open input file.\n");
                return;
        }


        TopIndex(finput,fmap,fditn);
        fclose(finput);
        fclose(fmap);
        fclose(fditn);
        ShowMessage("END OF TOPINDEX");
} //END  RunTopIndex()
//==============================================================================



void __fastcall TMainForm::ParaSensitivity1Click(TObject *Sender)
{
        CloseAllMDIChild(); //close all MdiChild Windows
        if(WorkDirDlg->ShowModal()==mrOk)
        {
                if(ModelParaDlg->ShowModal()==mrOk)
                {
                        if(DateDlg->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("Parameter Sensitivity");
                        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Month(3);}
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MPSAmap1Click(TObject *Sender)
{
        CloseAllMDIChild(); //close all MdiChild Windows
        if(frmMPSAmap) delete frmMPSAmap;
        frmMPSAmap = new TfrmMPSAmap(Application);
        frmMPSAmap->ShowModal();

}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

void __fastcall TMainForm::PUSA1Click(TObject *Sender)
{
        CloseAllMDIChild(); //close all MdiChild Windows
        if(WorkDirDlg->ShowModal()==mrOk)
        {
                if(ModelParaDlg->ShowModal()==mrOk)
                {
                        if(DateDlg->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("Parameter Uncertainty");
                        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Month(4);}
                }
        }
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::HMCM1Click(TObject *Sender)
{
        CloseAllMDIChild(); //close all MdiChild Windows
        if(WorkDirDlg->ShowModal()==mrOk)
        {
                if(ModelParaDlg->ShowModal()==mrOk)
                {
                        if(DateDlg->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("Hessian Matrix & Covariance Matrix");
                        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Month(5);}
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::MetropolisMCMC1Click(TObject *Sender)
{
        int iModalResult;
        CloseAllMDIChild(); //close all MdiChild Windows
        LabelDir:
        iModalResult = WorkDirDlg->ShowModal();
        if(iModalResult==mrOk)
        {       iModalResult = ModelParaDlg->ShowModal();
                if(iModalResult==mrOk)
                {
                        if(DlgMCMCINI->ShowModal()==mrOk)
                        {if(MDIChildCount<1) CreateMDIChild("Metropolis Monte Carlo Markov Chain");
                        ((TMDIChild*)ActiveMDIChild)->RunDTVGM_Month(6);}
                }

        }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::UncertaintyofParaandStruct1Click(
      TObject *Sender)
{
        if(WorkDirDlg->ShowModal()==mrCancel) return;
        AnsiString asInput,asOut;
        FILE *finput,*fout;
        char *cFile;
        long NSamp,NID,i;
        int NPara,NTime,j;
        double dvar;
        double *theta = new double[NPara];
        double *Qcal = new double[NTime];
        double *Qcal2 = new double[NTime];

		if(DlgTOPIndex->ShowModal()==mrOk)
		{
			asInput = DlgTOPIndex->EdtINPUT->Text; //"D:\\DTVGM_CBG\\TextFile\\MCMCdata.txt";
			asOut = DlgTOPIndex->EdtOutMap->Text; //"D:\\DTVGM_CBG\\TextFile\\MCMCups.txt";
		}
		else
			return;
//MCMCdata.txt
//Line 1: NSamp (long),NPara (int),NTime (int):
//NSamp: Number of Samples;
//NPara: Number of Parameters;
//NTime: Number of Time Steps;
//Line 2 to Line N: NID (long), Theta[0...NPara - 1] (double) ,Qcal[0...NTime - 1] (double);

//MCMCups.txt
//Line 1 to Line N: NID (long), Var (double) ,Qcal[0...NTime - 1] (double);

//*  Read in and open data files
        cFile = asInput.c_str();
        if(FileExists(cFile))
        {
                if((finput=fopen(cFile,"r"))==NULL)
                {
                        ShowMessage("Cannot open input file.\n");
                        return;
                }
        }
        else
        {
                ShowMessage(AnsiString("Couldnot find the file:")+cFile+".");
                return;
        }

        cFile = asOut.c_str();
        if((fout=fopen(cFile,"w+"))==NULL)
        {
                fprintf(stderr,"Cannot open input file.\n");
                return;
        }

        Randomize();
        fscanf(finput,"%D",&NSamp);
        fscanf(finput,"%d%d",&NPara,&NTime);
        for(i=0;i<NSamp;i++)
        {
                fscanf(finput,"%D",&NID);
                for(j=0;j<NPara;j++) theta[j] = fscanfdouble(finput);
                fprintf(fout,"%d\t%10.2f\t",NID,theta[NPara - 1]);
                for(j=0;j<NTime;j++)
                {       Qcal[j] = fscanfdouble(finput);
                        dvar = RandG(sqrt(Qcal[j]),sqrt(theta[NPara - 1]));
                        dvar = max(0.0,dvar);
                        Qcal2[j] = Power(dvar,2.0);
                        fprintf(fout,"%10.2f\t",Qcal2[j]);
                }
                fprintf(fout,"\n");

        }
        fclose(finput);
        fclose(fout);
        ShowMessage("ok! Uncertainty of Para and Struct ");
} //END: UncertaintyofParaandStruct
//---------------------------------------------------------------------------

