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
    Tank(uint8_t scale_pin_dout, uint8_t scale_pin_sck, uint16_t volume);

    // Возвращает процент заполненности бака
    uint8_t getCurrentPercent(void);

    // Возвращает текущий объем
    uint16_t getCurrentVolume(void);

    // Возвращает указатель на поле максимального значения
    int32_t* getMaxValuePointer(void);

    // Устанавливает настройки бака (макс., мин. значение)
    void setSettings(TankSettings settings);

    // Устанавливает макс. значение
    void setMaxValueByScale(void);

    // Устанавливает мин. значение
    void setMinValueByScale(void);
};

#endif // TANK_H
