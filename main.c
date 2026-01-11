#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image.h"
#include "fractal.h"

int main(void)
{
	printf("Генератор фракталов - создание нескольких фрактальных изображений...\n");
	
	/* Инициализация генератора случайных чисел */
	srand((unsigned int)time(NULL));
	
	/* 1. Множество Мандельброта */
	printf("Генерация множества Мандельброта...\n");
	image_p mandelbrot = create_image(800, 600);  // Создаем изображение 800x600
	clear_image(mandelbrot);                      // Очищаем (черный фон)
	// Генерируем фрактал: область [-2.5,1.0]x[-1.0,1.0], 256 итераций
	mandelbrot_fractal(mandelbrot, -2.5, 1.0, -1.0, 1.0, 256);
	save_bmp(mandelbrot, "mandelbrot.bmp");      // Сохраняем в BMP
	save_pgm(mandelbrot, "mandelbrot.pgm");      // Сохраняем в PGM
	free_image(mandelbrot);                       // Освобождаем память
	printf("  Сохранено: mandelbrot.bmp и mandelbrot.pgm\n");
	
	/* 2. Множество Жюлиа */
	printf("Генерация множества Жюлиа...\n");
	image_p julia = create_image(800, 600);      // Создаем изображение 800x600
	clear_image(julia);                          // Очищаем (черный фон)
	// Генерируем фрактал Жюлиа с константой c = (-0.7, 0.27015i)
	// Область [-1.5,1.5]x[-1.0,1.0], 256 итераций
	julia_fractal(julia, -0.7, 0.27015, -1.5, 1.5, -1.0, 1.0, 256);
	save_bmp(julia, "julia.bmp");                // Сохраняем в BMP
	save_pgm(julia, "julia.pgm");                // Сохраняем в PGM
	free_image(julia);                           // Освобождаем память
	printf("  Сохранено: julia.bmp и julia.pgm\n");
	
	/* 3. Треугольник Серпинского */
	printf("Генерация треугольника Серпинского...\n");
	image_p sierpinski = create_image(800, 700);  // Создаем изображение 800x700
	clear_image(sierpinski);                     // Очищаем (черный фон)
	// Рисуем треугольник Серпинского: верхняя вершина (400,50), размер 600, глубина 7
	sierpinski_triangle(sierpinski, 400, 50, 600, 7);
	save_bmp(sierpinski, "sierpinski.bmp");      // Сохраняем в BMP
	save_pgm(sierpinski, "sierpinski.pgm");      // Сохраняем в PGM
	free_image(sierpinski);                      // Освобождаем память
	printf("  Сохранено: sierpinski.bmp и sierpinski.pgm\n");
	
	/* 4. Древовидный фрактал */
	printf("Генерация древовидного фрактала...\n");
	image_p tree = create_image(800, 800);       // Создаем квадратное изображение 800x800
	clear_image(tree);                           // Очищаем (черный фон)
	// Рисуем фрактальное дерево: начало (400,750), угол 0°, длина 150, глубина 10
	tree_fractal(tree, 400, 750, 0.0, 150.0, 10);
	save_bmp(tree, "tree.bmp");                  // Сохраняем в BMP
	save_pgm(tree, "tree.pgm");                  // Сохраняем в PGM
	free_image(tree);                            // Освобождаем память
	printf("  Сохранено: tree.bmp и tree.pgm\n");
	
	printf("\nВсе фракталы успешно сгенерированы!\n");
	return 0;
}
