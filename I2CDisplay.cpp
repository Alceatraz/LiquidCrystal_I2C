#include "I2CDisplay.h"

I2CDisplay::I2CDisplay() {}

void I2CDisplay::initialization(
        uint8_t displayAddress,
        uint8_t displayWidth,
        uint8_t displayLines,
        bool isOLED) {
    this->_Address = displayAddress;
    this->_Cols = displayWidth;
    this->_Rows = displayLines;
    this->_OLED = isOLED;
    Wire.begin();
    _Function = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    if (displayLines > 1) _Function |= LCD_2LINE;
    _Lines = displayLines;
    if (displayLines == 1) _Function |= LCD_5x10DOTS;
    delay(50);
    expanderWrite(_Light);
    delay(1000);
    write4bits(0x03 << 4);
    delayMicroseconds(4500);
    write4bits(0x03 << 4);
    delayMicroseconds(4500);
    write4bits(0x03 << 4);
    delayMicroseconds(150);
    write4bits(0x02 << 4);
    command(LCD_FUNCTIONSET | _Function);
    _Control = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    _Control |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _Control);
    clear();
    _Mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _Mode);
    command(LCD_RETURNHOME);
    delayMicroseconds(2000);
}

void I2CDisplay::home() {
    command(LCD_RETURNHOME);
    delayMicroseconds(2000);
}

void I2CDisplay::clear() {
    command(LCD_CLEARDISPLAY);
    delayMicroseconds(2000);
    if (_OLED) {
        setCursor(0, 0);
    }
}

void I2CDisplay::setCursor(uint8_t col, uint8_t row) {
    int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row > _Lines) {
        row = _Lines - 1; // we count rows starting w/0
    }
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void I2CDisplay::setDisplay(bool enable = true) {
    if (enable) {
        _Control |= LCD_DISPLAYON;
    } else {
        _Control &= ~LCD_DISPLAYON;
    }
    command(LCD_DISPLAYCONTROL | _Control);
}

void I2CDisplay::setBackLight(bool enable) {
    if (enable) {
        _Light = LCD_BACKLIGHT;
    } else {
        _Light = LCD_NOBACKLIGHT;
    }
    expanderWrite(0);
}

void I2CDisplay::setShowCursor(bool enable) {
    if (enable) {
        _Control |= LCD_CURSORON;
    } else {
        _Control &= ~LCD_CURSORON;
    }
    command(LCD_DISPLAYCONTROL | _Control);
}

void I2CDisplay::setCursorBlink(bool enable) {
    if (enable) {
        _Control |= LCD_BLINKON;
    } else {
        _Control &= ~LCD_BLINKON;
    }
    command(LCD_DISPLAYCONTROL | _Control);
}

void I2CDisplay::scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void I2CDisplay::scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void I2CDisplay::setAutoScroll(bool enable = false) {

    if (enable) {
        _Mode |= LCD_ENTRYSHIFTINCREMENT;
    } else {
        _Mode &= ~LCD_ENTRYSHIFTINCREMENT;
    }
    command(LCD_ENTRYMODESET | _Mode);
}

void I2CDisplay::setTextFlowDirection(bool leftToRight = true) {
    if (leftToRight) {
        _Mode |= LCD_ENTRYLEFT;
    } else {
        _Mode &= ~LCD_ENTRYLEFT;
    }
    command(LCD_ENTRYMODESET | _Mode);
}


void I2CDisplay::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}


void I2CDisplay::createChar(uint8_t location, const char *charmap) {
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(pgm_read_byte_near(charmap++));
    }
}


// ===========================================================================


void I2CDisplay::command(uint8_t value) {
    send(value, 0);
}

void I2CDisplay::send(uint8_t value, uint8_t mode) {
    uint8_t highnib = value & 0xf0;
    uint8_t lownib = (value << 4) & 0xf0;
    write4bits((highnib) | mode);
    write4bits((lownib) | mode);
}

size_t I2CDisplay::write(uint8_t value) {
    send(value, Rs);
    return 1;
}

void I2CDisplay::write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

void I2CDisplay::expanderWrite(uint8_t _data) {
    Wire.beginTransmission(_Address);
    Wire.write((int) (_data) | _Light);
    Wire.endTransmission();
}

void I2CDisplay::pulseEnable(uint8_t _data) {
    expanderWrite(_data | En);    // En high
    delayMicroseconds(1);        // enable pulse must be >450ns
    expanderWrite(_data & ~En); // En low
    delayMicroseconds(50);        // commands need > 37us to settle
}
