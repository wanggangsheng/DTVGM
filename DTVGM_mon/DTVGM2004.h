//============================================================================//
//=  DTVGM.h                                                               ===//
//=  Distributed Time-Variant Gain Model                                   ===//
//=                                                                        ===//
//=  Written by GANGSHENG WANG                                             ===//
//=  Key Lab. of Water Cycle & Related Land Surface Processes,             ===//
//=  Institute of Geographic Sciences & Natural Resources Research (IGSNRR)===//
//=  Chinese Academy of Sciences (CAS)                                     ===//
//=  Anwai, Datun Road, No.11A, 100101, Beijing, China                     ===//
//=  Tel./Fax: +86 10 64889010 / 64856534                                  ===//
//=  Email: wanggs@igsnrr.ac.cn                                            ===//
//=                                                                        ===//
//=  October 18 - 24, 2004.                                                ===//
//============================================================================//



//#include<stdio.h>
//#include<stdlib.h>
#include <math.h>
#include "iostream.h" //max(a,b),min(a,b),FILE
#include <dateutils.hpp> //YearOf
#include <DBTables.hpp>  //TTable
#include <ADODB.hpp> //TADOTable

#include "GlobalVariables.h"   //blVar

//#include <algorithm>

/*************************************************
*          The Definition of Constant            *
*                                                *
*************************************************/



/*************************************************
*      The Definition of Global Variables        *
*************************************************/
//int isDTVGM;  //isDTVGM = 1, RUN DTVGM; else, RUN functn
//double bestpara[16]; //optimized Paramters
struct TFPoint
        {
                TFPoint() {}
                TFPoint(double _x, double _y) : x(_x), y(_y) {}
                TFPoint(POINT& pt)
                {
                        x = pt.x;
                        y = pt.y;
                }
                operator POINT() const
                {
                        POINT pt;
                        pt.x = x;
                        pt.y = y;
                        return pt;
                }
                double  x;
                double  y;
        };  //TFPoint

/*************************************************
*         Declaration of Prototype               *
*************************************************/
double hmle(int ndata, double *obsq, double *q);

double DTVGM_Month(int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
                              String strDirRain,String strPrefix_rainfile,
                              int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
                              int *gridSubID,double *gridArea,double *gridRCC,
                              double *Array_ETp,double *RunoffObs,
                              double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
                              double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
                              double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC);

double ETatoETp(double PtoETp,double BagrovN,int nBagrovN,double *ArrayofBagrovN);
int wgsMonthsBetween(TDate dateBegin,TDate dateEnd);
AnsiString IntToStrF(int iNum, int iLength);
void LinearInterp(struct TFPoint FPoint[]);

double FuncETatoETp(double PtoETp,double BagrovN);
void SaveGridData(bool blSave, char *txtFileName,
                            int ngrid, int *gridID,double *gridP, double *gridETa, double *gridSM,
                            double *gridWU,double *gridR, double *gridRS, double *gridRSS, double *gridRG);
//------------------------------------
double fscanfdouble(FILE *finput);
void de_allocate(double **data,int m);
int DeleteAllRecords(TTable *Table);
int DeleteAllRecords(TADOTable *ADOTable);
double NashSutcliffeEC(int DataSize,double *DataObs,double *DataCal);
double CorrelationCoefficient(int n, double *xx, double *yy);
double HMLE(double a,int n,double *DataObs,double *DataCal);
double FuncLxy(int n, double *xx, double *yy);

void sort(int n,int m,double **rb,double *ra);
void sort1(int n,double *ra);
void sort1(int n,int *ra);
void indexx(int n, double *arrin, int *indx);
double  ran1(int &idum);
double  gasdev(int idum);
//==============================================================================================


/*************************************************
*                    Main Program                *
*************************************************/

