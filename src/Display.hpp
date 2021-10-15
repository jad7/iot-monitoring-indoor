#ifndef DISPLAY_H
#define DISPLAY_H
#include <MeteoStruct.h>
#include <LCD_I2C.h>

class Display {
    public:
        Display(uint8_t address = 0x27, uint8_t chars = 16, uint8_t lines = 2);
        void print(MeteoData* dsata);
        void setup();
    private:
        uint8_t cols, rows;
        LCD_I2C *lcd;
        //LiquidCrystal_I2C *lcd;
};
#endif