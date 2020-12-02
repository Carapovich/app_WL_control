#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// Время закртыия/открытия мотором крана
#define MTR_WORKTIME_MS  36000

/// Подключает пины для управления мотором
void motorSetPins(uint8_t pin_A, uint8_t pin_B);

/// Переключает мотор
bool motorSwitch(void);

#endif // MOTOR_H