//==================================================================
double DTVGM_Month(int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
                              String strDirRain,String strPrefix_rainfile,
                              int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
                              int *gridSubID,double *gridArea,double *gridRCC,
                              double *Array_ETp,double *RunoffObs,
                              double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
                              double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
                              double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC)
{
//int nBasinGrid: Grids(subbasins) of Wbasin(the whole basin);
//int ngrid: subbasins of Obasin (the basin controled by an outlet);
//int *gridSubID: subbasin ID of Obasin in Wbasin;
//double *gridArea: Area of subbasin in Obasin
//double *gridRCC: Region Convert Coefficient of subbasin Runoff
//double *Mean_wateruse: Mean of subbasin wateruse in Obasin

        int iyear,imonth,iyear0,imonth0,nMonths;
        int i,j,k;
        double AW0,AWC,g1,g2,Kr,KAW,PDc,PDc_basic,BasinArea,KET;//
        double ETp,PtoETp,dlETatoETp,doublevalue;
        double Weight_WB,ObjectFunction;
        float ftemp;
        AnsiString asFile;
        FILE *stream;
        char *saveFile;

//------------------------------------------------------------------------------
//Monthly Influence Factor, Chaobaihe River Basin
        double facym;//Influence Factor of a certain month
        double facmon[12] = {0.70,0.80,1.10,0.80,0.60,0.40,0.01,0.01,0.40,0.50,0.60,0.70}; //YeAizhong
        //double facmon[12] = {0.62,1.12,1.71,0.69,-0.62,-0.38,0.09,-0.10,0.86,0.34,0.36,0.42};//chao:1973-1979
        //double facmon[12] = {0.16,0.52,1.24,0.17,-0.75,-0.73,-0.42,-0.53,-0.21,-0.13,-0.19,-0.19}; //chao: 1980-1990
        //double facmon[12] = {-0.19,0.15,0.84,0.11,-0.73,-0.81,-0.54,-0.61,-0.16,-0.18,-0.29,-0.27}; //chao: 1983-1988

        //double facmon[12] = {0.25,0.80,1.11,0.18,-0.66,-0.55,-0.48,0.04,-0.22,-0.18,-0.11,-0.10}; //chao: 1991-2001

        //double facmon[12] = {0.21,0.66,1.18,0.18,-0.71,-0.64,-0.47,-0.19,-0.22,-0.16,-0.15,-0.15};//chao: 1980-2001
        //double facmon[12] = {0.10 ,0.78 ,1.36 ,0.61 ,-0.43 ,-0.55 ,-0.41 ,-0.24 ,-0.41 ,-0.42 ,-0.32 ,-0.25};//bai: 1973-2001

        //double facmon[12] = {0.42,1.09,1.82,0.99,-0.32,-0.46,-0.29,-0.17,-0.25,-0.29,-0.17,-0.04}; //bai: 1973-1988
        //double facmon[12] = {-0.28 ,0.20 ,1.22 ,0.00 ,-0.55 ,-0.62 ,-0.64 ,-0.69 ,-0.46 ,-0.60 ,-0.59 ,-0.56}; //bai: 1983-1988
        //double facmon[12] = {0.38,0.93,1.66,0.95,-0.17,-0.34,-0.26,-0.14,-0.07,-0.08,0.03,0.08}; //bai: 1973-1988new2


        //double facmon[12] = {-0.22,0.47,0.91,0.22,-0.55,-0.64,-0.53,-0.35,-0.58,-0.55,-0.48,-0.45}; //bai: 1989-2001
        //double facmon[12] = {-0.2170,0.4681,0.9078,0.2195,-0.5521,-0.6364,-0.5344,-0.3513,-0.5752,-0.5525,-0.4818,-0.4486}; //bai: 1989-2001

//------------------------------------------------------------------------------

        g1 = Parameter_optimize[0];
        g2 = Parameter_optimize[1];
        Kr = Parameter_optimize[2];
        KAW = Parameter_optimize[3];
        AW0 = Parameter_optimize[4];
//        aRCC = Parameter_optimize[5];  //产流折算

        AWC = Parameter[0];
        PDc = Parameter[1];
        PDc_basic = Parameter[2];
        KET = Parameter[3];
//        BasinArea = Parameter[4];      //这里不用该参数
        Weight_WB = Parameter[5];  //Weight_WaterBalance

        nMonths  = wgsMonthsBetween(dateBegin,dateEnd) ;  //计算月数，MonthsBetween(2003-1-1,2004-1-1) = 11,实际上包括头尾有13个月
        iyear = YearOf(dateBegin);
        imonth = MonthOf(dateBegin) - 1;

        float *gridrain = new float[nBasinGrid];  //必须定义成float类型，才能从文本文件中正确读取网格雨量
        double *dlgridrain = new double[ngrid];
        double *gridSurfaceRunoff = new double[ngrid];
        double *gridSubsurfaceRunoff = new double[ngrid];
        double *gridRunoff = new double[ngrid];
        double *gridETa = new double[ngrid];
        double *gridsm0 = new double[ngrid];  //时段初土壤湿度
        double *gridsm = new double[ngrid];   //时段末土壤湿度
        double *gridWateruse = new double[ngrid];  //Wateruse of subbasin
        double *gridRunoffV = new double[ngrid];  //=gridRunoff*gridArea

        float *gridETp = new float[nBasinGrid];  //2003-11-20必须定义成float类型，才能从文本文件中正确读取网格ETp
        double *dlgridETp = new double[ngrid];  //2003-11-20

        for(k=0;k<ngrid;k++)
                gridsm[k] = AW0;

        for(i=0;i<nMonths;i++) //处理iMonth个月的数据
        {
                imonth = imonth + 1;
                if(imonth>12)
                        {iyear = iyear + 1;imonth = 1;}
                //ETp = KET*Array_ETp[i];  //2003-11-20,Potential Evapotranspiration,KET为水面蒸发折算为潜在蒸发的系数
                //Kr0 = 0.5*Kr + 0.25*ETp/AWC; //构造另一个参数
                asFile = Trim(strDirRain + strPrefix_rainfile + IntToStr(iyear) + IntToStrF(imonth,2) + ".txt");//Open File: Gridrain

                if(FileExists(asFile))
                {
                        if((stream=fopen(asFile.c_str(),"rt"))==NULL)
                        {
                                fprintf(stderr,"Cannot open input file.\n");
                                return -100;
                        }
                }
                else
                {
                        ShowMessage(AnsiString("Couldnot find the file:")+ asFile +".");
                        return -200;
                }

                for(k=0;k<nBasinGrid;k++)  // !feof(stream)
                {       fscanf(stream,"%d %d %d %f %f",&iyear0,&imonth0,&j,&gridrain[k],&gridETp[k]); // 2003-11-20,fscanf(stream,"\n");
                }
                fclose(stream);

                 for(k=0;k<ngrid;k++)  // !feof(stream)
                        {dlgridrain[k] = gridrain[gridSubID[k]];
                        if(fabs(KET - 1.0) < 1.0e-2) //采用子流域雨量文件中的ETp
                                dlgridETp[k] = KET*gridETp[gridSubID[k]]; //2003-11-20,KET为水面蒸发折算为潜在蒸发的系数
                        else   //KET < 1 ,通常，KET ＝ 0 
                                dlgridETp[k] = (1.0 - KET)*Array_ETp[i]; //2005-01-17,直接使用MonFlow表中的潜在蒸散发
                         gridsm0[k] = gridsm[k];
                        }

                for(k=0;k<ngrid;k++)
                { //Runoff Generation
                        ETp = dlgridETp[k];   //2003-11-20
                        PtoETp = dlgridrain[k]/ETp;//max(1.0e-2,ETp);
                        //PtoETp = KAW*(gridsm0[k] + dlgridrain[k])/ETp;//2005-01-19
                        doublevalue = gridBagrovN[k];
                        dlETatoETp = ETatoETp(PtoETp,doublevalue,nBagrovN,ArrayofBagrovN);
                        gridETa[k] = ETp*((1 - KAW)*dlETatoETp + KAW*gridsm0[k]/AWC);   // 2003-10-30 (1 - KAW)*

                        doublevalue = (dlgridrain[k]>PDc?(PDc/PDc_basic):1)*g1*Power(gridsm0[k]/AWC,g2);  //Power(dlETatoETp + KAW*gridsm0[k]/AWC,g2)
                        gridSurfaceRunoff[k] = min(doublevalue,1.0)*dlgridrain[k]; //Attention,min(doublevalue,1)出错

                        doublevalue = (dlgridrain[k] - gridSurfaceRunoff[k] - gridETa[k] + (1 - 0.5*Kr)*gridsm0[k])/(1 + 0.5*Kr);   //sm(t+1)
                        if(doublevalue>=0)
                                {if(doublevalue>AWC)
                                        {gridsm[k] = AWC;
                                         gridSubsurfaceRunoff[k] = 0.5*Kr*(gridsm0[k] + gridsm[k]);
                                         ftemp = gridsm0[k] + dlgridrain[k] - gridSubsurfaceRunoff[k] - gridSurfaceRunoff[k] - gridsm[k];
                                         gridETa[k]= min(ftemp,ETp);
                                         gridSurfaceRunoff[k] = gridsm0[k] + dlgridrain[k] - gridSubsurfaceRunoff[k] - gridETa[k] - gridsm[k];
                                        }
                                 else  //doublevalue = [0,AWC]
                                        {gridsm[k] = doublevalue;
                                         gridSubsurfaceRunoff[k] = 0.5*Kr*(gridsm0[k] + gridsm[k]);}
                                }
                        else   //doublevalue < 0
                                {if((gridsm0[k] + dlgridrain[k] - gridSurfaceRunoff[k] - gridETa[k])<0)
                                        {gridSubsurfaceRunoff[k] = 0;gridETa[k] = gridsm0[k] + dlgridrain[k] - gridSurfaceRunoff[k];}
                                 else
                                        {gridSubsurfaceRunoff[k] = gridsm0[k] + dlgridrain[k] - gridSurfaceRunoff[k] - gridETa[k];}
                                 gridsm[k] = 0;
                                }

                        if(fabs(gridRCC[k] - 1.0) < 1.0e-3) //不考虑影响因子
                                facym = 1.0;
                        else
                                facym = gridRCC[k] + facmon[imonth - 1];  //年因子＋月因子

                        gridRunoff[k] = (gridSurfaceRunoff[k] + gridSubsurfaceRunoff[k])*facym;
                        gridRunoffV[k] = gridRunoff[k]*gridArea[k];
                        gridWateruse[k] = gridSurfaceRunoff[k] + gridSubsurfaceRunoff[k] - gridRunoff[k];

                        //Save Grid Data
                        asFile = Trim(strDirRain + "GData\\GR" + IntToStr(iyear) + IntToStrF(imonth,2) + ".dat");
                        saveFile = asFile.c_str();
                        SaveGridData(blVar,saveFile,ngrid, gridSubID,dlgridrain, gridETa, gridsm, gridWateruse,gridRunoff, gridSurfaceRunoff,gridSubsurfaceRunoff, gridSubsurfaceRunoff);
                } //k
                RunoffCal[i] = Sum(gridRunoffV,ngrid - 1)/(3*24*36);  //mm->m^3/s
                Mean_gridrain[i] = Mean(dlgridrain,ngrid - 1);
                Mean_gridRunoff[i] = Mean(gridRunoff,ngrid - 1);
                Mean_gridSurfaceRunoff[i] = Mean(gridSurfaceRunoff,ngrid - 1);
                Mean_gridSubsurfaceRunoff[i] = Mean(gridSubsurfaceRunoff,ngrid - 1);
                Mean_gridETa[i] = Mean(gridETa,ngrid - 1);
                Mean_gridsm[i] = Mean(gridsm,ngrid - 1);
                Mean_wateruse[i] = Mean(gridWateruse,ngrid - 1);
                ProgressBar->Position = 100.0*i/nMonths;
        }//i

//=*********************************************************
//=ATTENTION: 计算数组的平均值，                           *
//=用函数Mean(const double * Data, const int Data_Size)时, *
//=Data_Size应该是【数组元素个数 － 1】                    ×
//=必须是double类型数组                                    *
//=*******************************************************=//
        WB = Sum(RunoffCal,nMonths - 1)/Sum(RunoffObs,nMonths - 1);
        NSEC  = NashSutcliffeEC(nMonths,RunoffObs,RunoffCal);
        ObjectFunction = Weight_WB*fabs(1.0 - WB) + (1.0 - Weight_WB)*fabs(1.0 - NSEC);

        delete[] gridrain;
        delete[] gridSurfaceRunoff;
        delete[] gridSubsurfaceRunoff;
        delete[] gridRunoff;
        delete[] gridETa;
        delete[] gridsm0;
        delete[] gridsm;
        delete[] dlgridrain;
        delete[] gridWateruse;
        delete[] gridRunoffV;

        delete[] gridETp;
        delete[] dlgridETp;
        
        return ObjectFunction;
}  //End  DTVGM_Month
//==============================================================================

