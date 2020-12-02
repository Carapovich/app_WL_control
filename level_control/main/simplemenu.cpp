#include "cyrillic_charset.h"
#include "simplemenu.h"

// Идентификатор титульного экрана
#define TITLE_SCREEN    0x01

// Максимальное количество пунктов в меню
#define MAX_ITEMS       0xFB

// Значения id_jump при выполнении действий
#define JUMP_ARG_UINT   0xFC
#define JUMP_ARG_INT    0xFD
#define JUMP_ARG_FLOAT  0xFE
#define JUMP_ACTION     0xFF

// Флаги отрисовки меню на экране
#define LCD_NODRAW      0x10
#define LCD_TTL_FIRST   0x20
#define LCD_TITLE       0x25
#define LCD_LIST        0x30
#define LCD_TMNL_FIRST  0x40
#define LCD_TERMINAL    0x45
#define LCD_EMPTY       0x50
#define LCD_SUCCESS     0x55

// Флаги нажатых кнопок
#define BT_EMPTY        0x60
#define BT_UP           0x70
#define BT_DOWN         0x80
#define BT_FORWARD      0x90

// Галочка для экрана подтверждения
static const char success_0[] PROGMEM = {
    0b00011, 0b10001, 0b11000, 0b11100, 0b11100, 0b11110, 0b11110, 0b11110
};
static const char success_1[] PROGMEM = {
    0b00111, 0b00111, 0b00111, 0b01111, 0b01111, 0b01111, 0b01111, 0b01111
};
static const char success_2[] PROGMEM = {
    0b11111, 0b11110, 0b11000, 0b10001, 0b10001, 0b00011, 0b00011, 0b00011
};

// Служебные строки по умолчанию
static String str_back ("Назад");
static String str_title("Меню");
static String str_buff[4];

// Указатель на текущий объект SimpleMenu, используемый в прерывании
static SimpleMenu *interrupt_menu;

SimpleMenu::SimpleMenu(String *name) {
    this->id_counter = 0;               // Обнуление счетчика пунктов
    this->id_curr = TITLE_SCREEN;       // Выставка значений текущего идентификатора

    this->last_event_time = millis();   // Выставка времени последнего нажатия кнопки
    this->pin_bt_up = 0;                // Выставка пина для кнопки вверх

    this->timeout_flag = false;         // Выставка флага бездействия
    this->timeout_val  = DFLT_TIMEOUT;  // Выставка времени бездействия
    
    this->draw_flag = LCD_TTL_FIRST;    // Выставка флага прорисовки экрана
    this->carriage_pos = 0;             // Выставка значений каретки (по строке)
    this->lcd = NULL;                   // Выставка значений экрана
    this->lcd_width = 1;
    this->lcd_height = 1;

    // Добавление пункта титульного экрана
    addItem(&str_title);
    this->items[TITLE_SCREEN].id_next = 0;
    this->items[TITLE_SCREEN].id_prev = 0;

    // Добавление первого элемента с именем name
    addChildItem(TITLE_SCREEN, name);
}

SimpleMenu::~SimpleMenu(void) {
    Serial.println("Destructor message");
}

