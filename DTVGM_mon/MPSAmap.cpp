//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "iostream.h" //max(a,b),min(a,b),FILE

#include "MPSAmap.h"

#include "DirectoryDialog.h"//wgs
#include "DTVGM.h"
#include "ChartAxisX.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMPSAmap *frmMPSAmap;
//---------------------------------------------------------------------------
__fastcall TfrmMPSAmap::TfrmMPSAmap(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::ImportFile1Click(TObject *Sender)
{
        int ImportOK;
        AnsiString asFile,strBegin;
        char *cFile;
        if(DirectoryDlg->ShowModal()==mrOk)
                {asFile = DirectoryDlg->Label_dir->Caption + "\\"  + DirectoryDlg->EditFile->Text;}
        cFile = asFile.c_str();
        strBegin = "MonteCarlo-Simulation-Results";
        ImportOK = ImportMPSAfile(cFile,strBegin);
        if(ImportOK == 1)
        {
                MPSA1->Enabled = true;
                GLUE1->Enabled = true;
                GLUEplot(0,nrun,VOBF);
        }
        else
        {
                MPSA1->Enabled = false;
                GLUE1->Enabled = false;
                ShowMessage("Import File Failure!\nPlease Try Again.");
        }
}
//---------------------------------------------------------------------------
int TfrmMPSAmap::ImportMPSAfile(char *sFile,AnsiString strBegin)
{
        FILE *fmpsa;
        char str1[50],strStation[20];

        int i,j,k,nobf,iwgs;
        double NSEC;


        PageControl1->ActivePageIndex = 0;  //TabSheet1
        ProgressBar1->Visible = true;
        
        const int nChart = 6;

        wgsChart = new TChart*[nChart*5];  //12 MPSA + 12 Dotty + 6 Uncertainty
        wgsSeries = new TLineSeries*[nChart*5]; //2*12 MPSA + 6 Uncertainty
        wgsPSeries = new TPointSeries*[nChart*2]; //2*6 Dotty
        wgsBSeries = new TBarSeries*[nChart]; //6 Uncertainty
//Chart MPSA Plot
        wgsChart[0] = frmMPSAmap->Chart1;
        wgsChart[1] = frmMPSAmap->Chart2;
        wgsChart[2] = frmMPSAmap->Chart3;
        wgsChart[3] = frmMPSAmap->Chart4;
        wgsChart[4] = frmMPSAmap->Chart5;
        wgsChart[5] = frmMPSAmap->Chart6;
        wgsChart[6] = frmMPSAmap->Chart7;
        wgsChart[7] = frmMPSAmap->Chart8;
        wgsChart[8] = frmMPSAmap->Chart9;
        wgsChart[9] = frmMPSAmap->Chart10;
        wgsChart[10] = frmMPSAmap->Chart11;
        wgsChart[11] = frmMPSAmap->Chart12;
//Chart Dotty Plot
        wgsChart[12] = frmMPSAmap->Chart13;
        wgsChart[13] = frmMPSAmap->Chart14;
        wgsChart[14] = frmMPSAmap->Chart15;
        wgsChart[15] = frmMPSAmap->Chart16;
        wgsChart[16] = frmMPSAmap->Chart17;
        wgsChart[17] = frmMPSAmap->Chart18;
        wgsChart[18] = frmMPSAmap->Chart19;
        wgsChart[19] = frmMPSAmap->Chart20;
        wgsChart[20] = frmMPSAmap->Chart21;
        wgsChart[21] = frmMPSAmap->Chart22;
        wgsChart[22] = frmMPSAmap->Chart23;
        wgsChart[23] = frmMPSAmap->Chart24;
//Chart Uncertainty
        wgsChart[24] = frmMPSAmap->Chart25;
        wgsChart[25] = frmMPSAmap->Chart26;
        wgsChart[26] = frmMPSAmap->Chart27;
        wgsChart[27] = frmMPSAmap->Chart28;
        wgsChart[28] = frmMPSAmap->Chart29;
        wgsChart[29] = frmMPSAmap->Chart30;

//LineSeries
        wgsSeries[0] = frmMPSAmap->Series01;
        wgsSeries[1] = frmMPSAmap->Series02;

        wgsSeries[2] = frmMPSAmap->Series11;
        wgsSeries[3] = frmMPSAmap->Series12;

        wgsSeries[4] = frmMPSAmap->Series21;
        wgsSeries[5] = frmMPSAmap->Series22;

        wgsSeries[6] = frmMPSAmap->Series31;
        wgsSeries[7] = frmMPSAmap->Series32;

        wgsSeries[8] = frmMPSAmap->Series41;
        wgsSeries[9] = frmMPSAmap->Series42;

        wgsSeries[10] = frmMPSAmap->Series51;
        wgsSeries[11] = frmMPSAmap->Series52;

        wgsSeries[12] = frmMPSAmap->LineSeries1;
        wgsSeries[13] = frmMPSAmap->LineSeries2;

        wgsSeries[14] = frmMPSAmap->LineSeries3;
        wgsSeries[15] = frmMPSAmap->LineSeries4;

        wgsSeries[16] = frmMPSAmap->LineSeries5;
        wgsSeries[17] = frmMPSAmap->LineSeries6;

        wgsSeries[18] = frmMPSAmap->LineSeries7;
        wgsSeries[19] = frmMPSAmap->LineSeries8;

        wgsSeries[20] = frmMPSAmap->LineSeries9;
        wgsSeries[21] = frmMPSAmap->LineSeries10;

        wgsSeries[22] = frmMPSAmap->LineSeries11;
        wgsSeries[23] = frmMPSAmap->LineSeries12;
//LineSeries Uncertianty
        wgsSeries[24] = frmMPSAmap->LineSeries13;
        wgsSeries[25] = frmMPSAmap->LineSeries14;
        wgsSeries[26] = frmMPSAmap->LineSeries15;
        wgsSeries[27] = frmMPSAmap->LineSeries16;
        wgsSeries[28] = frmMPSAmap->LineSeries17;
        wgsSeries[29] = frmMPSAmap->LineSeries18;

//PointSeries
        wgsPSeries[0] = frmMPSAmap->Series1;
        wgsPSeries[1] = frmMPSAmap->Series2;
        wgsPSeries[2] = frmMPSAmap->Series3;
        wgsPSeries[3] = frmMPSAmap->Series4;
        wgsPSeries[4] = frmMPSAmap->Series5;
        wgsPSeries[5] = frmMPSAmap->Series6;

        wgsPSeries[6] = frmMPSAmap->PointSeries1;
        wgsPSeries[7] = frmMPSAmap->PointSeries2;
        wgsPSeries[8] = frmMPSAmap->PointSeries3;
        wgsPSeries[9] = frmMPSAmap->PointSeries4;
        wgsPSeries[10] = frmMPSAmap->PointSeries5;
        wgsPSeries[11] = frmMPSAmap->PointSeries6;
//PointSeries
        wgsBSeries[0] = frmMPSAmap->BarSeries0;
        wgsBSeries[1] = frmMPSAmap->BarSeries1;
        wgsBSeries[2] = frmMPSAmap->BarSeries2;
        wgsBSeries[3] = frmMPSAmap->BarSeries3;
        wgsBSeries[4] = frmMPSAmap->BarSeries4;
        wgsBSeries[5] = frmMPSAmap->BarSeries5;

//----------------------------------------------------
        if(FileExists(sFile))
        {
                if((fmpsa=fopen(sFile,"r"))==NULL)
                {
                        ShowMessage("Cannot open input file.");
                        return 0;
                }
        }
        else
        {
                ShowMessage(AnsiString("Couldnot find the file:")+sFile+".");
                return 0;
        }

        fscanf(fmpsa,"%s%s",&str1,&strStation);
        fscanf(fmpsa,"%s%d",&str1,&nrun);
        fscanf(fmpsa,"%s%d",&str1,&nopt);
        fscanf(fmpsa,"%s%d",&str1,&nobf);  //number of objective function

        if(nopt <= 6)
        {
                TabSheet2->TabVisible = false;
                TabSheet4->TabVisible = false;
        }
        else
        {
                TabSheet2->TabVisible = true;
                TabSheet4->TabVisible = true;
        }

        AnsiString *paraName = new AnsiString[nopt];
        double *paramin = new double[nopt];
        double *paramax = new double[nopt];

        VOBF = new double*[nrun];
        for (i = 0; i < nrun; i++)
                VOBF[i] = new double[nobf];  // STEP 2: SET UP THE COLUMNS
        //double **para;
        para = new double*[nrun];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < nrun; i++)
                para[i] = new double[nopt];  // STEP 2: SET UP THE COLUMNS

        for(j=0;j<nopt;j++)
        {
                fscanf(fmpsa,"%s",&str1);
                paraName[j] = str1;
                paramin[j] = fscanfdouble(fmpsa);
                paramax[j] = fscanfdouble(fmpsa);
        }
        this->Caption = "MPSA Map [Model Runs = " + String(nrun) + "]";

        for(j=0;j<nopt;j++)
        {
        //MSPA

                wgsChart[j]->BottomAxis->Title->Caption = paraName[j]+ " ["
                                                        + String(paramin[j])+ ","
                                                        + String(paramax[j])+ "]";

        //Dotty

                wgsChart[nChart*2 + j]->Title->Text->Clear();
                wgsChart[nChart*2 + j]->Title->Text->Add("Chart"+String(nChart*2 + j + 1));
                wgsChart[nChart*2 + j]->BottomAxis->Title->Caption = paraName[j]+ "["
                                                                   + String(paramin[j])+ ","
                                                                   + String(paramax[j])+ "]";

        }
        //Uncertainty
    
//-----------------------------------------------------
        for(;!feof(fmpsa);)
        {
        fscanf(fmpsa,"%s",&str1);


        if(Trim(String(str1)) == strBegin)
        {
                ShowMessage("Find the Beginning Point at:\n"+Trim(String(str1)));
                for(j=0;j<nopt + nobf + 1;j++)
                        fscanf(fmpsa,"%s",&str1); //read column name: i,g1,g2,Kr,KAW,AW0,VOBFr
//-----------------------------------------------------

//-----------------------------------------------------
                for(k=0;k<nrun;k++)
                {
                        fscanf(fmpsa,"%d",&iwgs);  //k

                        for(j=0;j<nopt;j++)
                                para[k][j] = fscanfdouble(fmpsa);
                        for(j=0;j<nobf;j++)
                                VOBF[k][j] = fscanfdouble(fmpsa); //Value of Objective Function
                        ProgressBar1->Position = 100.0*k/nrun;
                }   //end k

                break;
        }   //end if(Trim(String(str1)) == strwgs)

        } //end  for(;!feof(fmpsa);)
        ShowMessage("Finish Reading MPSA Map File.");

        fclose(fmpsa);
//-----------------------------------------------------
        ProgressBar1->Visible = false;
        delete[] paraName,paramin,paramax;
        return 1;
} //end Import

