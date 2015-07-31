//============================================================================//
//=  TOPIndex.h                                                            ===//
//=  CALCULATE TOPOGRAPHIC INDEX                                           ===//
//=                                                                        ===//
//=  Translated by GANGSHENG WANG                                          ===//
//=  Key Lab. of Water Cycle & Related Land Surface Processes,             ===//
//=  Institute of Geographic Sciences & Natural Resources Research (IGSNRR)===//
//=  Chinese Academy of Sciences (CAS)                                     ===//
//=  Anwai, Datun Road, No.11A, 100101, Beijing, China                     ===//
//=  Tel./Fax: +86 10 64889010 / 64856534                                  ===//
//=  Email: wanggs@igsnrr.ac.cn                                            ===//
//=                                                                        ===//
//=  October 28 , 2004.                                                    ===//
//============================================================================//

#include <math.h>
#include "iostream.h" //max(a,b),min(a,b),FILE
#include "DTVGM.h"    //double fscanfdouble(FILE *finput);void de_allocate(double **data,int m);
//C***********************************************************************         
//C  GRIDATB                                                                             
//C  PROGRAM TO CALCULATE A/TANB VALUES FROM GRIDDED ELEVATION DATA               
//C                                                                               
//C  VERSION 95.01
//C  for MS-DOS PC with EGA graphics and maths co-processor
//C
//C  Compiled using Lahey Fortran77 and Grafmatic Graphics
//C
//C
//C  Originally written by Keith Beven 1983, revised for distribution
//C  1993,1995                                                                                
//C***********************************************************************         
//C  This program is distributed freely with only two conditions.
//C
//C  1. In any use for commercial or paid consultancy purposes a 
//C     suitable royalty agreement must be negotiated with Lancaster 
//C     University (Contact Keith Beven)
//C
//C  2. In any publication arising from use for research purposes the
//C     source of the program should be properly acknowledged and a 
//C     pre-print of the publication sent to Keith Beven at the address
//C     below.
//C
//C  All rights retained 1993
//C  Keith Beven
//C  Centre for Research on Environmental Systems and Statistics
//C  Institute of Environmental and Biological Sciences
//C  Lancaster University, Lancaster LA1 4YQ, UK
//C
//C  Tel: (+44) 1524 593892  Fax: (+44) 1524 593985
//C  Email:  K.Beven@UK.AC.LANCASTER
//C  
//C****************************************************************
//==============================================================================
//==============================================================================
//void RunTopIndex();
void TopIndex(FILE *finput,FILE *foutmap,FILE *foutditn);
void ATANB(FILE *foutmap,FILE *foutditn,int NX,int NY,double DX,double novalue,
           int &NATB,double **E,double **ATB,double **A);



//==============================================================================
//==============================================================================

