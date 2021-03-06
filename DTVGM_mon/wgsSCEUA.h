//#include<stdio.h>
//#include<stdlib.h>
#include <math.h>
#include "iostream.h" //max(a,b),min(a,b),FILE

#include "DTVGM.h"

//#include <algorithm>
//============================================================================//
//=  SCEUA.h                                                               ===//
//=  SHUFFLED COMPLEX EVOLUTION METHOD FOR GLOABL MINIMUM OPTIMIZATION     ===//
//=                                                                        ===//
//=  Translated and revised by GANGSHENG WANG                              ===//
//=  Key Lab. of Water Cycle & Related Land Surface Processes,             ===//
//=  Institute of Geographic Sciences & Natural Resources Research (IGSNRR)===//
//=  Chinese Academy of Sciences (CAS)                                     ===//
//=  Anwai, Datun Road, No.11A, 100101, Beijing, China                     ===//
//=  Tel./Fax: +86 10 64888128 / 64856534                                  ===//
//=  Email: wanggs@igsnrr.ac.cn                                            ===//
//=                                                                        ===//
//=  October 18 - 24, 2004.                                                ===//
//============================================================================//


//c  SHUFFLED COMPLEX EVOLUTION METHOD FOR GLOBAL OPTIMIZATION
//c     -- Version 2.1
//c
//c  by QINGYUN DUAN
//c  DEPARTMENT OF HYDROLOGY & WATER RESOURCES
//c  UNIVERSITY OF ARIZONA, TUCSON, AZ 85721
//c  (602) 621-9360, email: duan@hwr.arizona.edu
//c                                                         
//c  WRITTEN IN OCTOBER 1990.
//c  REVISED IN AUGUST 1991
//c  REVISED IN APRIL 1992
//c
//c  STATEMENT BY AUTHOR:
//c  --------------------
//c
//c     This general purpose global optimization program is developed at
//c     the Department of Hydrology & Water Resources of the University
//c     of Arizona.  Further information regarding the SCE-UA method can
//c     be obtained from Dr. Q. Duan, Dr. S. Sorooshian or Dr. V.K. Gupta
//c     at the address and phone number listed above.  We request all
//c     users of this program make proper reference to the paper entitled
//c     'Effective and Efficient Global Optimization for Conceptual
//c     Rainfall-runoff Models' by Duan, Q., S. Sorooshian, and V.K. Gupta,
//c     Water Resources Research, Vol 28(4), pp.1015-1031, 1992.
//c

//c  LIST OF INPUT ARGUEMENT VARIABLES
//c
//c     a(.) = initial parameter set
//c     bl(.) = lower bound on parameters
//c     bu(.) = upper bound on parameters
//c     nopt = number of parameters to be optimized
//c
//c
//c  LIST OF SCE ALGORITHMIC CONTROL PARAMETERS:
//c
//c     ngs = number of complexes in the initial population
//c     npg = number of points in each complex
//c     npt = total number of points in initial population (npt=ngs*npg)
//c     nps = number of points in a sub-complex
//c     nspl = number of evolution steps allowed for each complex before
//c         complex shuffling
//c     mings = minimum number of complexes required, if the number of
//c         complexes is allowed to reduce as the optimization proceeds
//c     iseed = initial random seed
//c     iniflg = flag on whether to include the initial point in population
//c         = 0, not included
//c         = 1, included
//c     iprint = flag for controlling print-out after each shuffling loop
//c         = 0, print information on the best point of the population
//c         = 1, print information on every point of the population
//c
//c
//c  CONVERGENCE CHECK PARAMETERS
//c
//c     maxn = max no. of trials allowed before optimization is terminated
//c     kstop = number of shuffling loops in which the criterion value must
//c         chang by the given percentage before optimization is terminated
//c     pcento = percentage by which the criterion value must change in
//c         given number of shuffling loops
//c     ipcnvg = flag indicating whether parameter convergence is reached
//c         (i.e., check if gnrng is less than 0.001)
//c         = 0, parameter convergence not satisfied
//c         = 1, parameter convergence satisfied
//c
//c
//c  LIST OF LOCAL VARIABLES
//c     x(.,.) = coordinates of points in the population
//c     xf(.) = function values of x(.,.)
//c     xx(.) = coordinates of a single point in x
//c     cx(.,.) = coordinates of points in a complex
//c     cf(.) = function values of cx(.,.)
//c     s(.,.) = coordinates of points in the current simplex
//c     sf(.) = function values of s(.,.)
//c     bestx(.) = best point at current shuffling loop
//c     bestf = function value of bestx(.)
//c     worstx(.) = worst point at current shuffling loop
//c     worstf = function value of worstx(.)
//c     xnstd(.) = standard deviation of parameters in the population
//c     gnrng = normalized geometric mean of parameter ranges
//c     lcs(.) = indices locating position of s(.,.) in x(.,.)
//c     bound(.) = bound on ith variable being optimized
//c     ngs1 = number of complexes in current population
//c     ngs2 = number of complexes in last population
//c     iseed1 = current random seed
//c     criter(.) = vector containing the best criterion values of the last
//c         10 shuffling loops
//c


/*************************************************
*          The Definition of Constant            *
*                                                *
*************************************************/



/*************************************************
*      The Definition of Global Variables        *
*************************************************/
//#define npar 3
//int iobj;
//int ndata;

//int loc[npar];
//double pt[npar];
//double obsq[1000], q[1000]; //
/*
extern int iff;            //Global Variables
extern int ix1,ix2,ix3;    //Global Variables
extern double r[97];       //Global Variables
*/
//int iff;            //Global Variables
//int ix1,ix2,ix3;    //Global Variables
//double r[97];       //Global Variables

//int isDTVGM;  //isDTVGM = 1, RUN DTVGM; else, RUN functn
double bestpara[16]; //optimized Paramters 

AnsiString FileIn,FileOut;
FILE *fin,*ipr;
int npt; //nopt,

//int maxn,kstop,ngs,iseed;//ideflt;
//double pcento;
//int npg,nps,nspl,mings,iniflg,iprint;
//double a[npar], bl[npar], bu[npar];


/*************************************************
*         Declaration of Prototype               *
*************************************************/
double (*functn)(int nopt,double *x);
void FunctSet();

double functn1(int nopt,double *x);
double functn2(int nopt,double *x);
double functn3(int nopt,double *x);
double functn4(int nopt,double *x);
double functn5(int nopt,double *x);
double functn6(int nopt,double *x);
double functn7(int nopt,double *x);  //result different
double functn9(int nopt,double *x);

void chkcst(int nopt,double *x,double *bl,double *bu,int &ibound);
double hmle(int ndata, double *obsq, double *q);

//==============================================================================================
void scein(double *a,double *bl,double *bu,int &nopt,int &maxn,int &kstop,double &pcento,int &iseed,
           int &ngs,int &npg,int &nps,int &nspl,int &mings,int &iniflg,int &iprint);

void sceua(double *a,double *bl,double *bu,int nopt,int maxn,int kstop,double pcento,int iseed,
           int ngs,int npg,int nps,int nspl,int mings,int iniflg,int iprint);
void sceua(double *a,double *bl,double *bu,int nopt,int maxn,int kstop,double pcento,int iseed,
           int ngs,int npg,int nps,int nspl,int mings,int iniflg,int iprint,
           int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
           String strDirRain,String strPrefix_rainfile,
           int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
           int *gridSubID,double *gridArea,double *gridRCC,
           double *Array_ETp,double *RunoffObs,
           double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
           double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
           double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC);

void cce(int nopt,int nps,double **s,double *sf,double *bl,double *bu,double *xnstd,
         int &icall,int maxn,int iseed,int &iff,int *ix,double *r);
void cce(int nopt,int nps,double **s,double *sf,double *bl,double *bu,double *xnstd,
         int &icall,int maxn,int iseed,
         int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
         String strDirRain,String strPrefix_rainfile,
         int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
         int *gridSubID,double *gridArea,double *gridRCC,
         double *Array_ETp,double *RunoffObs,
         double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
         double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
         double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC,int &iff,int *ix,double *r);

void getpnt(int nopt,int idist,int &iseed,double *x,double *bl,double *bu,double *std,double *xi,int &iff, int *ix,double *r);

void parstt(int npt,int nopt,double **x,double *xnstd,double *bound,double &gnrng,int &ipcnvg);
void comp(int n,int npt,int ngs1,int ngs2,int npg,double **a,double *af,double **b,double *bf);
double  ran1(int &idum,int &iff,int *ix,double *r);
double  gasdev(int &idum,int &iff,int *ix,double *r);
/*
void sort(int n,int m,double **rb,double *ra);
void sort1(int n,double *ra);
void sort1(int n,int *ra);
void indexx(int n, double *arrin, int *indx);
void de_allocate(double **data,int m);
*/
/*************************************************
*                    Main Program                *
*************************************************/
void FunctSet()
{

        functn = &functn5;
        FileIn = "E:\\RecentWork\\zSCEUA\\scein5.dat";
        FileOut = "E:\\RecentWork\\zSCEUA\\wgsout5.txt";
}
//==================================================================

//==============================================================================

