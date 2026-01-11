#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "image.h"
/**
 * @brief Структура для хранения данных изображения и метаданных
 */
struct image
{
    pixel_coord width, height;  // Ширина и высота изображения
    pixel_data *data;           // Массив пикселей (ширина * высота)
};

// Создает новое изображение с заданными размерами
image_p create_image(pixel_coord width, pixel_coord height)
{
    assert(width > 0 && height > 0);
    assert(width <= 65536 && height <= 65536); /* Разумные максимальные размеры */
    
    // Выделяем память под структуру изображения
    image_t *v = malloc(sizeof(image_t));
    assert(v != NULL);
    
    // Инициализируем поля структуры
    v->width = width;
    v->height = height;
    // Выделяем память под данные пикселей
    v->data = malloc(sizeof(pixel_data) * width * height);
    assert(v->data != NULL);

    return v;
}

// Очищает изображение (заполняет нулями/черным цветом)
void clear_image(image_p picture)
{
    assert(picture != NULL);
    assert(picture->data != NULL);
    // Заполняем весь массив данных нулями
    memset(picture->data, 0, sizeof(pixel_data) * picture->width * picture->height);
}

// Заполняет изображение случайными значениями
void fill_random(image_p picture)
{
    assert(picture != NULL);
    assert(picture->data != NULL);
    
    pixel_data *p = picture->data;
    // Заполняем каждый пиксель случайным значением
    for (pixel_coord z = 0; z < picture->height * picture->width; ++z, ++p)
        *p = (pixel_data)rand();
}

// Освобождает память, выделенную под изображение
void free_image(image_p picture)
{
    if (picture) {
        free(picture->data);   // Освобождаем данные пикселей
        free(picture);         // Освобождаем саму структуру
    }
}

// Сохраняет изображение в формате PGM (Portable Graymap)
int save_pgm(image_p picture, const char *filename)
{
    assert(picture != NULL);
    assert(filename != NULL);
    
    // Открываем файл для записи
    FILE *to = fopen(filename, "w");
    if (!to)
        return -1;
    
    // Записываем заголовок PGM
    fprintf(to, "P2\n%u %u\n255\n", picture->width, picture->height);

    pixel_data *p = picture->data;
    // Записываем данные пикселей
    for (pixel_coord y = 0; y < picture->height; ++y) {
        for (pixel_coord x = 0; x < picture->width; ++x) {
            fprintf(to, "%u%c", *(p++), x == picture->width - 1 ? '\n' : ' ');
        }
    }

    fclose(to);
    return 0;
}

// Устанавливает цвет пикселя в заданной позиции
void set_pixel(image_p picture, pixel_coord x, pixel_coord y, pixel_data color)
{
    assert(("Координаты вне изображения", x >= 0 && y >= 0 && x < picture->width && y < picture->height));
    picture->data[picture->width * y + x] = color;
}

// Получает цвет пикселя из заданной позиции
pixel_data get_pixel(image_p picture, pixel_coord x, pixel_coord y)
{
    assert(("Координаты вне изображения", x >= 0 && y >= 0 && x < picture->width && y < picture->height));
    return picture->data[picture->width * y + x];
}

// Возвращает ширину изображения
pixel_coord get_image_width(image_p picture)
{
    assert(picture != NULL);
    return picture->width;
}

// Возвращает высоту изображения
pixel_coord get_image_height(image_p picture)
{
    assert(picture != NULL);
    return picture->height;
}

// Сохраняет изображение в формате BMP
int save_bmp(image_p picture, const char *filename)
{
    assert(picture != NULL);
    assert(filename != NULL);
    
    // Открываем файл для записи в бинарном режиме
    FILE *f = fopen(filename, "wb");
    if (!f)
        return -1;
    
    /* BMP требует выравнивания строк по границе 4 байт */
    pixel_coord row_padded = (picture->width + 3) & (~3);
    uint32_t image_size = row_padded * picture->height;
    uint32_t file_size = 54 + image_size; /* 54 = размер заголовка */
    
    /* Заголовок BMP (14 байт) */
    uint8_t bmp_header[14] = {
        'B', 'M',                    /* Магическое число */
        file_size & 0xFF,            /* Размер файла */
        (file_size >> 8) & 0xFF,
        (file_size >> 16) & 0xFF,
        (file_size >> 24) & 0xFF,
        0, 0, 0, 0,                  /* Зарезервировано */
        54, 0, 0, 0                  /* Смещение до данных пикселей */
    };
    
    /* Заголовок DIB (40 байт) */
    uint8_t dib_header[40] = {
        40, 0, 0, 0,                            /* Размер заголовка DIB */
        picture->width & 0xFF,                  /* Ширина */
        (picture->width >> 8) & 0xFF,
        (picture->width >> 16) & 0xFF,
        (picture->width >> 24) & 0xFF,
        picture->height & 0xFF,                 /* Высота */
        (picture->height >> 8) & 0xFF,
        (picture->height >> 16) & 0xFF,
        (picture->height >> 24) & 0xFF,
        1, 0,                                   /* Количество цветовых плоскостей */
        8, 0,                                   /* Бит на пиксель */
        0, 0, 0, 0,                            /* Без сжатия */
        image_size & 0xFF,                      /* Размер изображения */
        (image_size >> 8) & 0xFF,
        (image_size >> 16) & 0xFF,
        (image_size >> 24) & 0xFF,
        0x13, 0x0B, 0, 0,                      /* Разрешение печати */
        0x13, 0x0B, 0, 0,
        0, 0, 0, 0,                            /* Количество цветов в палитре */
        0, 0, 0, 0                             /* Важные цвета */
    };
    
    // Записываем заголовки
    fwrite(bmp_header, 1, 14, f);
    fwrite(dib_header, 1, 40, f);
    
    /* Записываем палитру оттенков серого (256 цветов) */
    for (int i = 0; i < 256; i++) {
        uint8_t color[4] = {i, i, i, 0};  // R=G=B=i, альфа=0
        fwrite(color, 1, 4, f);
    }
    
    /* Записываем данные пикселей (BMP хранится снизу вверх) */
    uint8_t padding[3] = {0, 0, 0};
    pixel_coord pad_size = row_padded - picture->width;
    
    // Проходим по строкам снизу вверх
    for (int y = picture->height - 1; y >= 0; y--) {
        // Записываем строку пикселей
        fwrite(&picture->data[y * picture->width], 1, picture->width, f);
        // Добавляем выравнивание, если нужно
        if (pad_size > 0)
            fwrite(padding, 1, pad_size, f);
    }
    
    fclose(f);
    return 0;
}
