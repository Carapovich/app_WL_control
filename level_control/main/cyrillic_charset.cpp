#include "cyrillic_charset.h"

static const char cyr_B[] PROGMEM = {
    0b11111,
    0b10000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b00000
};

static const char cyr_G[] PROGMEM = {
    0b11111,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b00000
};

static const char cyr_D[] PROGMEM = {
    0b00110,
    0b01010,
    0b01010,
    0b01010,
    0b01010,
    0b01010,
    0b11111,
    0b10001
};

static const char cyr_YO[] PROGMEM = {
    0b01010,
    0b00000,
    0b11111,
    0b10000,
    0b11110,
    0b10000,
    0b11111,
    0b00000
};

static const char cyr_J[] PROGMEM = {
    0b10101,
    0b10101,
    0b10101,
    0b01110,
    0b10101,
    0b10101,
    0b10101,
    0b00000
};

static const char cyr_Z[] PROGMEM = {
    0b11110,
    0b00001,
    0b00001,
    0b01110,
    0b00001,
    0b00001,
    0b11110,
    0b00000
};

static const char cyr_I[] PROGMEM = {
    0b10001,
    0b10001,
    0b10011,
    0b10101,
    0b11001,
    0b10001,
    0b10001,
    0b00000
};

static const char cyr_II[] PROGMEM = {
    0b00100,
    0b10101,
    0b10001,
    0b10011,
    0b10101,
    0b11001,
    0b10001,
    0b00000
};

static const char cyr_L[] PROGMEM = {
    0b00011,
    0b00101,
    0b01001,
    0b01001,
    0b01001,
    0b01001,
    0b10001,
    0b00000
};

static const char cyr_P[] PROGMEM = {
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b00000
};

static const char cyr_U[] PROGMEM = {
    0b10001,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b00001,
    0b11110,
    0b00000
};

static const char cyr_F[] PROGMEM = {
    0b01110,
    0b10101,
    0b10101,
    0b10101,
    0b01110,
    0b00100,
    0b00100,
    0b00000
};

static const char cyr_TC[] PROGMEM = {
    0b10010,
    0b10010,
    0b10010,
    0b10010,
    0b10010,
    0b10010,
    0b11111,
    0b00001
};

static const char cyr_CH[] PROGMEM = {
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b01111,
    0b00001,
    0b00001,
    0b00000
};

static const char cyr_SH[] PROGMEM = {
    0b10001,
    0b10101,
    0b10101,
    0b10101,
    0b10101,
    0b10101,
    0b11111,
    0b00000
};

static const char cyr_soft_SH[] PROGMEM = {
    0b10001,
    0b10101,
    0b10101,
    0b10101,
    0b10101,
    0b10101,
    0b11111,
    0b00001
};

static const char cyr_hard_big[] PROGMEM = {
    0b11000,
    0b01000,
    0b01000,
    0b01110,
    0b01001,
    0b01001,
    0b01110,
    0b00000
};

static const char cyr_hard_I[] PROGMEM = {
    0b10001,
    0b10001,
    0b10001,
    0b11001,
    0b10101,
    0b10101,
    0b11001,
    0b00000
};

static const char cyr_soft_big[] PROGMEM = {
    0b10000,
    0b10000,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b11110,
    0b00000
};

static const char cyr_hard_E[] PROGMEM = {
    0b01110,
    0b10001,
    0b00001,
    0b00111,
    0b00001,
    0b10001,
    0b01110,
    0b00000
};

static const char cyr_Y[] PROGMEM = {
    0b10010,
    0b10101,
    0b10101,
    0b11101,
    0b10101,
    0b10101,
    0b10010,
    0b00000
};

static const char cyr_YA[] PROGMEM = {
    0b01111,
    0b10001,
    0b10001,
    0b01111,
    0b00101,
    0b01001,
    0b10001,
    0b00000
};

static const char cyr_b[] PROGMEM = {
    0b00010,
    0b01100,
    0b10000,
    0b11110,
    0b10001,
    0b10001,
    0b01110,
    0b00000
};

static const char cyr_v[] PROGMEM = {
    0b00000,
    0b00000,
    0b11100,
    0b10010,
    0b11100,
    0b10010,
    0b11100,
    0b00000
};

static const char cyr_g[] PROGMEM = {
    0b00000,
    0b00000,
    0b11110,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b00000
};

static const char cyr_d[] PROGMEM = {
    0b00000,
    0b00000,
    0b00110,
    0b01010,
    0b01010,
    0b01010,
    0b11111,
    0b10001
};

static const char cyr_yo[] PROGMEM = {
    0b01010,
    0b00000,
    0b01110,
    0b10001,
    0b11111,
    0b10000,
    0b01110,
    0b00000
};

static const char cyr_j[] PROGMEM = {
    0b00000,
    0b00000,
    0b10101,
    0b10101,
    0b01110,
    0b10101,
    0b10101,
    0b00000
};

static const char cyr_z[] PROGMEM = {
    0b00000,
    0b00000,
    0b11100,
    0b00010,
    0b01100,
    0b00010,
    0b11100,
    0b00000
};

