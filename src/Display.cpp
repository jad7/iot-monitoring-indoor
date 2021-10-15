#include <Display.hpp>


Display::Display(uint8_t address, uint8_t cols, uint8_t rows) {
    
    //lcd = new LiquidCrystal_I2C(address, cols, rows);
    lcd = new LCD_I2C(address);
    this->cols = cols;
    this->rows = rows;
}
 void Display::setup() {
    Serial.println("Begin init display");
    //status = lcd->begin(cols, rows);
    lcd->begin();
    lcd->backlight();
    
    
    lcd->print(FPSTR("Hi JAD and"));
    lcd->setCursor(0,1);
    lcd->print(FPSTR("Solarfox!"));  
    delay(2000);
 }

 void Display::print(MeteoData* dsata) {
     //lcd->clear();
     //lcd->home();
     lcd->setCursor(0,0);
     lcd->print(FPSTR("IN: Temp:"));
     lcd->print(dsata->tempr);
     lcd->setCursor(0,1);
     lcd->print(FPSTR("Hum:"));
     lcd->print(dsata->humidity);
     lcd->print(FPSTR("% "));
     //lcd->setCursor(0,1);
     lcd->print(FPSTR("Co2:"));
     lcd->print(dsata->co2);
     lcd->flush();

 }

