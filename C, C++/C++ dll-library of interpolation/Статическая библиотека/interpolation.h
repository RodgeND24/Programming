/*Функция построения канонического полинома для интеролирования функции
t-точка, в которой нужно найти значение интерполирующей функции
(x,y)-массивы исходных данных функции, n-количество узловых точек
*s-указатель на переменную типа double (значение функции в точке t после интерполирования)*/
void kanon(double t,double *x,double *y,int n,double *s);  

/*Функция построения полинома Ньютона для интеролирования функции
t-точка, в которой нужно найти значение интерполирующей функции
(x,y)-массивы исходных данных функции, n-количество узловых точек
*s-указатель на переменную типа double (значение функции в точке t после интерполирования)*/
void newton(double t,double *x,double *y,int n,double *s);

/*Функция построения полинома Лагранжа для интеролирования функции
t-точка, в которой нужно найти значение интерполирующей функции
(x,y)-массивы исходных данных функции, n-количество узловых точек
*s-указатель на переменную типа double (значение функции в точке t после интерполирования)*/
void lagrange(double t,double *x,double *y,int n,double *s);

/*Функция построения линейного сплайна для интеролирования функции
t-точка, в которой нужно найти значение интерполирующей функции
(x,y)-массивы исходных данных функции, n-количество узловых точек
*s-указатель на переменную типа double (значение функции в точке t после интерполирования)*/
void lininterp(double t,double *x,double *y,int n,double *p);

/*Функция построения кубического сплайна для интеролирования функции
t-точка, в которой нужно найти значение интерполирующей функции
(x,y)-массивы исходных данных функции, n-количество узловых точек
*s-указатель на переменную типа double (значение функции в точке t после интерполирования)*/
void spline(double t,double *x,double *y,int n,double *s);