double functn1(int nopt,double *x)
{
//c  This is the Goldstein-Price Function
//c  Bound X1=[-2,2], X2=[-2,2]
//c  Global Optimum: 3.0,(0.0,-1.0)
//      implicit real*8 (a-h,o-z)
//      dimension x(nopt)

      double rfunctn;
      double x1,x2,u1,u2,u3,u4,u5,u6;
      if (nopt > 2)
      {
        ShowMessage(" ???? CHECK THE INPUT FILE ???? ");
        return -9999;
      }//end if

      x1 = x[0];
      x2 = x[1];
      u1 = pow((x1 + x2 + 1.0),2);
      u2 = 19. - 14.*x1 + 3.*x1*x1 - 14.*x2 + 6.*x1*x2 +3.*x2*x2;
      u3 = (2.*x1 - 3.*x2)*(2.*x1 - 3.*x2);
      u4 = 18. - 32.*x1 + 12.*x1*x1 + 48.*x2 -36.*x1*x2 + 27.*x2*x2;
      u5 = u1 * u2;
      u6 = u3 * u4;
      rfunctn = (1. + u5) * (30. + u6);

      return rfunctn;
}     // end functn1
//==================================================================
double functn2(int nopt,double *x)
{
//c  This is the Rosenbrock Function
//c  Bound: X1=[-5,5], X2=[-2,8]
//c  Global Optimum: 0,(1,1)
//      implicit real*8 (a-h,o-z)
//      dimension x(nopt)
      double rfunctn;

      double a = 100.0;
      rfunctn = a * (x[1] - x[0]*x[0])*(x[1] - x[0]*x[0]) + (1 - x[0])*(1 - x[0]);

      return rfunctn;
} //      end functn2
//==================================================================
double functn3(int nopt,double *x)
{
//c  This is the Six-hump Camelback Function.
//c  Bound: X1=[-5,5], X2=[-5,5]
//c  True Optima: -1.03628453489877, (-0.08983,0.7126), (0.08983,-0.7126)
//      implicit real*8 (a-h,o-z)
//      dimension x(nopt)
      double rfunctn;
      if (nopt > 2)
      {
        ShowMessage(" ???? CHECK THE INPUT FILE ???? ");
        return -9999;
      }//end if

      rfunctn = (4. - 2.1*x[0]*x[0] + pow(x[0],4)/3.)*x[0]*x[0] +
                x[0]*x[1] + (-4. + 4.*x[1]*x[1])*x[1]*x[1];
      return rfunctn;
} //      end functn3
//==================================================================
double functn4(int nopt,double *x)
{
//c  This is the Rastrigin Function
//c  Bound: X1=[-1,1], X2=[-1,1]
//c  Global Optimum: -2, (0,0)
//      implicit real*8 (a-h,o-z)
//      dimension x(nopt)
      double rfunctn;
      double x1,x2;
      if (nopt > 2) 
      {
        ShowMessage(" ???? CHECK THE INPUT FILE ???? ");
        return -9999;
      }//end if

      x1 = x[1];
      x2 = x[2];
      rfunctn = x1*x1 + x2*x2 - cos(18.0*x1) - cos(18.0*x2);

      return rfunctn;
} //      end functn4

//==================================================================
double functn5(int nopt,double *x)
{
//c  This is the Griewank Function (2-D or 10-D)
//c  Bound: X(i)=[-600,600], for i=1,2,...,10
//c  Global Optimum: 0, at origin
//      implicit real*8 (a-h,o-z)
//      dimension x(nopt)

      double rfunctn;
      double d,u1,u2;
      if (nopt == 2)
        d = 200.0;
      else
        d = 4000.0;


      u1 = 0.0;
      u2 = 1.0;
      for(int j = 0;j< nopt;j++)
      {
        u1 = u1 + x[j]*x[j] / d;
        u2 = u2 * cos(x[j]/sqrt(j + 1.0));
      }
      rfunctn = u1 - u2 + 1;

      return rfunctn;
} //      end functn5
//==================================================================
double functn6(int nopt,double *x)
{
//c  This is the Shekel Function
//c  Bound: X(i)=[0,10], j=1,2,3,4
//c  Global Optimum:-10.5364098252,(4,4,4,4)
      int i,j;
      double rfunctn,u;
//      implicit real*8 (a-h,o-z)
//      dimension x(nopt)
//c  Data for Skekel function coefficients (n=4, m=10)
      double a1[10][4] = {{4.,1.,8.,6.},{3.,2.,5.,8.},{6.,7.,4.,1.},
                          {8.,6.,7.,9.},{5.,1.,2.,3.6},{4.,1.,8.,6.},
                          {3.,2.,3.,8.},{6.,7.,4.,1.},{8.,6.,7.,9.},{3.,1.,2.,3.6}};
      double c1[10] = {.1,.2,.2,.4,.4,.6,.3,.7,.5,.5};

      if (nopt > 4)
      {
        ShowMessage(" ???? CHECK THE INPUT FILE ???? ");
        return -9999;
      }//end if

      rfunctn = 0.0;
      for(i = 0;i< 10;i++)
      {
        u = 0.0;
        for(j = 0;j< nopt;j++)
          u = u + (x[j] - a1[i][j])*(x[j] - a1[i][j]);

        u = 1.0 / (u + c1[i]);
        rfunctn = rfunctn - u;
      } //i

      return rfunctn;
} //      end functn6

//==================================================================
double functn7(int nopt,double *x)
{
//c   This is the Hartman Function
//c   Bound: X(j)=[0,1], j=1,2,...,6
//c   Global Optimum:-3.322368011415515,
//c   (0.201,0.150,0.477,0.275,0.311,0.657)
//      implicit real*8 (a-h,o-z)
//      dimension x(nopt),a2(4,6),c2(4),p2(4,6),a3(4,3),c3(4),p3(4,3)

//c  Data for Hartman function coefficients (6-D)
      double a2[4][6] = {{10.,0.05,3.,17.,3.,10.},
                         {3.5,8.,17.,17.,1.7,.05},
                         {3.5,0.1,10.,10.,1.7,8.},
                         {17.,.1,8.,14.,8.,14.}};
      double c2[4] = {1.,1.2,3.,3.2};
      double p2[4][6] = {{.1312,.2329,.2348,.4047,.1696,.4135},
                         {.1451,.8828,.5569,.8307,.3522,.8732},
                         {.0124,.3736,.2883,.5743,.8283,.1004},
                         {.3047,.1091,.5886,.9991,.6650,.0381}};

//cc  Data for Hartman function coefficient (3-D)
      double a3[4][3] = {{3.,.1,3.},{.1,10.,10.},{10.,10.,30.},{35.,30.,35.}};
      double c3[4] =  {1.,1.2,3.,3.2};
      double p3[4][3] = {{.3689,.4699,.1091},{.03815,.1170,.4387},
                         {.8732,.5743,.2673},{.7470,.5547,.8828}};
      double rfunctn,u;
      if (nopt > 6)
      {
        ShowMessage(" ???? CHECK THE INPUT FILE ???? ");
        return -9999;
      }//end if

      rfunctn = 0.0;
      for(int i = 0;i< 4;i++)
      {
        u = 0.0;
        for(int j = 0;j< nopt;j++)
        {
          if (nopt == 3)
          {
            a2[i][j] = a3[i][j];
            p2[i][j] = p3[i][j];
          }//end if
          u = u + a2[i][j] * (x[j] - p2[i][j])*(x[j] - p2[i][j]);
        }//j 40   continue
        if (nopt == 3) c2[i] = c3[i];
        rfunctn = rfunctn - c2[i] * exp(-u);
      }  //i 50 continue
      return rfunctn;
} //      end functn7

//==================================================================

double functn9(int nopt,double *x)
{

//c     This function is originally from Rosenbrock (1960)
//c     Optimum at (24,12,12)
//c
//c     if ifunc = 1, 0.0<=x(i)<=42.0
//c     if ifunc = 2, 0.0<=x(i)<=1.0

//      implicit real*8 (a-h,o-z)
//      dimension x(nopt)

      int ifunc;
      double rfunctn;

      if (nopt != 3)
      {
        ShowMessage(" ???? CHECK THE INPUT FILE ???? ");
        return -9999;
      }//end if

      ifunc = 1;
      if (ifunc == 1) rfunctn = - x[0]*x[1]*x[2];
      if (ifunc == 2) rfunctn = 2.0 - x[0]*x[1]*x[2];
      return rfunctn;
}//  END OF FUNCTION functn

//==================================================================
void chkcst(int nopt,double *x,double *bl,double *bu,int &ibound)
{
//$debug

//c     This subroutine check if the trial point satisfies all
//c     constraints.
//c
//c     ibound - violation indicator
//c            = -1 initial value
//c            = 0  no violation
//c            = 1  violation
//c     nopt = number of optimizing variables
//c     ii = the ii'th variable of the arrays x, bl, and bu


//      dimension x(nopt),bl(nopt),bu(nopt)
      int ii;
      int ifunc;
      double c1,cbound;
      ibound = -1;

//c     Check if explicit constraints are violated

      for(ii=0;ii< nopt;ii++)
        if (x[ii] < bl[ii] || x[ii] > bu[ii]) goto wgs10;

      if (nopt == 1) goto wgs9;

//c     Check if implicit constraints are violated
//c
      /*
      constraints for Functn9
      c1 = x[0] + 2.0*x[1] + 2.0*x[2];
      ifunc = 1;
      if (ifunc == 1) cbound = 72.;
      if (ifunc == 2) cbound = 2.;
      if (c1 > cbound) goto wgs10;
      */

//c     No constraints are violated

    wgs9:
      ibound = 0;
      return;
    wgs10:
      ibound = 1;
      return;
} //      end subroutine chkcst(nopt,x,bl,bu,ibound)