//------------------------------------------------------------------------------
void TfrmMPSAmap::MPSAplot(int iVOBF,int nrun,int nopt,double **para,double **VOBF)
{
//iVOBF=

        int i,j,k;
        double MPSANSEC,VOBFmax;
        const int nChart = 6;
        AnsiString asMPSA[] = {"EVOBS","EVMID","HMLEOBS","HMLEMID","NSEC",
                               "MPSA6","MPSA7","MPSA8","MPSA9","MPSA10","MPSA11","MPSA12"};

        ProgressBar1->Visible = true;
        ProgressBar2->Visible = true;
        for (i = 0; i < MPSA1->Count; i++)
        {
                MPSA1->Items[i]->Checked = false;
        }
        if(iVOBF < MPSA1->Count) MPSA1->Items[iVOBF]->Checked = true;

        double *VOBFr = new double[nrun];

        double **paraSort;
        paraSort = new double*[nrun];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < nrun; i++)
                paraSort[i] = new double[nopt];  // STEP 2: SET UP THE COLUMNS

        for(i=0;i<nrun;i++)
        {
                VOBFr[i] = max(0.0,VOBF[i][iVOBF]);
                for(j=0;j<nopt;j++)
                        paraSort[i][j] = para[i][j];
        }

        

        int nrunA,nrunUA; //numbers of Acceptable and UnAcceptable
        nrunA = nrun/2;
        nrunUA = nrun - nrunA;    //nrunUA = nrunA or nrunA + 1
        double *paraA = new double[nrunA];      //Acceptable
        double *paraUA = new double[nrunUA];   //UnAcceptable

        sort(nrun,nopt,paraSort,VOBFr);
        VOBFmax = max(1.0,VOBFr[nrun - 1]);

        for(j=0;j<nopt;j++)
        {

                wgsSeries[2*j + 0]->Clear();
                wgsSeries[2*j + 1]->Clear();

                wgsPSeries[j]->Clear();
                wgsPSeries[j]->Pointer->Style = psCircle;
                wgsPSeries[j]->Pointer->HorizSize = 2;
                wgsPSeries[j]->Pointer->VertSize = 2;

                wgsChart[nChart*2 + j]->LeftAxis->Automatic = true;
                wgsChart[nChart*2 + j]->LeftAxis->Title->Caption = asMPSA[iVOBF] + " [Max = " + String(int(VOBFmax))+"]";
                for(k=0;k<nrun;k++)
                {

                        wgsPSeries[j]->AddXY(paraSort[k][j],VOBFr[k]/VOBFmax,"",clBlue);//
                        ProgressBar1->Position = 100.0*k/nrun;
                }

                for(k=0;k<nrunA;k++)
                        paraA[k] = paraSort[k][j];
                for(k=0;k<nrunUA;k++)
                        paraUA[k] = paraSort[nrunA + k][j];
                sort1(nrunA,paraA);
                sort1(nrunUA,paraUA);
                for(k=0;k<nrunA;k++)
                {
                        wgsSeries[2*j + 0]->AddXY(paraA[k],(k + 1.0)/nrunUA,"",clRed);
                        wgsSeries[2*j + 1]->AddXY(paraUA[k],(k + 1.0)/nrunUA,"",clBlue);
                        ProgressBar1->Position = 100.0*k/nrunA;
                }
                if(nrunUA > nrunA)
                {
                        wgsSeries[2*j + 0]->AddXY(paraA[nrunA - 1],nrunA*1.0/nrunUA,"",clRed);
                        wgsSeries[2*j + 1]->AddXY(paraUA[nrunUA - 1],1.0,"",clBlue);
                }
                wgsChart[j]->LeftAxis->Maximum = 1.0;
                wgsChart[j]->BottomAxis->Maximum =  max(wgsSeries[2*j + 0]->MaxXValue(),wgsSeries[2*j + 1]->MaxXValue());
                MPSANSEC  = NashSutcliffeEC(nrunUA,paraA,paraUA);
                wgsChart[j]->Title->Text->Clear();
                wgsChart[j]->Title->Text->Add("Chart"+String(j + 1)+
                                              " [EC=" + FloatToStrF(MPSANSEC,ffFixed,6,4)+"]");

                ProgressBar2->Position = 100.0*(j + 1)/nopt;
        }//end j



        PageControl1->ActivePageIndex = 2;  //TabSheet1
        ProgressBar1->Visible = false;
        ProgressBar2->Visible = false;

        de_allocate(paraSort,nrun);
        delete[] VOBFr,paraA,paraUA;
        return;
} //end Import
//------------------------------------------------------------------------------
void TfrmMPSAmap::GLUEplot(int iTF,int nrun,double **VOBF)
{//针对EVOBS，EVMID，HMLEOBS进行变换，并实施"Fuzzy Add"（等权重）,"Fuzzy Union","Fuzzy Intersection"
//iTF = 0,归一化[UNI]，(max - x)/(max - min)
//iTF = 1,类似NSEC变换[NEC]，1 - x/EV0,EV0是实测值方差
//iTF = 2,Box-Taio变换[POW]，x^(-n)
//iTF = 3,指数变换[EXP]，exp(-n x)
//iTF = 4，NSEC,EVOBS^(-n),exp(-n EVOBS)[SYN]，但是归一化后，三者基本是NSEC最大，exp(-n EVOBS)最小

        int i,j,k;
        const int nx = 20;
        int num[nx];
        double xxmid[nx],xxleft[nx],xxright[nx];
        const int nChart = 6;
        double VGLUEmin[3],VGLUEmax[3],VG[3],b;
        AnsiString asUncertain[] = {"NSEC","Box Taio","exp[EV]","Fuzzy Add","Fuzzy Union","Fuzzy Intersection"};

        for (i = 0; i < GLUE1->Count; i++)
        {
                GLUE1->Items[i]->Checked = false;
        }
        if(iTF < GLUE1->Count) GLUE1->Items[iTF]->Checked = true;

        ProgressBar1->Visible = true;
        ProgressBar2->Visible = true;

        double *VOBFr = new double[nrun];
        double **VGLUE;
        VGLUE = new double*[nrun];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < nrun; i++)
                VGLUE[i] = new double[nChart];  // STEP 2: SET UP THE COLUMNS

        double **UGLUE;  //归一化VGLUE
        UGLUE = new double*[nrun];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < nrun; i++)
                UGLUE[i] = new double[nChart/2];  // STEP 2: SET UP THE COLUMNS