AnsiString IntToStrF(int iNum, int iLength)
{
        AnsiString sRet;
        int iLen;
        //sRet = Trim(IntToStr(iNum));
        iLen = (Trim(IntToStr(iNum))).Length();
        for(int i=0;i<(iLength - iLen);i++)
                sRet = sRet + "0";
        sRet = sRet + Trim(IntToStr(iNum));
       
        return sRet;
}

//============================================================================
int wgsMonthsBetween(TDate dateBegin,TDate dateEnd)
{//计算两个日期之间的月数，包括开始和结束的月份，如1989-9-3与1990-6-9之间有10个月
        int imonths;
        if(dateBegin > dateEnd)
                {ShowMessage(DateToStr(dateBegin)+" > "+ DateToStr(dateEnd));
                 return 0;}
        if(YearOf(dateBegin) == YearOf(dateEnd))
                imonths = MonthOf(dateEnd) - MonthOf(dateBegin) + 1;
        else
                imonths = (12 - MonthOf(dateBegin) + 1) + (MonthOf(dateEnd))
                         +((YearOf(dateEnd) - YearOf(dateBegin) - 1)*12);
        return imonths;

}//TMDIChild::wgsMonthBetween
//============================================================================
double ETatoETp(double PtoETp,double BagrovN,int nBagrovN,double *ArrayofBagrovN)
{//PtoETp=[0,5], Compute ETa/ETp according to P/ETp and BagrovN
        int i,NumofArray;
        double ETatoETp;
        struct TFPoint *FPoint = new struct TFPoint[3];
        if(PtoETp > 5.0)
                PtoETp = 5.0;
        else if(PtoETp < 0)
                PtoETp = 0.0;
        FPoint[0].x = BagrovN;
        //ReadArrayofBagrovN(ArrayofBagrovN);
        //NumofArray = sizeof(ArrayofBagrovN)/sizeof(ArrayofBagrovN[0]);
        //ShowMessage(String(sizeof(ArrayofBagrovN))+":"+String(sizeof(ArrayofBagrovN[0]))+":"+String(NumofArray));

        for(i=0;i<nBagrovN - 1;i++)
        {
                if((BagrovN >= ArrayofBagrovN[i])&&(BagrovN < ArrayofBagrovN[i + 1]))
                        {       FPoint[1].x = RoundTo(ArrayofBagrovN[i],-1);
                                FPoint[2].x = RoundTo(ArrayofBagrovN[i + 1],-1);
                                goto Label_GoTo;
                        }
        }
        Label_GoTo:
        FPoint[1].y = FuncETatoETp(PtoETp,FPoint[1].x);
        FPoint[2].y = FuncETatoETp(PtoETp,FPoint[2].x);
        /*
        String strAlias,strTable,strField,strQuery;
        strAlias = DM->ADOConnection1->DefaultDatabase;
        strTable = "BagrovN";
        strField = "PtoETp";
        
        strQuery = "Select BagrovN,PtoETp,ETatoETp FROM " + strTable + " WHERE " + strField + "=" + FloatToStrF(PtoETp,ffFixed,4,1)
                  + " and (BagrovN = " + FloatToStrF(FPoint[1].x,ffFixed,4,1) + " or BagrovN = " + FloatToStrF(FPoint[2].x,ffFixed,4,1) + ")";
        DataQuery(false,strAlias,strTable,strField,strQuery);
        if(ADOQuery1->Active)
                {
                ADOQuery1->First();
                for(i=0;i<2;i++)
                        {FPoint[i + 1].y = ADOQuery1->FieldValues["ETatoETp"];
                         ADOQuery1->Next();
                        }
                }  */
        LinearInterp(FPoint);
        ETatoETp = FPoint[0].y;
        delete[] FPoint;
        return ETatoETp;
}
//============================================================================
void LinearInterp(struct TFPoint FPoint[])
{//根据FPoint[1]、FPoint[2]以及FPoint[0].x, 计算FPoint[0].y
        if((FPoint[2].x - FPoint[1].x)!=0)
                FPoint[0].y = FPoint[1].y +
                              (FPoint[0].x - FPoint[1].x)*
                              (FPoint[2].y - FPoint[1].y)/
                              (FPoint[2].x - FPoint[1].x);
        else
                FPoint[0].y = 0.5*(FPoint[2].y + FPoint[1].y);

}  //TMDIChild::LinearInterp
//============================================================================
void SaveGridData(bool blSave, char *txtFileName,
                            int ngrid, int *gridID,double *gridP, double *gridETa, double *gridSM,
                            double *gridWU,double *gridR, double *gridRS, double *gridRSS, double *gridRG)
{
//gridRS: Grid Surface Runoff
//gridRSS: grid SubSurface Runoff
//gridRG: grid Gound Runoff
//gridWU: grid Water Use
        if(blSave == false) return;
        FILE *stream;
        int iFileHandle;
        if(!FileExists(txtFileName))
        {       iFileHandle = FileCreate(txtFileName);FileClose(iFileHandle);}
        if((stream = fopen(txtFileName, "w+"))==NULL)
        {
                ShowMessage("File Open Failed!!!");
                return;
        }
        fprintf(stream, "%s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t",
                        "SubBasin","gridP","gridETa","gridSM","gridWU","gridR","gridRS","gridRSS","gridRG");
        fprintf(stream, "\n");
        for(int j=0;j<ngrid;j++)
        {
                fprintf(stream,"%d\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t %6.2f\t",
                                gridID[j],gridP[j],gridETa[j],gridSM[j],gridWU[j],gridR[j],gridRS[j],gridRSS[j],gridRG[j]);
                fprintf(stream, "\n");
        }

        fclose(stream);
        return;
}