//==============================================================================
//==============================================================================
void TopIndex(FILE *finput,FILE *foutmap,FILE *foutditn)
{
        double xmin,ymin,novalue,elev;

        char ncols[10],nrows[10],xllcorner[10],yllcorner[10],cellsize[10],nodata[20];
        int I,J,NX,NY,DX,NATB,NAC,index;

        double AC[30],ST[30],Y[30];
        double atbmax,atbmin,datb,total,fATB;


        fscanf(finput,"%s%d",&ncols,&NX);
	fscanf(finput,"%s%d",&nrows,&NY);
        fscanf(finput,"%s",&xllcorner);
        xmin = fscanfdouble(finput);  //ATTENTION!!!xmin必须定义成double，否则小数点后的数字转换不正确

	fscanf(finput,"%s",&yllcorner);
        ymin = fscanfdouble(finput);

	fscanf(finput,"%s%d",&cellsize,&DX);
	fscanf(finput,"%s",&nodata);
        novalue = fabs(fscanfdouble(finput)); //ArcView输出-9999.,转成9999.

//        E(500,500),ATB(500,500),A(500,500)
        double **E; // x[imax][jmax],
                E = new double*[NX];        // STEP 1: SET UP THE ROWS.
                for (I = 0; I < NX; I++)
                        E[I] = new double[NY];  // STEP 2: SET UP THE COLUMNS

        double **ATB; // x[imax][jmax],
                ATB = new double*[NX];        // STEP 1: SET UP THE ROWS.
                for (I = 0; I < NX; I++)
                        ATB[I] = new double[NY];  // STEP 2: SET UP THE COLUMNS

        double **A; // x[imax][jmax],
                A = new double*[NX];        // STEP 1: SET UP THE ROWS.
                for (I = 0; I < NX; I++)
                        A[I] = new double[NY];  // STEP 2: SET UP THE COLUMNS

//*  READS ELEVATIONS BY ROWS STARTING FROM BOTTOM LEFT HAND CORNER
        for(J=0;J<NY;J++)
        {
                for(I=0;I<NX;I++)
                {
                        //fscanf(finput,"%f",&elev);
                        elev = fscanfdouble(finput);
                        E[I][J] = fabs(elev);
                }
        }// 9 CONTINUE

        fprintf(foutmap,"%s\t%d\n",ncols,NX);
	fprintf(foutmap,"%s\t%d\n",nrows,NY);
	fprintf(foutmap,"%s\t%20.4f\n",xllcorner,xmin);
	fprintf(foutmap,"%s\t%20.4f\n",yllcorner,ymin);
	fprintf(foutmap,"%s\t%d\n",cellsize,DX);
	fprintf(foutmap,"%s\t%10.4f\n",nodata,-1*novalue);
                                           
//*  SET ALL NON-CATCHMENT A/TANB VALUES TO 99999 AND ALL CATCHMENT VALUES        
//*  TO -9.9.  SET ALL A VALUES TO DX*DX                                          
        NATB=0;
        for(I=0;I<NX;I++)
        {
                for(J=0;J<NY;J++)
                {
                        A[I][J]=DX*DX;

	                if(fabs(E[I][J] - novalue)<0.001) //E[I][J] = NOVALUE//if(E[I][J] >= 9999.)THEN   //wgs 2002-8-28
                        {
	                        NATB=NATB+1;
                                ATB[I][J]=E[I][J];  //*	  ATB[I][J]=E[I][J]    //wgs 2002-8-28
                        }
                        else
	                        ATB[I][J]=-9.9;
                }   //end j
        }// end i, 10 CONTINUE
        ShowMessage("Number of NODATA = " + String(NATB)
                    + "\nCatchment Grids = " + String(NX*NY - NATB));
        fprintf(foutditn,"Number of NODATA = %d\nNumber of Basin Grids = %d\n",NATB, NX*NY - NATB);
        fprintf(foutditn,"Area of the Basin = %15.4f KM*KM\n\n",(NX*NY - NATB)*DX*DX*1.0e-6);

//*  CALCULATE A/TANB VALUES FOR CATCHMENT GRID SQUARES                           
        ATANB(foutmap,foutditn,NX,NY,DX,novalue,NATB,E,ATB,A);
//*                                                                               
//*  Calculate ATANB histogram for use in TOPMODEL
        NAC = 30;
//c  find limits
        atbmax = 0.;
        atbmin = novalue; //9999

        for(I=0;I<NX;I++)
        {
                for(J=0;J<NY;J++)//do 20 j=1,ny
                {
                        if(ATB[I][J] < novalue)
	                {
                                if(ATB[I][J]>atbmax)atbmax = ATB[I][J];
	                        if(ATB[I][J]<atbmin)atbmin = ATB[I][J];
                        }//endif
                }
        }//20 continue

        datb = (atbmax-atbmin)/(NAC-1);
//c
//c  Initialise histogram count 
        for(I = 0;I<NAC;I++)
                Y[I] = 0.;//15 y(i) = 0.

        total = 0.;
        for(I=0;I<NX;I++)//i=1,nx
        {
                for(J=0;J<NY;J++)//do 30 j=1,ny
                {
                        if(ATB[I][J] < novalue)
                        {
	                        index = floor((ATB[I][J]-atbmin)/datb);  //INT((ATB[I][J]-atbmin)/datb)+1;
	                        if(index>NAC-1) index = NAC-1;
	                        Y[index]=Y[index]+1;
	                        total = total + 1;
                        }//endif
                }//end j,30 continue
        } //end i
//-----------------------------------------------------------
for(J=0;J<NY;J++)
        {
        for(I=0;I<NX;I++)
        {
                if(ATB[I][J] > novalue) //9999.9的那些值, 
                        fATB = 0.5*(atbmax + atbmin); //均值代替
                else if(ATB[I][J] < novalue)
                        fATB = ATB[I][J];
                else      //9999. 转成ArcView的-9999.,即Nodata
                        fATB = -1*ATB[I][J];

                fprintf(foutmap,"%10.4f",fATB);
        }
        fprintf(foutmap,"\n");
}//20 CONTINUE
//-----------------------------------------------------------
        AC[0]=0.;
        ST[0]=atbmax;
        for(I=1;I<NAC;I++)
        {
                AC[I]= Y[NAC - I]/total;  //[NAC - I + 1]
                ST[I]= atbmax - I*datb; //(I - 1)*datb
        } // 40 continue
        fprintf(foutditn,"NAC = %d\n\n",NAC);
        fprintf(foutditn,"I\tAC\t\tST\n");
        for(I=0;I<NAC;I++)
                fprintf(foutditn,"%d\t%f\t%f\n",I,AC[I],ST[I]);
        

        de_allocate(E,NX);
        de_allocate(ATB,NX);
        de_allocate(A,NX);
        return;
} //      END TOPIndex

