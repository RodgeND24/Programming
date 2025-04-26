#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

/* ФУНКЦИИ ДЛЯ ТЕСТОВ */
double Func(double *x){
	
	/* 2 переменные */
    //return 4*pow(x[0]-5,2)+pow(x[1]-6,2);
    //return 3*x[0]*x[0]+5*x[1]*x[1];
    //return pow((x[0]-4),2)+7*pow((2*x[1]-3),2);
    //return 2* x[0]*x[0] + 3*x[1]*x[1]+x[0]*x[1];
    return pow(x[0]-1,2)+2*pow(x[1]+3,2)+(x[0]-1)*(x[1]+3);
    
    /* 3 переменные */
    //return pow(7-2*x[0],2)+4*pow(x[1]-5,2)+1.5*pow(x[2]+2,2);
    //return 5*pow(4*x[0]-6,2)+pow(1+6*x[1],2)+20*pow(2*x[2]-0.75,2)+(4*x[0]-6)*(1+6*x[1]);
    
    /* 4 переменные */
    //return pow(2*x[0]+x[1]-1,2)+pow(x[2]-3*x[3]+2,2)+x[1]*x[1]+x[3]*x[3];
    //return pow(3*x[1]-0.25*x[3],2)+pow(10*x[2]+x[0]-6,2)+0.2*x[3]*x[3];
}

double* vector(int n){
	return (double*)malloc(n*sizeof(double));
}

double** matrix(int n){
	
	double **A = (double**)malloc(n*sizeof(double*));
	for (int i=0;i<n;i++)
		A[i] = vector(n);
	return A;
}

void free_matr(double **A, int n){
	int i;
	for (i=0;i<n;i++)
		free(A[i]);
	free(A);
}

double NormVect(double *x, int n){
	
	int i;
	double d=0;
	
	for (i=0;i<n;i++)
		d += x[i]*x[i];
	return sqrt(d);
}

double f_line(double *x, double l, double *d, int n){
	
	double *r = vector(n);
	int i;
	for (i=0;i<n;i++)
		r[i] = x[i]+l*d[i];
	return Func(r);
}

double golden_section_search(double *x, double *s, int n, double a, double b, double eps,int *k){
    /* Поиск минимума функции одной переменной методом золотого сечения*/
    
    double gr = (sqrt(5)+1)/2;  // Золотое сечение
    
    double aa = a;
    double bb = b;    
    double c = bb-(bb-aa)/gr;
    double d = aa+(bb-aa)/gr;
    
    while (fabs(c-d) > eps){
        if (f_line(x,c,s,n) < f_line(x,d,s,n))
            bb = d;
        else
            aa = c;
        
        c = bb-(bb-aa)/gr;
        d = aa+(bb-aa)/gr;
        *k += 1;
    }

    return (bb + aa)/2;
}

double* Powell_metod(double *x0, int n, double a, double b, int *k, int maxiter, double eps){
	
	int i,j;
	double *x_old = vector(n);
	double *x = vector(n);
	double **s = matrix(n);
	double *d = vector(n);
	double *d_new = vector(n);
	double f, l_min;
	
	for (i=0;i<n;i++)
		for (j=0;j<n;j++){
			x[j] = x0[j];			//сохраняем начальный вектор
			s[i][j] = (i==j)? 1: 0;	//Матрица векторов направлений (координатные оси)
		}
	int kk = 0;
	for (;kk<maxiter;kk++){
		
		for (i=0;i<n;i++){
			x_old[i] = x[i];	//копируем начальное значение x0
		}
		f = Func(x_old);		//запоминаем значение функции при x0
		//printf("%f\n",f);
		for (i=0;i<n;i++){	//одномерная минимизация по каждому из n-направлений
			
			for (j=0;j<n;j++)
				d[j] = s[i][j];	//выбираем i-ый вектор направления
			
			l_min = golden_section_search(x,d,n,a,b,eps,k);	//ищем коэффициент минимизации функции по каждому i-му направлению
			//printf("%f\n",l_min);
			for (j=0;j<n;j++)
				x[j] = x[j]+l_min*d[j];		//формируем новое значение x на основе коэффициента и вектора i-го направления
		}
		
		for (i=0;i<n;i++)
			d_new[i] = x[i]-x_old[i];	//находим новый вектор направления минимизации
		
		//printf("Норма: %f\n",NormVect(d_new,n));
		if (NormVect(d_new,n)<eps)	//если норма разности нового и старого значения x меньше точности - выходим из цикла
			break;
		else{
			double *dx = vector(n);
			double *df = vector(n);
			for (i=0;i<n;i++){
				for (j=0;j<n;j++)
					dx[j] = x_old[j]+l_min*s[i][j];	//находим, при каких значениях x_old по каждому направлению s происходим максимальная минимизация функции
				df[i] = Func(dx)-f;
			}
			double maxf = df[0];
			int ind = 0;
			for (i=1;i<n;i++)
				if (df[i]>maxf){
					maxf = df[i];	//ищем индекс направления s, которое максимально уменьшает функцию
					ind = i;
				}
			
			for (j=1;j<n;j++)
				s[ind][j] = d_new[j]/NormVect(d_new,n);	//заменяем направление в матрице s на новое нормализованное направление
		}	
	}
	/* освобождение памяти массивов */
	free(x_old);
	free(d);
	free(d_new);
	free_matr(s,n);	
	
	return x;
}

int main(){
	
	SetConsoleOutputCP(CP_UTF8);
	
	int N = 2;	// ТУТ ИЗМЕНЯЕТСЯ ЧИСЛО ПЕРЕМЕННЫХ ФУНКЦИИ
	double x0[] = {0, 0};  // ТУТ ЗАДАЮТСЯ НАЧАЛЬНЫЕ ЗНАЧЕНИЯ
	int i, k = 0;
	double eps =1e-6;	//точность
	int maxiter = 2000; //макс.число операций
	double a = -10, b = 10; //границы промежутка поиска минимума по направлению, ИНОГДА НУЖНО МЕНЯТЬ, В ЗАВИСИМОСТИ ОТ ЗАДАЧИ
	
	double *x_opt = Powell_metod(x0,N,a,b,&k,maxiter, eps);
	printf("*Оптимальная точка*\n");
	for (i=0;i<N;i++)
		printf("%9.6f ",x_opt[i]);
	printf("\n*Значение функции: %f*\n",Func(x_opt));
	printf("\nКоличество шагов: %d", k);
	getchar();
}
