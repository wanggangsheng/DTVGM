/*************************************************
*        Simple Genetic Algorithm                *
*                 Debug                          *
* Rivised by Wang Gangsheng 2003                 *
*************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <math.h>
//#include "ChildWin.h"


/*************************************************
*          The Definition of Constant            *
*                                                *
*************************************************/
#define  POPSIZE  50  /* population size 500*/
#define  MAXIMIZATION 1  /* maximization flag */
#define  MINIMIZATION 2  /* minimization flag */
/*************************************************
*         The Definition of User Data            *
*For different problem ,there are some difference*
*************************************************/
#define Cmax     1  /* certain maximal value of Objective Function 100*/
#define Cmin     0   /* certain minimum value of Objective Function 0*/
//#define VariableMax     8  /* certain maximal value of Variable 100*/
//#define VariableMin     1   /* certain minimum value of Variable 0*/
#define LENGTH  10  /* the chromosome length of a variable */
#define NUMV    5  /* the Number of variables 5*/

#define CHROMLENGTH  LENGTH*NUMV  /* total length of chromosome */
////#define LENGTH1   10 /* the chromosome length of 1st variable */
////#define LENGTH2  10/* the chromosome length of 2nd variable */
////#define CHROMLENGTH  LENGTH1+LENGTH2  /* total length of chromosome */

int  FunctionMode =MINIMIZATION ;    /*optimazation type */
int MaxGeneration;   /*max number of generation 200*/
double Pc  =0.6;  /* probability of crossover */
double Pm = 1e-3 ; /* probability of mutation */
//double Variable_min[NUMV],Variable_max[NUMV];
double Variable_min[] = {-5,-5,-5,-5,-2};
double Variable_max[] = {5,5,5,5,2};

double Vx[NUMV];

FILE *fpga0;
/*************************************************
*         The Definition of Data Structure       *
*************************************************/
struct individual /* data structure of individual */
{
 char chrom[CHROMLENGTH+1];/* a string of code representing individual */
 double value;             /* object value of this individual */
 double fitness;           /* fitness value of this individual */
 };
/*************************************************
*      The Definition of Global Variables        *
*************************************************/
int  generation; /* number of generation */
int best_index;  /* index of best individual */
int worst_index; /* index of worst individual */
struct  individual bestindividual; /*best individual of current generation*/
struct individual worstindividual;/*worst individual of current generation*/
struct individual currentbest; /* best individual by now*/
struct individual population[POPSIZE]; /* population */
/*************************************************
*         Declaration of Prototype               *
*************************************************/
void GenerateInitialPopulation (void);
void GenerateNextPopulation (void);
void EvaluatePopulation(void);
long DecodeChromosome(char*,int,int);
void CalculateObjectValue(void);
void CalculateFitnessValue(void);
void FindBestAndWorstIndividual(void);
void PerformEvolution(void);
void SelectionOperator(void);
void CrossoverOperator(void);
void NutationOperator(void);
void OutputTextReport (void);
double VariableValue(long DecodeChorm,double Vmin,double Vmax);

/*************************************************
*                    Main Program                *
*************************************************/
void main(void)
{
 generation=0;
 GenerateInitialPopulation();
 EvaluatePopulation();
 while (generation<MaxGeneration)
  {
   generation++;
   GenerateInitialPopulation();
   EvaluatePopulation();
   PerformEvolution();
   OutputTextReport ();
   }
}
/*************************************************
*   Function: Generate the first population      *
*             Variable: None                     *
*************************************************/
void GenerateInitialPopulation (void)
{
 int i,j;
 randomize();
 for(i=0;i<POPSIZE;i++)
  {
   for(j=0;j<CHROMLENGTH;j++)
    {
    population[i].chrom[j]=(random(10)<5)?'0':'1';
    }
   population[i].chrom[CHROMLENGTH]='\0';
  }
}
/*************************************************
*   Function: Initialize the first generation.   *
*             Variable: None                     *
*************************************************/
void GenerateNextPopulation(void)
{
  SelectionOperator();
  CrossoverOperator();
  NutationOperator();
} 
/*************************************************
* Function:Evaluate population according         *
*                              to certain formula*
*             Variable: None                     *
*************************************************/
void EvaluatePopulation(void)
{
  CalculateObjectValue();  /* calculate object value */
  CalculateFitnessValue( ); /* calculate fitness value */
  FindBestAndWorstIndividual( ); /* find the best and worst individual */
}
/*************************************************
* Function:To decode a binary chromosome         *
*                         into a decimal integer *
*             Variable: None                     *
* Note: The returned value may be plus, or minus.*
*       For different coding method, this value  *
*        may be changed into "unsigned int".     *
*************************************************/
long DecodeChromosome(char *string,int point,int length)
 {
  int i;
  long decimal=0l;
  char *pointer;

  for(i=0,pointer=string+point;i<length;i++,pointer++)
    {
     decimal+=(*pointer-'0')<<(length-1-i);
    }
    return (decimal);
  }
