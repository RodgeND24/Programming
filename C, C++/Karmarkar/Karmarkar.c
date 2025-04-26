#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <sys/stat.h>

int empty_file(const char *filename) {	//проверка файла на пустоту
    struct stat st;
    if (stat(filename, &st) == -1) {
        //printf("Файла %s не существует/ошибка чтения\n",filename);
        return -1; // Ошибка чтения файла
    }
    if (st.st_size == 0){
		//printf("Файл %s пуст!",filename);
		return 1;
	}
	else{
		//printf("Файл %s не пуст!",filename);
		return 0;
	}
}

double* vector(int n){	//выделение памяти для одномерного массива (вектора)
	return (double*)malloc(n*sizeof(double));
}

double** matrix(int n, int m){	//выделение памяти для двумерного массива (матрицы), заполненного нулями
	
	double **A = (double**)malloc(n*sizeof(double*));
	for (int i=0;i<n;i++)
		A[i] = (double*)calloc(m,sizeof(double));
	return A;
}

void free_matr(double **A, int n){	//освобождение памяти от матрицы
	int i;
	for (i=0;i<n;i++)
		free(A[i]);
	free(A);
}

void ReadSizeMatr(char *path, int *n, int *m){	//считывание размера матрицы
	
	FILE *file;
	int nn,mm;
	file = fopen(path,"r");
	fscanf(file,"%d",&nn);
	fscanf(file,"%d",&mm);
	*n = (int)nn;
	*m = (int)mm;
	fclose(file);
}

double** ReadMatrix(char *path, int n, int m){	//считывание матрицы
	
	int i,j;
	FILE *file;
	
	file = fopen(path,"r");
	int nn,mm;
	fscanf(file,"%d",&nn);	//костыль для считывания с 3 элемента
	fscanf(file,"%d",&mm);

	double **A = matrix(n,m);
	double a;
	
	for (i=0;i<n;i++)
		for (j=0;j<m;j++){
			fscanf(file,"%lf",&a);
			A[i][j] = a;
		}
	fclose(file);
	return A;
}

double* ReadVector(char *path, int n){	//считывание вектора
	
	int i;
	FILE *file;
		
	double *x =vector(n);
	double a;
	
	file = fopen(path,"r");
	for (i=0;i<n;i++){
		fscanf(file,"%lf",&a);
		x[i] = a;
	}
	fclose(file);
	return x;
}

double *MultV_M(double *x, int p, double **A, int n, int m){	//умножение вектора на матрицу
	
	int j,k;
	double *Mult = vector(m);
		
	for (j=0;j<p;j++)
		Mult[j] = 0;
			
	for (j=0;j<m;j++)
		for (k=0;k<n;k++)
			Mult[j] += x[k]*A[k][j];
			
	return Mult;
}

double **MultM_M(double **A, int n1, int m1, double **B, int n2, int m2){	//умножение матрицы на матрицу
	
	int i, j, k;
	double **Mult = matrix(n1,m2);
	
	for (i=0;i<n1;i++)
		for (j=0;j<m2;j++){
			Mult[i][j] = 0;
			for (k=0;k<m1;k++)
				Mult[i][j] += A[i][k]*B[k][j];
			}

	return Mult;
}

void Print_Matr(double **A, int n, int m){	//вывод матрицы в консоль
	
	int i,j;
	printf("\n");
	for (i=0;i<n;i++){
		printf("|");
		for (j=0;j<m;j++)
			printf(" %7.3lf ",A[i][j]);
		printf("|\n");
	}
}

void Print_Vect(double *x, int n){	//вывод вектора в консоль
	
	int i;
	printf("   {");
	for (i=0;i<n;i++)
		printf(" %7.3lf ",x[i]);
	printf("}");
}

double **A_T(double **A, int n, int m){	//транспонирование матрицы
	
	int i,j;
	double **A_T = matrix(m,n);
	
	for (i=0;i<n;i++)
		for (j=0;j<m;j++)
			A_T[j][i] = A[i][j];
	
	return A_T;
}

