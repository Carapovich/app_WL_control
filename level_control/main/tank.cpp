#include "tank.h"

Tank::Tank(uint8_t scale_pin_dout, uint8_t scale_pin_sck, uint16_t volume)
{
    this->scale.begin(scale_pin_dout, scale_pin_sck, 64);
    this->volume = volume;
    this->set_flag = false;
}

uint8_t Tank::getCurrentPercent(void)
{
    if (this->set_flag) {
        // Проверка деления на ноль
        if (this->settings.val_max != 0) {
            uint16_t prc = 100 * abs(this->scale.get_value()) / this->settings.val_max;
            return (prc > 100) ? (uint8_t)100 : (uint8_t)prc;     // Если больше ста процентов, выводим сто процентов
        }
        else
            return 0;
    }
    else
        return 0;
}

uint16_t Tank::getCurrentVolume(void)
{
    return this->getCurrentPercent() * this->volume;
}

int32_t* Tank::getMaxValuePointer(void)
{
    return &(this->settings.val_max);
}

void Tank::setSettings(TankSettings settings)
{
    this->settings.val_max = settings.val_max;
    this->settings.val_min = settings.val_min;
    this->scale.set_offset(this->settings.val_min);
    this->set_flag = true;
}

void Tank::setMaxValueByScale(void)
{
    this->settings.val_max = abs(this->scale.get_value());
    this->set_flag = true;
}

void Tank::setMinValueByScale(void)
{
    this->scale.tare();
    this->settings.val_min = this->scale.get_offset();
    this->set_flag = true;
}