/******************************************************
*   Function: To calculate object value.              *
*             Variable: None                          *
* Note: For different problem,user must change these  *
*       code.This example is dealing with Rosenbrock  *
*       function.Rosenbrok function is defined as:    *
*       f(x1,x2)=100*(x1**2-x2)**2+(1-x1)**2          *
*       Its maximal value is:                         *
*       f(-2.048,-2.048)=3905.926227                  *
******************************************************/
/*
void  CalculateObjectValue(void)  //
{
 int i,j;
 long temp;
 double Vx[NUMV];

 for (i=0;i<POPSIZE;i++)
 {
  for(j=0;j<NUMV;j++)
  {
        temp = DecodeChromosome(population[i].chrom,j*LENGTH,LENGTH);
        Vx[j] = VariableValue(temp,VariableMin,VariableMax);
  }

 population[i].value=Vx[0]*Vx[1]*Vx[2]*Vx[3]*(100.0 - Vx[0] - Vx[1] - Vx[2] - Vx[3]);

  }
} */
/*************************************************
*  Function: To calculate fitness value.         *
*             Variable: None                     *
*************************************************/
void CalculateFitnessValue(void)
{
 int i;
 double temp;
 for(i=0;i<POPSIZE; i++)
  {
   if(FunctionMode==MAXIMIZATION)
    {
     if((population[i].value+Cmin)>0.0)
           temp=Cmin+population[i].value;
     else
           temp=0.0;
     }
     else if(FunctionMode==MINIMIZATION)  /* minimization */
      {
        if(population[i].value < Cmax)
            temp=Cmax-population[i].value;
        else
            temp=0.0;
       }
    population[i].fitness=temp;
   }
}
/*************************************************
*  Function: To find out the best individual so  *
*                         far current generation.*
*             Variable: None                     *
*************************************************/
void FindBestAndWorstIndividual(void)
 {
  int i;
  double sum=0.0;
  /*find out the best and worst individual of this generation */
  bestindividual=population[0];
  worstindividual=population[0];
  for(i=1;i<POPSIZE;i++)
   {
    if(population[i].fitness>bestindividual.fitness)
          {
             bestindividual=population[i];
             best_index=i;
          }
      else if(population[i].fitness<worstindividual.fitness)
         {
          worstindividual=population[i];
          worst_index=i;
         }
      sum+=population[i].fitness;
    }
  /* find out the best individual so far */
  if(generation==0)   /* initialize the best individual */
       currentbest=bestindividual;
  else
       if(bestindividual.fitness>currentbest.fitness)
           currentbest=bestindividual;
 }
/********************************************************
*   Function: To perform evolution operation based on   *
*             Elitise model.Elitist model is to replace *                          *
*             the worst individual of this generation by*
*             the current best one.                     *
*   Variable: None                                      *
********************************************************/
void PerformEvolution(void)
{
 if(bestindividual.fitness>currentbest.fitness)
       currentbest=population[best_index];
  else
       population[worst_index]=currentbest;
}
/********************************************************
*   Function: To repoduce a chromosome by proportional  *
*             selection.                                *
*   Variable: None                                      *
********************************************************/
void SelectionOperator(void)
{
  int i,index;
  double p,sum=0.0;
  double cfitness[POPSIZE];/* cumulative fitness value */
  struct individual newpopulation [POPSIZE];
  /* calculate relative fitness */
  for(i=0;i<POPSIZE;i++)
     sum+=population[i].fitness;
  for(i=0;i<POPSIZE;i++)
     cfitness[i]=population[i].fitness/sum;
  /* calculate cumulative fitness */
  for(i=1;i<POPSIZE;i++)
     cfitness[i]=cfitness[i-1]+cfitness[i];
  /* selection operation */
  for (i=0;i<POPSIZE;i++)
    {
     p=rand()%1000/1000.0;
     index=0;
     while(p>cfitness[index])
        index++;
     newpopulation[i]=population[index];
     }
   for(i=0;i<POPSIZE;i++)
      population[i]=newpopulation[i];
  }
