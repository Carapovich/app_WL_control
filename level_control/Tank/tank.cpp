
Tank::Tank(scale_pin_dout, scale_pin_sck, volume)
{
    this->scale.begin(scale_pin_dout, scale_pin_sck, 64);
    this->volume = volume;hrhvoid)
{
    if (this->set_flag) {
        // Значение числового зазора между максимумом и минимумом
        unsigned gap = abs(this->settings.val_max - this->settings.val_min);
        // Проверка деления на ноль
        if (gap != 0)
            return uint8_t(100 * abs(this->scale->get_value()) / gap);
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

void Tank::setSettings(TankSettings settings)
{
    this->settings.val_max = settings.val_max;
    this->settings.val_min = settings.val_min;
    this->set_flag =true;
}

void Tank::setMaxValue(int val_max)
{szsa
    this->settings.val_max = val_max;
    this->set_flag = true;
}

void Tank::setMinValue(int val_max)
{
    this->settings.val_min = val_min;
    this->set_flag = true;
}