uint8_t SimpleMenu::addItem(String *name) {
    this->id_counter++;                                 // Увеличиваем счетчик пунктов
    if (this->id_counter <= MAX_ITEMS) {                // Проверка на переполнение идентификатора
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

void SimpleMenu::connectNearby(uint8_t id_prev, uint8_t id_next) {
    this->items[id_prev].id_next = id_next;
    this->items[id_next].id_prev = id_prev;
}

bool SimpleMenu::isExists(uint8_t id_check) {
    return (id_check <= this->id_counter);
}

bool SimpleMenu::isNotAction(uint8_t id_check) {
    return (!this->items[id_check].id_jump);
}

// MAIN HANDLER. CHECK VOLATILE
void interruptHandlerShell() {
    // Смотрит, на каком пину замкнуло и отправляет данные в переменную bt_last
    volatile static long last_interrupt;
    if (abs(millis() - last_interrupt) >= 200) {
        interrupt_menu->last_event_time = millis();
        last_interrupt = interrupt_menu->last_event_time;

        if (digitalRead(interrupt_menu->pin_bt_up) == LOW)            // Нажата клавиша вверх
            interrupt_menu->bt_last = BT_UP;
        else if (digitalRead(interrupt_menu->pin_bt_down) == LOW)     // Нажата клавиша вниз
            interrupt_menu->bt_last = BT_DOWN;
        else if (digitalRead(interrupt_menu->pin_bt_forward) == LOW)  // Нажата клавиша вперед
            interrupt_menu->bt_last = BT_FORWARD;
        else
            interrupt_menu->bt_last = BT_EMPTY;  
    }
}

void SimpleMenu::handleButtonClick(void) {
    if (this->bt_enable) {
        switch (this->bt_last) {
        case BT_UP:
            this->handleOnUpButtonClick();
            break;
        case BT_DOWN:
            this->handleOnDownButtonClick();
            break;
        case BT_FORWARD:
            this->handleOnForwardButtonClick();
            break;
        }

        this->bt_last = BT_EMPTY;
    }
}

void SimpleMenu::handleOnUpButtonClick(void) {
    if (this->id_curr != TITLE_SCREEN) {                // Проверка на нахождение не в главном меню
        if (this->id_curr <= MAX_ITEMS) {               // Проверка на нахождение в списках меню
            if (this->carriage_pos > 0)                 // Установка значения каретки на экране
                this->carriage_pos--;
            if (this->items[this->id_curr].id_prev) {   // Установка текущего значения идентификатора
                this->id_curr = this->items[this->id_curr].id_prev;
                this->draw_flag = LCD_LIST;             // Установка флага на прорисовку списка меню
            }
        }
        else {                                          // Нахождение в пункте настройки параметра
            switch (this->id_curr) {
            case JUMP_ARG_UINT:                         // Целочисленный беззнаковый параметр
                if ( *((uint32_t*) this->items[this->id_old].act_arg) < UINT32_MAX )
                    (*((uint32_t*) this->items[this->id_old].act_arg))++;
                break;
            case JUMP_ARG_INT:                          // Целочисленный параметр со знаком
                if ( *((int32_t*) this->items[this->id_old].act_arg) < INT32_MAX )
                    (*((int32_t*) this->items[this->id_old].act_arg))++;
                break;
            case JUMP_ARG_FLOAT:                        // Вещественный параметр
                (*((float*) this->items[this->id_old].act_arg)) += 0.1;
                break;
            }
            this->draw_flag = LCD_TERMINAL;             // Установка флага на прорисовку экрана настроек параметров
        }
    }
}

void SimpleMenu::handleOnDownButtonClick(void) {
    if (this->id_curr != TITLE_SCREEN) {                // Проверка на нахождение не в главном меню
        if (this->id_curr <= MAX_ITEMS) {               // Проверка на нахождение в списках меню
            if (this->carriage_pos < this->lcd_height - 1)  // Установка значения каретки на экране
                this->carriage_pos++;
            if (this->items[this->id_curr].id_next) {   // Установка текущего значения идентификатора
                this->id_curr = this->items[this->id_curr].id_next;
                this->draw_flag = LCD_LIST;             // Установка флага на прорисовку списка меню
            }
        }
        else {                                          // Нахождение в пункте настройки параметра
            switch (this->id_curr) {
            case JUMP_ARG_UINT:                         // Целочисленный беззнаковый параметр
                if ( *((uint32_t*) this->items[this->id_old].act_arg) > 0 )
                    (*((uint32_t*) this->items[this->id_old].act_arg))--;
                break;
            case JUMP_ARG_INT:                          // Целочисленный параметр со знаком
                if ( *((int32_t*) this->items[this->id_old].act_arg) > INT32_MIN )
                    (*((int32_t*) this->items[this->id_old].act_arg))--;
                break;
            case JUMP_ARG_FLOAT:                        // Вещественный параметр
                (*((float*) this->items[this->id_old].act_arg)) -= 0.1;
                break;
            }
            this->draw_flag = LCD_TERMINAL;             // Установка флага на прорисовку экрана настроек параметров
        }
    }
}

void SimpleMenu::handleOnForwardButtonClick(void) {
    // Проверка на нахождение на экране настройки параметра
    if (this->id_curr > MAX_ITEMS) {  
        this->id_curr = this->id_old;
        this->carriage_pos = 0;
        this->draw_flag = LCD_LIST;
    }
    
    // Для всех остальных случаев
    else if (!isNotAction(this->id_curr)) {
        // Настройка экрана, к которому выполняется переход   
        uint8_t jump = this->items[this->id_curr].id_jump;
        switch (jump) {
        case TITLE_SCREEN:        // Переход к титульному экрану
            this->id_curr = jump;
            this->draw_flag = LCD_TTL_FIRST;
            break;
        case JUMP_ARG_UINT:       // Переход к экрану настроек параметров
        case JUMP_ARG_INT:
        case JUMP_ARG_FLOAT:
            this->id_old = this->id_curr;   // Для перехода от настроек параметра к списку меню
            this->id_curr = jump;
            this->draw_flag = LCD_TMNL_FIRST;
            break;
        case JUMP_ACTION:         // Переход к действию пункта меню
            this->handleItemAction();       // Выполняем действие текущего пункта
            this->draw_flag = LCD_LIST;     // Переход к списку меню
            break;
        default:                  // Переход к списку меню
            this->id_curr = jump;
            this->carriage_pos = 0;
            this->draw_flag = LCD_LIST;
        }
    }
}

void SimpleMenu::handleItemAction(void) {
    this->disableButtons();             // Отключаем управление навигацией
    this->drawMenuEmpty();              // Рисуем пустой экран
    this->items[id_curr].action();      // Выполняем действие текущего пункта
    this->drawMenuSuccess();            // Рисуем экран подтверждения
    this->last_event_time = millis();   // Действие может затянуться - обновляем время последнего нажатия кнопки
    this->enableButtons();              // Включаем управление навигацией
}

void SimpleMenu::drawMenuList(void) {
    uint8_t id_tmp;

    // Очистка буфера строк списка меню
    for (uint8_t i = 0; i < 4; i++)
        str_buff[i] = "";

    // Очистка буфера кириллических символов
    clearCyrillicBuffer();

    // Заполнение буфера вверх по экрану
    id_tmp = this->id_curr;
    for (int16_t i = this->carriage_pos - 1; i >= 0; i--) {
         id_tmp = this->items[id_tmp].id_prev;
         convertToCyrillic(&(str_buff[i]), this->items[id_tmp].name);
    }

    // Заполнение буфера вниз по экрану
    id_tmp = this->id_curr;
    for (int16_t i = this->carriage_pos + 1; i < this->lcd_height; i++) {
         id_tmp = this->items[id_tmp].id_next;
         convertToCyrillic(&(str_buff[i]), this->items[id_tmp].name);
    }

    // Заполнение буфера текущим пунктом
    convertToCyrillic(&(str_buff[this->carriage_pos]), this->items[id_curr].name);

    // Отрисовка списка на экран
    this->lcd->clear();
    for (uint8_t i = 0; i < this->lcd_height; i++) {
        this->lcd->setCursor(0, i);
        if (i == this->carriage_pos)
            this->lcd->print("> ");
        this->lcd->print(str_buff[i]);
    }
}

void SimpleMenu::drawMenuTerminal(void) {
    // Отрисовка строки шапки (выполняется только один раз при переходе из списка меню)
    if (this->draw_flag == LCD_TMNL_FIRST) {
        str_buff[0] = "";
        clearCyrillicBuffer();        // Очистка буфера кириллических символов
        convertToCyrillic(&(str_buff[0]), this->items[id_old].name);
        
        uint8_t space_gap = this->lcd_width - str_buff[0].length();   // Количество ячеек на экране, заполняемой пробелами
        this->lcd->clear();
        this->lcd->setCursor(space_gap - space_gap / 2, this->lcd_height / 2 - 1);
        this->lcd->print(str_buff[0]);
    }

    // Отрисовка строки параметра
    switch (this->id_curr) {
    case JUMP_ARG_UINT:
        str_buff[1] = String(*((uint32_t*) this->items[this->id_old].act_arg));
        break;
    case JUMP_ARG_INT:
        str_buff[1] = String(*((int32_t*) this->items[this->id_old].act_arg));
        break;
    case JUMP_ARG_FLOAT:
        str_buff[1] = String(*((float*) this->items[this->id_old].act_arg), 1);
        break;
    }       
    str_buff[1] = " " + str_buff[1];    // Заполнение пробелом спереди строки
    str_buff[1] += " ";                 // Заполнение пробелом сзади строки
    this->lcd->setCursor((this->lcd_width - str_buff[1].length()) / 2, this->lcd_height / 2);
    this->lcd->print(str_buff[1]);
}

void SimpleMenu::drawMenuEmpty(void) {
    uint8_t w = this->lcd_width;
    uint8_t h = this->lcd_height;
    for (uint8_t i = 0; i < w*h; i++) {
        this->lcd->setCursor(i % w, i / w);
        this->lcd->print("\xFF");
    }
}

void SimpleMenu::drawMenuSuccess(void) {
    // Добавление галочки в память экрана
    this->lcd->createChar(00, success_0);
    this->lcd->createChar(01, success_1);
    this->lcd->createChar(02, success_2);
    
    // Отрисовка
    uint8_t w = this->lcd_width;
    uint8_t h = this->lcd_height;
    for (uint8_t i = 0; i < w*h; i++) {
        this->lcd->setCursor(i % w, i / w);
        this->lcd->print("\xFF");
    }
    delay(130);
    this->lcd->setCursor(round(w/2.) - 1, round(h/2.));
    this->lcd->write(00);
    delay(130);
    this->lcd->setCursor(round(w/2.), round(h/2.));
    this->lcd->write(01);
    delay(100);
    this->lcd->setCursor(round(w/2.), round(h/2.) - 1);
    this->lcd->write(02);
    delay(800);
}

uint8_t SimpleMenu::addChildItem(uint8_t id_parent, String *name) {
    if (isExists(id_parent) && isNotAction(id_parent)) {
        uint8_t id_child, id_back;
        id_child = addItem(name);
        id_back = addItem(&str_back);                   // Пункт возврата к родительскому пункту

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

uint8_t SimpleMenu::addItem(uint8_t id_prev, String *name) {
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

void SimpleMenu::setAction(uint8_t id_item, void (*action)(void)) {
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].action = action;
        this->items[id_item].id_jump = JUMP_ACTION;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::renameItem(uint8_t id_item, String *new_name) {
    this->items[id_item].name = new_name;
}

void SimpleMenu::setArgumentAction(uint8_t id_item, uint32_t *arg) {
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].act_arg = arg;
        this->items[id_item].id_jump = JUMP_ARG_UINT;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::setArgumentAction(uint8_t id_item, int32_t *arg) {
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].act_arg = arg;
        this->items[id_item].id_jump = JUMP_ARG_INT;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::setArgumentAction(uint8_t id_item, float *arg) {
    if (isExists(id_item) && isNotAction(id_item)) {
        this->items[id_item].act_arg = arg;
        this->items[id_item].id_jump = JUMP_ARG_FLOAT;
    }
    else {
        // Exception here: Unnable to set item action.
    }
}

void SimpleMenu::setButtons(uint8_t pin_interrupt,
                            uint8_t pin_bt_up, uint8_t pin_bt_down, uint8_t pin_bt_forward) {
    if (this->pin_bt_up == 0) {
  
        this->bt_enable = true;
    
        this->pin_bt_up = pin_bt_up;
        this->pin_bt_down = pin_bt_down;
        this->pin_bt_forward = pin_bt_forward;

        pinMode(pin_interrupt, INPUT_PULLUP);
        pinMode(this->pin_bt_up, INPUT_PULLUP);
        pinMode(this->pin_bt_down, INPUT_PULLUP);
        pinMode(this->pin_bt_forward, INPUT_PULLUP);

        uint8_t int_entry = digitalPinToInterrupt(pin_interrupt);
        interrupt_menu = this;
        attachInterrupt(int_entry, interruptHandlerShell, FALLING);
    }
    else {
        // Exception here: Unnable to attach buttons. Buttons attached already
    }
}

void SimpleMenu::enableButtons(void) {
    this->bt_enable = true;
}

void SimpleMenu::disableButtons(void) {
    this->bt_enable = false;
}

void SimpleMenu::setLCD(LiquidCrystal_I2C* lcd, uint8_t lcd_width, uint8_t lcd_height) {
    this->lcd = lcd;
    this->lcd_width = lcd_width;
    this->lcd_height = lcd_height;
    this->draw_flag = LCD_TITLE;

    setCyrillicLCD(lcd);
}

void SimpleMenu::setTitleDrawFunction(void (*titleDrawFunc)(void)) {
    this->items[TITLE_SCREEN].action = titleDrawFunc;
}

uint32_t* SimpleMenu::getTimeoutValuePointer(void) {
    return &(this->timeout_val);
}

void SimpleMenu::drawMenu(void) {
    if (this->lcd != NULL) {
        // Если таймаут - отключаем экран
        if (this->timeout_flag)
            this->lcd->noBacklight();
        else
            this->lcd->backlight();

        // Обработка значения флага отрисовки
        switch (this->draw_flag) {
        case LCD_TTL_FIRST:
            clearCyrillicBuffer();
            this->lcd->clear();
            this->draw_flag = LCD_TITLE;
        case LCD_TITLE:
            this->items[TITLE_SCREEN].action();
            break;
        case LCD_LIST:
            this->drawMenuList();
            this->draw_flag = LCD_NODRAW;
            break;
        case LCD_TMNL_FIRST:
        case LCD_TERMINAL:
            this->drawMenuTerminal();
            this->draw_flag = LCD_NODRAW;
            break;
        }
    }
    else {
        // Exception here: Unnable to draw menu. LCD null pointer
    }
}

void SimpleMenu::updateMenu(void) {
    // Проверка на возможные изменения
    if (this->bt_last != BT_EMPTY) {
        if (this->timeout_flag) {         // Если выходим из таймаута
            this->timeout_flag = false;
            this->bt_last = BT_EMPTY;     // Сбрасываем значение последней нажатой кнопки
        }
        else                              // Иначе - обрабатываем нажатую кнопку
            this->handleButtonClick();
    }
    else {
        // Половина времени от таймаута - переходим на титульный экран
        if (abs(millis() - this->last_event_time) > 1000*this->timeout_val / 2 && this->draw_flag != LCD_TITLE) {
            this->id_curr = TITLE_SCREEN;
            this->draw_flag = LCD_TTL_FIRST;
        }
        // Время таймаута - выставляем флаг бездействия в true
        if (abs(millis() - this->last_event_time) > 1000*this->timeout_val && !this->timeout_flag)
            this->timeout_flag = true;
    }

    // Нижнее ограничение на время бездействия экрана
    if (this->timeout_val < DFLT_TIMEOUT)
        this->timeout_val = DFLT_TIMEOUT;

    this->drawMenu();
}
