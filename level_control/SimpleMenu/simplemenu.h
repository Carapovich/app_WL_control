#ifndef SIMPLEMENU_H
#define SIMPLEMENU_H

#include <string.h>
#include <stdint.h>

// Идентификатор первого элемента по умолчанию
#define ID_FIRST        0x02

// Структура элемента меню
struct MenuItem {
    uint8_t id_prev;    // Идентификатор элемента меню выше по списку
    uint8_t id_next;    // Идентификатор элемента меню ниже по списку
    uint8_t id_jump;    // Идентификатор элемента меню, к которому осуществляется переход

    char    *name;              // Название элемента меню
    void    (*action)(void);    // Действие элемента меню
    void*   act_arg;            // Настраиваемый аргумент элемента меню
};

class SimpleMenu {
private:
    uint8_t id_counter;     // Счетчик для идентификаторов
    uint8_t id_old;         // Предыдущий идентификатор элемента
    uint8_t id_curr;        // Идентификатор текущего элемента
    MenuItem *items;        // Массив пунктов меню

    bool    bt_enable;      // Флаг активации кнопок навигации
    uint8_t pin_bt_up,      // Пины кнопок навигации по меню
            pin_bt_down,
            pin_bt_forward;

    uint8_t draw_flag;      // Флаг отрисовки меню на экране
    uint8_t carriage_pos;   // Положение каретки на экране
    uint8_t carr_pos_max;   // Максимальное значение положения каретки

    /// Добавляет новый пункт меню в память и возвращает его идентификатор
    uint8_t addItem(char* name);

    /// Связывает соседние пункты меню
    void connectNearby(uint8_t id_prev, uint8_t id_next);

    /// Проверка пункта на существование
    bool isExists(uint8_t id_check);

    /// Проверка пункта на отсутствие действий
    bool isNotAction(uint8_t id_check);

    /// Обработчик нажатия кнопок навигации меню
    void handleButtonClick(void);

    /// Обработчик нажатия кнопки "Вверх"
    void handleOnUpButtonClick(void);

    /// Обработчик нажатия кнопки "Вверх"
    void handleOnDownButtonClick(void);

    /// Обработчик нажатия кнопки "Вверх"
    void handleOnForwardButtonClick(void);

public:
    /// Конструктор. name - имя первого элемента
    SimpleMenu(char *name);

    /// Деструктор
    virtual ~SimpleMenu(void);

    /// Добавление дочернего пункта меню
    uint8_t addChildItem(uint8_t id_parent, char* name);

    /// Добавление пункта меню
    uint8_t addItem(uint8_t id_prev, char* name);

    /// Назначение произвольного действия для пункта меню
    void setAction(uint8_t id_item, void (*action)(void));

    /// Назначение действия для изменения числового параметра
    void setArgumentAction(uint8_t id_item, unsigned *arg);

    /// Назначение действия для изменения числового параметра
    void setArgumentAction(uint8_t id_item, int *arg);

    /// Назначение действия для изменения числового параметра
    void setArgumentAction(uint8_t id_item, double *arg);

    /// Назначает кнопки для навигации по меню
    void setButtons(uint8_t pin_interrupt,
                    uint8_t pin_bt_up, uint8_t pin_bt_down, uint8_t pin_bt_forward);

    /// Активирует использование кнопок навигации
    void enableButtons(void);

    /// Блокирует использование кнопок навигации
    void disableButtons(void);

    /// Установка пользовательской функции прорисовки титульного экрана
    void setTitleDrawFunction(void (*titleDrawFunc)(void));

    /// Отрисовывает меню согласно его текущим параметрам
    void drawMenu(void);

};

#endif // SIMPLEMENU_H