//============================================================================
double FuncETatoETp(double PtoETp,double BagrovN)
{//Bagrov方法的多项式拟合
       double ETatoETp;
       if(BagrovN==0.1)
                ETatoETp = -0.0008*Power(PtoETp,4) + 0.0112*Power(PtoETp,3) - 0.0629*Power(PtoETp,2) + 0.2463*PtoETp + 0.0131;
       else if(BagrovN==0.2)
                ETatoETp = -0.0015*Power(PtoETp,4) + 0.0204*Power(PtoETp,3) - 0.1127*Power(PtoETp,2) + 0.3871*PtoETp + 0.0184;
       else if(BagrovN==0.3)
                ETatoETp = -0.0020*Power(PtoETp,4) + 0.0278*Power(PtoETp,3) - 0.1537*Power(PtoETp,2) + 0.4924*PtoETp + 0.0211;
       else if(BagrovN==0.5)
                ETatoETp = -0.0029*Power(PtoETp,4) + 0.0398*Power(PtoETp,3) - 0.2208*Power(PtoETp,2) + 0.6492*PtoETp + 0.0229;
       else if(BagrovN==0.7)
                ETatoETp = -0.0035*Power(PtoETp,4) + 0.0497*Power(PtoETp,3) - 0.2764*Power(PtoETp,2) + 0.7674*PtoETp + 0.0209;
       else if(BagrovN==1.0)
                ETatoETp = -0.0044*Power(PtoETp,4) + 0.0632*Power(PtoETp,3) - 0.3483*Power(PtoETp,2) + 0.9054*PtoETp + 0.0147;
       else if(BagrovN==1.6)
                ETatoETp = 0.0005*Power(PtoETp,6) - 0.0067*Power(PtoETp,5) + 0.0292*Power(PtoETp,4) + 0.0037*Power(PtoETp,3) - 0.3779*Power(PtoETp,2) + 1.0707*PtoETp + 0.0008;
       else if(BagrovN==2.0)
                ETatoETp = 0.0054*Power(PtoETp,6) - 0.0637*Power(PtoETp,5) + 0.2794*Power(PtoETp,4) - 0.5060*Power(PtoETp,3) + 0.0732*Power(PtoETp,2) + 0.9671*PtoETp + 0.0066;
       else if(BagrovN==3.0)
                ETatoETp = 0.0136*Power(PtoETp,6) - 0.1524*Power(PtoETp,5) + 0.6488*Power(PtoETp,4) - 1.2287*Power(PtoETp,3) + 0.6853*Power(PtoETp,2) + 0.8450*PtoETp + 0.0104;
       else if(BagrovN==5.0)
                ETatoETp = -0.1087*Power(PtoETp,6) + 0.6090*Power(PtoETp,5) - 1.0460*Power(PtoETp,4) + 0.3317*Power(PtoETp,3) + 0.1869*Power(PtoETp,2) + 0.8984*PtoETp + 0.0098;
       else if(BagrovN==8.0)
                ETatoETp = -0.3027*Power(PtoETp,6) + 1.8127*Power(PtoETp,5) - 3.7628*Power(PtoETp,4) + 3.0122*Power(PtoETp,3) - 0.8961*Power(PtoETp,2) + 1.0469*PtoETp + 0.0075;

       if(ETatoETp > 1.0) ETatoETp = 1.0;
       return ETatoETp;
}
//============================================================================
//==============================================================================
double fscanfdouble(FILE *finput)
{
        char *endptr,cdouble[20];
        double rdata;
        fscanf(finput,"%s",&cdouble);
        rdata = strtod(cdouble,&endptr);  //ATTENTION!!!xmin必须定义成double，否则小数点后的数字转换不正确
        return rdata;
}
//==============================================================================