static const char cyr_i[] PROGMEM = {
    0b00000,
    0b00000,
    0b10010,
    0b10010,
    0b10110,
    0b11010,
    0b10010,
    0b00000
};

static const char cyr_ii[] PROGMEM = {
    0b01100,
    0b00000,
    0b10010,
    0b10010,
    0b10110,
    0b11010,
    0b10010,
    0b00000
};

static const char cyr_k[] PROGMEM = {
    0b00000,
    0b00000,
    0b10010,
    0b10100,
    0b11000,
    0b10100,
    0b10010,
    0b00000
};

static const char cyr_l[] PROGMEM = {
    0b00000,
    0b00000,
    0b00110,
    0b01010,
    0b10010,
    0b10010,
    0b10010,
    0b00000
};

static const char cyr_n[] PROGMEM = {
    0b00000,
    0b00000,
    0b10010,
    0b10010,
    0b11110,
    0b10010,
    0b10010,
    0b00000
};

static const char cyr_p[] PROGMEM = {
    0b00000,
    0b00000,
    0b11110,
    0b10010,
    0b10010,
    0b10010,
    0b10010,
    0b00000
};

static const char cyr_t[] PROGMEM = {
    0b00000,
    0b00000,
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000
};

static const char cyr_f[] PROGMEM = {
    0b00000,
    0b00000,
    0b01110,
    0b10101,
    0b10101,
    0b01110,
    0b00100,
    0b00000
};

static const char cyr_tc[] PROGMEM = {
    0b00000,
    0b00000,
    0b10010,
    0b10010,
    0b10010,
    0b10010,
    0b11111,
    0b00001
};

static const char cyr_ch[] PROGMEM = {
    0b00000,
    0b00000,
    0b10010,
    0b10010,
    0b01110,
    0b00010,
    0b00010,
    0b00000
};

static const char cyr_sh[] PROGMEM = {
    0b00000,
    0b00000,
    0b10001,
    0b10101,
    0b10101,
    0b10101,
    0b11111,
    0b00000
};

static const char cyr_soft_sh[] PROGMEM = {
    0b00000,
    0b00000,
    0b10001,
    0b10101,
    0b10101,
    0b10101,
    0b11111,
    0b00001
};

static const char cyr_hard_small[] PROGMEM = {
    0b00000,
    0b00000,
    0b11000,
    0b01000,
    0b01110,
    0b01001,
    0b01110,
    0b00000
};

static const char cyr_hard_i[] PROGMEM = {
    0b00000,
    0b00000,
    0b10001,
    0b10001,
    0b11001,
    0b10101,
    0b11001,
    0b00000
};

static const char cyr_soft_small[] PROGMEM = {
    0b00000,
    0b00000,
    0b10000,
    0b10000,
    0b11100,
    0b10010,
    0b11100,
    0b00000
};

static const char cyr_hard_e[] PROGMEM = {
    0b00000,
    0b00000,
    0b01110,
    0b10001,
    0b00111,
    0b10001,
    0b01110,
    0b00000
};

static const char cyr_y[] PROGMEM = {
    0b00000,
    0b00000,
    0b10010,
    0b10101,
    0b11101,
    0b10101,
    0b10010,
    0b00000
};

static const char cyr_ya[] PROGMEM = {
    0b00000,
    0b00000,
    0b01110,
    0b10010,
    0b01110,
    0b01010,
    0b10010,
    0b00000
};

static LiquidCrystal_I2C *lcd;
static uint8_t addr;
static uint16_t code_buff[8];

static char addCyrillicChar(uint8_t ch_in_0, uint8_t ch_in_1, const char *sym)
{
    if (lcd != NULL) {
        uint16_t code_sym = (uint16_t)ch_in_0 << 8 | ch_in_1;
        for (uint8_t i = 0; i < 8; i++)
            if (code_buff[i] == code_sym)
                return i;

        if (addr > 7)
            addr = 0;
        lcd->createChar(addr, sym);
        code_buff[addr] = code_sym;
        return addr++;
    }
    else {
        return 8;
    }
}