double *C(char *path, int n, int *f){	//функция считывания целевой функции
	
	int i;
	double *c = vector(n);
	
	if (!(empty_file(path))){
		char minmax;
		double a;
		FILE *file;
		file = fopen(path,"r");
		fscanf(file,"%c",&minmax);	//считывание из файла с целевой функцией первого символа: + или -
		for (i=0;i<n;i++){
			fscanf(file,"%lf",&a);
			c[i] = a;
		}
		
		printf("\n\n*Целевая функция:*");
		Print_Vect(c,n);
		if ((int)minmax == 43){
			printf(" --> max\n");	//ANCII-код символа +: 43
			for (i=0;i<n;i++)
				c[i] = -c[i];
			*f = 1;
		}
		else{
			printf(" --> min\n");
			*f = 0;
		}
	}
	else{
		printf("Файл целевой функции %c пуст!",*path);
	}
	return c;
	
}

double NormVect(double *x, int n){	//норма вектора
	
	int i;
	double d=0;
	
	for (i=0;i<n;i++)
		d += x[i]*x[i];
	return sqrt(d);
}

double **inverseMatrix(double **A, int n) {	//обратная матрицы через метод Гаусса
    
    int i, j, k;
    // Создаем расширенную матрицу [A|I]
    double **AI = (double **)malloc(n * sizeof(double *));
    for (i = 0; i < n; i++) {
        AI[i] = (double *)malloc(2 * n * sizeof(double));
        // Копируем исходную матрицу
        for (j = 0; j < n; j++) {
            AI[i][j] = A[i][j];
        }
        
        // Добавляем единичную матрицу
        for (j = n; j < 2 * n; j++) {
            AI[i][j] = (i == (j - n)) ? 1.0 : 0.0;
        }
    }
    
    // Прямой ход метода Гаусса
    for (i = 0; i < n; i++) {
        // Если элемент на диагонали равен 0, находим строку для замены
        if (AI[i][i] == 0.0) {
            int swap_row = -1;
            for (k = i + 1; k < n; k++) {
                if (AI[k][i] != 0.0) {
                    swap_row = k;
                    break;
                }
            }

            // Меняем строки местами
            double *temp = AI[i];
            AI[i] = AI[swap_row];
            AI[swap_row] = temp;
        }
        
        // Нормируем текущую строку
        double M = AI[i][i];
        for (j = i; j < 2 * n; j++) {
           AI[i][j] /= M;
        }
        
        // Вычитаем текущую строку из других строк
        for (k = 0; k < n; k++) {
            if (k != i && AI[k][i] != 0.0) {
                double factor = AI[k][i];
                for (j = i; j < 2 * n; j++) {
                    AI[k][j] -= factor * AI[i][j];
                }
            }
        }
    }
    
    // Извлекаем обратную матрицу из правой части расширенной матрицы
    double **inverse = matrix(n,n);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            inverse[i][j] = AI[i][j + n];
        }
    }
    
    // Освобождаем память расширенной матрицы
    free_matr(AI,n);
    
    return inverse;
}

