#include <trios.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>


/* #define _DEBUG_ */


/* The next 2 functions should be in math.h, but something is wrong        */
/* I don't know if there is a problem with KHOROS or with the C library... */
/*int isinf(double value);
  int isnan(double value);*/


/*                                                            */
/* This file contains some routines from Numerical Recipes    */
/* Small modifications have been made in order to adapt them  */
/* to the necessities of PAC system                           */
/*                                                            */


/* This part is obsolete at this moment ... */
/* function used to evaluate the switching cost (Bayesian approach) */
/* by the switching algorithm                                       */
#define FUNC(x,pa,pb) ((*func)(x,pa,pb))


/* Constants and macros used by minimization routine */
#define NMAX 5000      /* maximum number of function eveluations */
                       /* done by "amoeba"                       */

#define GET_PSUM \
      for (j=1;j<=ndim;j++) {\
      for (sum=0.0,i=1;i<=mpts;i++) sum += p[i][j];\
      psum[j]=sum;}

#define SWAP(a,b) {swap=(a);(a)=(b);(b)=swap;}


double drand48(void);
/* HERALDO 14/3/2001. Prototypes */

 double gammln (double xx);
 void   spline (float x[], float y[], int n, double yp1, double ypn, float
y2[]);
 void   splint (float xa[], float ya[], float y2a[], int n, double x, float
*y);
 float  qgaus  (double (*func)(double,double,double), double a, double b,
double pa, double pb);
 float  trapzd (float (*func)(double,double,double), double a, double b, int
n, double pa, double pb);
 float  qsimp  (float (*func)(double,double,double), double a, double b,
double pa, double pb);
 float  qromb  (float (*func)(double,double,double), double a, double b,
double pa, double pb);



/*
     -------------------------------------------
     FUNCTION: pac_gen_seed
     -------------------------------------------
*/

long            /*+ Purpose: generates a seed to be used for random
                             number generation                             +*/
  pac_gen_seed( void
  )
/*+ Return: a seed value obtained from current time                        +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Aug 12 1999                                      */

  long     timeseed ;
  /*
  struct   timeval *tv ;
  struct   timezone *tz ;
  */
  struct   timeval tv ;
  /*
  struct   timezone tz ;
  */

  /*
  tv = (struct timeval *)malloc(sizeof(struct timeval)) ;
  tz = (struct timezone *)malloc(sizeof(struct timezone)) ;
  */

  gettimeofday(&tv, NULL) ;

  timeseed = tv.tv_usec ;

  return(timeseed) ;
}

/*
     -------------------------------------------
     FUNCTION: ran1
     -------------------------------------------
*/

float           /*+ Purpose: Generate a uniform random deviate between
                     0.0 and 1.0 (exclusive of the endpoint values)        +*/
  ran1(
    long *idum         /*+ In: call with a negative value and, thereafter,
                               do not alter it between successive calls    +*/
)
/*+ Return: a uniform random deviate between 0.0 and 1.0 (exclusive of the
    endpoint values)                                                       +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Jul 30 1998                                                   */

  int    j ;
  long   k ;
  static long iy=0 ;
  static long iv[NTAB] ;
  float  temp ;

  if(*idum <= 0 || !iy) {
    if(-(*idum) < 1) *idum = 1 ;
    else *idum = -(*idum) ;
    for(j=NTAB+7; j>=0;j--) {
      k=(*idum)/IQ ;
      *idum = IA*(*idum - k*IQ) - IR*k ;
      if(*idum < 0) *idum += IM ;
      if(j < NTAB) iv[j] = *idum ;
    }
    iy = iv[0] ;
  }

  k = (*idum)/IQ ;
  *idum = IA*(*idum - k*IQ) - IR*k ;
  if(*idum < 0) *idum += IM ;
  j = iy/NDIV ;
  iy = iv[j] ;
  iv[j] = *idum ;
  if((temp=AM*iy) > RNMX) return RNMX ;
  else return temp ;
}


/*
     -------------------------------------------
     FUNCTION: ran2x
     -------------------------------------------
*/

float            /*+ Purpose: generate a random number                     +*/
   ran2x(
    long *idum         /*+ In/Out: current seed                            +*/
   )
