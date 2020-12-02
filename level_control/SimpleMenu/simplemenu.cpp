#include <stdlib.h>
#include "simplemenu.h"

// Идентификатор титульного экрана
#define TITLE_SCREEN     0x01

// Максимальное количество пунктов в меню
#define MAX_ITEMS       0xFB

// Значения id_jump при выполнении действий
#define JUMP_ARG_UINT   0xFC
#define JUMP_ARG_INT    0xFD
#define JUMP_ARG_FLOAT  0xFE
#define JUMP_ACTION     0xFF

// Флаги отрисовки меню на экране
#define LCD_TITLE     0x10
#define LCD_LIST      0x20
#define LCD_TERMINAL  0x30

SimpleMenu::SimpleMenu(char* name)
{
    this->id_counter = 0;           // Обнуление счетчика пунктов
    this->id_old = TITLE_SCREEN;    // Выставка значений текущего идентификатора
    this->id_curr = TITLE_SCREEN;
    this->draw_flag = LCD_TITLE;    // Выставка флага прорисовки экрана
    this->carriage_pos = 1;         // Выставка значений каретки
    this->carr_pos_max = 1;

    // Добавление пункта титульного экрана
    addItem("Menu");
    this->items[TITLE_SCREEN].id_next = 0;
    this->items[TITLE_SCREEN].id_prev = 0;

    // Добавление первого элемента с именем name
    addChildItem(TITLE_SCREEN, name);
}

uint8_t SimpleMenu::addItem(char *name)
{
    this->id_counter++;                                 // Увеличиваем счетчик пунктов
    if (this->id_counter > MAX_ITEMS) {                 // Проверка на переполнение идентификатора
        this->items = (MenuItem*) realloc(this->items, sizeof(MenuItem) * (this->id_counter + 1));  // Добавляем место в памяти для нового пункта

        this->items[this->id_counter].name = name;      // Присваивание имени
        this->items[this->id_counter].id_jump = 0;      // Выставка значений по умолчанию
        this->items[this->id_counter].action = NULL;
        this->items[this->id_counter].act_arg = NULL;
    }
    else {
        // Exception here: Unnable to add item. Maximum number of items has been exceeded: 0xFB
    }
    return this->id_counter;
}

void SimpleMenu::connectNearby(uint8_t id_prev, uint8_t id_next)
{
    this->items[id_prev].id_next = id_next;
    this->items[id_next].id_prev = id_prev;
}

bool SimpleMenu::isExists(uint8_t id_check)
{
    return (id_check <= this->id_counter);
}

bool SimpleMenu::isNotAction(uint8_t id_check)
{
    return (!this->items[id_check].id_jump);
}

void SimpleMenu::handleButtonClick(void)
{
    if (this->bt_enable)
        1;
    // Смотрит, на каком пину замкнуло и отправляет данные в переменную button_flag
}

void SimpleMenu::handleOnUpButtonClick(void)
{
    if (this->id_curr != TITLE_SCREEN) {                // Проверка на нахождение не в главном меню
        if (this->id_curr <= MAX_ITEMS) {               // Проверка на нахождение в списках меню
            if (this->carriage_pos > 1)                 // Установка значения каретки на экране
                this->carriage_pos--;
            if (this->items[this->id_curr].id_prev) {   // Установка текущего значения идентификатора
                this->id_old = this->id_curr;
                this->id_curr = this->items[this->id_curr].id_prev;
            }
        }
        else {                                          // Нахождение в пункте настройки параметра
            switch (this->id_curr) {
            case JUMP_ARG_UINT:                         // Целочисленный беззнаковый параметр
                if ( *((unsigned*) this->items[this->id_old].act_arg) < UINT32_MAX )
                    (*((unsigned*) this->items[this->id_old].act_arg))++;
                break;
            case JUMP_ARG_INT:                          // Целочисленный параметр со знаком
                if ( *((int*) this->items[this->id_old].act_arg) < INT32_MAX )
                    (*((int*) this->items[this->id_old].act_arg))++;
                break;
            case JUMP_ARG_FLOAT:                        // Вещественный параметр
                (*((float*) this->items[this->id_old].act_arg)) += 0.1;
                break;
            }
        }
    }
}

