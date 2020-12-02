#ifndef SIMPLEMENU_H
#define SIMPLEMENU_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Идентификатор первого элемента по умолчанию
#define ID_FIRST        0x02

// Время бездействия по умолчанию (сек.)
#define DFLT_TIMEOUT    10

// Структура элемента меню
struct MenuItem {
    uint8_t id_prev;        // Идентификатор элемента меню выше по списку
    uint8_t id_next;        // Идентификатор элемента меню ниже по списку
    uint8_t id_jump;        // Идентификатор элемента меню, к которому осуществляется переход

    String  *name;          // Название элемента меню
    void (*action)(void);   // Действие элемента меню
    void* act_arg;          // Настраиваемый аргумент элемента меню
};

class SimpleMenu {
private:
    uint8_t id_counter;     // Счетчик для идентификаторов
    uint8_t id_old;         // Предыдущий идентификатор элемента
    uint8_t id_curr;        // Идентификатор текущего элемента
    MenuItem *items;        // Массив пунктов меню

    bool    bt_enable;      // Флаг активации кнопок навигации
    volatile uint8_t  bt_last;            // Значение последней нажатой кнопки
    volatile int32_t  last_event_time;    // Время, когда была последний раз нажата кнопка
    uint8_t pin_bt_up,      // Пины кнопок навигации по меню
            pin_bt_down,
            pin_bt_forward;

    bool timeout_flag;      // Флаг бездействия 
    uint32_t timeout_val;   // Время бездействия экрана
    uint8_t draw_flag;      // Флаг отрисовки меню на экране
    uint8_t carriage_pos;   // Положение каретки на экране
    LiquidCrystal_I2C *lcd; // Указатель на экран отрисовки
    uint8_t lcd_width;      // Ширина экрана
    uint8_t lcd_height;     // Высота экрана

    /// Добавляет новый пункт меню в память и возвращает его идентификатор
    uint8_t addItem(String *name);

    /// Связывает соседние пункты меню
    void connectNearby(uint8_t id_prev, uint8_t id_next);

    /// Проверка пункта на существование
    bool isExists(uint8_t id_check);

    /// Проверка пункта на отсутствие действий
    bool isNotAction(uint8_t id_check);

    // Оболочка обработчика прерываний
    friend void interruptHandlerShell();

    /// Основной обработчик нажатия кнопок навигации меню
    void handleButtonClick(void);

    /// Обработчик нажатия кнопки "Вверх"
    void handleOnUpButtonClick(void);

    /// Обработчик нажатия кнопки "Вверх"
    void handleOnDownButtonClick(void);

    /// Обработчик нажатия кнопки "Вверх"
    void handleOnForwardButtonClick(void);

    // Обработчик действия пункта меню
    void handleItemAction(void);

    /// Отрисовка списка меню
    void drawMenuList(void);

    /// Отрисовка терминального экрана
    void drawMenuTerminal(void);

    /// Отрисовка пустого экрана
    void drawMenuEmpty(void);
    
    /// Отрисовка экрана подтверждения
    void drawMenuSuccess(void);

public:
    /// Конструктор. name - имя первого элемента
    SimpleMenu(String *name);

    /// Деструктор
    ~SimpleMenu(void);

    /////////////////////////////////
    // РАБОТА С НАСТРОЙКОЙ ПУНКТОВ //
    /////////////////////////////////
    
    /// Добавление дочернего пункта меню
    uint8_t addChildItem(uint8_t id_parent, String *name);

    /// Добавление пункта меню
    uint8_t addItem(uint8_t id_prev, String *name);

    // Переименование пункта меню
    void renameItem(uint8_t id_item, String *new_name);

    /// Назначение произвольного действия для пункта меню
    void setAction(uint8_t id_item, void (*action)(void));

    /// Назначение действия для изменения числового параметра
    void setArgumentAction(uint8_t id_item, uint32_t *arg);

    /// Назначение действия для изменения числового параметра
    void setArgumentAction(uint8_t id_item, int32_t *arg);

    /// Назначение действия для изменения числового параметра
    void setArgumentAction(uint8_t id_item, float *arg);

    ///////////////////////
    // РАБОТА С КНОПКАМИ //
    ///////////////////////
    
    /// Назначает кнопки для навигации по меню
    void setButtons(uint8_t pin_interrupt,
                    uint8_t pin_bt_up, uint8_t pin_bt_down, uint8_t pin_bt_forward);

    /// Активирует использование кнопок навигации
    void enableButtons(void);

    /// Блокирует использование кнопок навигации
    void disableButtons(void);

    //////////////////////
    // РАБОТА С ЭКРАНОМ //
    //////////////////////

    /// Добавляет экран для отрисовки меню
    void setLCD(LiquidCrystal_I2C* lcd, uint8_t lcd_width, uint8_t lcd_height);

    /// Установка пользовательской функции прорисовки титульного экрана
    void setTitleDrawFunction(void (*titleDrawFunc)(void));

    // Возвращает указатель на значение времени бездействия экрана
    uint32_t* getTimeoutValuePointer(void);

    /// Отрисовывает меню согласно его текущим параметрам
    void drawMenu(void);

    /// Отрисовывает меню согласно параметрам, полученным по последнему прерыванию
    void updateMenu(void);

};

#endif // SIMPLEMENU_H