switch(iTF)
{
case 0:
        asUncertain[0] = "UNI[EVOBS]";
        asUncertain[1] = "UNI[EVMID]";
        asUncertain[2] = "UNI[HMLE]";
        for(j =0;j<3;j++)
        {
                for(i=0;i<nrun;i++)
                        VOBFr[i] = VOBF[i][j];
                VGLUEmin[j] = MinValue(VOBFr,nrun - 1);
                VGLUEmax[j] = MaxValue(VOBFr,nrun - 1);
                for(i=0;i<nrun;i++)
                {
                        VGLUE[i][j] = (VGLUEmax[j] - VOBF[i][j]) /(VGLUEmax[j] - VGLUEmin[j]);
                        UGLUE[i][j] = VGLUE[i][j];
                }
        }//end j
        break; //end case 0
//------------------------------------------------------------------------------
case 1:
        asUncertain[0] = "NSEC";   //NSEC
        asUncertain[1] = "NEC[EVMID]";
        asUncertain[2] = "NEC[HMLE]";
        for (i = 0; i < nrun; i++)
        {
                VGLUE[i][0] = max(0.0,VOBF[i][4]);      //NSEC                  [显示原值]
                VGLUE[i][1] = max(0.0,1.0 - (1 - VOBF[i][4])*VOBF[i][1]/VOBF[i][0]);     // [显示原值]
                VGLUE[i][2] = max(0.0,1.0 - (1 - VOBF[i][4])*VOBF[i][2]/VOBF[i][0]); //    [显示原值]
                ProgressBar1->Position = 100.0*i/nrun;
        }//end i

        for(j =0;j<3;j++)
        {
                for(i=0;i<nrun;i++)
                        VOBFr[i] = VGLUE[i][j];
                VGLUEmin[j] = MinValue(VOBFr,nrun - 1);
                VGLUEmax[j] = MaxValue(VOBFr,nrun - 1);
                for(i=0;i<nrun;i++) //归一化
                {
                        UGLUE[i][j] = (VGLUE[i][j] - VGLUEmin[j]) /(VGLUEmax[j] - VGLUEmin[j]);
                }
        }//end j
        break;    //end case 1
//------------------------------------------------------------------------------
case 2:
        asUncertain[0] = "POW[EVOBS]";
        asUncertain[1] = "POW[EVMID]";
        asUncertain[2] = "POW[HMLE]";
        b = -0.5;
        for (i = 0; i < nrun; i++)
        {
                VGLUE[i][0] = pow(VOBF[i][0],b);      // [显示原值]
                VGLUE[i][1] = pow(VOBF[i][1],b);;     // [显示原值]
                VGLUE[i][2] = pow(VOBF[i][2],b);; //    [显示原值]
                ProgressBar1->Position = 100.0*i/nrun;
        }//end i

        for(j =0;j<3;j++)
        {
                for(i=0;i<nrun;i++)
                        VOBFr[i] = VGLUE[i][j];
                VGLUEmin[j] = MinValue(VOBFr,nrun - 1);
                VGLUEmax[j] = MaxValue(VOBFr,nrun - 1);
                for(i=0;i<nrun;i++) //归一化
                {
                        UGLUE[i][j] = (VGLUE[i][j] - VGLUEmin[j]) /(VGLUEmax[j] - VGLUEmin[j]);
                }
        }//end j
        break;  // end case 2
//------------------------------------------------------------------------------
case 3:
        asUncertain[0] = "EXP[EVOBS]";
        asUncertain[1] = "EXP[EVMID]";
        asUncertain[2] = "EXP[HMLE]";
        b = -1.0e-2;
        for (i = 0; i < nrun; i++)
        {
                VGLUE[i][0] = exp(b*VOBF[i][0]);      // [显示原值]
                VGLUE[i][1] = exp(b*VOBF[i][1]);     // [显示原值]
                VGLUE[i][2] = exp(b*VOBF[i][2]); //    [显示原值]
                ProgressBar1->Position = 100.0*i/nrun;
        }//end i

        for(j =0;j<3;j++)
        {
                for(i=0;i<nrun;i++)
                        VOBFr[i] = VGLUE[i][j];
                VGLUEmin[j] = MinValue(VOBFr,nrun - 1);
                VGLUEmax[j] = MaxValue(VOBFr,nrun - 1);
                for(i=0;i<nrun;i++)  //归一化
                {
                        UGLUE[i][j] = (VGLUE[i][j] - VGLUEmin[j]) /(VGLUEmax[j] - VGLUEmin[j]); //归一化
                }
        }//end j
        break;  //end case 3
//------------------------------------------------------------------------------
case 4:
        asUncertain[0] = "NSEC";
        asUncertain[1] = "POW[EVOBS]";
        asUncertain[2] = "EXP[EVOBS]";
        for (i = 0; i < nrun; i++)
        {
                VGLUE[i][0] = max(0.0,VOBF[i][4]);      //NSEC                  [显示原值]
                VGLUE[i][1] = pow(VOBF[i][0],-0.5);     //BoxTaio,pow(EVOBS,-n) [显示原值]
                VGLUE[i][2] = exp(-1.0e-2*VOBF[i][0]); //EVexp,exp(-n*EVOBS)    [显示原值]
                ProgressBar1->Position = 100.0*i/nrun;
        }//end i

        for(j =0;j<3;j++)
        {
                for(i=0;i<nrun;i++)
                        VOBFr[i] = VGLUE[i][j];
                VGLUEmin[j] = MinValue(VOBFr,nrun - 1);
                VGLUEmax[j] = MaxValue(VOBFr,nrun - 1);
                for(i=0;i<nrun;i++)  //归一化
                {
                        UGLUE[i][j] = (VGLUE[i][j] - VGLUEmin[j]) /(VGLUEmax[j] - VGLUEmin[j]); //归一化
                }
        }//end j
        break; //end case 4
} //end switch
//------------------------------------------------------------------------------
//Fuzzy运算之前，必须进行归一化处理
        for (i = 0; i < nrun; i++)
        {
                for(j =0;j<3;j++)
                {
                        VG[j] = UGLUE[i][j];
                }
                VGLUE[i][3] = Mean(VG,2);     //FuzzyAdd                        [显示归一化值]
                VGLUE[i][4] = MaxValue(VG,2); //FuzzyUnion                      [显示归一化值]
                VGLUE[i][5] = MinValue(VG,2); //FuzzyIntersection               [显示归一化值]
                ProgressBar1->Position = 100.0*i/nrun;
        }//end i

        for(j=0;j<nChart;j++)
        {
                wgsSeries[nChart*4 + j]->Clear();
                wgsBSeries[j]->Clear();
                for(i=0;i<nrun;i++)
                {
                        VOBFr[i] = VGLUE[i][j];    //原值
                }//end i

                sort1(nrun,VOBFr);

//                wgsChart[nChart*4 + j]->BottomAxis->Maximum =  VOBFr[nrun - 1];//;
//                wgsChart[nChart*4 + j]->BottomAxis->Minimum =  VOBFr[0];//0.0;
//                wgsChart[nChart*4 + j]->BottomAxis->Increment =  0.2;

                wgsChart[nChart*4 + j]->LeftAxis->Title->Caption = "Density";
                wgsChart[nChart*4 + j]->LeftAxis->Title->Font->Color = clBlue;
                wgsChart[nChart*4 + j]->LeftAxis->Automatic = true;
                wgsChart[nChart*4 + j]->LeftAxis->Increment = 0.05;

                wgsChart[nChart*4 + j]->RightAxis->Automatic = true;
                wgsChart[nChart*4 + j]->RightAxis->Title->Caption = "Cumulative Density";
                wgsChart[nChart*4 + j]->RightAxis->Title->Angle = 90;
                wgsChart[nChart*4 + j]->RightAxis->Title->Font->Color = clBlue;
//                wgsChart[nChart*4 + j]->RightAxis->Maximum = 1.0;
//                wgsChart[nChart*4 + j]->RightAxis->Minimum = 0.0;
                wgsChart[nChart*4 + j]->RightAxis->Increment = 0.2;

                for(k=0;k<nx;k++)  //20 intervals for density plots
                {
                        num[k] = 0;
                        xxleft[k] = VOBFr[0] + (VOBFr[nrun - 1] - VOBFr[0])/nx*k;
                        xxright[k] = VOBFr[0] + (VOBFr[nrun - 1] - VOBFr[0])/nx*(k + 1);
                        xxmid[k] = 0.5*(xxleft[k] + xxright[k]);

                }//end k

                for(i=0;i<nrun;i++)
                {
                        for(k=0;k<nx;k++)  //20 intervals for density plots
                        {
                                if(VOBFr[i] >= xxleft[k] && VOBFr[i] < xxright[k])
                                        num[k] += 1;
                        }//end k
                        ProgressBar1->Position = 100.0*i/nrun;
                }// end i

                for(k=0;k<nx;k++)  //20 intervals for density plots
                {
                        wgsBSeries[j]->AddXY(xxmid[k],1.0*num[k]/nrun,"",clBlue);
                } //end k

                for(i=0;i<nrun;i++)
                {
                        wgsSeries[nChart*4 + j]->AddXY(VOBFr[i],(1.0 + i)/nrun,"",clRed);
                        ProgressBar1->Position = 100.0*i/nrun;
                }

                wgsChart[nChart*4 + j]->BottomAxis->Title->Caption = asUncertain[j];
                wgsChart[nChart*4 + j]->Title->Text->Clear();
                wgsChart[nChart*4 + j]->Title->Text->Add("Chart"+String(nChart*4 + j + 1)
                                                         + " [5%=" + FloatToStrF(VOBFr[int(0.05*nrun - 1)],ffFixed,6,2)
                                                         + ", 95%=" + FloatToStrF(VOBFr[int(0.95*nrun - 1)],ffFixed,6,2) + "]") ;

                ProgressBar2->Position = 100.0*(j + 1)/nChart;
        } //end j

        PageControl1->ActivePageIndex = 4;  //TabSheet1
        ProgressBar1->Visible = false;
        ProgressBar2->Visible = false;

        delete[] VOBFr;
        de_allocate(VGLUE,nrun);
        de_allocate(UGLUE,nrun);
}// end GLUEplot