void de_allocate(double **data,int m)
{
   for (int i = 0; i < m;  i++)
       delete[] data[i];                 // STEP 1: DELETE THE COLUMNS

   delete[] data;                        // STEP 2: DELETE THE ROWS

} // end  de_allocate
//============================================================================
int DeleteAllRecords(TTable *Table)
{
        int iRecordCount = Table->RecordCount;;
        Table->First();
        while(Table->RecordCount>0)
                { Table->Delete();
                  Table->FindNext();}
        return iRecordCount;
}

int DeleteAllRecords(TADOTable *ADOTable)
{
        int iRecordCount = ADOTable->RecordCount;
        if(iRecordCount > 0)
                {       ADOTable->First();
                        while(!ADOTable->Eof)
                                { ADOTable->Delete();
                                  ADOTable->Next();}
                }
        return iRecordCount;
}
//============================================================================
double NashSutcliffeEC(int DataSize,double *DataObs,double *DataCal)
{//Nash-Sutcliffe Efficiency Criterion
        double EC,MeanDataObs;
        double sum1 = 0,sum2 = 0;
        MeanDataObs = Mean(DataObs,DataSize - 1);
        for(int i=0;i<DataSize;i++)
        {
                sum1 = sum1 + (DataObs[i] - MeanDataObs)*(DataObs[i] - MeanDataObs);
                sum2 = sum2 + (DataObs[i] - DataCal[i])*(DataObs[i] - DataCal[i]);
        }
        EC = (sum1 = 0 ? 0:(1 - sum2/sum1));
        return EC;
}
//==============================================================================
double CorrelationCoefficient(int n, double *xx, double *yy)
{
        double rCor,Lxx,Lyy,Lxy;
        Lxx = FuncLxy(n,xx,xx);
        Lyy = FuncLxy(n,yy,yy);
        Lxy = FuncLxy(n,xx,yy);
        rCor = Lxy/sqrt(Lxx*Lyy);
        return rCor;
} //end CorrelationCoefficient
//==============================================================================
double HMLE(double a,int n,double *DataObs,double *DataCal)
{//HMLE（Heteroscedastic Maximum Likelihood Estimator, 异方差最大似然估计）
//a:[0,1]
        double rHMLE,wi;
        double sum1 = 0,prd1 = 1.0;

        for(int i=0;i<n;i++)
        {
                wi = pow(DataObs[i],2*(a - 1));
                sum1 += wi*(DataObs[i] - DataCal[i])*(DataObs[i] - DataCal[i]);
                prd1 *= wi;
        }
        rHMLE = sum1/n*pow(prd1,-1.0/n);
        return rHMLE;
} //end HMLE
//==============================================================================
double ErrorVariance(int n,double *DataObs,double *DataCal)
{//Error Variance: sum of squared errors）

        double rEV;
        double sum1 = 0;

        for(int i=0;i<n;i++)
        {
                sum1 += (DataObs[i] - DataCal[i])*(DataObs[i] - DataCal[i]);
        }
        rEV = sum1/n;
        return rEV;
} //end ErrorVariance
//==============================================================================
double FuncLxy(int n, double *xx, double *yy)
{
        double rLxy,sumxy,sumx,sumy;
        int i;

        sumxy = 0.0;
        for(i=0;i<n;i++)
                sumxy += xx[i]*yy[i];
        sumx = Sum(xx,n - 1);
        sumy = Sum(yy,n - 1);
        rLxy = sumxy - sumx*sumy/n;
        return rLxy;

} //end Lxy
//==============================================================================
//===================================================================
void sort(int n,int m,double **rb,double *ra)
{//[0...n]
//$debug

//c  SORTING SUBROUTINE ADAPTED FROM "NUMERICAL RECIPES"
//c  BY W.H. PRESS ET AL., pp. 233-234

//c  LIST OF VARIABLES
//c     ra(n) = array to be sorted
//c     rb(n,m) = arrays ordered corresponding to rearrangement of ra(.)
//c     wk(.,.), iwk(.) = local varibles

//      implicit real*8 (a-h,o-z)
//      dimension ra(2000),rb(2000,16),wk(2000,16),iwk(2000)
      int i,j;
//      double wk[2000][16];
//      int iwk[2000];
      int *iwk = new int[n];

      double **wk; // wk[n][m],
      wk = new double*[n];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < n; i++)
                wk[i] = new double[m];  // STEP 2: SET UP THE COLUMNS

      indexx(n,ra,iwk);
      for(i=0;i<n;i++)
        wk[i][0] = ra[i];

      for(i=0;i<n;i++)
        ra[i] = wk[iwk[i]][0];

      for(j=0;j<m;j++)
      {
        for(i=0;i<n;i++)
                wk[i][j] = rb[i][j];
      }

      for(j=0;j<m;j++)
      {
        for(i=0;i<n;i++)
                rb[i][j] = wk[iwk[i]][j];
      }

      delete[] iwk;
      de_allocate(wk,m);
      return;
}//c  END OF SUBROUTINE SORT
      