/*+ Return: a random number                                                +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Feb 26 1998                                                   */

  int    j ;
  long   k ;
  float  temp;
  static long idum2x=123456789;
  static long iyx=0;
  static long ivx[NTAB];


  if (*idum <= 0) {		 /*Initialize*/
    if (-(*idum) < 1) *idum=1;	 /*Be sure to prevent idum=0*/
    else *idum = -(*idum);
    idum2x=(*idum);
    for (j=NTAB+7;j>=0;j--) {	 /*Load the shuffle table(after 8 warm-ups)*/
      k=(*idum)/IQ1;
      *idum=IA1*(*idum-k*IQ1)-k*IR1;
      if (*idum < 0) *idum += IM1;
      if (j < NTAB) ivx[j] = *idum;
    }
    iyx=ivx[0];
  }
  k=(*idum)/IQ1;		  /*Start here when not initializing*/
  *idum=IA1*(*idum-k*IQ1)-k*IR1;  /*Compute idum(IA1*idum)%IM1 without*/
  if (*idum < 0) *idum += IM1;	  /* overflows by Scharge's method*/
  k=idum2x/IQ2;
  idum2x=IA2*(idum2x-k*IQ2) - k*IR2; /*Compute idum2=(IA2*idum)% IM2 likewise*/
  if (idum2x < 0) idum2x +=IM2;
  j=iyx/NDIV1;			    /*Will be in the range 0..NTAB-1.*/
  iyx=ivx[j]-idum2x;	       	/*Here idum is shuffled, idum and idum2 are*/
  ivx[j] = *idum;      		/*     combined to generate output.	*/
  if (iyx <1) iyx +=IMM1;
  if ((temp= AM1*iyx) > RNMX) return RNMX;  /* Because users dont expect
endpoint values.*/
  else return temp;
}


/*
     -------------------------------------------
     FUNCTION: ran2y
     -------------------------------------------
*/

float            /*+ Purpose: generate a random number                     +*/
   ran2y(
    long *idum         /*+ In/Out: current seed                            +*/
   )
/*+ Return: a random number                                                +*/
{

/*  author: Nina S. Tomita, R. Hirata Jr. (nina@ime.usp.br)                 */
/*  date: Thu Feb 26 1998                                                   */

  int j;
  long k;
  float temp;
  static long idum2y=123456789;
  static long iyy=0;
  static long ivy[NTAB];

  if (*idum <= 0) {			      /*Initialize*/
    if (-(*idum) < 1) *idum=1;	       /*Be sure to prevent idum=0*/
    else *idum = -(*idum);
    idum2y=(*idum);
    for (j=NTAB+7;j>=0;j--) {  /*Load the shuffle table(after 8 warm-ups)*/
      k=(*idum)/IQ1;
      *idum=IA1*(*idum-k*IQ1)-k*IR1;
      if (*idum < 0) *idum += IM1;
      if (j < NTAB) ivy[j] = *idum;
    }
    iyy=ivy[0];
  }
  k=(*idum)/IQ1;       		 /*Start here when not initializing*/
  *idum=IA1*(*idum-k*IQ1)-k*IR1; /*Compute idum(IA1*idum)%IM1 without*/
  if (*idum < 0) *idum += IM1;   /* overflows by Scharge's method*/
  k=idum2y/IQ2;
  idum2y=IA2*(idum2y-k*IQ2) - k*IR2;	/*Compute idum2=(IA2*idum)% IM2 likewise*/
  if (idum2y < 0) idum2y +=IM2;
  j=iyy/NDIV1;		/*Will be in the range 0..NTAB-1.*/
  iyy=ivy[j]-idum2y;	/*Here idum is shuffled, idum and idum2 are*/
  ivy[j] = *idum;	/*     combined to generate output.	*/
  if (iyy <1) iyy +=IMM1;
  if ((temp= AM*iyy) > RNMX) return RNMX;  /* Because users dont expect
endpoint values.*/
  else return temp;
}


float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
  float *v;

  v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
  if (!v) trios_fatal("allocation failure in vector()");
  return v-nl+NR_END;
}