void __fastcall TfrmMPSAmap::BtnAxisXClick(TObject *Sender)
{
        int iChart;
        double xmax,xmin,xinc;

        if(DlgChartEdit->ShowModal()==mrOk)
        {
                iChart = DlgChartEdit->ComboBox1->Text.ToInt();
                xmax = DlgChartEdit->EdtMax->Text.ToDouble();
                xmin = DlgChartEdit->EdtMin->Text.ToDouble();
                xinc = DlgChartEdit->EdtInc->Text.ToDouble();

                wgsChart[iChart - 1]->BottomAxis->Maximum =  xmax;
                wgsChart[iChart - 1]->BottomAxis->Minimum =  xmin;
                wgsChart[iChart - 1]->BottomAxis->Increment =  xinc;
        }
}
//---------------------------------------------------------------------------


void __fastcall TfrmMPSAmap::FormClose(TObject *Sender,
      TCloseAction &Action)
{
        delete[] wgsChart;
        delete[] wgsSeries;
        delete[] wgsPSeries;
        delete[] wgsBSeries;
        de_allocate(para,nrun);
        de_allocate(VOBF,nrun);
}
//---------------------------------------------------------------------------





void __fastcall TfrmMPSAmap::Exit1Click(TObject *Sender)
{
        this->Close();        
}
//---------------------------------------------------------------------------


