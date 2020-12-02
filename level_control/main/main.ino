#include <LiquidCrystal_I2C.h>
#include <HX711.h>

#include "tank.h"
#include "save_settings.h"
#include "motor.h"
#include "cyrillic_charset.h"
#include "simplemenu.h"
#include "title_charset.h"

#define LCD_WIDTH       16
#define LCD_HEIGHT      2

#define VOL_DEFAULT     200
#define SCALE_DOUT      0x0B
#define SCALE_SCK       0x0C

#define MOTOR_1A        0x08
#define MOTOR_1B        0x09

#define INTERRUPT       0x03
#define BUTTON_UP       0x04
#define BUTTON_DOWN     0x05
#define BUTTON_FORWARD  0x06

// Строки меню
String str_tank           ("Кол-во воды");
String str_fill           ("Залить бак");
String str_stop           ("Закрыть бак");
String str_settings       ("Настройки");
String str_timeout        ("Таймаут (сек.)");
String str_sett_tank      ("Настройки бака");
String str_set_up_lim     ("Установ. макс.");
String str_set_down_lim   ("Установ. мин.");
String str_up_lim         ("Макс. значение");

// Основные экземпляры объектов программы
LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);     // Экран
Tank tank(SCALE_DOUT, SCALE_SCK, VOL_DEFAULT);          // Бак теплица
SimpleMenu menu(&str_fill);                             // Список меню

// Положение крана
// true - кран открыт
// false - кран закрыт
bool valve_pos = false;

/// Прорисовка титульного экрана для бака
void drawTitlePage(void) {
    static String str_buff, str_prc;
    uint8_t w = LCD_WIDTH;
    uint8_t h = LCD_HEIGHT;
  
    // Отрисовка заголовка
    str_buff = "";
    convertToCyrillic(&str_buff, &str_tank);
    lcd.setCursor(w - str_buff.length() - (w - str_buff.length()) / 2, h/2 - 1);
    lcd.print(str_buff);

    // Отрисовка шкалы заполнения
    str_buff = "L";
    uint8_t prc = tank.getCurrentPercent();             // Текущий процент заполнения бака
    uint8_t fl_itms = trunc(prc / (100. / (w - 1)));    // Число заполненных ячеек шкалы
    if (fl_itms > w - 2)                                // Проверка на перезаполненность шкалы
        fl_itms = w - 2;
    for (uint8_t i = 0; i < fl_itms; i++)               // Рисуем шкалу (сколько заполненно)
        str_buff += "\xFF";
    for (uint8_t i = fl_itms; i < w - 2; i++)           // Рисуем шкалу (сколько свободно)
        str_buff += " ";
    str_buff += "F";

    str_prc = String((int)prc) + "%";                   // Строка с числом процентов заполненности
    uint8_t x_curs = w - str_prc.length() - (w - str_prc.length()) / 2;   // Координата строки процента по x
    for (uint8_t i = 0; i < str_prc.length() - 1; i++)                    // Вставка строки процентов в строку шкалы заполненности
        if (str_buff[x_curs + i] == '\xFF') {                             // Если на вставляемом месте находится заполненность
            // str_prc[i] - 0x30 - хитрый переход от типа char к целочисленному типу, если в char закодирована цифра
            lcd.createChar(07 - str_prc.length() + i + 1, title_charset[str_prc[i] - 0x30]);    // Создаем закрашенный символ
            str_buff[x_curs + i] = char(07 - str_prc.length() + i + 1);   // Вставляем его в строку
        }
        else
            str_buff[x_curs + i] = str_prc[i];          // Иначе просто вставляем цифру из строки процентов
    // То же самое делаем для символа процента
    if (str_buff[x_curs + str_prc.length() - 1] == '\xFF') {                     
            lcd.createChar(07, title_charset[10]);   
            str_buff[x_curs + str_prc.length() - 1] = char(07);
        }
        else
            str_buff[x_curs + str_prc.length() - 1] = str_prc[str_prc.length() - 1];

    // Выводим на экран
    lcd.setCursor(0, h/2);
    lcd.print(str_buff);
}

// Переключение мотора
void actionMotorSwitch(void) {
    // Если кран открыт
    if (valve_pos) {
        valve_pos = motorSwitch();
        menu.renameItem(ID_FIRST, &str_fill);
    }
    // Если кран закрыт, а бак не полный
    else if (tank.getCurrentPercent() < 100) {
        valve_pos = motorSwitch();
        menu.renameItem(ID_FIRST, &str_stop);
    }
}

// Установка максимального значения на весах
void actionSetMaxValueByScale(void) { 
    tank.setMaxValueByScale(); 
}

// Установка минимального значения на весах
void actionSetMinValueByScale(void) { 
    tank.setMinValueByScale();
}

void setup() {
    Serial.begin(115200);

    // Настройка экрана
    lcd.init();
    lcd.backlight();

    // Настройка баков
    tank.setSettings({10000, 100000});

    // Настройка пинов мотора
    motorSetPins(MOTOR_1A, MOTOR_1B);

    // Настройка меню
    uint8_t id, id_tank;
              menu.setAction(ID_FIRST,    actionMotorSwitch);   // Залить бак
    id      = menu.addItem(ID_FIRST,      &str_settings);       // Настройки
    id      = menu.addChildItem(id,       &str_timeout);        // Таймаут (сек.)
              menu.setArgumentAction(id,  menu.getTimeoutValuePointer());
    id_tank = menu.addItem(id,            &str_sett_tank);      // Настройки бака
    id      = menu.addChildItem(id_tank,  &str_set_up_lim);     // Уст. макс.
              menu.setAction(id,          actionSetMaxValueByScale);
    id      = menu.addItem(id,            &str_set_down_lim);   // Уст. мин.
              menu.setAction(id,          actionSetMinValueByScale);
    id      = menu.addItem(id,            &str_up_lim);         // Макс. знач.
              menu.setArgumentAction(id,  tank.getMaxValuePointer());
    
    menu.setButtons(INTERRUPT, BUTTON_UP, BUTTON_DOWN, BUTTON_FORWARD);
    menu.setLCD(&lcd, LCD_WIDTH, LCD_HEIGHT);
    menu.setTitleDrawFunction(drawTitlePage);
}

void loop() {
    // Если кран открыт и бак заполнен, закрываем кран
    if (valve_pos && tank.getCurrentPercent() >= 100)
        actionMotorSwitch();

    // Обновляем экран
    menu.updateMenu();
}