void polint(float xa[], float ya[], int n, double x, float *y, float *dy)
{
  int   i, m, ns=1;
  float den, dif, dift, ho, hp, w;
  float *c, *d;

  dif=fabs(x-xa[1]);
  c=vector(1,n);
  d=vector(1,n);
  for (i=1;i<=n;i++) {
    if ( (dift=fabs(x-xa[i])) < dif) {
      ns=i;
      dif=dift;
    }
    c[i]=ya[i];
    d[i]=ya[i];
  }
  *y=ya[ns--];
  for (m=1;m<n;m++) {
    for (i=1;i<=n-m;i++) {
      ho=xa[i]-x;
      hp=xa[i+m]-x;
      w=c[i+1]-d[i];
      if ( (den=ho-hp) == 0.0) trios_fatal("Error in routine polint");
      den=w/den;
      d[i]=hp*den;
      c[i]=ho*den;
    }
    *y += (*dy=(2*ns < (n-m) ? c[ns+1] : d[ns--]));
  }
  free_vector(d,1,n);
  free_vector(c,1,n);
}



/************************************************************************/
/* Module name : vector							*/
/* Functionality :	Allocates a 1-D float array, whose indices	*/
/*			range from "nl" through "nh", and returns a	*/
/*			pointer to this array.				*/
/* Parameters :	nl,nh : lowest and highest indices.			*/
/* Calls modules :	error						*/
/************************************************************************/
float *fvector(int nl, int nh)
{
  float *v ;

  v=(float *)malloc((unsigned)(nh-nl+1)*sizeof(float)) ;
  if (v==NULL) trios_fatal("Vector : Memory allocation failure.") ;
  return (v-nl) ;
}




/************************************************************************/
/* Module name : free_fvector						*/
/* Functionality :	Frees a 1-D float array. 			*/
/* Parameters :	v : pointer to the array				*/
/*		nl,nh : lowest and highest indices.			*/
/* Remarks: It is possible that the memory deallocation modules do not  */
/*          work well always. This should not be a major problem in most*/
/*          cases, however.                                             */
/************************************************************************/
void free_fvector(float *v, int nl, int nh)
{
  free((char*)(v+nl));
}



void free_vector(float *v, long nl, long nh)
/* free a float vector allocated with vector() */
{
  free((FREE_ARG) (v+nl-NR_END));
}




/************************************************************************/
/* Module name : MyLog2                                                  */
/* Functionality : Computes the log-base-2 of a double, and returns      */
/*                 a float.                                              */
/************************************************************************/
float mylog2(double x)
{
  return((float)(log(x)/log((double)2.0))) ;
}




/************************************************************************/
/* Module name :	myrandom					*/
/* Functionality :	Generates a random number between 0 and 1, and	*/
/*			scales it to given range.        		*/
/* Parameters :	above, below : lower and upper limits, respectively on	*/
/*		the random number to be generated.			*/
/* Returns :	a floating point number.				*/
/* Calls modules :	drand48 (C library call)			*/
/* Remarks: If your system doesn't support drand48(), you can substitute*/
/*          the call below with any good pseudo random number generator */
/*          function call.                                              */
/************************************************************************/
float myrandom(double above, double below)
{
    double r = rand();
    r = above + drand48() * (below - above);
    float res = (float) r;
    return res;
}




/************************************************************************/
/* Module name : Average                                                */
/* Functionality : Computes the average of a float array.               */
/* Parameters: a: float array, indices in the range 1,n                 */
/*             n: length of the float array.                            */
/************************************************************************/
float average(float *a, int n)
{
  float sum=0 ;
  int i ;

  for (i=1 ;i<=n ;i++) sum += a[i] ;
  return(sum/n) ;
}



/************************************************************************/
/* Module name : Min                                                    */
/* Functionality : Returns the minimum entry of a float array           */
/* Parameters: a: float array, indices in the range 1,n                 */
/*             n: length of the float array.                            */
/************************************************************************/
float min(float *a, int n)
{
  float xmin ;
  int i ;

  xmin = a[1] ;
  for (i=2; i<=n; i++) if (a[i] < xmin) xmin = a[i] ;
  return(xmin) ;
}




