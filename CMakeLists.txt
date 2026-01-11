# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта и язык
project(FractalGenerator C)

# Установка стандарта языка C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Флаги компиляции
# Для Windows (MSVC) и других компиляторов
if(MSVC)
    # Для Microsoft Visual C++ 
    add_compile_options(/W4)  # Уровень предупреждений 4
else()
    # Для GCC, Clang и других Unix-подобных компиляторов
    add_compile_options(-Wall -Wextra -pedantic)  # Все предупреждения
endif()

# Поиск математической библиотеки (на Windows не нужна)
if(NOT MSVC)
    find_library(MATH_LIBRARY m)  # Библиотека libm
endif()

# Исходные файлы
set(IMAGE_SOURCES image.c image.h)      # Файлы для работы с изображениями
set(FRACTAL_SOURCES fractal.c fractal.h) # Файлы для фракталов

# Создание исполняемого файла
add_executable(fractal_generator 
    main.c               # Основной файл
    ${IMAGE_SOURCES}     # Исходники изображений
    ${FRACTAL_SOURCES}   # Исходники фракталов
)

# Подключение математической библиотеки (если найдена и не Windows)
if(NOT MSVC AND MATH_LIBRARY)
    target_link_libraries(fractal_generator ${MATH_LIBRARY})
endif()

# Установка типа сборки по умолчанию
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Информация о сборке
message(STATUS "Тип сборки: ${CMAKE_BUILD_TYPE}")
message(STATUS "Компилятор C: ${CMAKE_C_COMPILER}")
