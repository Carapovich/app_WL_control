#ifndef TANK_H
#define TANK_H

#include <HX711.h>
#include "save_settings.h"

// Класс бака
class Tank
{
private:
    HX711 scale;            // Экземпляр весов
    TankSettings settings;  // Структура настроек
    uint16_t volume;        // Объем бака (в литрах)
    bool set_flag;          // Флаг выставки значений настроек

public:
    // Конструктор
    Tank(scale_pin_dout, scale_pin_sck, volume);

    // Возвращает процент заполненности бака
    uint8_t getCurrentPercent(void);

    // Возвращает текущий объем
    uint16_t getCurrentVolume(void);

    // Устанавливает настройки бака (макс., мин. значение)
    void setSettings(TankSettings settings);

    // Устанавливает макс. значение
    void setMaxValue(int val_max);

    // Устанавливает мин. значение
    void SetMinValue(int val_min);
};

#endif // TANK_H
