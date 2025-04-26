#include <math.h>
int gauss(double **aa,double *pp,int n,double *x)
{
	int i,j,k,r;
	double c,m,max,s;
	double **a, *p;
a=new double *[n];
for(i=0;i<n;i++)
a[i]=new double[n];
p=new double [n];
for(i=0;i<n;i++)
		 for(j=0;j<n;j++)
			a[i][j]=aa[i][j];
	 for(i=0;i<n;i++)
		 p[i]=pp[i];
		for(k=0;k<n;k++)
	{
		max=fabs(a[k][k]);
		r=k;
		for(i=k+1;i<n;i++)
			if (fabs(a[i][k])>max)
			{
				max=fabs(a[i][k]);
				r=i;
			}
		for(j=0;j<n;j++)
		{
			c=a[k][j];
			a[k][j]=a[r][j];
			a[r][j]=c;
		}
		c=p[k];
		p[k]=p[r];
		p[r]=c;
		for(i=k+1;i<n;i++)
		{
			for(m=a[i][k]/a[k][k],j=k;j<n;j++)
				a[i][j]-=m*a[k][j];
			p[i]-=m*p[k];
		}
	}
	if (a[n-1][n-1]==0)
		if(p[n-1]==0)
			return -1;
		else return -2;
	else
	{
		for(i=n-1;i>=0;i--)
		{
			for(s=0,j=i+1;j<n;j++)
				s+=a[i][j]*x[j];
			x[i]=(p[i]-s)/a[i][i];
		}
		return 0;
	}	

}

void kanon(double t,double *x,double *y,int n,double *s)
{
	int i,j;
	double **c,*a;
	
	c=new double *[n];
	for (i=0;i<n;i++)
	  c[i]=new double [n];

    a=new double [n];
	
	for (i=0;i<n;i++)
	  for (j=0;j<n;j++)
	    c[i][j]=pow(x[i],j);
	   gauss(c,y,n,a);
	   
	   for (*s=i=0;i<n;i++)
	      *s=*s+a[i]*pow(t,i);
}

void newton(double t,double *x,double *y,int n,double *s)
{
	int i,j;
	double **c,*a,p;
	
	c=new double *[n];
	for (i=0;i<n;i++)
	c[i]=new double [n];
	
	a=new double [n];
	
	for (i=0;i<n;i++)
	  c[i][0]=y[i];
	  
	  
	for (i=0;i<n;i++)
	for (j=0;j<n;j++)
	  if (i<j)
	     c[i][j]=0;
	  
	for (j=1;j<n;j++)
	for (i=1;i<n;i++)
	  if (i>=j)
	    c[i][j]=(c[j-1][j-1]-c[i][j-1])/(x[j-1]-x[i]);
	    
	for (i=0;i<n;i++)
	  a[i]=c[i][i];
    
    for (*s=i=0;i<n;i++)
     {
		 p=1;
       for (j=0;j<i;j++)
         p=p*(t-x[j]);
       *s=*s+a[i]*p;
     }   
}

void lagrange(double t,double *x,double *y,int n,double *s)
{
	int i,j;
	double p;
	
	for (*s=i=0;i<n;i++)
	 {
		 p=1;
	    for (j=0;j<n;j++)
	      if (i!=j)
	         p=p*(t-x[j])/(x[i]-x[j]);
	    *s=*s+y[i]*p;    
     }

}

void lininterp(double t,double *x,double *y,int n,double *s)
{
	int i,j;
	double a,b;
	
 	if (t>x[n-1]) 
	    j=n-1;
	  else 
	     for (i=0;i<n-1;i++)
		    if (t<=x[i+1]) 
		     {
		      j=i+1;
		      break;		      
		     }
		     
	 	
	a=y[j-1]-((y[j]-y[j-1])*x[j-1])/(x[j]-x[j-1]);
	b=(y[j]-y[j-1])/(x[j]-x[j-1]);
	*s=a+b*t;	
}

void koef_spline(double *x,double *y,int n,double *b,double *c,double *d)
{
	int i;
	double *h,*l,*de,*lam;
	
	h=new double [n];
	l=new double [n];
	de=new double [n];
	lam=new double [n];
	
	for (i=1;i<n;i++)
	{
	  h[i]=x[i]-x[i-1];
	  l[i]=(y[i]-y[i-1])/h[i];
    }
	
	de[1]=-h[2]/(2*(h[1]+h[2]));
	lam[1]=1.5*(l[2]-l[1])/(h[1]+h[2]);
	
	for (i=3;i<n;i++)
	 {
	   de[i-1]=(-h[i])/(2*h[i-1]+2*h[i]+h[i-1]*de[i-2]);
	   lam[i-1]=(3*l[i]-3*l[i-1]-h[i-1]*lam[i-2])/(2*h[i-1]+2*h[i]+h[i-1]*de[i-2]);
	 }
	
	c[n-1]=0;
	for (i=(n-1);i>=2;i--) 
	  c[i-1]=de[i-1]*c[i]+lam[i-1];
	  
	
	for (i=1;i<n;i++)
	 {
	   d[i]=(c[i]-c[i-1])/(3*h[i]); 
	   b[i]=l[i]+(2*c[i]*h[i]+h[i]*c[i-1])/3;
     }
}

void spline(double t,double *x,double *y,int n,double *s)
{
	int i,j;
	double *a,*b,*c,*d;
	
	a=new double [n];
	b=new double [n];
	c=new double [n];
	d=new double [n];
	
	koef_spline(x,y,n,b,c,d);
	
	for (i=1;i<n;i++)
	  a[i]=y[i];
	  
	if (t>=x[n-1])
	   j=n-1;
	else
	   for (i=1;i<=(n-1);i++)
	      if (t<x[i]) 
	       {
	        j=i;
	        break;
		   }
	
	*s=a[j]+b[j]*(t-x[j])+c[j]*pow(t-x[j],2)+d[j]*pow(t-x[j],3);
}
