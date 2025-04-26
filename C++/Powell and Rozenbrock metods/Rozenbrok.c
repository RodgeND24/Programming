#include <stdio.h>
#include <math.h>
#include <windows.h>

#define N 2       // ТУТ ИЗМЕНЯЕТСЯ КОЛИЧЕСТВО ПЕРЕМЕННЫХ
#define MAX_ITER 1000
#define EPSILON 1e-6

// ФУНКЦИИ ДЛЯ ТЕСТА
double objective_function(double x[N]) {
	
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

// Ортогонализация Грама-Шмидта для направлений
void gram_schmidt(double directions[N][N]) {
	double S[N][N];
    for (int i = 0; i < N; i++) {
		
		for (int k = 0; k < N; k++) {
            S[i][k] = directions[i][k];
        }
        for (int j = 0; j < i; j++) {
			
			double dot = 0;  // Скалярное произведение
            double norm = 0; // Норма вектора
            
            // Вычисляем скалярное произведение и норму
            for (int k = 0; k < N; k++) {
                dot += S[j][k] * directions[i][k];
                norm += S[j][k] * S[j][k];
            }
            // Вычитаем проекцию
            for (int k = 0; k < N; k++) {
                S[i][k] -= (dot / norm) * S[j][k];
            }
        }
        // Нормализация направления
        double norm = 0.0;
        for (int k = 0; k < N; k++) {
            norm += S[i][k] * S[i][k];
        }
        norm = sqrt(norm);
        if (norm > EPSILON) {
            for (int k = 0; k < N; k++) {
                S[i][k] /= norm;
            }
        }
    }
    for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			directions[i][j] = S[i][j];
}

// Метод Розенброка
void rosenbrock(double x_start[N]) {
    double x[N], new_x[N];
    double directions[N][N]; // Массив направлений
    double step[N];          // Шаги для каждого направления
    int iter = 0;
    double f_prev, f_current;

    // Инициализация
    for (int i = 0; i < N; i++) {
        x[i] = x_start[i];
        step[i] = 0.1; // Начальный шаг
        for (int j = 0; j < N; j++) {
            directions[i][j] = (i == j) ? 1.0 : 0.0; // Начальные направления
        }
    }

    f_prev = objective_function(x);

    while (iter < MAX_ITER) {
        // Проход по всем направлениям
        for (int i = 0; i < N; i++) {
            // Пробуем шаг в направлении i
            for (int j = 0; j < N; j++) {
                new_x[j] = x[j] + step[i] * directions[i][j];
            }
            f_current = objective_function(new_x);

            if (f_current < f_prev) { // Успешный шаг
                f_prev = f_current;
                for (int j = 0; j < N; j++) {
                    x[j] = new_x[j];
                }
                step[i] *= 3.0; // Увеличиваем шаг
            } else {            // Неудачный шаг
                step[i] *= -0.5; // Меняем направление и уменьшаем шаг
            }
        }

        // Проверка на сходимость (малое изменение шагов)
        double step_sum = 0.0;
        for (int i = 0; i < N; i++) {
            step_sum += fabs(step[i]);
        }
        if (step_sum < EPSILON) break;

        // Обновление направлений через ортогонализацию
        gram_schmidt(directions);
        iter += 1;
    }

    // Вывод результатов
    printf("Оптимальное решение найдено за %d итераций:\n", iter);
    for (int i = 0; i < N; i++) {
        printf("x[%d] = %.6f\n", i, x[i]);
    }
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
    double x_start[N] = {0, 0}; // ТУТ ИЗМЕНЯЕТСЯ НАЧАЛЬНАЯ ТОЧКА
    rosenbrock(x_start);
    return 0;
}