/************************************************************************/
/* Module name : SDev                                                   */
/* Functionality : Computes the standard deviation of a float array.    */
/* Parameters: a: float array, indices in the range 1,n                 */
/*             n: length of the float array.                            */
/* Calls Modules: Average                                               */
/************************************************************************/
float sdev(float *a, int n)
{
  int i ;
  float mean ;
  double sum=0 ;

  mean = average(a,n) ;
  for (i=1; i<=n; i++)
    sum += ((a[i]-mean)*(a[i]-mean)) ;

  if (n > 1) sum = sum/(n-1) ;
  sum = sqrt(sum) ;
  return((float)sum) ;
}




/************************************************************************/
/* Module name : ivector						*/
/* Functionality :	Allocates a 1-D integer array, whose indices	*/
/*			range from "nl" through "nh", and returns a	*/
/*			pointer to this array.				*/
/* Parameters :	nl,nh : lowest and highest indices.			*/
/* Calls modules :	error						*/
/************************************************************************/
int *ivector(int nl, int nh)
{
  int *v ;

  v=(int *)malloc((unsigned)(nh-nl+1)*sizeof(int)) ;
  if (v==NULL) trios_fatal("Ivector : Memory allocation failure.") ;
  return(v-nl) ;
}



/************************************************************************/
/* Module name : dvector						*/
/* Functionality :	Allocates a 1-D double array, whose indices	*/
/*			range from "nl" through "nh", and returns a	*/
/*			pointer to this array.				*/
/* Parameters :	nl,nh : lowest and highest indices.			*/
/* Calls modules :	error						*/
/************************************************************************/
double *dvector(int nl, int nh)
{
  double *v ;

  v=(double *)malloc((unsigned)(nh-nl+1)*sizeof(double)) ;
  if (v==NULL) trios_fatal("Dvector : Memory allocation failure.") ;
  return (v-nl) ;
}



/************************************************************************/
/* Module name : free_ivector						*/
/* Functionality :	Frees a 1-D integer array. 			*/
/* Parameters :	v : pointer to the array				*/
/*		nl,nh : lowest and highest indices.			*/
/* Remarks: It is possible that the memory deallocation modules do not  */
/*          work well always. This should not be a major problem in most*/
/*          cases, however.                                             */
/************************************************************************/
void free_ivector(int *v, int nl, int nh)
{
  free((char*)(v+nl)) ;
}



/************************************************************************/
/* Module name : free_dvector						*/
/* Functionality :	Frees a 1-D double array. 			*/
/* Parameters :	v : pointer to the array				*/
/*		nl,nh : lowest and highest indices.			*/
/* Remarks: It is possible that the memory deallocation modules do not  */
/*          work well always. This should not be a major problem in most*/
/*          cases, however.                                             */
/************************************************************************/
void free_dvector(double *v, int nl, int nh)
{
  free((char*)(v+nl)) ;
}



/************************************************************************/
/* Module name :	generate_random_hyperplane			*/
/* Functionality :	generates coefficients of a hyperplane randomly.*/
/* Parameters :	array_name, length of the array.                        */
/*              max_value : maximum absolute  value of any coefficient  */
/* Returns :	Nothing.						*/
/* Calls modules :	myrandom        				*/
/* Is called by modules :	oblique_split (mktree.c)		*/
/*				perturb_randomly (perturb.c)		*/
/************************************************************************/
void generate_random_hyperplane(float *array_name, int length, double
max_value)
{
  int i ;

  for (i=1; i<=length; i++)
    array_name[i] = myrandom(-1.0 * max_value, max_value);

}


float trapzd(
  float (*func)(double,double,double),
  double a,
  double b,
  int n,
  double pa,
  double pb
)
{
  float  x, tnm, sum, del;
  static float s;
  int    it, j;

  if (n == 1) {
    return (s=0.5*(b-a)*(FUNC(a,pa,pb)+FUNC(b,pa,pb)));
  }
  else {
    for (it=1,j=1; j<n-1;j++) it <<= 1;
    tnm = it;
    del = (b-a)/tnm;
    x = a+0.5*del;
    for (sum=0.0,j=1; j<=it; j++,x+=del) sum += FUNC(x,pa,pb);
    s = 0.5*(s+(b-a)*sum/tnm);
    return s;
  }
}


