#ifndef TITLE_CHARSET_H
#define TITLE_CHARSET_H

static const char hl_0[] PROGMEM = {
    0b10001,
    0b01110,
    0b01100,
    0b01010,
    0b00110,
    0b01110,
    0b10001,
    0b11111
};

static const char hl_1[] PROGMEM = {
    0b11011,
    0b10011,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b10001,
    0b11111
};

static const char hl_2[] PROGMEM = {
    0b10001,
    0b01110,
    0b11110,
    0b11101,
    0b11011,
    0b10111,
    0b00000,
    0b11111
};

static const char hl_3[] PROGMEM = {
    0b00000,
    0b11101,
    0b11011,
    0b11101,
    0b11110,
    0b01110,
    0b10001,
    0b11111
};

static const char hl_4[] PROGMEM = {
    0b11101,
    0b11001,
    0b10101,
    0b01101,
    0b00000,
    0b11101,
    0b11101,
    0b11111
};

static const char hl_5[] PROGMEM = {
    0b00000,
    0b01111,
    0b00001,
    0b11110,
    0b11110,
    0b01110,
    0b10001,
    0b11111
};

static const char hl_6[] PROGMEM = {
    0b11001,
    0b10111,
    0b01111,
    0b00001,
    0b01110,
    0b01110,
    0b10001,
    0b11111
};

static const char hl_7[] PROGMEM = {
    0b00000,
    0b11110,
    0b11101,
    0b11011,
    0b10111,
    0b10111,
    0b10111,
    0b11111
};

static const char hl_8[] PROGMEM = {
    0b10001,
    0b01110,
    0b01110,
    0b10001,
    0b01110,
    0b01110,
    0b10001,
    0b11111
};

static const char hl_9[] PROGMEM = {
    0b10001,
    0b01110,
    0b01110,
    0b10000,
    0b11110,
    0b11101,
    0b10011,
    0b11111
};

static const char hl_percent[] PROGMEM = {
    0b00111,
    0b00110,
    0b11101,
    0b11011,
    0b10111,
    0b01100,
    0b11100,
    0b11111
};

static const char* title_charset[] = {
    hl_0, hl_1, hl_2, hl_3, hl_4, hl_5, hl_6, hl_7, hl_8, hl_9, hl_percent
};



#endif // TITLE_CHARSET_H