//==============================================================================
//==============================================================================
//==============================================================================
void scein(double *a,double *bl,double *bu,int &nopt,int &maxn,int &kstop,double &pcento,int &iseed,
           int &ngs,int &npg,int &nps,int &nspl,int &mings,int &iniflg,int &iprint)
{
//$debug

//c   THIS SUBROUTINE READS AND PRINTS THE INPUT VARIABLES FOR
//c   SHUFFLED COMPLEX EVOLUTION METHOD FOR GLOBAL OPTIMIZATION
//c     -- Version 2.1

//c   WRITTEN BY QINGYUN DUAN - UNIVERSITY OF ARIZONA, APRIL 1992

//      implicit real*8 (a-h,o-z)
//      dimension a(16),bl(16),bu(16)
//      common /iopar/ in,ipr
//      character*10 pcntrl,deflt,usrsp
//      character*4 reduc,initl,ysflg,noflg,xname(16)
AnsiString pcntrl,deflt,usrsp,ysflg,noflg,reduc,initl ;
deflt = "DEFAULT";
usrsp = "USER SPEC.";
ysflg = "YES ";
noflg = "NO ";

//      data xname /'  X1','  X2','  X3','  X4','  X5','  X6','  X7',
//     &'  X8','  X9',' X10',' X11',' X12',' X13',' X14',' X15',' X16'/

//      write (*,*) ' ENTER THE SCEIN SUBROUTINE --- '

//c  INITIALIZE I/O VARIABLES

 int ierror,iwarn,iopt,ideflt,use0;
 float pcento0,a0,bl0,bu0;
 char* sFile;
// AnsiString FileIn,FileOut;

// FileIn = "E:\\RecentWork\\zSCEUA\\scein6.dat";
// FileOut = "E:\\RecentWork\\zSCEUA\\wgsout6.txt";
 sFile = FileIn.c_str();
 if(FileExists(sFile))
 {
   if((fin=fopen(sFile,"r"))==NULL)
   {
     fprintf(stderr,"Cannot open input file.\n");
     return;
   }
 }
 else
 {
  ShowMessage(AnsiString("Couldnot find the file:")+sFile+".");
  return;
 }

   sFile = FileOut.c_str();
   if((ipr=fopen(sFile,"w+"))==NULL)
   {
     fprintf(stderr,"Cannot open input file.\n");
     return;
   }
   

      ierror = 0;
      iwarn = 0;
      fprintf(ipr,"SHUFFLED COMPLEX EVOLUTION GLOBAL OPTIMIZATION");

//c  READ THE SCE CONTROL PARAMETERS
      ideflt = 0;
      fscanf(fin, "%d%d%f%d%d%d",&maxn,&kstop,&pcento0,&ngs,&iseed,&ideflt);
      pcento = pcento0;  //float->double
      if (iseed == 0) iseed = 1969;

//c  IF ideflt IS EQUAL TO 1, READ THE SCE CONTROL PARAMETERS
//    if (ideflt == 1)
      {
        fscanf(fin,"%d%d%d%d%d%d",&npg,&nps,&nspl,&mings,&iniflg,&iprint);
        pcntrl = usrsp;
      }
//      else
      {
//        fscanf(fin,"%d%d%d%d%d%d",&use0,&use0,&use0,&use0,&use0,&use0);
//        pcntrl = deflt;
      }

//      ShowMessage("Finish SCEUAIN step1");

//c  READ THE INITIAL PARAMETER VALUES AND THE PARAMETER BOUNDS
      iopt = 0;
      while(!feof(fin))
      {
        fscanf(fin,"%f%f%f",&a0,&bl0,&bu0);
        a[iopt] = a0;
        bl[iopt] = bl0;
        bu[iopt] = bu0;
        iopt = iopt + 1;
      }
      nopt = iopt;

      
//c  IF ideflt IS EQUAL TO 0, SET THE SCE CONTROL PARAMETERS TO
//c  THE DEFAULT VALUES
      if (ideflt == 0)
      {
        npg = 2*nopt + 1;
        nps = nopt + 1;
        nspl = npg;
        mings = ngs;
        iniflg = 0;
        iprint = 0;
        pcntrl = deflt;
      }//end if

//c  CHECK IF THE SCE CONTROL PARAMETERS ARE VALID
      if (ngs < 1 || ngs >= 1320)
      {
        fprintf(ipr,"**ERROR** NUMBER OF COMPLEXES IN INITIAL POPULATION %d IS NOT A VALID CHOICE", ngs);
        ierror = ierror + 1;
      }//end if

      if (kstop < 0 || kstop >= 20)
      {
        fprintf(ipr,"**WARNING** THE NUMBER OF SHUFFLING LOOPS IN WHICH THE CRITERION VALUE MUST CHANGE kstop = %d WAS SPECIFIED.BUT kstop = 5 WILL BE USED INSTEAD.", kstop);
        iwarn = iwarn + 1;
        kstop=5;
      }//end if
//===========================================================================
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//===========================================================================

//c  COMPUTE THE TOTAL NUMBER OF POINTS IN INITIAL POPULATION
      npt = ngs * npg;

      if (npt > 1320)
      {
        fprintf(ipr,"**WARNING** THE NUMBER OF POINTS IN INITIAL POPULATION %d  EXCEED THE POPULATION LIMIT,SET NGS TO 2, AND NPG, NPS AND NSPL TO DEFAULTS", npt);
  
        iwarn = iwarn + 1;
        ngs = 2;
        npg = 2*nopt + 1;
        nps = nopt + 1;
        nspl = npg;
      }//end if
//      ShowMessage("Finish SCEUAIN step2");

//  PRINT OUT THE TOTAL NUMBER OF ERROR AND WARNING MESSAGES
      if (ierror >= 1) fprintf(ipr,"*** TOTAL NUMBER OF ERROR MESSAGES IS %d", ierror);

      if (iwarn >= 1) fprintf(ipr,"*** TOTAL NUMBER OF WARNING MESSAGES IS %d",iwarn);
 
      if (mings < ngs)
        reduc = ysflg;
      else
        reduc = noflg;


      if (iniflg != 0)
        initl = ysflg;
      else
        initl = noflg;
//===========================================================================
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//===========================================================================

   fclose(fin);   //close the input file
//   fclose(ipr); //SCEUA中还要写入数据

   ShowMessage("Finish SCEUAIN to END ");
  
}
//C  END OF SUBROUTINE SCEIN

//==============================================================================
//==============================================================================
void sceua(double *a,double *bl,double *bu,int nopt,int maxn,int kstop,double pcento,int iseed,
           int ngs,int npg,int nps,int nspl,int mings,int iniflg,int iprint)
{

//c  ARRAYS FROM THE INPUT DATA
//      dimension a(16),bl(16),bu(16)
//c
//c  LOCAL ARRAYS
      int iff = 0;
      int *ix = new int[3];
      double *r = new double[97];

      int imax = ngs * npg;   //total number of points 
      int jmax = nopt;        //number of optimized variables
      int kmax = nps;         //number of points in each complex
      double *xx     = new double[jmax];
      double *bestx  = new double[jmax];
      double *worstx = new double[jmax];
      double *xnstd  = new double[jmax];
      double *bound  = new double[jmax];
      double *unit   = new double[jmax];

      double *xf = new double[imax];
      double *cf = new double[imax];

      double *sf = new double[kmax];
      int   *lcs = new int[kmax];
      
      double criter[20]; //vector containing the best criterion values of the last 10 shuffling loops
      

      double fa,bestf,worstf,gnrng,rand,denomi,timeou;
      
      int i,j,k,k1,k2,l,icall,ipcnvg,lpos;
      int nloop,loop,igs,nopt1,nopt2,iseed1,npt,npt1,ngs1,ngs2;

      double **x; // x[imax][jmax],
      x = new double*[imax];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < imax; i++)
                x[i] = new double[jmax];  // STEP 2: SET UP THE COLUMNS

      double **s;      // s[kmax][jmax],
      s = new double*[kmax];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < kmax; i++)
                s[i] = new double[jmax];  // STEP 2: SET UP THE COLUMNS

      double **cx;     //cx[imax][jmax],
      cx = new double*[imax];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < imax; i++)
                cx[i] = new double[jmax];  // STEP 2: SET UP THE COLUMNS
//      common /iopar/ in,ipr

//      character*4 xname(16)
//      AnsiString xname[] = {"X1","X2","X3","X4","X5","X6","X7",
//                            "X8","X9","X10","X11","X12","X13","X14","X15","X16"};
      AnsiString *xname = new AnsiString[nopt];
      for(i = 0;i < nopt;i++)
        xname[i] = "X" + IntToStr(i); 

      fprintf(ipr,"\n=============================================");
      fprintf(ipr,"\n=====<<< ENTER THE SCEUA SUBROUTINE >>>======");
      fprintf(ipr,"\n=============================================\n");


//c  INITIALIZE VARIABLES
      nloop = 0;
      loop = 0;
      igs = 0;
      nopt1 = 8;
      if (nopt<8) nopt1 = nopt;
      nopt2 = 12;
      if (nopt<12) nopt2 = nopt;
//c
//c  INITIALIZE RANDOM SEED TO A NEGATIVE INTEGER
      iseed1 = -abs(iseed);
//c
//c  COMPUTE THE TOTAL NUMBER OF POINTS IN INITIAL POPUALTION
      npt = ngs * npg;
      ngs1 = ngs;
      npt1 = npt;

      fprintf(ipr,"\nENTER THE SHUFFLED COMPLEX EVOLUTION  <<>>  GLOBAL SEARCH");
//      fprintf(ipr,"\n***  Evolution Loop Number = %d",nloop);

//c  COMPUTE THE BOUND FOR PARAMETERS BEING OPTIMIZED
      for(j = 0;j< nopt;j++)
      {
        bound[j] = bu[j] - bl[j];
        unit[j] = 1.0;
      }//end do j
//c
//c  COMPUTE THE FUNCTION VALUE OF THE INITIAL POINT
      fa = functn(nopt,a);
//c
//c  PRINT THE INITIAL POINT AND ITS CRITERION VALUE



      fprintf(ipr,"\n*** PRINT THE INITIAL POINT AND ITS CRITERION VALUE ***");
      fprintf(ipr,"\n");
      fprintf(ipr,"CRITERION\t");
      for(j=0;j<nopt2;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");

      fprintf(ipr,"%10.3f",fa);
      for(j=0;j<nopt2;j++)
        fprintf(ipr,"%10.3f",a[j]);
      fprintf(ipr,"\n");

      if (nopt<13) goto wgs101;
      for(j=12;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");

      for(j=12;j<nopt;j++)
        fprintf(ipr,"%10.3f",a[j]);
      fprintf(ipr,"\n");

  wgs101:

//c
//c  GENERATE AN INITIAL SET OF npt1 POINTS IN THE PARAMETER SPACE
//c  IF iniflg IS EQUAL TO 1, SET x(1,.) TO INITIAL POINT a(.)
      if (iniflg == 1)
      {
        for(j = 0;j< nopt;j++)
          x[0][j] = a[j];
        xf[0] = fa;
      }
//c
//c  ELSE, GENERATE A POINT RANDOMLY AND SET IT EQUAL TO x(1,.)
      else
      {
        getpnt(nopt,1,iseed1,xx,bl,bu,unit,bl,iff,ix,r);
        for(j = 0;j< nopt;j++)
          x[0][j] = xx[j];

        xf[0] = functn(nopt,xx);
      }//end if
      icall = 1;
      if (icall >= maxn) goto wgs9000;
//c
//c  GENERATE npt1-1 RANDOM POINTS DISTRIBUTED UNIFORMLY IN THE PARAMETER
//c  SPACE, AND COMPUTE THE CORRESPONDING FUNCTION VALUES
      for(i = 1;i< npt1;i++)
      {  getpnt(nopt,1,iseed1,xx,bl,bu,unit,bl,iff,ix,r);
        for(j = 0; j<nopt;j++)
          x[i][j] = xx[j];

        xf[i] = functn(nopt,xx);
        icall = icall + 1;
        if (icall >= maxn)
        {
          npt1 = i;
          goto wgs45;
        }//end if
      } //end do i
//c
//c  ARRANGE THE POINTS IN ORDER OF INCREASING FUNCTION VALUE
   wgs45: sort(npt1,nopt,x,xf);
//c
//c  RECORD THE BEST AND WORST POINTS
      for(j = 0;j< nopt;j++)
      {
        bestx[j] = x[0][j];
        worstx[j] = x[npt1 - 1][j];
      }//end do j
      bestf = xf[0];
      worstf = xf[npt1 - 1];
//c
//c  COMPUTE THE PARAMETER RANGE FOR THE INITIAL POPULATION
      parstt(npt1,nopt,x,xnstd,bound,gnrng,ipcnvg);
//c
//c  PRINT THE RESULTS FOR THE INITIAL POPULATION

  
  

      fprintf(ipr,"*** PRINT THE RESULTS OF THE SCE SEARCH ***");
      fprintf(ipr,"\n");
      fprintf(ipr,"LOOP  TRIALS  COMPLXS  BEST-F  WORST-F  PAR-RNG\t\t");

      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }
      //fprintf(ipr,"\n");

      if (nopt < 9) goto wgs201;
       for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }

  wgs201:
      fprintf(ipr,"\n");
      fprintf(ipr,"%d\t%d\t%d%10.3f%10.3f%10.3f", nloop,icall,ngs1,bestf,worstf,gnrng);
      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

      if (nopt < 9) goto wgs301;
      for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

  wgs301:
      fprintf(ipr,"\n");
      if (iprint == 1)
      {
        fprintf(ipr,"POPULATION AT LOOP %d",nloop);
        fprintf(ipr,"\n");
        for(i = 0;i< npt1;i++)
        {
          fprintf(ipr,"%10.3f\t", xf[i]);
          for(j =0;j<nopt1;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);
          if (nopt < 9) goto wgs401;
          for(j =8;j<nopt;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);
         }//i
  wgs401:
      }//end if

      if (icall >= maxn) goto wgs9000;
      if (ipcnvg == 1) goto wgs9200;