double gammln(double xx)
{
  double x, y, tmp, ser;
  static double cof[6]={76.18009172947146,-86.50532032941677,
        24.01409824083091,-1.231739572450155,
        0.1208650973866179e-2,-0.5395239384953e-5};
  int j;

  y = x = xx ;
  tmp = x+5.5 ;
  tmp -= (x+0.5)*log(tmp) ;
  ser = 1.000000000190015 ;

  for(j=0;j<=5;j++) ser += cof[j]/++y;

  return -tmp+log(2.5066282746310005*ser/x) ;
}


double beta(double z, double w)
{
  return exp(gammln(z)+gammln(w)-gammln(z+w)) ;
}


float qgaus(double (*func)(double,double,double), double a, double b,
            double pa, double pb)
{
  int   j ;
  float xr, xm, dx, s ;
  static float x[]={0.0,0.1488743389,0.4333953941,
        0.6794095682,0.8650633666,0.9739065285} ;
  static float w[]={0.0,0.2955242247,0.2692667193,
        0.2190863625,0.1494513491,0.0666713443} ;

  xm = 0.5*(b+a) ;
  xr = 0.5*(b-a) ;
  s = 0 ;
  for (j=1;j<=5;j++) {
    dx = xr*x[j] ;
    s += w[j]*((*func)(xm+dx,pa,pb)+(*func)(xm-dx,pa,pb)) ;
  }
  return s *= xr;
}


void spline(float x[], float y[], int n, double yp1, double ypn, float y2[])
{
  int   i, k ;
  float p, qn, sig, un, *u ;

  u = vector(1,n-1) ;
  if (yp1 > 0.99e30)
    y2[1] = u[1] = 0.0 ;
  else {
    y2[1] = -0.5 ;
    u[1] = (3.0/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1) ;
  }
  for (i=2;i<=n-1;i++) {
    sig = (x[i]-x[i-1])/(x[i+1]-x[i-1]) ;
    p = sig*y2[i-1]+2.0 ;
    y2[i] = (sig-1.0)/p ;
    u[i] = (y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]) ;
    u[i] = (6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p ;
  }
  if (ypn > 0.99e30)
    qn = un = 0.0 ;
  else {
    qn = 0.5 ;
    un = (3.0/(x[n]-x[n-1]))*(ypn-(y[n]-y[n-1])/(x[n]-x[n-1])) ;
  }
  y2[n] = (un-qn*u[n-1])/(qn*y2[n-1]+1.0) ;
  for (k=n-1;k>=1;k--)
    y2[k] = y2[k]*y2[k+1]+u[k] ;
  free_vector(u, 1, n-1) ;
}


