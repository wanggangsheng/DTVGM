#include <math.h> //M_PI
#include <math.hpp>
double pdf_gauss(double x);
double cdf_gauss(double a,double b);


double LikelihoodGauss(int NT,double Var,double *qobs,double *qcal);
double AccProb(int iProDis,int isel,int noptM,double *theta0,double *theta,double Tao,
			   int NT,double *Robs,double *Rcal0,double *Rcal);

double modelabc(int nn,double *theta,double *Pobs,double *Robs,double *GWobs,
				double *Rcal,double *GWcal,double &NSEC_R,double &NSEC_GW);

//==============================================================================
double pdf_gauss(double x)
{

	return 1.0/sqrt(2.0*M_PI)*exp(-x*x/2.0);
}
//==============================================================================
double cdf_gauss(double a,double b)
{
	int j;
	double dx,xm,xr,ss;

	const double w[5] = {.2955242247,.2692667193,.2190863625,.1494513491,.0666713443};
	const double x[5] ={.1488743389,.4333953941,.6794095682,.8650633666,.9739065285};

	if(b<a) return 0;
	xm=0.5*(b+a);
	xr=0.5*(b-a);
	ss=0.0;
	for(j=0;j<5;j++)
	{
		dx=xr*x[j];
		ss=ss+w[j]*(pdf_gauss(xm+dx)+pdf_gauss(xm-dx));
	}
	ss=xr*ss;
	return ss;
}
//==============================================================================
double modelabc(int nn,double *theta,double *Pobs,double *Robs,double *GWobs,
				double *Rcal,double *GWcal,double &NSEC_R,double &NSEC_GW)
{
	int i;
	double a,b,c,GW0,fobs,w;

	a = theta[0];
	b = theta[1];
	c = theta[2];
	GW0 = 500;

	/*
	a = 0.1;
	b = 0.75;
	c = theta[0];
	GW0 = theta[1]*1000;
	*/
	w = 0.5;

	double *RS = new double[nn];
	double *RSS = new double[nn];
	double *ETa = new double[nn];
	double *Rech = new double[nn];


	Rech [0] = a*Pobs[0];
	ETa[0] = b*Pobs[0];
	RS[0] = max(0.0,(1.0 - a - b)*Pobs[0]);
	RSS[0] = c*GW0;
	GWcal[0] = max(0.0,GW0 + Rech [0] - RSS[0]);
	Rcal[0] = RS[0] + RSS[0];

	for(i=1;i<nn;i++)
	{
		Rech [i] = a*Pobs[i];
		ETa[i] = b*Pobs[i];
		RS[i] = max(0.0,(1.0 - a - b)*Pobs[i]);
		RSS[i] = c*GWcal[i - 1];
		GWcal[i] = max(0.0,GWcal[i - 1] + Rech[i] - RSS[i]);
		Rcal[i] = RS[i] + RSS[i];
	}

	NSEC_R = NashSutcliffeEC(nn,Robs,Rcal);
	NSEC_GW = NashSutcliffeEC(nn,GWobs,GWcal);
	fobs = w*NSEC_R + (1.0 - w)*NSEC_GW;

	double sum1 = 0.0;
	for(i=1;i<nn;i++)
	{
		sum1 += (sqrt(Robs[i])-sqrt(Rcal[i]))*(sqrt(Robs[i])-sqrt(Rcal[i]));
	}
	NSEC_GW = sum1/nn; //Variation
	return fobs;

}
//==============================================================================

//==============================================================================
double LikelihoodGauss(int NT,double Var,double *qobs,double *qcal)
{//
	//const double PI = 3.1415926;
	int i;
	double ss;
	ss = 1.0;
	for(i = 0;i<NT;i++)
		ss = ss*exp(-(qobs[i] - qcal[i])*(qobs[i] - qcal[i])*0.5/Var)/sqrt(2*M_PI*Var)/
		(1.0 - cdf_gauss(-10.0,-1*qcal[i]/sqrt(Var)));
	return ss;
}
//==============================================================================
double AccProb(int iProDis,int isel,int noptM,double *theta0,double *theta,double Tao,
			   int NT,double *Robs,double *Rcal0,double *Rcal)
{//Acceptance Probability
	//const double PI = 3.1415926;
	int i;
	long double ss;
        double Var0,Var;

	double *qobs = new double[NT];
	double *qcal0 = new double[NT];
	double *qcal = new double[NT];

        Var0 = theta0[noptM - 1];
        Var = theta[noptM - 1];

        ss = 0.0;
	for(i = 0;i<NT;i++)
	{
		qcal0[i] = sqrt(max(0,Rcal0[i]));
		qobs[i] = sqrt(max(0,Robs[i]));
		qcal[i] = sqrt(max(0,Rcal[i]));
		ss += ((qobs[i] - qcal0[i])*(qobs[i] - qcal0[i])/Var0
			-(qobs[i] - qcal[i])*(qobs[i] - qcal[i])/Var)*0.5
			+ log(1.0 - cdf_gauss(-10.0,-1*qcal0[i]/sqrt(Var0)))
			- log(1.0 - cdf_gauss(-10.0,-1*qcal[i]/sqrt(Var)))
                        + 0.5*log(2*M_PI*Var0) - 0.5*log(2*M_PI*Var);
	} //end for

	switch (iProDis)
	{
	case 1: //asymmetric uniform
		ss += log(theta0[isel]) - log(theta[isel]);
		break;
	case 3: //asymmetric normal
		ss += log(theta0[isel]) - log(theta[isel])
			+0.5*(theta0[isel] - theta[isel])*(theta0[isel] - theta[isel])/(Tao*Tao)
			*(1.0/(theta0[isel]*theta0[isel]) - 1.0/(theta[isel]*theta[isel]));
		break;
	default: //symmetric distribution
		ss = ss;
	}//end switch

        delete[] qobs;
        delete[] qcal;
        delete[] qcal0;

	if(ss>0) return 1;
	else if(ss<-100) return 0;
	else return expl(ss);

}
