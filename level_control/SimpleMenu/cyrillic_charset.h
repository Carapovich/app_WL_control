#ifndef CYRILLIC_CHARSET_H
#define CYRILLIC_CHARSET_H

#include <LiquidCrystal_I2C.h>

/// Задает экран, который будет вести запись кириллических символов
void setLCD(LiquidCrystal_I2C *local_lcd);

/// Преобразует исходную строку с кириллицей к выводу на экран
String convertToCyrillic(String src_str);

#endif // CYRILLIC_CHARSET_H