void splint(float xa[], float ya[], float y2a[], int n, double x, float *y)
{
  int   klo, khi, k ;
  float h, b, a ;

  klo = 1 ;
  khi = n ;
  while (khi-klo > 1) {
    k = (khi+klo) >> 1 ;
    if (xa[k] > x) khi = k ;
    else klo = k ;
  }
  h = xa[khi]-xa[klo] ;
  if (h == 0.0) trios_fatal("Bad xa input to routine splint") ;
  a = (xa[khi]-x)/h ;
  b = (x-xa[klo])/h ;
  *y = a*ya[klo]+b*ya[khi]+((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0 ;
}


float amotry(float **p, float y[], float psum[], int ndim,
    float (*funk)(float [], float [], int), int ihi, double fac,
        float *px, int npx)
{
  int   j ;
  float fac1, fac2, ytry, *ptry ;

  ptry = vector(1,ndim) ;
  fac1 = (1.0-fac)/ndim ;
  fac2 = fac1-fac ;
  for (j=1;j<=ndim;j++) ptry[j] = psum[j]*fac1-p[ihi][j]*fac2 ;
  ytry = funk(ptry, px, npx) ;
  if (ytry < y[ihi]) {
    y[ihi] = ytry ;
    for (j=1;j<=ndim;j++) {
      psum[j] += ptry[j]-p[ihi][j] ;
      p[ihi][j] = ptry[j] ;
    }
  }
  free_vector(ptry, 1, ndim) ;
  return ytry ;
}

void amoeba(float **p, float y[], int ndim, double ftol,
    float (*funk)(float [], float [], int), int *nfunk,
        float *px, int npx)
{
  int   i, ihi, ilo, inhi, j, mpts=ndim+1 ;
  float rtol, sum, swap, ysave, ytry, *psum;

  psum = vector(1,ndim) ;
  *nfunk = 0 ;
  GET_PSUM
  for (;;) {
    ilo = 1 ;
    ihi = y[1]>y[2] ? (inhi=2,1) : (inhi=1,2) ;
    for (i=1;i<=mpts;i++) {
      if (y[i] <= y[ilo]) ilo = i ;
      if (y[i] > y[ihi]) {
    inhi = ihi ;
    ihi = i ;
      } else if (y[i] > y[inhi] && i != ihi) inhi=i ;
    }
    rtol = 2.0*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo])) ;
    if (rtol < ftol) {
      SWAP(y[1],y[ilo])
      for (i=1;i<=ndim;i++) SWAP(p[1][i],p[ilo][i])
      break;
    }
    if (*nfunk >= NMAX) return ;
    *nfunk += 2;
    ytry = amotry(p, y, psum, ndim, funk, ihi, -1.0, px, npx) ;
    if (ytry <= y[ilo])
      ytry = amotry(p, y, psum, ndim, funk, ihi, 2.0, px, npx) ;
    else if (ytry >= y[inhi]) {
      ysave = y[ihi] ;
      ytry = amotry(p, y, psum, ndim, funk, ihi, 0.5, px, npx) ;
      if (ytry >= ysave) {
        for (i=1;i<=mpts;i++) {
          if (i != ilo) {
            for (j=1;j<=ndim;j++)
              p[i][j] = psum[j] = 0.5*(p[i][j]+p[ilo][j]) ;
        y[i] = (*funk)(psum, px, npx) ;
      }
        }
        *nfunk += ndim ;
        GET_PSUM
      }
    } else --(*nfunk) ;
  }
  free_vector(psum, 1, ndim) ;
}



float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  long i, nrow=nrh-nrl+1, ncol=nch-ncl+1 ;
  float **m ;

  /* allocate pointers to rows */
  m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
  if (!m) trios_fatal("allocation failure 1 in matrix()");
  m += NR_END ;
  m -= nrl ;

  /* allocate rows and set pointers to them */
  m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float))) ;
  if (!m[nrl]) trios_fatal("allocation failure 2 in matrix()") ;
  m[nrl] += NR_END ;
  m[nrl] -= ncl ;
  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol ;

  /* return pointer to array of pointers to rows */
  return m ;
}

void free_matrix(float **m, long nrl, long nrh, long ncl, long nch)
/* free a float matrix allocated by matrix() */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}

#ifdef EPS
#undef EPS
#endif
/* Integral evaluation - Simpson's method */
#define EPS 1.0e-6
/* #define JMAX  20 */ /* original */
#define JMAX  50

float qsimp(
  float (*func)(double,double,double),
  double a,
  double b,
  double pa,
  double pb
)
{
  int   j;
  float s, st, ost, os;

  ost = os = -1.0e30;
  for (j=1; j<=JMAX; j++) {
    st = trapzd(func, a, b, j, pa, pb);
    s = (4.0*st-ost)/3.0;
    if (fabs(s-os) < EPS*fabs(os)) return s;
    if (s == 0.0 && os == 0.0 && j > 6) return s;
    os = s;
    ost = st;
  }
  trios_fatal("Too many steps in routine qsimp");
  return 0.0;
}


/* Integral evaluation - ??? Method */
#define JMAXP (JMAX+1)
#define K     5

float qromb(  float (*func)(double,double,double),
double a, double b,
  double pa,
  double pb
)
{
  float ss,dss;
  float s[JMAXP],h[JMAXP+1];
  int j;

  h[1]=1.0;
  for (j=1;j<=JMAX;j++) {
    s[j]=trapzd(func,a,b,j,pa,pb);
    if (j >= K) {
      polint(&h[j-K],&s[j-K],K,0.0,&ss,&dss);
      if (fabs(dss) <= EPS*fabs(ss)) return ss;
    }
    h[j+1]=0.25*h[j];
  }
  trios_fatal("Too many steps in routine qromb");
  return 0.0;
}