//c  BEGIN THE MAIN LOOP ----------------
 wgs1000:
      nloop = nloop + 1;

//      fprintf (ipr," ***  Evolution Loop Number  = %d",nloop);

//c  BEGIN LOOP ON COMPLEXES
      for(igs = 0;igs<ngs1;igs++)//do 3000 igs = 1, ngs1
      {
//c  ASSIGN POINTS INTO COMPLEXES
      for(k1 = 0;k1< npg;k1++)
      {
        k2 = k1 * ngs1 + igs; //(k1-1) * ngs1 + igs;
        for(j = 0;j< nopt;j++)
          cx[k1][j] = x[k2][j];

        cf[k1] = xf[k2];
      }//end do k1

//c  BEGIN INNER LOOP - RANDOM SELECTION OF SUB-COMPLEXES ---------------
      for(loop = 0;loop<nspl;loop++)//do 2000 loop = 1, nspl
      {
//c  CHOOSE A SUB-COMPLEX (nps points) ACCORDING TO A LINEAR
//c  PROBABILITY DISTRIBUTION
      if (nps == npg)
      {
        for(k = 0;k< nps;k++)
          lcs[k] = k;

        goto wgs85;
      }//end if
////***************************************************************************
////调试到此处
      rand = ran1(iseed1,iff,ix,r);//random(1e10)/1.0e10;

      if(((npg+.5)*(npg+.5) - npg * (npg+1) * rand ) < 0.e-10) ShowMessage("ERROR sqrt, floor1 " );

      lcs[0] = floor(npg + 0.5 - sqrt( (npg+.5)*(npg+.5) - npg * (npg+1) * rand )); //1 + floor(), 小写
      for(k = 1;k< nps;k++)
      {
   wgs60:
        rand = ran1(iseed1,iff,ix,r);//random(1e10)/1.0e10;

        if(((npg+.5)*(npg+.5) - npg * (npg+1) * rand ) < 0.e-10) ShowMessage("ERROR sqrt, floor2");

        lpos = floor(npg + 0.5 - sqrt((npg+.5)*(npg+.5) - npg * (npg+1) * rand )); //1 +
        for(k1 = 0;k1 < k;k1++)
          if (lpos == lcs[k1]) goto wgs60;

        lcs[k] = lpos;
      }//end do k
//c
//c  ARRANGE THE SUB-COMPLEX IN ORDER OF INCEASING FUNCTION VALUE
      sort1(nps,lcs);
//c
//c  CREATE THE SUB-COMPLEX ARRAYS
   wgs85:
      for(k = 0;k< nps;k++)
      {
        for(j = 0;j< nopt;j++)
          s[k][j] = cx[lcs[k]][j];

        sf[k] = cf[lcs[k]];
      }//end do k
//c
//c  USE THE SUB-COMPLEX TO GENERATE NEW POINT(S)
      cce(nopt,nps,s,sf,bl,bu,xnstd,icall,maxn,iseed1,iff,ix,r);
//c
//c  IF THE SUB-COMPLEX IS ACCEPTED, REPLACE THE NEW SUB-COMPLEX
//c  INTO THE COMPLEX
      for(k = 0;k< nps;k++)
      {
        for(j = 0;j< nopt;j++)
          cx[lcs[k]][j] = s[k][j];

        cf[lcs[k]] = sf[k];
      } //end do k
//c
//c  SORT THE POINTS
      sort(npg,nopt,cx,cf);
//c
//c  IF MAXIMUM NUMBER OF RUNS EXCEEDED, BREAK OUT OF THE LOOP
      if (icall >= maxn) goto wgs2222;
      
      }//end for(loop = 0;loop<nsp1;loop++)
//c  END OF INNER LOOP ------------

// 2000 continue
 wgs2222:
//c
//c  REPLACE THE NEW COMPLEX INTO ORIGINAL ARRAY x(.,.)
      for(k1 = 0;k1< npg;k1++)
      {
        k2 = k1 * ngs1 + igs;      //(k1 - 1) * ngs1 + igs
        for(j = 0; j<nopt;j++)
          x[k2][j] = cx[k1][j];

        xf[k2] = cf[k1];
      }//end do k1
      if (icall >= maxn) goto wgs3333;
}//end for(igs = 0;igs<ngs1;igs++)
//c  END LOOP ON COMPLEXES
// 3000 continue
//c
//c  RE-SORT THE POINTS
 wgs3333:
      sort(npt1,nopt,x,xf);
//c
//c  RECORD THE BEST AND WORST POINTS
      for(j = 0;j< nopt;j++)
      {
        bestx[j] = x[0][j];
        worstx[j] = x[npt1 - 1][j];
      } //end do j
      bestf = xf[0];
      worstf = xf[npt1 - 1];
//c
//c  TEST THE POPULATION FOR PARAMETER CONVERGENCE
      parstt(npt1,nopt,x,xnstd,bound,gnrng,ipcnvg);
//c
//c  PRINT THE RESULTS FOR CURRENT POPULATION
      if (div(nloop,5).rem != 0) goto wgs501;
      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"===%s",xname[j]);
        fprintf(ipr,"\t");
      }

      if (nopt < 9) goto wgs501;
      for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t");
      }

  wgs501: 
      fprintf(ipr,"\n");
      fprintf(ipr,"%d\t%d\t%d%10.3f%10.3f%10.3f", nloop,icall,ngs1,bestf,worstf,gnrng);

      //ShowMessage(String(nloop)+ ":" + String(icall)+ ":" + String(ngs1)+ ":" + String(bestf)+ ":" + String(worstf)+ ":" + String(gnrng));

      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

      if (nopt<9) goto wgs601;
      for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

  wgs601:
      fprintf(ipr,"\n");
      if (iprint == 1)
      {
        fprintf(ipr,"POPULATION AT LOOP %d",nloop);
        fprintf(ipr,"\n");
        for(i = 0;i< npt1;i++)
        {
          fprintf(ipr,"%10.3f\t", xf[i]);
          for(j =0;j<nopt1;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);

          if (nopt < 9) goto wgs701;
          for(j =8;j<nopt;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);
        }// end for i
  wgs701:
      } //end if
//c
//c  TEST IF MAXIMUM NUMBER OF FUNCTION EVALUATIONS EXCEEDED
      if (icall >= maxn) goto wgs9000;
//c
//c  COMPUTE THE COUNT ON SUCCESSIVE LOOPS W/O FUNCTION IMPROVEMENT
      criter[19] = bestf;
      if (nloop < (kstop+1)) goto wgs132;
      denomi = fabs(criter[19-kstop] + criter[19]) / 2.0;
      timeou = fabs(criter[19-kstop] - criter[19]) / denomi;
      if (timeou < pcento) goto wgs9100;
  wgs132:
      for(l = 0;l<19;l++)
        criter[l] = criter[l+1];

//c
//c  IF POPULATION IS CONVERGED INTO A SUFFICIENTLY SMALL SPACE
      if (ipcnvg == 1) goto wgs9200;
//c
//c  NONE OF THE STOPPING CRITERIA IS SATISFIED, CONTINUE SEARCH
//c
//c  CHECK FOR COMPLEX NUMBER REDUCTION
      if (ngs1 > mings)
      {
        ngs2 = ngs1;
        ngs1 = ngs1 - 1;     //reduce the number of complexes
        npt1 = ngs1 * npg;
        comp(nopt,npt1,ngs1,ngs2,npg,x,xf,cx,cf);
      }// end if
//c
//c  END OF MAIN LOOP -----------
      goto wgs1000;
//c
//c  SEARCH TERMINATED
 wgs9000:
      fprintf(ipr,"\n*** OPTIMIZATION SEARCH TERMINATED BECAUSE THE LIMIT ON THE MAXIMUM\t NUMBER OF TRIALS %d EXCEEDED. \n SEARCH WAS STOPPED AT SUB-COMPLEX %d OF COMPLEX %d IN SHUFFLING LOOP %d ***\n", maxn,loop,igs,nloop);
      goto wgs9999;
 wgs9100:
      fprintf(ipr,"\n*** OPTIMIZATION TERMINATED BECAUSE THE CRITERION VALUE HAS NOT CHANGED \n%5.2f %% IN %d SHUFFLING LOOPS ***\n", pcento*100.,kstop);
      goto wgs9999;
 wgs9200:
      fprintf(ipr,"\n*** OPTIMIZATION TERMINATED BECAUSE THE POPULATION HAS CONVERGED INTO \n%5.2f % OF THE FEASIBLE SPACE ***\n", gnrng*100.);
 wgs9999:
//c
//c  PRINT THE FINAL PARAMETER ESTIMATE AND ITS FUNCTION VALUE
      fprintf(ipr,"\n*** PRINT THE FINAL PARAMETER ESTIMATE AND ITS CRITERION VALUE ***\n");
      fprintf(ipr,"%s","CRITERION\t");
      for(j=0;j<nopt2;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }
      fprintf(ipr,"\n");

      fprintf(ipr,"%10.3f\t",bestf);
       for(j=0;j<nopt2;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");


      if (nopt<13) goto wgs801;
      for(j=12;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }
      fprintf(ipr,"\n");
      for(j=12;j<nopt;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");
      
  wgs801:
//c
//c  END OF SUBROUTINE SCEUA

      delete[] xx,bestx,worstx,xnstd,bound,unit,xf,cf,sf;
      delete[] xname;
      
      de_allocate(x,imax);
      de_allocate(s,kmax);
      de_allocate(cx,imax);
      
//      ShowMessage(String(xx[jmax - 1]));
//      ShowMessage(String(x[imax - 1][jmax - 1]));
      return;
}  //End of SCEUA




//====================================================================
void cce(int nopt,int nps,double **s,double *sf,double *bl,double *bu,double *xnstd,
         int &icall,int maxn,int iseed,int &iff,int *ix,double *r)
{
//$debug

//c  ALGORITHM GENERATE A NEW POINT(S) FROM A SUB-COMPLEX

//c  SUB-COMPLEX VARIABLES
//      implicit real*8 (a-h,o-z)
      //double c1=0.8;
      //double c2=0.4;
//      dimension s(50,16),sf(50),bu(16),bl(16),xnstd(16)

//c  LIST OF LOCAL VARIABLES
//c    sb(.) = the best point of the simplex
//c    sw(.) = the worst point of the simplex
//c    w2(.) = the second worst point of the simplex
//c    fw = function value of the worst point
//c    ce(.) = the centroid of the simplex excluding wo
//c    snew(.) = new point generated from the simplex
//c    iviol = flag indicating if constraints are violated
//c          = 1 , yes
//c          = 0 , no
      int iviol,n,m,i,j,ibound;
      const int nmax = 16;
      double sw[nmax],sb[nmax],ce[nmax],snew[nmax];
      double alpha,beta,fw,fnew;

//c  EQUIVALENCE OF VARIABLES FOR READABILTY OF CODE
      n = nps;
      m = nopt;
      alpha = 1.0;
      beta = 0.5;

//c  IDENTIFY THE WORST POINT wo OF THE SUB-COMPLEX s
//c  COMPUTE THE CENTROID ce OF THE REMAINING POINTS
//c  COMPUTE step, THE VECTOR BETWEEN wo AND ce
//c  IDENTIFY THE WORST FUNCTION VALUE fw
      for(j = 0;j < m;j++)
      {
        sb[j] = s[0][j];
        sw[j] = s[n - 1][j];
        ce[j] = 0.0;
        for(i = 0;i < n-1;i++)
          ce[j] = ce[j] + s[i][j];

        ce[j] = ce[j]/(n-1);
      }// end do j
      fw = sf[n - 1];

//c  COMPUTE THE NEW POINT snew

//c  FIRST TRY A REFLECTION STEP
      for(j = 0;j < m;j++)
        snew[j] = ce[j] + alpha * (ce[j] - sw[j]);


//c  CHECK IF snew SATISFIES ALL CONSTRAINTS
      chkcst(nopt,snew,bl,bu,ibound);

//c  snew IS OUTSIDE THE BOUND,
//c  CHOOSE A POINT AT RANDOM WITHIN FEASIBLE REGION ACCORDING TO
//c  A NORMAL DISTRIBUTION WITH BEST POINT OF THE SUB-COMPLEX
//c  AS MEAN AND STANDARD DEVIATION OF THE POPULATION AS STD
      if (ibound >= 1)
        getpnt(nopt,2,iseed,snew,bl,bu,xnstd,sb,iff,ix,r);

//c  COMPUTE THE FUNCTION VALUE AT snew
      fnew = functn(nopt,snew);
      icall = icall + 1;

//c  COMPARE fnew WITH THE WORST FUNCTION VALUE fw

//c  fnew IS LESS THAN fw, ACCEPT THE NEW POINT snew AND RETURN
      if (fnew <= fw) goto wgs2000;
      if (icall >= maxn) goto wgs3000;


//c  fnew IS GREATER THAN fw, SO TRY A CONTRACTION STEP
      for(j = 0;j< m;j++)
        snew[j] = ce[j] - beta * (ce[j] - sw[j]);


//c  COMPUTE THE FUNCTION VALUE OF THE CONTRACTED POINT
      fnew = functn(nopt,snew);
      icall = icall + 1;

//c  COMPARE fnew TO THE WORST VALUE fw
//c  IF fnew IS LESS THAN OR EQUAL TO fw, THEN ACCEPT THE POINT AND RETURN
      if (fnew <= fw) goto wgs2000;
      if (icall >= maxn) goto wgs3000;


//c  IF BOTH REFLECTION AND CONTRACTION FAIL, CHOOSE ANOTHER POINT
//c  ACCORDING TO A NORMAL DISTRIBUTION WITH BEST POINT OF THE SUB-COMPLEX
//c  AS MEAN AND STANDARD DEVIATION OF THE POPULATION AS STD
 wgs1000: getpnt(nopt,2,iseed,snew,bl,bu,xnstd,sb,iff,ix,r);

//c  COMPUTE THE FUNCTION VALUE AT THE RANDOM POINT
      fnew = functn(nopt,snew);
      icall = icall + 1;


//c  REPLACE THE WORST POINT BY THE NEW POINT
 wgs2000:
      for(j = 0;j< m;j++)
        s[n - 1][j] = snew[j];

      sf[n - 1] = fnew;
 wgs3000:


      return;
}//      c  END OF SUBROUTINE CCE

//==============================================================================
void sceua(double *a,double *bl,double *bu,int nopt,int maxn,int kstop,double pcento,int iseed,
           int ngs,int npg,int nps,int nspl,int mings,int iniflg,int iprint,
           int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
           String strDirRain,String strPrefix_rainfile,
           int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
           int *gridSubID,double *gridArea,double *gridRCC,
           double *Array_ETp,double *RunoffObs,
           double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
           double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
           double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC)
{

//c  ARRAYS FROM THE INPUT DATA
//      dimension a(16),bl(16),bu(16)
//c
//c  LOCAL ARRAYS
        int iff = 0;
        int ix[3];
        double r[97];
//        int *ix = new int[3];
//        double *r = new double[97];

      int imax = ngs * npg;   //total number of points 
      int jmax = nopt;        //number of optimized variables
      int kmax = nps;         //number of points in each complex
      double *xx     = new double[jmax];
      double *bestx  = new double[jmax];
      double *worstx = new double[jmax];
      double *xnstd  = new double[jmax];
      double *bound  = new double[jmax];
      double *unit   = new double[jmax];

      double *xf = new double[imax];
      double *cf = new double[imax];

      double *sf = new double[kmax];
      int   *lcs = new int[kmax];
      
      double criter[20]; //vector containing the best criterion values of the last 10 shuffling loops
      

      double fa,bestf,worstf,gnrng,rand,denomi,timeou;

      int i,j,k,k1,k2,l,icall,ipcnvg,lpos;
      int nloop,loop,igs,nopt1,nopt2,iseed1,npt,npt1,ngs1,ngs2;

      double **x; // x[imax][jmax],
      x = new double*[imax];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < imax; i++)
                x[i] = new double[jmax];  // STEP 2: SET UP THE COLUMNS

      double **s;      // s[kmax][jmax],
      s = new double*[kmax];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < kmax; i++)
                s[i] = new double[jmax];  // STEP 2: SET UP THE COLUMNS

      double **cx;     //cx[imax][jmax],
      cx = new double*[imax];        // STEP 1: SET UP THE ROWS.
        for (i = 0; i < imax; i++)
                cx[i] = new double[jmax];  // STEP 2: SET UP THE COLUMNS
//      common /iopar/ in,ipr

//      character*4 xname(16)
//      AnsiString xname[] = {"X1","X2","X3","X4","X5","X6","X7",
//                            "X8","X9","X10","X11","X12","X13","X14","X15","X16"};
      AnsiString *xname = new AnsiString[nopt];
      for(i = 0;i < nopt;i++)
        xname[i] = "X" + IntToStr(i); 

      fprintf(ipr,"\n=============================================");
      fprintf(ipr,"\n=====<<< ENTER THE SCEUA SUBROUTINE >>>======");
      fprintf(ipr,"\n=============================================\n");


//c  INITIALIZE VARIABLES
      nloop = 0;
      loop = 0;
      igs = 0;
      nopt1 = 8;
      if (nopt<8) nopt1 = nopt;
      nopt2 = 12;
      if (nopt<12) nopt2 = nopt;
//c
//c  INITIALIZE RANDOM SEED TO A NEGATIVE INTEGER
      iseed1 = -abs(iseed);
//c
//c  COMPUTE THE TOTAL NUMBER OF POINTS IN INITIAL POPUALTION
      npt = ngs * npg;
      ngs1 = ngs;
      npt1 = npt;

      fprintf(ipr,"\nENTER THE SHUFFLED COMPLEX EVOLUTION  <<>>  GLOBAL SEARCH");
//      fprintf(ipr,"\n***  Evolution Loop Number = %d",nloop);

//c  COMPUTE THE BOUND FOR PARAMETERS BEING OPTIMIZED
      for(j = 0;j< nopt;j++)
      {
        bound[j] = bu[j] - bl[j];
        unit[j] = 1.0;
      }//end do j
//c
//c  COMPUTE THE FUNCTION VALUE OF THE INITIAL POINT

        fa = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
                     strDirRain,strPrefix_rainfile,
                     nBagrovN,ArrayofBagrovN,gridBagrovN,
                     gridSubID,gridArea,gridRCC,
                     Array_ETp,RunoffObs,
                     Parameter,a,ProgressBar,
                     Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
                     Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