double *proj_tranform(double *c, double **A, double **D, double *x, int n, int m, double r, double alf){	//проективное преобразование
	
	double **C = matrix(1,m);
	double **AD = matrix(n,m);
	double **P = matrix(n+1,m);
	double **PT = matrix(m,n+1);
	double **P_PT = matrix(n+1,n+1);
	double **P_PT_1 = matrix(n+1,n+1);
	double **P1 = matrix(n+1,m);
	double **P2 = matrix(m,m);
	double **I = matrix(m,m);
	double **I_P = matrix(m,m);
	double **cD = matrix(1,m);
	double **cDT = matrix(m,1);
	double **cp = matrix(m,1);
	double *cp_n = vector(m);
	double *Y = vector(m);
	double *X = vector(m);
	double *DY = vector(m);
	
	int i,j;
	
	//Print_Vect(c,m);
	//Print_Matr(A,n,m);
	//Print_Matr(D,m,m);
	//Print_Vect(x,m);
	
	//printf("\n");
	for (i=0;i<m;i++){
		C[0][i] = c[i];
	}
	
	cD = MultM_M(C,1,m,D,m,m);
	//Print_Matr(cD,1,m);
	AD = MultM_M(A,n,m,D,m,m);
	//Print_Matr(AD,n,m);
	
	for (i=0;i<(n+1);i++)
		for (j=0;j<m;j++){
			if (i == n) P[i][j] = 1;
			else P[i][j] = AD[i][j];	//Расширенная матрица с дополнительной единичной строкой
		}
		
	for (i=0;i<m;i++)
		for (j=0;j<(n+1);j++)
			PT[i][j] = P[j][i];	//
	//Print_Matr(P,n+1,m);
	//Print_Matr(PT,m,n+1);
	
	P_PT = MultM_M(P,n+1,m,PT,m,n+1);	//
	P_PT_1 = inverseMatrix(P_PT,n+1);		//
	//Print_Matr(P_PT,n+1,n+1);
	//Print_Matr(P_PT_1,n+1,n+1);
	
	P1 = MultM_M(P_PT_1,n+1,n+1,P,n+1,m);	//
	P2 = MultM_M(PT,m,n+1,P1,n+1,m);		//
	//Print_Matr(P2,m,m);
	
	for (i=0;i<m;i++)
		for (j=0;j<m;j++){
			I[i][j] = (i == j)? 1: 0;	//Единичная матрица
			I_P[i][j] = I[i][j]-P2[i][j];	//Проекционная матрица сохраняет выполнение равенства Ax = 0
		}									//поправляет значения переменных, чтобы точка не выходила на границы допустимой области
	//Print_Matr(I,m,m);
	//Print_Matr(I_P,m,m);
	
	for (i=0;i<m;i++)
		cDT[i][0] = cD[0][i];
	//Print_Matr(cDT,m,1);
	
	cp = MultM_M(I_P,m,m,cDT,m,1);	//Проекция градиента, в направлении которого 
	//Print_Matr(cp,m,1);			//определяется новая точка решения
	for (i=0;i<m;i++)
		cp_n[i] = cp[i][0];	//
	//Print_Vect(cp_n,m);
	
	double Norm = NormVect(cp_n,m);
	//printf("%f",Norm);
	
	double *x0 = vector(m);
	double sumDY = 0;
	for (i=0;i<m;i++){
		x0[i] = 1.0/m;
		Y[i] = x0[i] - r*alf*(1/Norm)*cp_n[i];	//
		DY[i] = D[i][i]*Y[i];
		sumDY += DY[i];	
	}
	
	//Print_Vect(DY,m);
	//printf("%f",sumDY);

	for (i=0;i<m;i++)
		X[i] = (DY[i])/sumDY;
	
	/*Освобождение памяти*/
	free_matr(C,1);
	free_matr(AD,n);
	free_matr(P,n+1);
	free_matr(PT,m);
	free_matr(P_PT,n+1);
	free_matr(P_PT_1,n+1);
	free_matr(P1,n+1);
	free_matr(P2,m);
	free_matr(I,m);
	free_matr(I_P,m);
	free_matr(cD,1);
	free_matr(cDT,m);
	free_matr(cp,m);
	free(cp_n);
	free(Y);
	free(DY);
	
	
	return X;
	
}

