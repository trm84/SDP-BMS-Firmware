#include "SSD1306.h"

void SSD1306_Init(){
    i2cStart();
    i2cWrite(SSD1306_WRITE);
}

void SSD1306_Test(){
            SSD1306_Init();
            SSD1306_Command(1, 0xB0);
            SSD1306_Command(1, 0x00);
            SSD1306_Command(1, 0x13);
            
            char cmd[] = {0x8D, 0x14};
            SSD1306_Command(1, 0xAE);
            SSD1306_Command(1, 0x8D);
            SSD1306_Command(1, 0x14);
            SSD1306_Command(1, 0xAF);
            
            i2cWrite(0x40);
            int zi = 0;
            for(zi = 0; zi<4; zi ++){
                i2cWrite(0xFF);
            }
            i2cStop();
            i2cStart();
            i2cWrite(SSD1306_DATA);
}

void SSD1306_Command(int numCmd, char cmd[]){
    i2cWrite(SSD1306_CMD);
    int a = 0;
    for(; a < numCmd; a ++){
        i2cWrite(cmd);
    }
}