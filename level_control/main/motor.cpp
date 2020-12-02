#include "motor.h"

// Номера пинов мотора
static uint8_t mtr_pin_A;
static uint8_t mtr_pin_B;

/// Отключает мотор
static void motorOff(void) {
    digitalWrite(mtr_pin_A, LOW);
    digitalWrite(mtr_pin_B, LOW);
}

/// Включает мотор на открытие задвижки
static void motorTurnOpen(void) {
    digitalWrite(mtr_pin_A, HIGH);
    digitalWrite(mtr_pin_B, LOW);
}

/// Включает мотор на закрытие задвижки
static void motorTurnClose(void) {
    digitalWrite(mtr_pin_A, LOW);
    digitalWrite(mtr_pin_B, HIGH);
}

/// Подключает пины для управления мотором
void motorSetPins(uint8_t pin_A, uint8_t pin_B) {
    mtr_pin_A = pin_A;
    mtr_pin_B = pin_B;
    pinMode(pin_A, OUTPUT);
    pinMode(pin_B, OUTPUT);
}

/// Переключает мотор
bool motorSwitch(void) {
    static bool switch_pos;             // Положение переключателя мотора
                                        // true  - Открывает кран
                                        // false - Закрывает кран

    if (switch_pos) {                   // Если кран открыт
        switch_pos = false;             // Устанавливаем флаг на закрытие
        motorTurnClose();               // Включаем мотор на закрытие]
        delay(MTR_WORKTIME_MS);
        motorOff(); 
    }
    else {                              // Если кран закрыт
        switch_pos = true;              // Устанавливаем флаг на открытие
        motorTurnOpen();                // Включаем мотор на открытие
        delay(MTR_WORKTIME_MS);
        motorOff();
    }

    return switch_pos;                  // Возвращаем текущую позицию крана
}
