#include <iostream>
#include <math.h>
#include <fstream> //подключение библиотеки для работы с файлами
#include "interpolation.h"
using namespace std;
int main()
{
	int i, n = 0;
	double* x, * y, s, t, a;

	fstream f1;
	fstream f2;
	f1.open("x.txt", fstream::in | fstream::out);  //открываем файл с массивом X
	f2.open("y.txt", fstream::in | fstream::out);  //открываем файл с массивом Y

	while (!f1.eof())  //узнаём количество узловых точек n
	{
		f1 >> a; n++;
	}

	x = new double[n];
	y = new double[n];

	f1.seekg(0);
	for (i = 0; i < n; i++)  //считываем массивы X и Y из файлов
	{
		f1 >> x[i]; f2 >> y[i];
	}

	cout << "Massive X:" << endl;
	for (i = 0, cout << "|"; i < n; i++)
		if (i == n - 1) cout << x[i];
		else cout << x[i] << "  ";
	cout << "|" << endl;

	cout << "Massive Y:" << endl;
	for (i = 0, cout << "|"; i < n; i++)
		if (i == n - 1) cout << y[i];
		else cout << y[i] << "  ";
	cout << "|" << endl;

	cout << "Enter point of interpolation:";  //Звпрос на введение точки интерполяции
	cin >> t; cout << endl;

	/*Вызываем функцию kanon(t,x,y,n,&s), где t-точка интерполяции
	  s-значение интерполированной функции в этой точке*/
	cout << endl << "Kanon interpolation:" << endl;
	kanon(t, x, y, n, &s);
	cout << s << endl;

	/*Вызываем функцию newton(t,x,y,n,&s), где t-точка интерполяции
	  s-значение интерполированной функции в этой точке*/
	cout << endl << "Newton interpolation:" << endl;
	newton(t, x, y, n, &s);
	cout << s << endl;

	/*Вызываем функцию lagrange(t,x,y,n,&s), где t-точка интерполяции
	  s-значение интерполированной функции в этой точке*/
	cout << endl << "Lagrange interpolation:" << endl;
	lagrange(t, x, y, n, &s);
	cout << s << endl;

	/*Вызываем функцию lininterp(t,x,y,n,&s), где t-точка интерполяции
	  s-значение интерполированной функции в этой точке*/
	cout << endl << "Linear interpolation:" << endl;
	lininterp(t, x, y, n, &s);
	cout << s << endl;

	/*Вызываем функцию spline(t,x,y,n,&s), где t-точка интерполяции
	  s-значение интерполированной функции в этой точке*/
	cout << endl << "Spline interpolation:" << endl;
	spline(t, x, y, n, &s);
	cout << s << endl;


	system("pause");
	return 0;
}
