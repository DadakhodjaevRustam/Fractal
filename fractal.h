#ifndef _FRACTAL_H_
#define _FRACTAL_H_

#include "image.h"

/**
 * @brief Рисует пустой фрактал (заглушку), предполагая, что изображение чистое
 *
 * @param picture Указатель на изображение
 */
void empty_fractal(image_p picture);

/**
 * @brief Рисует фрактал множества Мандельброта
 *
 * @param picture Изображение для рисования
 * @param x_min Минимальная координата X в комплексной плоскости
 * @param x_max Максимальная координата X в комплексной плоскости
 * @param y_min Минимальная координата Y в комплексной плоскости
 * @param y_max Максимальная координата Y в комплексной плоскости
 * @param max_iter Максимальное количество итераций для проверки сходимости
 */
void mandelbrot_fractal(image_p picture, double x_min, double x_max,
			double y_min, double y_max, int max_iter);

/**
 * @brief Рисует фрактал множества Жюлиа
 *
 * @param picture Изображение для рисования
 * @param c_real Действительная часть комплексной константы
 * @param c_imag Мнимая часть комплексной константы
 * @param x_min Минимальная координата X в комплексной плоскости
 * @param x_max Максимальная координата X в комплексной плоскости
 * @param y_min Минимальная координата Y в комплексной плоскости
 * @param y_max Максимальная координата Y в комплексной плоскости
 * @param max_iter Максимальное количество итераций для проверки сходимости
 */
void julia_fractal(image_p picture, double c_real, double c_imag,
		   double x_min, double x_max, double y_min, double y_max,
		   int max_iter);

/**
 * @brief Рисует фрактал треугольника Серпинского
 *
 * @param picture Изображение для рисования
 * @param x X-координата верхней вершины
 * @param y Y-координата верхней вершины
 * @param size Размер треугольника
 * @param depth Глубина рекурсии
 */
void sierpinski_triangle(image_p picture, int x, int y, int size, int depth);

/**
 * @brief Рисует простой древовидный фрактал
 *
 * @param picture Изображение для рисования
 * @param x Начальная X-координата
 * @param y Начальная Y-координата
 * @param angle Угол в градусах
 * @param length Длина ветви
 * @param depth Глубина рекурсии
 */
void tree_fractal(image_p picture, int x, int y, double angle,
		  double length, int depth);

#endif // _FRACTAL_H_
