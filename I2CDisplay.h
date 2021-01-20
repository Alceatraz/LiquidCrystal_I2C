#include "Wire.h"
#include "Print.h"
#include "Arduino.h"
#include "inttypes.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100 // Enable bit
#define Rw B00000010 // Read/Write bit
#define Rs B00000001 // Register select bit


/**
* I'm only a Java programmer
* Only thing I did is rewrite LC_I2C into modern java style
* I only test this on my NodeMCU + 1602
*
* @author Alceatraz Warprays
*/
class I2CDisplay : public Print {


public:


    /**
    * That do noting
    */
    I2CDisplay();


    /**
    * The real "constructor"
    *
    * @param Address default 0x27
    * @param Width 16 with 1602
    * @param Lines  2 with 1602
    */
    void initialization(uint8_t, uint8_t, uint8_t, bool);


    /**
    * Set cursor 0,0
    */
    void home();


    /**
    * Clean screen
    */
    void clear();


    /**
    * Set cursor location
    *
    * @param columns
    * @param row
    */
    void setCursor(uint8_t, uint8_t);


    /**
    * Turn ON/OFF display
    *
    * @param true if enable, false if disable
    */
    void setDisplay(bool);


    /**
    * Turn ON/OFF back light
    *
    * @param true if enable, false if disable
    */
    void setBackLight(bool);


    /**
    * Turn ON/OFF show cursor
    *
    * @param true if enable, false if disable
    */
    void setShowCursor(bool);


    /**
    * Turn ON/OFF blink cursor
    *
    * @param true if enable, false if disable
    */
    void setCursorBlink(bool);


    /**
    * Turn ON/OFF auto scroll
    *
    * @param true if enable, false if disable
    */
    void setAutoScroll(bool);


    /**
    * Set text flow direction
    *
    * @param true is left to right
    */
    void setTextFlowDirection(bool);


    /**
    *  Shift display
    */
    void scrollDisplayLeft();


    /**
    *  Shift display
    */
    void scrollDisplayRight();


    /**
    * I dont know what is that
    */
    void leftToRight();


    /**
    * I dont know what is that
    */
    void rightToLeft();


    /**
    * I dont know what is that
    */
    void shiftIncrement();


    /**
    * I dont know what is that
    */
    void shiftDecrement();


    /**
    * That seem not working
    *
    * ORIGIN COMMENT:
    * Allows us to fill the first 8 CGRAM locations
    * with custom characters
    */
    void createChar(uint8_t, uint8_t[]);


    /**
    * That seem not working
    *
    * ORIGIN COMMENT:
    * createChar with PROGMEM input
    * const char bell[8] PROGMEM = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000};
    */
    void createChar(uint8_t, const char *);


private:
    //

    bool _OLED = false;
    uint8_t _Cols;
    uint8_t _Rows;
    uint8_t _Mode;
    uint8_t _Lines;
    uint8_t _Light;
    uint8_t _Address;
    uint8_t _Control;
    uint8_t _Function;

    //

    virtual size_t write(uint8_t);

    void command(uint8_t);

    void send(uint8_t, uint8_t);

    void write4bits(uint8_t);

    void expanderWrite(uint8_t);

    void pulseEnable(uint8_t);

};