void SimpleMenu::handleOnDownButtonClick(void)
{
    if (this->id_curr != TITLE_SCREEN) {                // Проверка на нахождение не в главном меню
        if (this->id_curr <= MAX_ITEMS) {               // Проверка на нахождение в списках меню
            if (this->carriage_pos < this->carr_pos_max)    // Установка значения каретки на экране
                this->carriage_pos++;
            if (this->items[this->id_curr].id_next) {   // Установка текущего значения идентификатора
                this->id_old = this->id_curr;
                this->id_curr = this->items[this->id_curr].id_next;
            }
        }
        else {                                          // Нахождение в пункте настройки параметра
            switch (this->id_curr) {
            case JUMP_ARG_UINT:                         // Целочисленный беззнаковый параметр
                if ( *((unsigned*) this->items[this->id_old].act_arg) > 0 )
                    (*((unsigned*) this->items[this->id_old].act_arg))--;
                break;
            case JUMP_ARG_INT:                          // Целочисленный параметр со знаком
                if ( *((int*) this->items[this->id_old].act_arg) > INT32_MIN )
                    (*((int*) this->items[this->id_old].act_arg))--;
                break;
            case JUMP_ARG_FLOAT:                        // Вещественный параметр
                (*((float*) this->items[this->id_old].act_arg)) -= 0.1;
                break;
            }
        }
    }
}

void SimpleMenu::handleOnForwardButtonClick(void)
{
    if (!isNotAction(this->id_curr)) {
        // Установка текущего значения идентификатора
        if (this->id_curr > MAX_ITEMS)              // Проверка на нахождение на экране настройки параметра
            this->id_curr = this->id_old;
        else {                                      // Во всех остальных случаях
            this->id_old = this->id_curr;
            this->id_curr = this->items[this->id_curr].id_jump;
        }

        // Настройка экрана, к которому был выполнен переход
        if (this->id_curr == TITLE_SCREEN)              // Титульный экран
            this->draw_flag = LCD_TITLE;
        else if (this->id_curr <= MAX_ITEMS) {          // Список меню
            this->draw_flag = LCD_LIST;
            this->carriage_pos = 1;
        }
        else                                            // Экран настройки параметра
            this->draw_flag = LCD_TERMINAL;
    }
}

uint8_t SimpleMenu::addChildItem(uint8_t id_parent, char* name)
{
    if (isExists(id_parent) && isNotAction(id_parent)) {
        uint8_t id_child, id_back;
        id_child = addItem(name);
        id_back = addItem("Back");                      // Пункт возврата к родительскому пункту

        this->items[id_parent].id_jump = id_child;      // Связывание родительского и дочернего пункта
        this->items[id_back].id_jump = id_parent;       // Связывание пункта возврата и родительского пункта
        this->items[id_child].id_prev = 0;              // Выставка значений по умолчанию
        this->items[id_back].id_next = 0;
        connectNearby(id_child, id_back);

        return id_child;
    }
    else {
        // Exception here: Unnable to add child item.
        return 0;
    }
}

uint8_t SimpleMenu::addItem(uint8_t id_prev, char* name)
{
    // Проверка на существование пункта с идентификатором id_prev
    // и то, что он не является последним пунктом в списке
    if (isExists(id_prev) && this->items[id_prev].id_next) {
        uint8_t id, id_next;
        id = addItem(name);
        id_next = this->items[id_prev].id_next;

        // Вставка нового элемента между двух существующих соседних
        connectNearby(id_prev, id);
        connectNearby(id, id_next);

        return id;
    }
    else {
        // Exception here: Unnable to add item. Wrong previous id number
        return 0;
    }
}

void SimpleMenu::setAction(uint8_t id_item, void (*action)(void))
{
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].action = action;
        this->items[id_item].id_jump = JUMP_ACTION;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::setArgumentAction(uint8_t id_item, unsigned *arg)
{
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].act_arg = arg;
        this->items[id_item].id_jump = JUMP_ARG_UINT;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::setArgumentAction(uint8_t id_item, int *arg)
{
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].act_arg = arg;
        this->items[id_item].id_jump = JUMP_ARG_INT;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::setArgumentAction(uint8_t id_item, double *arg)
{
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].act_arg = arg;
        this->items[id_item].id_jump = JUMP_ARG_FLOAT;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::setButtons(uint8_t pin_interrupt,
                            uint8_t pin_bt_up, uint8_t pin_bt_down, uint8_t pin_bt_forward)
{
    this->bt_enable = true;
    this->pin_bt_up = pin_bt_up;
    this->pin_bt_down = pin_bt_down;
    this->pin_bt_forward = pin_bt_forward;

    // Далее: подключение по digital pin
}

void SimpleMenu::enableButtons(void)
{
    this->bt_enable = true;
}

void SimpleMenu::disableButtons(void)
{
    this->bt_enable = false;
}

void SimpleMenu::setTitleDrawFunction(void (*titleDrawFunc)(void))
{
    this->items[TITLE_SCREEN].action = titleDrawFunc;
}

void SimpleMenu::drawMenu(void)
{
    switch (this->draw_flag) {
    case LCD_TITLE:
        break;
    case LCD_LIST:
        break;
    case LCD_TERMINAL:
        break;
    }
}