//===========================================================
void sort1(int n,double *ra)
{//[0...n]
//$debug
//sort ASCending
//c  SORTING SUBROUTINE ADAPTED FROM "NUMERICAL RECIPES"
//c  BY W.H. PRESS ET AL., pp. 231

//c  LIST OF VARIABLES
//c     ra(.) = integer array to be sorted

//      implicit real*8 (a-h,o-z)
//      dimension ra(n)
      int i,j,l,ir;
      double rra;

      l = (n / 2)  ;  //ie: n=5,l=2; n=6,l=3
      ir = n - 1;
      wgs10:
      if (l > 0)
      {
        l = l - 1;
        rra = ra[l];
      }
      else
      { rra = ra[ir];
        ra[ir] = ra[0];
        ir = ir - 1;
        if (ir == 0)
        {
                ra[0] = rra;
                return;
        }
      }
      i = l;
      j = l + l + 1;
      wgs20:
      if (j <= ir)
      {
        if (j < ir)
        {
                if (ra[j] < ra[j + 1]) j = j + 1;
        }
        if (rra < ra[j]) 
        {       ra[i] = ra[j];
                i = j;
                j = j + j + 1;
        }
        else
                j = ir + 1 ;
      //end if(rra .lt. ra[j])
        goto wgs20;
      }//end if (j <= ir)
      ra[i] = rra;
      goto wgs10;

}//c  END OF SUBROUTINE SORT1