//c
//c  PRINT THE INITIAL POINT AND ITS CRITERION VALUE



      fprintf(ipr,"\n*** PRINT THE INITIAL POINT AND ITS CRITERION VALUE ***");
      fprintf(ipr,"\n");
      fprintf(ipr,"CRITERION\t");
      for(j=0;j<nopt2;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");

      fprintf(ipr,"%10.3f",fa);
      for(j=0;j<nopt2;j++)
        fprintf(ipr,"%10.3f",a[j]);
      fprintf(ipr,"\n");

      if (nopt<13) goto wgs101;
      for(j=12;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");

      for(j=12;j<nopt;j++)
        fprintf(ipr,"%10.3f",a[j]);
      fprintf(ipr,"\n");

  wgs101:

//c
//c  GENERATE AN INITIAL SET OF npt1 POINTS IN THE PARAMETER SPACE
//c  IF iniflg IS EQUAL TO 1, SET x(1,.) TO INITIAL POINT a(.)
      if (iniflg == 1)
      {
        for(j = 0;j< nopt;j++)
          x[0][j] = a[j];
        xf[0] = fa;
      }
//c
//c  ELSE, GENERATE A POINT RANDOMLY AND SET IT EQUAL TO x(1,.)
      else
      {
        getpnt(nopt,1,iseed1,xx,bl,bu,unit,bl,iff,ix,r);
        for(j = 0;j< nopt;j++)
          x[0][j] = xx[j];

//        xf[0] = functn(nopt,xx);
        
                xf[0] = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
                     strDirRain,strPrefix_rainfile,
                     nBagrovN,ArrayofBagrovN,gridBagrovN,
                     gridSubID,gridArea,gridRCC,
                     Array_ETp,RunoffObs,
                     Parameter,xx,ProgressBar,
                     Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
                     Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
      }//end if
      icall = 1;
      if (icall >= maxn) goto wgs9000;
//c
//c  GENERATE npt1-1 RANDOM POINTS DISTRIBUTED UNIFORMLY IN THE PARAMETER
//c  SPACE, AND COMPUTE THE CORRESPONDING FUNCTION VALUES
      for(i = 1;i< npt1;i++)
      {  getpnt(nopt,1,iseed1,xx,bl,bu,unit,bl,iff,ix,r);
        for(j = 0; j<nopt;j++)
          x[i][j] = xx[j];

//        xf[i] = functn(nopt,xx);

                xf[i] = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
                     strDirRain,strPrefix_rainfile,
                     nBagrovN,ArrayofBagrovN,gridBagrovN,
                     gridSubID,gridArea,gridRCC,
                     Array_ETp,RunoffObs,
                     Parameter,xx,ProgressBar,
                     Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
                     Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
        icall = icall + 1;
        if (icall >= maxn)
        {
          npt1 = i;
          goto wgs45;
        }//end if
      } //end do i
//c
//c  ARRANGE THE POINTS IN ORDER OF INCREASING FUNCTION VALUE
   wgs45: sort(npt1,nopt,x,xf);
//c
//c  RECORD THE BEST AND WORST POINTS
      for(j = 0;j< nopt;j++)
      {
        bestx[j] = x[0][j];
        worstx[j] = x[npt1 - 1][j];
      }//end do j
      bestf = xf[0];
      worstf = xf[npt1 - 1];
//c
//c  COMPUTE THE PARAMETER RANGE FOR THE INITIAL POPULATION
      parstt(npt1,nopt,x,xnstd,bound,gnrng,ipcnvg);
//c
//c  PRINT THE RESULTS FOR THE INITIAL POPULATION

  
  

      fprintf(ipr,"*** PRINT THE RESULTS OF THE SCE SEARCH ***");
      fprintf(ipr,"\n");
      fprintf(ipr,"LOOP  TRIALS  COMPLXS  BEST-F  WORST-F  PAR-RNG\t\t");

      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }
      //fprintf(ipr,"\n");

      if (nopt < 9) goto wgs201;
       for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }

  wgs201:
      fprintf(ipr,"\n");
      fprintf(ipr,"%d\t%d\t%d%10.3f%10.3f%10.3f", nloop,icall,ngs1,bestf,worstf,gnrng);
      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

      if (nopt < 9) goto wgs301;
      for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

  wgs301:
      fprintf(ipr,"\n");
      if (iprint == 1)
      {
        fprintf(ipr,"POPULATION AT LOOP %d",nloop);
        fprintf(ipr,"\n");
        for(i = 0;i< npt1;i++)
        {
          fprintf(ipr,"%10.3f\t", xf[i]);
          for(j =0;j<nopt1;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);
          if (nopt < 9) goto wgs401;
          for(j =8;j<nopt;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);
         }//i
  wgs401:
      }//end if

      if (icall >= maxn) goto wgs9000;
      if (ipcnvg == 1) goto wgs9200;

//c  BEGIN THE MAIN LOOP ----------------
 wgs1000:
      nloop = nloop + 1;

//      fprintf (ipr," ***  Evolution Loop Number  = %d",nloop);

//c  BEGIN LOOP ON COMPLEXES
      for(igs = 0;igs<ngs1;igs++)//do 3000 igs = 1, ngs1
      {
//c  ASSIGN POINTS INTO COMPLEXES
      for(k1 = 0;k1< npg;k1++)
      {
        k2 = k1 * ngs1 + igs; //(k1-1) * ngs1 + igs;
        for(j = 0;j< nopt;j++)
          cx[k1][j] = x[k2][j];

        cf[k1] = xf[k2];
      }//end do k1

//c  BEGIN INNER LOOP - RANDOM SELECTION OF SUB-COMPLEXES ---------------
      for(loop = 0;loop<nspl;loop++)//do 2000 loop = 1, nspl
      {
//c  CHOOSE A SUB-COMPLEX (nps points) ACCORDING TO A LINEAR
//c  PROBABILITY DISTRIBUTION
      if (nps == npg)
      {
        for(k = 0;k< nps;k++)
          lcs[k] = k;

        goto wgs85;
      }//end if
////***************************************************************************
////调试到此处
      rand = ran1(iseed1,iff,ix,r);//random(1e10)/1.0e10;

      if(((npg+.5)*(npg+.5) - npg * (npg+1) * rand ) < 0.e-10) ShowMessage("ERROR sqrt, floor1 " );

      lcs[0] = floor(npg + 0.5 - sqrt( (npg+.5)*(npg+.5) - npg * (npg+1) * rand )); //1 + floor(), 小写
      for(k = 1;k< nps;k++)
      {
   wgs60:
        rand = ran1(iseed1,iff,ix,r);//random(1e10)/1.0e10;

        if(((npg+.5)*(npg+.5) - npg * (npg+1) * rand ) < 0.e-10) ShowMessage("ERROR sqrt, floor2");

        lpos = floor(npg + 0.5 - sqrt((npg+.5)*(npg+.5) - npg * (npg+1) * rand )); //1 +
        for(k1 = 0;k1 < k;k1++)
          if (lpos == lcs[k1]) goto wgs60;

        lcs[k] = lpos;
      }//end do k
//c
//c  ARRANGE THE SUB-COMPLEX IN ORDER OF INCEASING FUNCTION VALUE
      sort1(nps,lcs);
//c
//c  CREATE THE SUB-COMPLEX ARRAYS
   wgs85:
      for(k = 0;k< nps;k++)
      {
        for(j = 0;j< nopt;j++)
          s[k][j] = cx[lcs[k]][j];

        sf[k] = cf[lcs[k]];
      }//end do k
//c
//c  USE THE SUB-COMPLEX TO GENERATE NEW POINT(S)
      cce(nopt,nps,s,sf,bl,bu,xnstd,icall,maxn,iseed1,
          nBasinGrid,ngrid,dateBegin,dateEnd,
          strDirRain,strPrefix_rainfile,
          nBagrovN,ArrayofBagrovN,gridBagrovN,
          gridSubID,gridArea,gridRCC,
          Array_ETp,RunoffObs,
          Parameter,Parameter_optimize,ProgressBar,
          Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
          Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC,iff,ix,r);
//c
//c  IF THE SUB-COMPLEX IS ACCEPTED, REPLACE THE NEW SUB-COMPLEX
//c  INTO THE COMPLEX
      for(k = 0;k< nps;k++)
      {
        for(j = 0;j< nopt;j++)
          cx[lcs[k]][j] = s[k][j];

        cf[lcs[k]] = sf[k];
      } //end do k
//c
//c  SORT THE POINTS
      sort(npg,nopt,cx,cf);
//c
//c  IF MAXIMUM NUMBER OF RUNS EXCEEDED, BREAK OUT OF THE LOOP
      if (icall >= maxn) goto wgs2222;
      
      }//end for(loop = 0;loop<nsp1;loop++)
//c  END OF INNER LOOP ------------

// 2000 continue
 wgs2222:
//c
//c  REPLACE THE NEW COMPLEX INTO ORIGINAL ARRAY x(.,.)
      for(k1 = 0;k1< npg;k1++)
      {
        k2 = k1 * ngs1 + igs;      //(k1 - 1) * ngs1 + igs
        for(j = 0; j<nopt;j++)
          x[k2][j] = cx[k1][j];

        xf[k2] = cf[k1];
      }//end do k1
      if (icall >= maxn) goto wgs3333;
}//end for(igs = 0;igs<ngs1;igs++)
//c  END LOOP ON COMPLEXES
// 3000 continue
//c
//c  RE-SORT THE POINTS
 wgs3333:
      sort(npt1,nopt,x,xf);
//c
//c  RECORD THE BEST AND WORST POINTS
      for(j = 0;j< nopt;j++)
      {
        bestx[j] = x[0][j];
        worstx[j] = x[npt1 - 1][j];
      } //end do j
      bestf = xf[0];
      worstf = xf[npt1 - 1];
//c
//c  TEST THE POPULATION FOR PARAMETER CONVERGENCE
      parstt(npt1,nopt,x,xnstd,bound,gnrng,ipcnvg);