//==============================================================================
//==============================================================================
void ATANB(FILE *foutmap,FILE *foutditn,int NX,int NY,double DX,double novalue,
           int &NATB,double **E,double **ATB,double **A)
{
//      COMMON/MAP/NX,NY,E(500,500),ATB(500,500),A(500,500)
      double ROUTE[9],TANB[9];
      int I,J,K,NROUT;
      double DX2,DX1,SUM,SUMTB,C;
      int NSINK,ITER,NSLP;
      DX2=1/(1.414*DX);   //对角线
      DX1=1/DX;
      NSINK=0;
      ITER=0;
//      Write(6,698)
//  698 Format(1x,'Subroutine ATB - counting iterations......')
                                                                              
   wgs50:
      ITER=ITER+1;
//      Write(6,699)iter,NATB
//  699 format(2i8)
//*                                                                               
//*  LOOP THROUGH GRID SQUARES...CHECK if THERE IS AN UPSLOPE ELEMENT THAT        
//*  DOES NOT HAVE AN ATANB VALUE.....if SO, THEN CANNOT CARRY OUT ELEMENT        
//*  CALCULATIONS.   CONTINUE ITERATIONS UNTIL NATB=NX*NY                         
//*                                                                               
      for(I=0;I<NX;I++)
      {
      for(J=0;J<NY;J++)
      {
//*                                                                         
//*      
//*  SKIP NON-CATCHMENT GRID SQUARES                                              
      if(abs(E[I][J] - novalue)<0.001)goto wgs10;  //说明不是流域中的点
//*
//*  SKIP SQUARES ALREADY DONE
      if(ATB[I][J]>-9.) goto wgs10;
//*                                                                               
//*  CHECK THE 8 POSSIBLE FLOW DIRECTIONS FOR UPSLOPE ELEMENTS WITHOUT            
//*  AN ATANB VALUE                                                               
      if(I-1>=0)
      {
	  if(J-1>=0)
          {
	      if(E[I-1][J-1]>E[I][J]&&ATB[I-1][J-1]<0.) goto wgs10;
	  }//ENDif  (J-1>=0)
	  if(E[I-1][J]>E[I][J] && ATB[I-1][J]<0.) goto wgs10;
	  if(J+1 < NY)
	      if(E[I-1][J+1]>E[I][J] && ATB[I-1][J+1]<0.) goto wgs10;
	  //ENDif                                                                 
      } //ENDif (I-1>=0)
      if(J-1 >= 0)
      {
	  if(E[I][J-1]>E[I][J] && ATB[I][J-1]<0.) goto wgs10;
      } //ENDif
      if(J+1 < NY)
      {
	  if(E[I][J+1]>E[I][J] && ATB[I][J+1]<0.) goto wgs10;
      }//ENDif
      if(I+1 < NX)
      {
	  if(J-1 >= 0)
	      if(E[I+1][J-1]>E[I][J] && ATB[I+1][J-1]<0.) goto wgs10;           
	  //ENDif
	  if(E[I+1][J]>E[I][J] && ATB[I+1][J]<0.) goto wgs10;                   
	  if(J+1 < NY)
	      if(E[I+1][J+1]>E[I][J] && ATB[I+1][J+1]<0.) goto wgs10;
	  //ENDif                                                                 
      }//ENDif
//*                                                                               
//*  THERE ARE NO UPSLOPE ELEMENTS WITHOUT AN A/TANB VALUE.....START              
//*  CALCULATIONS USING CURRENT VALUE OF A                                        
//*                                                                               
//*  FIND THE OUTFLOW DIRECTIONS AND CALCULATE THE SUM OF WEIGHTS USING          
//*  (TANB *CONTOUR LENGTH) WHERE CONTOUR LENGTH IS 0.5*DX FOR CARDINAL           
//*  DIRECTIONS AND 0.354*DX FOR DIAGONAL DIRECTIONS                              
//*                                                                               
      SUM=0.;
      for(K=0;K<9;K++)
        ROUTE[K]=0.;                                                               
      NROUT=0;                                                                   
      if(I-1 >= 0)
      {
	  if(J-1 >= 0)
          {
	      if(E[I-1][J-1]<E[I][J])
              {
		  TANB[0]=(E[I][J]-E[I-1][J-1])*DX2;
		  ROUTE[0]=0.354*DX*TANB[0];
		  SUM=SUM+ROUTE[0];
		  NROUT=NROUT+1;
	      }//ENDif
	  }//ENDif
	  if(E[I-1][J]<E[I][J])
          {
	      TANB[1]=(E[I][J]-E[I-1][J])*DX1;
	      ROUTE[1]=0.5*DX*TANB[1];
	      SUM=SUM+ROUTE[1];
	      NROUT=NROUT+1;
	  }//ENDif
	  if(J+1 < NY)
          {
	      if(E[I-1][J+1]<E[I][J])
              {
		  TANB[2]=(E[I][J]-E[I-1][J+1])*DX2;
		  ROUTE[2]=0.354*DX*TANB[2];
		  SUM=SUM+ROUTE[2];
		  NROUT=NROUT+1;
	      }//ENDif
	  }//ENDif                                                                 
      }//ENDif
      if(J-1 >= 0)
      {
	  if(E[I][J-1]<E[I][J])
          {
	      TANB[3]=(E[I][J]-E[I][J-1])*DX1;
	      ROUTE[3]=0.5*DX*TANB[3];
	      SUM=SUM+ROUTE[3];
	      NROUT=NROUT+1;                                                     
	  }//ENDif
      }//ENDif
      if(J+1 < NY)
      {
	  if(E[I][J+1]<E[I][J])
          {
	      TANB[5]=(E[I][J]-E[I][J+1])*DX1;
	      ROUTE[5]=0.5*DX*TANB[5];
	      SUM=SUM+ROUTE[5];
	      NROUT=NROUT+1;
	  }//ENDif
      }//ENDif
      if(I+1 < NX)
      {
	  if(J-1 >= 0)
          {
	      if(E[I+1][J-1]<E[I][J])
              {
		  TANB[6]=(E[I][J]-E[I+1][J-1])*DX2;
		  ROUTE[6]=0.354*DX*TANB[6];
		  SUM=SUM+ROUTE[6];
		  NROUT=NROUT+1;                                                 
	      }//ENDif
	  }//ENDif
	  if(E[I+1][J]<E[I][J])
          {
	      TANB[7]=(E[I][J]-E[I+1][J])*DX1;
	      ROUTE[7]=0.5*DX*TANB[7];
	      SUM=SUM+ROUTE[7];
	      NROUT=NROUT+1;
	  }//ENDif
	  if(J+1 < NY)
          {
	      if(E[I+1][J+1]<E[I][J])
              {
		  TANB[8]=(E[I][J]-E[I+1][J+1])*DX2;
		  ROUTE[8]=0.354*DX*TANB[8];
		  SUM=SUM+ROUTE[8];
		  NROUT=NROUT+1;                                                 
	      }//ENDif
	  }//ENDif
      }//ENDif
      if(NROUT==0)
      {//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//*                                                                               
//*  NO DOWNSLOPE DIRECTION MUST BE AN INTERNAL SINK OR AN OUTFLOW NODE ON        
//*  THE BOUNDARY                                                                 
      fprintf(foutditn,"SINK OR BOUNDARY NODE AT I = %d, J = %d\n",I,J);

      NSINK = NSINK+1;
//C                                                                               
//*  ASSUME THAT THERE IS A CHANNEL OF LENGTH DX RUNNING MIDWAY THROUGH           
//*  THE SINK OR BOUNDARY NODE.  TAKE AVERAGE INFLOW SLOPE ANGLE TO REPRES        
//*  TANB AND A/(2DX) TO REPRESENT a.                                             
      SUMTB=0.;
      NSLP=0;
      if(I-1 >= 0)
      {
	  if(J-1 >= 0)
          {
	      if(E[I-1][J-1] < novalue)  //E[I-1][J-1] < novalue
              {
	        SUMTB=SUMTB+(E[I-1][J-1]-E[I][J])*DX2;
	        NSLP=NSLP+1;
	      }//ENDif
          }//ENDif
	  if(E[I-1][J]<novalue)
          {
	        SUMTB=SUMTB+(E[I-1][J]-E[I][J])*DX1;
	        NSLP=NSLP+1;
	  }//ENDif
	  if(J+1 < NY)
          {
	      if(E[I-1][J+1]<novalue)
	      { SUMTB=SUMTB+(E[I-1][J+1]-E[I][J])*DX2;
	        NSLP=NSLP+1;
	      }//ENDif
	  }//ENDif
      }//ENDif (I-1 >= 0)
      if(J-1 >= 0)
      {
	  if(E[I][J-1]<novalue)
          {
	        SUMTB=SUMTB+(E[I][J-1]-E[I][J])*DX1;
	        NSLP=NSLP+1;
	  }//ENDif
      }//ENDif
      if(J+1 < NY)
      {
	  if(E[I][J+1]<novalue)
          {
	        SUMTB=SUMTB+(E[I][J+1]-E[I][J])*DX1;
	        NSLP=NSLP+1;
	  }//ENDif
      }//ENDif
      if(I+1 < NX)
      {
	  if(J-1 >= 0)
          {
	       if(E[I+1][J-1]<novalue)
               {
	                SUMTB=SUMTB+(E[I+1][J-1]-E[I][J])*DX2;
	                NSLP=NSLP+1;
	       }// ENDif
	  }//ENDif
	  if(E[I+1][J]<novalue)
          {
	        SUMTB=SUMTB+(E[I+1][J]-E[I][J])*DX1;
	        NSLP=NSLP+1;
	  }//ENDif
	  if(J+1 < NY)
          {
	       if(E[I+1][J+1]<novalue)
               {
	                SUMTB=SUMTB+(E[I+1][J+1]-E[I][J])*DX2;
	                NSLP=NSLP+1;
	       }//ENDif
	  }//ENDif
      }//ENDif

//*  CALCULATE AVERAGE INFLOW SLOPE ANGLE
      SUMTB=SUMTB/NSLP;
      if(SUMTB>0.000001)
      {
                ATB[I][J]=A[I][J]/(2*DX*SUMTB); //Average Inflow Slope Angle太小，会导致此式出问题;
                ATB[I][J]=log(ATB[I][J]);
      }
      else
      ATB[I][J]= 9999.9;//Average Inflow Slope Angle太小，直接设为9999.9;wgs觉得可以设为TOPIndex最大值
      //ENDif
      NATB=NATB+1;
//*  THESE NODES ARE IGNORED IN ATANB CALCULATIONS                                
       goto wgs10;
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
      }//ENDif IF(NROUT.EQ.0)THEN
                                                                       
////*  CALCULATE WEIGHTED AVERAGE A/TANB VALUE                                      
////*        
////*  NB. This formulation uses contour length distance weights for the        
////*      cardinal directions of 0.5 and for the diagonal directions of        
////*      0.354.  The  upslope flow area is distributed in the downslope        
////*      flow directions proportionally to tanb(i)*cld(i) where cld is        
////*      the contour length distance, so that:        
////*           A = A1 + A2 + A3 ....downslope directions only        
////*             = C * ( tanb(1)*cld(1) + tanb(2)*cld(2) ....)        
////*      or  C  = A / sum of (tanb(i)*cld(i))        
////*      and A1 = A * C * tanb(1) * cld(1)        
////*          A2 = A * C * tanb(2) * cld(2)  etc        
////*      To calculate a/tanb for this grid square a value of tanb is        
////*      calculated based on the same distance weights so that        
////*        tanb =  sum of (tanb(i)*cld(i)) / sum of (cld(i))        
////*                   (....downslope directions only...)        
////*       the value of a/tanb for this square will be        
////*        ATB  = A / (sum of (cld(i)) * tanb)        
////*             = A / (sum of (tanb(i)*cld(i))        
////*             = C        
      C=A[I][J]/SUM;
      ATB[I][J]= log(C);
      NATB=NATB+1;

      if(I-1 >= 0)
      {
	  if(J-1 >= 0) A[I-1][J-1]=A[I-1][J-1]+C*ROUTE[0];
	  A[I-1][J]=A[I-1][J]+C*ROUTE[1];
	  if(J+1 < NY) A[I-1][J+1]=A[I-1][J+1]+C*ROUTE[2];
      }//ENDif
      if(J-1 >= 0) A[I][J-1]=A[I][J-1]+C*ROUTE[3];
      if(J+1 < NY) A[I][J+1]=A[I][J+1]+C*ROUTE[5];
      if(I+1 < NX)
      {
	  if(J-1 >= 0) A[I+1][J-1]=A[I+1][J-1]+C*ROUTE[6];
	  A[I+1][J]=A[I+1][J]+C*ROUTE[7];
	  if(J+1 < NY) A[I+1][J+1]=A[I+1][J+1]+C*ROUTE[8];
      }//ENDif
      wgs10:
      }}// END FOR I,J  //10 CONTINUE

      if(NATB<NX*NY) goto wgs50;
/*
      for(J=0;J<NY;J++)
      {
        for(I=0;I<NX;I++)
        {
                if(fabs(ATB[I][J] - novalue) < 1.0e-3)
                        fATB = -1*ATB[I][J];
                else
                        fATB = ATB[I][J];
                fprintf(foutmap,"%10.4f",fATB);
        }
        fprintf(foutmap,"\n");
      }//20 CONTINUE
*/
      fprintf(foutditn,"\nNUMBER OF ITERATIONS REQUIRED FOR A/TANB CALCS = %d\nNUMBER OF SINK NODES = %d\n",ITER,NSINK);
      return;                                                                    
} //      END ATANB(DX,NATB)

//==============================================================================