//=============================================================================
void sort1(int n,int *ra)
{//[0...n],对int数组进行排序，上面的sort1（）是对double型进行排序
//$debug
//sort ASCending
//c  SORTING SUBROUTINE ADAPTED FROM "NUMERICAL RECIPES"
//c  BY W.H. PRESS ET AL., pp. 231

//c  LIST OF VARIABLES
//c     ra(.) = integer array to be sorted

//      implicit real*8 (a-h,o-z)
//      dimension ra(n)
      int i,j,l,ir;
      double rra;

      l = (n / 2)  ;  //ie: n=5,l=2; n=6,l=3
      ir = n - 1;
      wgs10:
      if (l > 0)
      {
        l = l - 1;
        rra = ra[l];
      }
      else
      { rra = ra[ir];
        ra[ir] = ra[0];
        ir = ir - 1;
        if (ir == 0)
        {
                ra[0] = rra;
                return;
        }
      }
      i = l;
      j = l + l + 1;
      wgs20:
      if (j <= ir)
      {
        if (j < ir)
        {
                if (ra[j] < ra[j + 1]) j = j + 1;
        }
        if (rra < ra[j]) 
        {       ra[i] = ra[j];
                i = j;
                j = j + j + 1;
        }
        else
                j = ir + 1 ;
      //end if(rra .lt. ra[j])
        goto wgs20;
      }//end if (j <= ir)
      ra[i] = rra;
      goto wgs10;

}//c  END OF SUBROUTINE SORT1



