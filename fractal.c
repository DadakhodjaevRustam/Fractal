#include <assert.h>
#include <math.h>
#include <stdlib.h> 
#include "image.h"
#include "fractal.h"


void empty_fractal(image_p picture)
{
	assert(picture != NULL);
	/* Пустая функция-заглушка для фрактала */
  (void)picture;
}

/**
 * @brief Вспомогательная функция для рисования линии алгоритмом Брезенхэма
 */
static void draw_line(image_p picture, int x0, int y0, int x1, int y1,
		      pixel_data color)
{
	assert(picture != NULL);
	
	// Разницы координат
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	// Направления движения
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	// Ошибка
	int err = dx - dy;
	
	// Основной цикл алгоритма Брезенхэма
	while (1) {
		// Устанавливаем пиксель, если он в пределах изображения
		if (x0 >= 0 && x0 < (int)get_image_width(picture) &&
		    y0 >= 0 && y0 < (int)get_image_height(picture))
			set_pixel(picture, x0, y0, color);
		
		// Если достигли конечной точки, выходим
		if (x0 == x1 && y0 == y1)
			break;
		
		// Корректировка ошибки и координат
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

/**
 * @brief Вспомогательная функция для рисования закрашенного треугольника
 */
static void draw_filled_triangle(image_p picture, int x1, int y1, int x2,
				 int y2, int x3, int y3, pixel_data color)
{
	assert(picture != NULL);
	
	// Рисуем контур треугольника
	draw_line(picture, x1, y1, x2, y2, color);
	draw_line(picture, x2, y2, x3, y3, color);
	draw_line(picture, x3, y3, x1, y1, color);
	
	/* Простая заливка - метод сканирующих строк */
	// Находим минимальную и максимальную Y-координаты
	int min_y = y1;
	int max_y = y1;
	
	if (y2 < min_y)
		min_y = y2;
	if (y3 < min_y)
		min_y = y3;
	if (y2 > max_y)
		max_y = y2;
	if (y3 > max_y)
		max_y = y3;
	
	// Сканируем каждую строку между min_y и max_y
	for (int y = min_y; y <= max_y; y++) {
		// Пропускаем строки вне изображения
		if (y < 0 || y >= (int)get_image_height(picture))
			continue;
		
		// Проверяем каждый пиксель в строке
		for (int x = 0; x < (int)get_image_width(picture); x++) {
			/* Проверка принадлежности точки треугольнику 
			   с использованием барицентрических координат */
			// Вычисляем векторные произведения
			int d1 = (x - x2) * (y1 - y2) - (x1 - x2) * (y - y2);
			int d2 = (x - x3) * (y2 - y3) - (x2 - x3) * (y - y3);
			int d3 = (x - x1) * (y3 - y1) - (x3 - x1) * (y - y1);
			
			// Проверяем знаки векторных произведений
			int has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
			int has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
			
			// Если все три имеют одинаковый знак, точка внутри треугольника
			if (!(has_neg && has_pos))
				set_pixel(picture, x, y, color);
		}
	}
}

void mandelbrot_fractal(image_p picture, double x_min, double x_max,
			double y_min, double y_max, int max_iter)
{
	assert(picture != NULL);
	assert(max_iter > 0);
	assert(x_max > x_min);
	assert(y_max > y_min);
	
	// Получаем размеры изображения
	pixel_coord width = get_image_width(picture);
	pixel_coord height = get_image_height(picture);
	
	// Обрабатываем каждый пиксель изображения
	for (pixel_coord py = 0; py < height; py++) {
		for (pixel_coord px = 0; px < width; px++) {
			// Преобразуем координаты пикселя в координаты комплексной плоскости
			double x0 = x_min + (x_max - x_min) * px / width;
			double y0 = y_min + (y_max - y_min) * py / height;
			
			// Итерации для проверки принадлежности множеству Мандельброта
			double x = 0.0;
			double y = 0.0;
			int iteration = 0;
			
			// Критерий остановки: |z| > 2 или достигнут max_iter
			while (x * x + y * y <= 4.0 && iteration < max_iter) {
				double xtemp = x * x - y * y + x0;
				y = 2.0 * x * y + y0;
				x = xtemp;
				iteration++;
			}
			
			/* Сопоставляем количество итераций с оттенком серого */
			// Если точка принадлежит множеству - черный цвет, иначе градации серого
			pixel_data color = (iteration == max_iter) ? 0 :
				(pixel_data)(255 * iteration / max_iter);
			set_pixel(picture, px, py, color);
		}
	}
}

void julia_fractal(image_p picture, double c_real, double c_imag,
		   double x_min, double x_max, double y_min, double y_max,
		   int max_iter)
{
	assert(picture != NULL);
	assert(max_iter > 0);
	assert(x_max > x_min);
	assert(y_max > y_min);
	
	// Получаем размеры изображения
	pixel_coord width = get_image_width(picture);
	pixel_coord height = get_image_height(picture);
	
	// Обрабатываем каждый пиксель изображения
	for (pixel_coord py = 0; py < height; py++) {
		for (pixel_coord px = 0; px < width; px++) {
			// Преобразуем координаты пикселя в координаты комплексной плоскости
			double x = x_min + (x_max - x_min) * px / width;
			double y = y_min + (y_max - y_min) * py / height;
			
			int iteration = 0;
			
			// Итерации для проверки принадлежности множеству Жюлиа
			// Используем фиксированную константу c вместо начальной точки
			while (x * x + y * y <= 4.0 && iteration < max_iter) {
				double xtemp = x * x - y * y + c_real;
				y = 2.0 * x * y + c_imag;
				x = xtemp;
				iteration++;
			}
			
			/* Сопоставляем количество итераций с оттенком серого */
			// Если точка принадлежит множеству - черный цвет, иначе градации серого
			pixel_data color = (iteration == max_iter) ? 0 :
				(pixel_data)(255 * iteration / max_iter);
			set_pixel(picture, px, py, color);
		}
	}
}

void sierpinski_triangle(image_p picture, int x, int y, int size, int depth)
{
	assert(picture != NULL);
	assert(size > 0);
	assert(depth >= 0);
	
	// Базовый случай рекурсии
	if (depth == 0) {
		/* Рисуем закрашенный треугольник */
		// Вершины равностороннего треугольника
		int x1 = x;              // Верхняя вершина
		int y1 = y;
		int x2 = x - size / 2;   // Левая нижняя вершина
		int y2 = y + size;
		int x3 = x + size / 2;   // Правая нижняя вершина
		int y3 = y + size;
		
		draw_filled_triangle(picture, x1, y1, x2, y2, x3, y3, 255);
	} else {
		/* Рекурсивно рисуем три меньших треугольника */
		int new_size = size / 2;
		
		/* Верхний треугольник */
		sierpinski_triangle(picture, x, y, new_size, depth - 1);
		
		/* Левый нижний треугольник */
		sierpinski_triangle(picture, x - new_size / 2, y + new_size,
				    new_size, depth - 1);
		
		/* Правый нижний треугольник */
		sierpinski_triangle(picture, x + new_size / 2, y + new_size,
				    new_size, depth - 1);
	}
}

void tree_fractal(image_p picture, int x, int y, double angle,
		  double length, int depth)
{
	assert(picture != NULL);
	assert(depth >= 0);
	
	// Условие остановки рекурсии
	if (depth == 0 || length < 2.0)
		return;
	
	// Преобразуем угол из градусов в радианы
	double angle_rad = angle * 3.14 / 180.0;

	/* Вычисляем конечную точку ветки */
	int x2 = x + (int)(length * sin(angle_rad));
	int y2 = y - (int)(length * cos(angle_rad));
	
	/* Рисуем ветку */
	// Цвет ветки зависит от глубины рекурсии (чем глубже, тем темнее)
	pixel_data color = (pixel_data)(255 - depth * 20);
	draw_line(picture, x, y, x2, y2, color);
	
	/* Рекурсивно рисуем левую и правую ветки */
	// Левая ветка: угол уменьшен на 25 градусов
	tree_fractal(picture, x2, y2, angle - 25.0, length * 0.7, depth - 1);
	// Правая ветка: угол увеличен на 25 градусов
	tree_fractal(picture, x2, y2, angle + 25.0, length * 0.7, depth - 1);
}
