#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Данные одного пикселя (на месте)
 * Тип для хранения значения одного пикселя (оттенок серого)
 */
typedef uint8_t pixel_data;

/**
 * @brief Координата пикселя и размерность изображения
 * Тип для хранения координат и размеров изображения
 */
typedef unsigned int pixel_coord;

// Предварительное объявление структуры изображения
struct image;

/**
 * @brief Изображение для приложения
 * Типы для работы с изображением: структура и указатель на нее
 */
typedef struct image image_t, *image_p;

/**
 * @brief Создает изображение заданной ширины и высоты
 * @param width,height Размеры изображения
 * @returns указатель на созданное изображение
 */
image_p create_image(pixel_coord width, pixel_coord height);

/**
 * @brief Заполняет изображение случайными значениями
 * @param picture Изображение для заполнения
 */
void fill_random(image_p picture);

/**
 * @brief Очищает изображение (заполняет черным цветом/нулями)
 * @param picture Изображение для очистки
 */
void clear_image(image_p picture);

/**
 * @brief Освобождает память, занятую изображением
 * @param picture Изображение для освобождения
 */
void free_image(image_p picture);

/**
 * @brief Сохраняет изображение в формате PGM
 * @param picture Изображение для сохранения
 * @param filename Имя выходного файла
 * @returns 0 при успехе, -1 при ошибке
 */
int save_pgm(image_p picture, const char *filename);

/**
 * @brief Устанавливает цвет пикселя изображения
 *
 * @param picture Изображение
 * @param x,y Координаты пикселя
 * @param color Значение цвета
 */
void set_pixel(image_p picture, pixel_coord x, pixel_coord y, pixel_data color);

/**
 * @brief Получает цвет пикселя изображения
 *
 * @param picture Изображение
 * @param x,y Координаты пикселя
 * @returns значение цвета пикселя
 */
pixel_data get_pixel(image_p picture, pixel_coord x, pixel_coord y);

/**
 * @brief Получает ширину изображения
 *
 * @param picture Изображение
 * @returns ширина в пикселях
 */
pixel_coord get_image_width(image_p picture);

/**
 * @brief Получает высоту изображения
 *
 * @param picture Изображение
 * @returns высота в пикселях
 */
pixel_coord get_image_height(image_p picture);

/**
 * @brief Сохраняет изображение в формате BMP
 *
 * @param picture Изображение для сохранения
 * @param filename Имя выходного файла
 * @returns 0 при успехе, -1 при ошибке
 */
int save_bmp(image_p picture, const char *filename);

#endif // _IMAGE_H_