//c=======================================================
void indexx(int n, double *arrin, int *indx)
{//[0...n]
//$debug

//c  THIS SUBROUTINE IS FROM "NUMERICAL RECIPES" BY PRESS ET AL.
//      implicit real*8 (a-h,o-z)
//      dimension arrin(n), indx(n)
// Attention: 1...n, excluding 0
      int i,j,l,ir,indxt;
      double q;

      for(j = 0;j<n;j++)
        indx[j] = j;
//   11 continue
      l = (n / 2) ;
      ir = n - 1;
      wgs10:
      if (l > 0)
      { l = l - 1;
        indxt = indx[l];
        q = arrin[indxt];
      }
      else
      {
        indxt = indx[ir];
        q = arrin[indxt];
        indx[ir] = indx[0];
        ir = ir - 1;
        if (ir == 0)
        {
                indx[0] = indxt;
                return;
        }//end if
      }//end if
      i = l;
      j = l + l + 1;
      wgs20:
      if (j <= ir)
      {
        if (j < ir)
        {
                if (arrin[indx[j]] < arrin[indx[j + 1]]) j = j + 1;
        }//end if (j < ir)
        if (q < arrin[indx[j]])
        {       indx[i] = indx[j];
                i = j;
                j = j + j + 1;
        }
        else
                j = ir + 1;
      //end if
      goto wgs20;
      }//end if(j <= ir)
      indx[i] = indxt;
      goto wgs10;

//c  END OF SUBROUTINE INDEXX
//      end
}  //c  END OF SUBROUTINE INDEXX

//==============================================================