//c
//c  PRINT THE RESULTS FOR CURRENT POPULATION
      if (div(nloop,5).rem != 0) goto wgs501;
      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"===%s",xname[j]);
        fprintf(ipr,"\t");
      }

      if (nopt < 9) goto wgs501;
      for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t");
      }

  wgs501: 
      fprintf(ipr,"\n");
      fprintf(ipr,"%d\t%d\t%d%10.3f%10.3f%10.3f", nloop,icall,ngs1,bestf,worstf,gnrng);

      //ShowMessage(String(nloop)+ ":" + String(icall)+ ":" + String(ngs1)+ ":" + String(bestf)+ ":" + String(worstf)+ ":" + String(gnrng));

      for(j=0;j<nopt1;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

      if (nopt<9) goto wgs601;
      for(j=8;j<nopt;j++)
      {
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }

  wgs601:
      fprintf(ipr,"\n");
      if (iprint == 1)
      {
        fprintf(ipr,"POPULATION AT LOOP %d",nloop);
        fprintf(ipr,"\n");
        for(i = 0;i< npt1;i++)
        {
          fprintf(ipr,"%10.3f\t", xf[i]);
          for(j =0;j<nopt1;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);

          if (nopt < 9) goto wgs701;
          for(j =8;j<nopt;j++)
                fprintf(ipr,"%10.3f\t",x[i][j]);
        }// end for i
  wgs701:
      } //end if
//c
//c  TEST IF MAXIMUM NUMBER OF FUNCTION EVALUATIONS EXCEEDED
      if (icall >= maxn) goto wgs9000;
//c
//c  COMPUTE THE COUNT ON SUCCESSIVE LOOPS W/O FUNCTION IMPROVEMENT
      criter[19] = bestf;
      if (nloop < (kstop+1)) goto wgs132;
      denomi = fabs(criter[19-kstop] + criter[19]) / 2.0;
      timeou = fabs(criter[19-kstop] - criter[19]) / denomi;
      if (timeou < pcento) goto wgs9100;
  wgs132:
      for(l = 0;l<19;l++)
        criter[l] = criter[l+1];

//c
//c  IF POPULATION IS CONVERGED INTO A SUFFICIENTLY SMALL SPACE
      if (ipcnvg == 1) goto wgs9200;
//c
//c  NONE OF THE STOPPING CRITERIA IS SATISFIED, CONTINUE SEARCH
//c
//c  CHECK FOR COMPLEX NUMBER REDUCTION
      if (ngs1 > mings)
      {
        ngs2 = ngs1;
        ngs1 = ngs1 - 1;     //reduce the number of complexes
        npt1 = ngs1 * npg;
        comp(nopt,npt1,ngs1,ngs2,npg,x,xf,cx,cf);
      }// end if
//c
//c  END OF MAIN LOOP -----------
      goto wgs1000;
//c
//c  SEARCH TERMINATED
 wgs9000:
      fprintf(ipr,"\n*** OPTIMIZATION SEARCH TERMINATED BECAUSE THE LIMIT ON THE MAXIMUM\t NUMBER OF TRIALS %d EXCEEDED. \n SEARCH WAS STOPPED AT SUB-COMPLEX %d OF COMPLEX %d IN SHUFFLING LOOP %d ***\n", maxn,loop,igs,nloop);
      goto wgs9999;
 wgs9100:
      fprintf(ipr,"\n*** OPTIMIZATION TERMINATED BECAUSE THE CRITERION VALUE HAS NOT CHANGED \n%5.2f %% IN %d SHUFFLING LOOPS ***\n", pcento*100.,kstop);
      goto wgs9999;
 wgs9200:
      fprintf(ipr,"\n*** OPTIMIZATION TERMINATED BECAUSE THE POPULATION HAS CONVERGED INTO \n%5.2f % OF THE FEASIBLE SPACE ***\n", gnrng*100.);
 wgs9999:
//c
//c  PRINT THE FINAL PARAMETER ESTIMATE AND ITS FUNCTION VALUE
      fprintf(ipr,"\n*** PRINT THE FINAL PARAMETER ESTIMATE AND ITS CRITERION VALUE ***\n");
      fprintf(ipr,"%s","CRITERION\t");
      for(j=0;j<nopt2;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }
      fprintf(ipr,"\n");

      fprintf(ipr,"%10.3f\t",bestf);
       for(j=0;j<nopt2;j++)
      {
        bestpara[j] = bestx[j];
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");


      if (nopt<13) goto wgs801;
      for(j=12;j<nopt;j++)
      {
        fprintf(ipr,"%s",xname[j]);
        fprintf(ipr,"\t\t");
      }
      fprintf(ipr,"\n");
      for(j=12;j<nopt;j++)
      {
        bestpara[j] = bestx[j];
        fprintf(ipr,"%10.3f",bestx[j]);
        fprintf(ipr,"\t");
      }
      fprintf(ipr,"\n");
      
  wgs801:
//c
//c  END OF SUBROUTINE SCEUA

      delete[] xx,bestx,worstx,xnstd,bound,unit,xf,cf,sf;
      delete[] xname;
      
      de_allocate(x,imax);
      de_allocate(s,kmax);
      de_allocate(cx,imax);
      
//      ShowMessage(String(xx[jmax - 1]));
//      ShowMessage(String(x[imax - 1][jmax - 1]));
      return;
}  //End of SCEUA




//====================================================================
void cce(int nopt,int nps,double **s,double *sf,double *bl,double *bu,double *xnstd,
         int &icall,int maxn,int iseed,
           int nBasinGrid,int ngrid,TDate dateBegin,TDate dateEnd,
           String strDirRain,String strPrefix_rainfile,
           int nBagrovN,double *ArrayofBagrovN,double *gridBagrovN,
           int *gridSubID,double *gridArea,double *gridRCC,
           double *Array_ETp,double *RunoffObs,
           double *Parameter,double *Parameter_optimize,TProgressBar *ProgressBar,
           double *Mean_gridrain,double *Mean_gridRunoff,double *Mean_gridSurfaceRunoff,double *Mean_gridSubsurfaceRunoff,
           double *Mean_wateruse,double *Mean_gridETa,double *Mean_gridsm,double *RunoffCal,double &WB,double &NSEC,int &iff,int *ix,double *r)
{
//$debug

//c  ALGORITHM GENERATE A NEW POINT(S) FROM A SUB-COMPLEX

//c  SUB-COMPLEX VARIABLES
//      implicit real*8 (a-h,o-z)
      //double c1=0.8;
      //double c2=0.4;
//      dimension s(50,16),sf(50),bu(16),bl(16),xnstd(16)

//c  LIST OF LOCAL VARIABLES
//c    sb(.) = the best point of the simplex
//c    sw(.) = the worst point of the simplex
//c    w2(.) = the second worst point of the simplex
//c    fw = function value of the worst point
//c    ce(.) = the centroid of the simplex excluding wo
//c    snew(.) = new point generated from the simplex
//c    iviol = flag indicating if constraints are violated
//c          = 1 , yes
//c          = 0 , no
      int iviol,n,m,i,j,ibound;
      const int nmax = 16;
      double sw[nmax],sb[nmax],ce[nmax],snew[nmax];
      double alpha,beta,fw,fnew;

//c  EQUIVALENCE OF VARIABLES FOR READABILTY OF CODE
      n = nps;
      m = nopt;
      alpha = 1.0;
      beta = 0.5;

//c  IDENTIFY THE WORST POINT wo OF THE SUB-COMPLEX s
//c  COMPUTE THE CENTROID ce OF THE REMAINING POINTS
//c  COMPUTE step, THE VECTOR BETWEEN wo AND ce
//c  IDENTIFY THE WORST FUNCTION VALUE fw
      for(j = 0;j < m;j++)
      {
        sb[j] = s[0][j];
        sw[j] = s[n - 1][j];
        ce[j] = 0.0;
        for(i = 0;i < n-1;i++)
          ce[j] = ce[j] + s[i][j];

        ce[j] = ce[j]/(n-1);
      }// end do j
      fw = sf[n - 1];

//c  COMPUTE THE NEW POINT snew

//c  FIRST TRY A REFLECTION STEP
      for(j = 0;j < m;j++)
        snew[j] = ce[j] + alpha * (ce[j] - sw[j]);


//c  CHECK IF snew SATISFIES ALL CONSTRAINTS
      chkcst(nopt,snew,bl,bu,ibound);

//c  snew IS OUTSIDE THE BOUND,
//c  CHOOSE A POINT AT RANDOM WITHIN FEASIBLE REGION ACCORDING TO
//c  A NORMAL DISTRIBUTION WITH BEST POINT OF THE SUB-COMPLEX
//c  AS MEAN AND STANDARD DEVIATION OF THE POPULATION AS STD
      if (ibound >= 1)
        getpnt(nopt,2,iseed,snew,bl,bu,xnstd,sb,iff,ix,r);

//c  COMPUTE THE FUNCTION VALUE AT snew
//      fnew = functn(nopt,snew);

                fnew = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
                     strDirRain,strPrefix_rainfile,
                     nBagrovN,ArrayofBagrovN,gridBagrovN,
                     gridSubID,gridArea,gridRCC,
                     Array_ETp,RunoffObs,
                     Parameter,snew,ProgressBar,
                     Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
                     Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
      icall = icall + 1;

//c  COMPARE fnew WITH THE WORST FUNCTION VALUE fw

//c  fnew IS LESS THAN fw, ACCEPT THE NEW POINT snew AND RETURN
      if (fnew <= fw) goto wgs2000;
      if (icall >= maxn) goto wgs3000;


//c  fnew IS GREATER THAN fw, SO TRY A CONTRACTION STEP
      for(j = 0;j< m;j++)
        snew[j] = ce[j] - beta * (ce[j] - sw[j]);


//c  COMPUTE THE FUNCTION VALUE OF THE CONTRACTED POINT
//      fnew = functn(nopt,snew);

                fnew = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
                     strDirRain,strPrefix_rainfile,
                     nBagrovN,ArrayofBagrovN,gridBagrovN,
                     gridSubID,gridArea,gridRCC,
                     Array_ETp,RunoffObs,
                     Parameter,snew,ProgressBar,
                     Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
                     Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
      icall = icall + 1;

//c  COMPARE fnew TO THE WORST VALUE fw
//c  IF fnew IS LESS THAN OR EQUAL TO fw, THEN ACCEPT THE POINT AND RETURN
      if (fnew <= fw) goto wgs2000;
      if (icall >= maxn) goto wgs3000;


//c  IF BOTH REFLECTION AND CONTRACTION FAIL, CHOOSE ANOTHER POINT
//c  ACCORDING TO A NORMAL DISTRIBUTION WITH BEST POINT OF THE SUB-COMPLEX
//c  AS MEAN AND STANDARD DEVIATION OF THE POPULATION AS STD
 wgs1000: getpnt(nopt,2,iseed,snew,bl,bu,xnstd,sb,iff,ix,r);

//c  COMPUTE THE FUNCTION VALUE AT THE RANDOM POINT
//      fnew = functn(nopt,snew);

                fnew = DTVGM_Month(nBasinGrid,ngrid,dateBegin,dateEnd,
                     strDirRain,strPrefix_rainfile,
                     nBagrovN,ArrayofBagrovN,gridBagrovN,
                     gridSubID,gridArea,gridRCC,
                     Array_ETp,RunoffObs,
                     Parameter,snew,ProgressBar,
                     Mean_gridrain,Mean_gridRunoff,Mean_gridSurfaceRunoff,Mean_gridSubsurfaceRunoff,
                     Mean_wateruse,Mean_gridETa,Mean_gridsm,RunoffCal,WB,NSEC);
      icall = icall + 1;


//c  REPLACE THE WORST POINT BY THE NEW POINT
 wgs2000:
      for(j = 0;j< m;j++)
        s[n - 1][j] = snew[j];

      sf[n - 1] = fnew;
 wgs3000:


      return;
}//      c  END OF SUBROUTINE CCE


//===================================================================
void getpnt(int nopt,int idist,int &iseed,double *x,double *bl,double *bu,double *std,double *xi,
            int &iff, int *ix,double *r)
{
//$debug
//c     This subroutine generates a new point within feasible region

//c     x(.) = new point
//c     xi(.) = focal point
//c     bl(.) = lower bound
//c     bu(.) = upper bound
//c     std(.) = standard deviation of probability distribution
//c     idist = probability flag
//c           = 1 - uniform distribution
//c           = 2 - Gaussian distribution
//c
//      implicit real*8 (a-h,o-z)
//      dimension x(16),bl(16),bu(16),std(16),xi(16)

//        int iff = 0;
//        int ix[3];// = new int[3];
//        double r[97];// = new double[97];

        int j,ibound;
        double rand;
        double x1[1],bl1[1],bu1[1];
//        iff = 0;  //保证idist变化时，ran1(idum)中的r[97]重新被赋值，否则，r[97]全部为0
    wgs1:
        for(j=0;j<nopt;j++)
        {
        wgs2:
                if (idist == 1) rand = ran1(iseed,iff,ix,r); //random(1e10)/1.0e10
                if (idist == 2)
                        rand = gasdev(iseed,iff,ix,r);
                x[j] = xi[j] + std[j] * rand * (bu[j] - bl[j]);

//c     Check explicit constraints
                x1[0] = x[j];
                bl1[0] = bl[j];
                bu1[0] = bu[j];
                chkcst(1,x1,bl1,bu1,ibound);
                if (ibound >= 1) goto wgs2;
        }//end for(j=0;j<nopt;j++)

//c     Check implicit constraints

      chkcst(nopt,x,bl,bu,ibound);
      if (ibound >= 1) goto wgs1;

      return;
} //     end  getpnt(nopt,idist,iseed,x,bl,bu,std,xi)




//=======================================================================
double hmle(int ndata, double *obsq, double *q)
{//[0...n]
      int i,lcount,ict,isign,rhmle;
      double rlamda, ad, rd, rn,a,w,ex;
      //common /block1/ ndata, ns, iobj
      //common /block2/ p(1000), obsq(1000), s0(2)
      //common /block3/ q(1000), r(1000), b(1000), s(1000)
      double ra[3];
      double eps=5.e-02;
      double del=5.e-02;
      int iflag = 0;

//  COMPUTE THE MEAN OF LOGARITHM OF OBSERVED FLOWS
      if (iflag == 0)
      {
        ad = 0.e0;
        for(i = 0;i < ndata;i++)
          ad = ad + log(obsq[i]);

        ad = ad / ndata;
        rlamda = 1.e0;
        iflag = 1;
      }//end if

//c  ESTIMATE THE LAMDA VALUE
      lcount = 0;
      ict = 1;
      ra[1] = 0.e0;
      ra[2] = 0.e0;
   wgs25:
      lcount = lcount + 1;
      if(lcount > 40)
      {
        //fprintf(*,*) 'LAMDA ITERATION GO OVER 40', rlamda, ra(1), ra(2)
        goto wgs50;
      }//end if
      rd = 0.e0;
      rn = 0.e0;
      for(i = 0;i < ndata;i++)
      {
        a = log(obsq[i]) / ad;
        w = pow(obsq[i],(2*(rlamda-1.e0)));
        rd = rd + w*(obsq[i] - q[i])*(obsq[i] - q[i]);
        rn = rn + w*(obsq[i] - q[i])*(obsq[i] - q[i]) * a;
      }//30 continue
      ra[ict] = rn / rd - 1;
      if (fabs(ra[ict]) <= eps) goto wgs50;
      isign = -1;
      if (ra[ict] < 0.0) isign = 1;
      rlamda = rlamda + isign * del;
      if (ict == 2) goto wgs35;
      ict = 2;
      goto wgs25;

   wgs35:
      if (ra[1]*ra[2] < 0.0) goto wgs40;
      ra[1] = ra[2];
      goto wgs25;

   wgs40:
      rlamda = rlamda - isign * del / 2.0;

//c  COMPUTE HMLE
   wgs50:
      rhmle = 0.0;
      ex = 2. * (rlamda - 1.);
      for(i = 0;i< ndata;i++)
        rhmle = rhmle + pow(obsq[i],ex) * (obsq[i] -q[i])*(obsq[i] -q[i]);

      rhmle = rhmle / ndata;
      rhmle = rhmle / exp(ex * ad);

      return rhmle;
}//      end HMLE


//===================================================================
void parstt(int npt,int nopt,double **x,double *xnstd,double *bound,double &gnrng,int &ipcnvg)
{//[0...n]
//$debug

//c  SUBROUTINE CHECKING FOR PARAMETER CONVERGENCE
//      implicit real*8 (a-h,o-z)
//      dimension x(2000,16),xmax(16),xmin(16)
//      dimension xmean(16),xnstd(16),bound(16)
      int i,k;
      double delta = 1.0e-20;
      double peps=1.0e-3;
      double xsum1,xsum2;

      double *xmax = new double[nopt];
      double *xmin = new double[nopt];
      double *xmean = new double[nopt];


//c  COMPUTE MAXIMUM, MINIMUM AND STANDARD DEVIATION OF PARAMETER VALUES
      double gsum = 0.e0;
      for(k = 0;k < nopt;k++)
      {
        xmax[k] = -1.0e+20;
        xmin[k] = 1.0e+20;
        xsum1 = 0.e0;
        xsum2 = 0.e0;
        for(i = 0;i < npt;i++)
        {
          xmax[k] = max(x[i][k], xmax[k]);
          xmin[k] = min(x[i][k], xmin[k]);
          xsum1 = xsum1 + x[i][k];
          xsum2 = xsum2 + x[i][k]*x[i][k];
        }//end do i
        xmean[k] = xsum1 / npt;
        xnstd[k] = (xsum2 / npt - xmean[k]*xmean[k]);
        if (xnstd[k] <= delta) xnstd[k] = delta;
       
        xnstd[k] = sqrt(xnstd[k]);
        xnstd[k] = xnstd[k] / bound[k];
        gsum = gsum + log( delta + (xmax[k]-xmin[k])/bound[k] );
      }//end do k
      gnrng = exp(gsum/nopt);

      delete[] xmax;
      delete[] xmin;
      delete[] xmean;
//c  CHECK IF NORMALIZED STANDARD DEVIATION OF PARAMETER IS <= eps
      ipcnvg = 0;
      if (gnrng <= peps) ipcnvg = 1;
      return;
}//  END OF SUBROUTINE PARSTT
      

//====================================================================
void comp(int n,int npt,int ngs1,int ngs2,int npg,double **a,double *af,double **b,double *bf)
{//[0...n]
//$debug

//  THIS SUBROUTINE REDUCE INPUT MATRIX a(n,ngs2*npg) TO MATRIX
//  b(n,ngs1*npg) AND VECTOR af(ngs2*npg) TO VECTOR bf(ngs1*npg)
//      implicit real*8 (a-h,o-z)
//      dimension a(2000,16),af(2000),b(2000,16),bf(2000)
      int igs,ipg,k1,k2,j,i;
      for(igs=0;igs<ngs1;igs++)
      {
        for(ipg=0;ipg<npg;ipg++)
        { k1=(ipg-1)*ngs2 + igs;
          k2=(ipg-1)*ngs1 + igs;
          for(i=0;i<n;i++)
          {
            b[k2][i] = a[k1][i];
          }//end do i
          bf[k2] = af[k1];
        } //ipg
      }//end do igs

      for(j=0;j<npt;j++)
      {
        for(i=0;i<n;i++)
        {  a[j][i] = b[j][i];}

        af[j] = bf[j];
      }//end do j
     return;
}//c  END OF SUBROUTINE COMP



//==============================================================
double  ran1(int &idum,int &iff,int *ix,double *r)
{//[0...n]
//$debug
//c
//c
//c  THIS SUBROUTINE IS FROM "NUMERICAL RECIPES" BY PRESS ET AL.
//      implicit real*8 (a-h,o-z)
      double rran;
//      double r = new double[97];    //已经设为Global
      int j;
      int ix1,ix2,ix3;

      int m1 = 259200;
      int ia1 = 7141;
      int ic1 = 54773;
      double rm1 = 3.8580247e-6;

      int m2 = 134456;
      int ia2 = 8121;
      int ic2 = 28411;
      double rm2 = 7.4373773e-6;

      int m3 = 243000;
      int ia3 = 4561;
      int ic3 = 51349;

      ix1 = ix[0];
      ix2 = ix[1];
      ix3 = ix[2];


//      int iff = 0; //第一次运行ran1(),iff = 0,之后再运行ran1(),iff=1
      if ((idum < 0) || (iff == 0))
      {
        iff = 1;
        ix1 = div(ic1 - idum,m1).rem;
        ix1 = div((ia1 * ix1) + ic1,m1).rem;
        ix2 = div(ix1,m2).rem;
        ix1 = div((ia1 * ix1) + ic1,m1).rem;
        ix3 = div(ix1,m3).rem;
        for(j=0;j<97;j++)
        {
          ix1 = div((ia1 * ix1) + ic1,m1).rem;
          ix2 = div((ia2 * ix2) + ic2,m2).rem;
          r[j] = (ix1 + (ix2 * rm2)) * rm1;
        }  //11 continue
        idum = 1;
      }//end if
      ix1 = div((ia1 * ix1) + ic1,m1).rem;
      ix2 = div((ia2 * ix2) + ic2,m2).rem;
      ix3 = div((ia3 * ix3) + ic3,m3).rem;
      j = 1 + ((97 * ix3) / m3);
      if ((j > 96) || (j < 0)) j = 96;   //if ((j .gt. 97) .or. (j .lt. 1)) pause
      rran = r[j];
      r[j] = (ix1 + (ix2 * rm2)) * rm1;

      ix[0] = ix1;
      ix[1] = ix2;
      ix[2] = ix3;
//c  END OF SUBROUTINE RAN1
      return rran;
//      end
}//END OF SUBROUTINE RAN1


//===============================================================
double  gasdev(int &idum,int &iff,int *ix,double *r)
{
//c  THIS SUBROUTINE IS FROM "NUMERICAL RECIPES" BY PRESS ET AL.
      
      int iset;
      double v1,v2,r1,fac,gset,rgasdev;
      iset = 0;
      if (iset == 0)
      {
      	Again:
      	v1 = (2. * ran1(idum,iff,ix,r)) - 1.;
        v2 = (2. * ran1(idum,iff,ix,r)) - 1.;
        r1 = v1*v1 + v2*v2;
        if (r1 >= 1.0) goto Again;

        fac = sqrt(- ((2. * log(r1)) / r1));
        gset = v1 * fac;
        rgasdev = v2 * fac;
        iset = 1;
      }
      else
      {
      rgasdev = gset;
      iset = 0;
      }
//c
//c  END OF SUBROUTINE GASDEV
      return rgasdev;
}  //END OF SUBROUTINE GASDEV

//===================================================================
/*
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
*/