/********************************************************
*   Function: Crossover two chromosome by means of      *
*             one_point crossover.                      *
*   Variable: None                                      *
********************************************************/
void CrossoverOperator(void)
{
  int i,j;
  int index[POPSIZE];
  int point,temp;
  double p;
  char ch;
  /* make a pair of individual randomly */
  for(i=0;i<POPSIZE; i++)
    index[i]=i;
  for(i=0;i<POPSIZE;i++)
    {
     point=random(POPSIZE-i);
     temp=index[i];
     index[i]=index[point+i];
     index[point+i]=temp;
     }
   /* one_point crossover operation */
   for( i=0;i<POPSIZE-1;i+=2)
     {
      p=rand()%1000/1000.0;
      if(p<Pc)
         {
          point=random(CHROMLENGTH-1)+1;
          for(j=point;j<CHROMLENGTH;j++)
             {
              ch=population[index[i]].chrom[j];
              population[index[i]].chrom[j]=population[index[i+1]].chrom[j];
              population[index[i+1]].chrom[j] = ch;
              }
          }
      }
 }
/********************************************************
*   Function: Mutation of a chromosome.                 *
*   Variable: None                                      *
********************************************************/
void MutationOperator(void)
{
 int i,j;
 double p;
 /*  bit double */
 for(i=0;i<POPSIZE;i++)
     for(j=0;j<CHROMLENGTH;j++)
        {
         p=rand()%1000/1000.0;
         if(p<Pm)
            population[i].chrom[j]=(population[i].chrom[j]=='0')?'1':'0';

         }
}
/********************************************************
*   Function: Output the results of current population. *
*   Variable: None                                      *
********************************************************/
void OutputTextReport(void)
 {
  int i,j;
  double sum;  /* temporary sum */
  double average;  /* average of population object value */
  //double Vx[NUMV];

   /* calculate average object value */
   sum=0.0;
   for(i=0;i<POPSIZE;i++)
     sum+=population[i].value;

   average =sum/POPSIZE;
   /* print results of this population */


   for(j=0;j<NUMV;j++)
   {
        Vx[j] = VariableValue(DecodeChromosome(currentbest.chrom,j*LENGTH,LENGTH),Variable_min[j],Variable_max[j]);
        fprintf(fpga0," Vx[ %d ] = %f,",j,Vx[j]);
   }
   //x1=VariableValue(DecodeChromosome(currentbest.chrom,0,LENGTH1),VariableMin,VariableMax);
   //x2=VariableValue(DecodeChromosome(currentbest.chrom,LENGTH1,LENGTH2),VariableMin,VariableMax);
   fprintf(fpga0,"\n");

   fprintf(fpga0," gen=%d,avg=%f,best=%f,",generation,average,currentbest.value);
   fprintf(fpga0,"\n");


   for(i=0;i<CHROMLENGTH;i++)
        {fprintf(fpga0,"%c",currentbest.chrom[i]);}
   fprintf(fpga0,"\n");
   }

/*************************************************
*                   TEST Main Program            *
*************************************************/
void GARun()
{
 generation=0;
 GenerateInitialPopulation();
 EvaluatePopulation();
 while (generation<MaxGeneration)
  {
   generation++;
   GenerateInitialPopulation();
   EvaluatePopulation();
   PerformEvolution();
   OutputTextReport ();
   }
}

double DeCode(char *char1)
{
        double dlDeCode;
        dlDeCode = DecodeChromosome(char1,0,LENGTH);
        return dlDeCode;
}

/********************************************************
*   Function: Construct a Value for a Variant           *
*     by DecodeChromosome(population[i].chrom,0,LENGTH1)*
*   Variable: MinValue and MaxValue of Variant          *
********************************************************/
double VariableValue(long DecodeChorm,double Vmin,double Vmax)
{
        double dlValue;
        dlValue = Vmin + (Vmax - Vmin)*DecodeChorm/1023.0;
        return dlValue;
}