double *Karmarkar_metod(double *c, double **A, int n, int m, int maxiter, double eps, int *k){
	
	int i, j;
	double r, z0 = 0, z = 0, alf;
	double *x0 = vector(m);
	double *X = vector(m);
	double **D = matrix(m,m);
	double *xx = vector(m);
			
	//printf("%d ",*k);
	
	while (*k <= maxiter){
		
		//printf("%d ",*k);
		
		if (*k == 0){
			//printf("\n*Вектора текущего приближения переменных на каждой итерации*\n\nИтерация %d\n",*k);
			for (i=0;i<m;i++)
				for (j=0;j<m;j++){
					x0[j] = 1.0/m;		//Начальная точка - центр симплекса x = (1/m,1/m,...,1/m)
					D[i][j] = (i==j)? x0[j] : 0;	//Диагональная матрица масштабирования
				}
			//printf("Матрица D:");
			//Print_Matr(D,m,m);
	
			r = 1.0/sqrt(m*(m-1));	//Радиус сферы, вписанной в симплекс
			alf = (m-1)/(3.0*m);	//Параметр выбирается для сходимости решения
			for (j=0;j<m;j++)
				z0 += c[j]*x0[j];	//начальное значение целевой функции
			z = 1.1*z0;
			//printf("%f, %f, %f, %f",r,alf,z, z0);
			
			for (i=0;i<m;i++)
				X[i] = 0;
			
			*k += 1;
		}
		else{
			
			for (i=0;i<m;i++)
				xx[i] = X[i]-x0[i];
						
			if ((fabs(z-z0)>eps) || (NormVect(xx,m)>eps)){
				//printf("\n\nИтерация [%d]:",*k);
				z0 = z;
				for (i=0;i<m;i++)
					x0[i] = X[i];
				

				X = proj_tranform(c,A,D,x0,n,m,r,alf);
				//Print_Vect(X,m);
				for (i=0;i<m;i++)
					for (j=0;j<m;j++){
						D[i][j] = (i==j)? X[j] : 0;
					}
				//printf("Матрица D:");
				//Print_Matr(D,m,m);
				z = 0;
				for (j=0;j<m;j++)
					z += c[j]*X[j];	//Новое значение функции на текущем шаге
				//printf("\nz(x) = %f",z);
				
				*k += 1;
			}
			else break;
		}	
	}
	
		
	
	return X;
}

