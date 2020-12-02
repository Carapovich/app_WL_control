#ifndef CYRILLIC_CHARSET_H
#define CYRILLIC_CHARSET_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

/// Задает экран, который будет вести запись кириллических символов
void setCyrillicLCD(LiquidCrystal_I2C *local_lcd);

/// Очищает буфер кириллических символов
void clearCyrillicBuffer(void);

/// Преобразует исходную строку с кириллицей к выводу на экран
void convertToCyrillic(String *out_str, String *src_str);

#endif // CYRILLIC_CHARSET_H