static char getCyrillicChar(uint8_t ch_in_0, uint8_t ch_in_1)
{
    switch (ch_in_0) {
    case 208:
        switch (ch_in_1) {
        case 144: return ('A');
        case 145: return addCyrillicChar(ch_in_0, ch_in_1, cyr_B);
        case 146: return ('B');
        case 147: return addCyrillicChar(ch_in_0, ch_in_1, cyr_G);
        case 148: return addCyrillicChar(ch_in_0, ch_in_1, cyr_D);
        case 149: return ('E');
        case 129: return addCyrillicChar(ch_in_0, ch_in_1, cyr_YO);
        case 150: return addCyrillicChar(ch_in_0, ch_in_1, cyr_J);
        case 151: return addCyrillicChar(ch_in_0, ch_in_1, cyr_Z);
        case 152: return addCyrillicChar(ch_in_0, ch_in_1, cyr_I);
        case 153: return addCyrillicChar(ch_in_0, ch_in_1, cyr_II);
        case 154: return ('K');
        case 155: return addCyrillicChar(ch_in_0, ch_in_1, cyr_L);
        case 156: return ('M');
        case 157: return ('H');
        case 158: return ('O');
        case 159: return addCyrillicChar(ch_in_0, ch_in_1, cyr_P);
        case 160: return ('P');
        case 161: return ('C');
        case 162: return ('T');
        case 163: return addCyrillicChar(ch_in_0, ch_in_1, cyr_U);
        case 164: return addCyrillicChar(ch_in_0, ch_in_1, cyr_F);
        case 165: return ('X');
        case 166: return addCyrillicChar(ch_in_0, ch_in_1, cyr_TC);
        case 167: return addCyrillicChar(ch_in_0, ch_in_1, cyr_CH);
        case 168: return addCyrillicChar(ch_in_0, ch_in_1, cyr_SH);
        case 169: return addCyrillicChar(ch_in_0, ch_in_1, cyr_soft_SH);
        case 170: return addCyrillicChar(ch_in_0, ch_in_1, cyr_hard_big);
        case 171: return addCyrillicChar(ch_in_0, ch_in_1, cyr_hard_I);
        case 172: return addCyrillicChar(ch_in_0, ch_in_1, cyr_soft_big);
        case 173: return addCyrillicChar(ch_in_0, ch_in_1, cyr_hard_E);
        case 174: return addCyrillicChar(ch_in_0, ch_in_1, cyr_Y);
        case 175: return addCyrillicChar(ch_in_0, ch_in_1, cyr_YA);
        case 176: return ('a');
        case 177: return addCyrillicChar(ch_in_0, ch_in_1, cyr_b);
        case 178: return addCyrillicChar(ch_in_0, ch_in_1, cyr_v);
        case 179: return addCyrillicChar(ch_in_0, ch_in_1, cyr_g);
        case 180: return addCyrillicChar(ch_in_0, ch_in_1, cyr_d);
        case 181: return ('e');
        case 182: return addCyrillicChar(ch_in_0, ch_in_1, cyr_j);
        case 183: return addCyrillicChar(ch_in_0, ch_in_1, cyr_z);
        case 184: return addCyrillicChar(ch_in_0, ch_in_1, cyr_i);
        case 185: return addCyrillicChar(ch_in_0, ch_in_1, cyr_ii);
        case 186: return addCyrillicChar(ch_in_0, ch_in_1, cyr_k);
        case 187: return addCyrillicChar(ch_in_0, ch_in_1, cyr_l);
        case 188: return ('m');
        case 189: return addCyrillicChar(ch_in_0, ch_in_1, cyr_n);
        case 190: return ('o');
        case 191: return addCyrillicChar(ch_in_0, ch_in_1, cyr_p);
        }
        break;

    case 209:
        switch (ch_in_1) {
        case 145: return addCyrillicChar(ch_in_0, ch_in_1, cyr_yo);
        case 128: return ('p');
        case 129: return ('c');
        case 130: return addCyrillicChar(ch_in_0, ch_in_1, cyr_t);
        case 131: return ('y');
        case 132: return addCyrillicChar(ch_in_0, ch_in_1, cyr_f);
        case 133: return ('x');
        case 134: return addCyrillicChar(ch_in_0, ch_in_1, cyr_tc);
        case 135: return addCyrillicChar(ch_in_0, ch_in_1, cyr_ch);
        case 136: return addCyrillicChar(ch_in_0, ch_in_1, cyr_sh);
        case 137: return addCyrillicChar(ch_in_0, ch_in_1, cyr_soft_sh);
        case 138: return addCyrillicChar(ch_in_0, ch_in_1, cyr_hard_small);
        case 139: return addCyrillicChar(ch_in_0, ch_in_1, cyr_hard_i);
        case 140: return addCyrillicChar(ch_in_0, ch_in_1, cyr_soft_small);
        case 141: return addCyrillicChar(ch_in_0, ch_in_1, cyr_hard_e);
        case 142: return addCyrillicChar(ch_in_0, ch_in_1, cyr_y);
        case 143: return addCyrillicChar(ch_in_0, ch_in_1, cyr_ya);
        }
    }
}

void setCyrillicLCD(LiquidCrystal_I2C *local_lcd)
{
    lcd = local_lcd;
    clearCyrillicBuffer();
}

void clearCyrillicBuffer(void)
{
    addr = 0;
    for (uint8_t i = 0; i < 8; i++)
        code_buff[i] = 0;
}

void convertToCyrillic(String *out_str, String *src_str)
{
    char cyr_ch;
    uint8_t len = src_str->length();

    for (uint8_t i = 0; i < len; i++) {
        if ((uint8_t)(*src_str)[i] == 0xD0 || (uint8_t)(*src_str)[i] == 0xD1) {
            cyr_ch = getCyrillicChar((*src_str)[i], (*src_str)[i + 1]);
            i++;
        }
        else
            cyr_ch = (*src_str)[i];

        *out_str += cyr_ch;
    }
};