void Karmarkar(char *c_path, char *A_path, char *b_path, char *Aeq_path, char *beq_path, int maxiter, double eps, int *k, double U){
	
	int i,j;
	int ni,mi;
	int ne,me;
	int n_new,m_new;
	int f = 0;	//флаг результата работы программы
	
	if ((empty_file(A_path)) && (empty_file(Aeq_path))){
		printf("Файлы %s и %s пустые!\n", A_path, Aeq_path);
		f = -1; // Конец программы
		
	}
	else{
		if (!(empty_file(A_path)) && (empty_file(Aeq_path))){
			if (!(empty_file(b_path))){
				printf("Дана следующая задача линейного программирования:");
				ne = 0;
				me = 0;

				ReadSizeMatr(A_path,&ni,&mi);
				double **A = matrix(ni,mi);
				double *b = vector(ni);
				
				A = ReadMatrix(A_path,ni,mi);
				b = ReadVector(b_path,ni);
				
				printf("\n\n*Ограничения-неравенства:*");
				Print_Matr(A,ni,mi);
				printf("\n*Вектор правых частей:*");
				Print_Vect(b,ni);
				
				n_new = ni + ne;
				m_new = ni + mi + 2;
				
				double **A_new = matrix(n_new,m_new);
				double *b_new = vector(n_new);
				double *c = vector(max(mi,me));
				double *c_new = vector(m_new);
				for (i=0;i<n_new;i++){
					for (j=0;j<m_new;j++){
						if ((i<ni)&&(j<mi))
							A_new[i][j] = A[i][j];
						if ((i<ni)&&(mi<=j)&&(j<(m_new-1)))
							A_new[i][j] = (i == (j-mi))? 1.0 : 0.0;
					}
					if (i<ni)
						b_new[i] = b[i];
				}

				int sign = 0;
				c = C(c_path,max(mi,me),&sign);
				
				for (j=0;j<m_new;j++)
					c_new[j] = (j <max(mi,me))? c[j] : 0;
				Print_Matr(A_new,n_new,m_new);
				Print_Vect(c_new,m_new);
				
				double max_s = -1e10;
				double *s = vector(n_new);
				for (i=0;i<n_new;i++){
					s[i] = 0;
					for (j=0;j<(m_new-2);j++)
						s[i] += A_new[i][j];
					if (s[i]>=max_s)
						max_s = s[i];
				}
				
				double U = abs(max_s)+1;
				printf("\n%f\n",U);
				for (i=0;i<n_new;i++){
					for (j=0;j<(m_new-1);j++){
						A_new[i][j] = U*A_new[i][j]-b_new[i];
					}
				}
				for (j=0;j<m_new;j++)
					c_new[j] = U*c_new[j];
				Print_Matr(A_new,n_new,m_new);
				Print_Vect(c_new,m_new);
				
				double *sum_i = vector(n_new);
				for (i=0;i<n_new;i++){
					sum_i[i] = 0;
					for (j=0;j<(m_new-1);j++)
						sum_i[i] += A_new[i][j];
					A_new[i][m_new-1] = - sum_i[i];
				}
				double sum_j = 0;
				for (j=0;j<(m_new-1);j++)
					sum_j += c_new[j];
				c_new[m_new-1] = -(sum_j+20);
				
				
				Print_Matr(A_new,n_new,m_new);
				Print_Vect(c_new,m_new);
				
				double *y_opt = vector(m_new);
				y_opt = Karmarkar_metod(c_new,A_new,n_new,m_new,maxiter,eps,k);
				Print_Vect(y_opt,m_new);
				
				double *x_opt = vector(max(mi,me));
				double z_opt = 0;
				for (i=0;i<max(mi,me);i++){
					x_opt[i] = U*y_opt[i];
					z_opt += x_opt[i];
				}
				
				printf("\n\nНайденное оптимальное решение: x  =");
				Print_Vect(x_opt,max(mi,me));
				printf("\nОртимальное значение целевой функции: z  =%f",z_opt);
				
				f = 2;
			}
			else{
				printf("Файл %s пуст!\n", b_path);
				f = -2;
			}
		}
		if ((empty_file(A_path)) && !(empty_file(Aeq_path))){
			if (!(empty_file(beq_path))){
				printf("Дана следующая задача линейного программирования:\n");
				ni = 0;
				mi = 0;
				
				ReadSizeMatr(Aeq_path,&ne,&me);
				double **Aeq = matrix(ne,me);
				double *beq = vector(ni);
				
				Aeq = ReadMatrix(Aeq_path,ne,me);
				beq = ReadVector(beq_path,ne);
				
				printf("\n\n*Ограничения-равенства:*");
				Print_Matr(Aeq,ne,me);
				printf("\n*Вектор правых частей:*");
				Print_Vect(beq,ne);
				
				f = 3;
			}
			else{
				printf("Файл %s пуст!\n", beq_path);
				f = -3;
			}
		}
		
	}
	
	if (f == 0){
		printf("Дана следующая задача линейного программирования:\n");
		ReadSizeMatr(A_path,&ni,&mi);
		ReadSizeMatr(Aeq_path,&ne,&me);
		double **A = matrix(ni,mi);
		double *b = vector(ni);
		double **Aeq = matrix(ne,me);
		double *beq = vector(ni);
		
		A = ReadMatrix(A_path,ni,mi);
		b = ReadVector(b_path,ni);
		Aeq = ReadMatrix(Aeq_path,ne,me);
		beq = ReadVector(beq_path,ne);
		
		printf("\n\n*Ограничения-неравенства:*");
		Print_Matr(A,ni,mi);
		printf("\n*Вектор правых частей:*");
		Print_Vect(b,ni);
		printf("\n\n*Ограничения-равенства:*");
		Print_Matr(Aeq,ne,me);
		printf("\n*Вектор правых частей:*");
		Print_Vect(beq,ne);
		
		n_new = ni + ne;
		m_new = ni + mi + 2;
		//printf("\n Новая размерность A_new: %d,%d\n",n_new,m_new);
		
		double **A_new = matrix(n_new,m_new);
		double *b_new = vector(n_new);
		double *c = vector(max(mi,me));
		double *c_new = vector(m_new);
		for (i=0;i<n_new;i++){
			for (j=0;j<m_new;j++){
				if ((i<ni)&&(j<mi))
					A_new[i][j] = A[i][j];
				if ((i<ni)&&(mi<=j)&&(j<(m_new-1)))
					A_new[i][j] = (i == (j-mi))? 1.0 : 0.0;
				if ((ni<=i)&&(j<mi))
					A_new[i][j] = Aeq[i-ni][j];
			}
			if (i<ni)
				b_new[i] = b[i];
			if ((ni<=i)&&(i<n_new))
				b_new[i] = beq[i-ni];
		}
		int sign = 0;
		c = C(c_path,max(mi,me),&sign);
		
		for (j=0;j<m_new;j++)
			c_new[j] = (j <max(mi,me))? c[j] : 0;
		//Print_Matr(A_new,n_new,m_new);
		//Print_Vect(c_new,m_new);
		
		double max_s = -1e10;
		double *s = vector(n_new);
		for (i=0;i<n_new;i++){
			s[i] = 0;
			for (j=0;j<(m_new-2);j++)
				s[i] += A_new[i][j];
			if (b_new[i]>=max_s)
				max_s = b_new[i];
		}
		
		
		for (i=0;i<n_new;i++){
			for (j=0;j<(m_new-1);j++){
				A_new[i][j] = U*A_new[i][j]-b_new[i];
			}
		}
		for (j=0;j<m_new;j++)
			c_new[j] = U*c_new[j];
		//Print_Matr(A_new,n_new,m_new);
		//Print_Vect(c_new,m_new);
		
		double *sum_i = vector(n_new);
		for (i=0;i<n_new;i++){
			sum_i[i] = 0;
			for (j=0;j<(m_new-1);j++)
				sum_i[i] += A_new[i][j];
			A_new[i][m_new-1] = - sum_i[i];
		}
		double sum_j = 0;
		for (j=0;j<(m_new-1);j++)
			sum_j += c_new[j];
		c_new[m_new-1] = -(sum_j);
		
		
		//Print_Matr(A_new,n_new,m_new);
		Print_Vect(c_new,m_new);
		
		double *y_opt = vector(m_new);
		y_opt = Karmarkar_metod(c_new,A_new,n_new,m_new,maxiter,eps,k);
		//Print_Vect(y_opt,m_new);
		
		double *x_opt = vector(max(mi,me));
		double z_opt = 0;
		for (i=0;i<max(mi,me);i++){
			x_opt[i] = U*y_opt[i];
			z_opt += c[i]*x_opt[i];
		}
		if (sign == 1)
			z_opt = - z_opt;
		
		printf("\n\nНайденное оптимальное решение: x  =");
		Print_Vect(x_opt,max(mi,me));
		printf("\nОртимальное значение целевой функции: z = %f",z_opt);
	}

}


int main() {
	
	SetConsoleOutputCP(CP_UTF8);	//установление кодировки
	/*
	 Метод Кармаркара решает ЗЛП в виде:
	 z = c^t*x ->min 
	  |Ax=0
	  |sum(xi)=1
	  |xi>=0
	   
	  Если ЗЛП задана не в специальной форме, то функция Karmarkar приводит заданную
	  систему ограничений к специальной форме и находит решение задачи оптимизации
	 */
	int maxiter = 1000;
	double eps = 1e-6;
	int k = 0;
	double U = 5;
	
    /* Пути к файлам со значениями для задачи */
	char c_path[] = "c.txt";	
	char A_path[] = "A.txt";
	char b_path[] = "b.txt";
	char Aeq_path[] = "Aeq.txt";
	char beq_path[] = "beq.txt";
	//char lb_path[] = "lb.txt";	нижняя граница значения переменных
	//char ub_path[] = "ub.txt";	верхняя граница значения переменных
    
    Karmarkar(c_path,A_path,b_path,Aeq_path,beq_path,maxiter,eps,&k,U);

    getchar();	//ожидание нажатия клавиши
     
    return 0;
}