void __fastcall TfrmMPSAmap::ChangeAxisX()
{
        int iChart;
        double xmax,xmin,xinc;

        if(DlgChartEdit->ShowModal()==mrOk)
        {
                iChart = DlgChartEdit->ComboBox1->Text.ToInt();
                xmax = DlgChartEdit->EdtMax->Text.ToDouble();
                xmin = DlgChartEdit->EdtMin->Text.ToDouble();
                xinc = DlgChartEdit->EdtInc->Text.ToDouble();

                wgsChart[iChart - 1]->BottomAxis->Maximum =  xmax;
                wgsChart[iChart - 1]->BottomAxis->Minimum =  xmin;
                wgsChart[iChart - 1]->BottomAxis->Increment =  xinc;
        }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMPSAmap::ChangeAxisX1Click(TObject *Sender)
{
        ChangeAxisX();
}
//---------------------------------------------------------------------------



void __fastcall TfrmMPSAmap::EVobs1Click(TObject *Sender)
{

        MPSAplot(0,nrun,nopt,para,VOBF);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::EVMID1Click(TObject *Sender)
{
        MPSAplot(1,nrun,nopt,para,VOBF);        
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::HMLEOBS1Click(TObject *Sender)
{
        MPSAplot(2,nrun,nopt,para,VOBF);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::HMLEMID1Click(TObject *Sender)
{
        MPSAplot(3,nrun,nopt,para,VOBF);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::NSEC2Click(TObject *Sender)
{
        MPSAplot(4,nrun,nopt,para,VOBF);
}
//---------------------------------------------------------------------------


void __fastcall TfrmMPSAmap::SYN1Click(TObject *Sender)
{
        GLUEplot(4,nrun,VOBF);
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::FormCreate(TObject *Sender)
{
        MPSA1->Enabled = false;
        GLUE1->Enabled = false;        
}
//---------------------------------------------------------------------------


void __fastcall TfrmMPSAmap::UNI1Click(TObject *Sender)
{
        GLUEplot(0,nrun,VOBF);        
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::NEC1Click(TObject *Sender)
{
        GLUEplot(1,nrun,VOBF);        
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::POW1Click(TObject *Sender)
{
        GLUEplot(2,nrun,VOBF);        
}
//---------------------------------------------------------------------------

void __fastcall TfrmMPSAmap::EXP1Click(TObject *Sender)
{
        GLUEplot(3,nrun,VOBF);        
}
//---------------------------------------------------------------------------

