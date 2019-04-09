#include "SSD1306.h"

char cmd[3];

void SSD1306_Init(){
    i2cStart();
    i2cWrite(SSD1306_WRITE);
    i2cWrite(0x02); //Possibly Reset?    
    SSD1306_EntireDisplayOnReset();
    SSD1306_NormalMode();
    SSD1306_DeactivateScroll();
    SSD1306_SetBrightness(0xFF);
    SSD1306_ChargePump();
    SSD1306_EntireDisplayOn();
    i2cStop();
}

void SSD1306_FillScreen(){
    i2cStart();
    i2cWrite(SSD1306_WRITE);
    i2cWrite(0x02);
    SSD1306_SetDisplayStartLine(0x40);
    SSD1306_PageAddressingMode();
    SSD1306_PageAddressingModeColumns(0x00, 0x1F); //Every column
    
    for(char k = 0; k < 8; k++){ 
        SSD1306_PageAddressingModePage(k); //increment page
        for(int j = 0; j < 127; j ++){ //Fill the Entire Screen
            SSD1306_Data(0xFF);
        }
    }

    i2cStop();
}

void SSD1306_DeactivateScroll(){
    cmd[0] = SSD1306_DEACTIVATE_SCROLL;
    SSD1306_Command(1, cmd);
}

void SSD1306_NormalMode(){
    cmd[0] = SSD1306_NORMAL_MODE;
    SSD1306_Command(1, cmd);
}

void SSD1306_ClearScreen(){
    i2cStart();
    i2cWrite(SSD1306_WRITE);
    SSD1306_HorizontalAddressingMode();
    SSD1306_SetPageAddressRange(0x00, 0x07);
    SSD1306_ColumnStartAndEndAddress(0x00, 0x7F);
    
    for(int j = 0; j < 889; j ++){ //Clear the Entire Screen
        SSD1306_Data(0x00);
    }
    
    i2cStop();
}

void SSD1306_SetDisplayStartLine(char start){
    cmd[0] = start;
    SSD1306_Command(1,cmd);
}

void SSD1306_SetPageAddressRange(char start, char end){
    cmd[0] = SSD1306_SET_PAGE_ADDRESS_RANGE;
    cmd[1] = start; //First Col
    cmd[2] = end; //Last Col
    SSD1306_Command(3, cmd);
}

void SSD1306_ColumnStartAndEndAddress(char start, char end){
    cmd[0] = SSD1306_SET_COLUMN_ADDRESS;
    cmd[1] = start; //First Col
    cmd[2] = end; //Last Col
    SSD1306_Command(3, cmd);
}

void SSD1306_HorizontalAddressingMode(){
    cmd[0] = SSD1306_SET_MEMORY_ADDRESSING_MODE;
    cmd[1] = SSD1306_HORIZONTAL_ADDRESSING_MODE;
    SSD1306_Command(2, cmd);
}

void SSD1306_PageAddressingModePage(char page){
    cmd[0] = 0xB7 & page;
    SSD1306_Command(1, cmd);
}

void SSD1306_PageAddressingModeColumns(char start, char end){
    cmd[0] = start;
    SSD1306_Command(1, cmd); 
    cmd[0] = end;
    SSD1306_Command(1, cmd);
}

void SSD1306_PageAddressingMode(){
    cmd[0] = SSD1306_SET_MEMORY_ADDRESSING_MODE;
    cmd[1] = SSD1306_PAGE_ADDRESSING_MODE;
    SSD1306_Command(2, cmd);
}

void SSD1306_ChargePump(){
    cmd[0] = 0xAE;
    SSD1306_Command(1, cmd); //Turn off Screen
    cmd[0] = 0x8D;
    cmd[1] = 0x14;
    SSD1306_Command(2, cmd); //Charge Pump Settings
    cmd[0] = 0xAF;
    SSD1306_Command(1, cmd); //Turn on Screen
}

void SSD1306_EntireDisplayOn(){
    cmd[0] = SSD1306_ENTIRE_DISPLAY_ON;
    SSD1306_Command(1, cmd);
}
void SSD1306_EntireDisplayOnReset(){
    cmd[0] = SSD1306_ENTIRE_DISPLAY_ON_RESET;
    SSD1306_Command(1, cmd);
}

void SSD1306_SetBrightness(char val){
    cmd[0] = SSD1306_CONTRAST;
    cmd[1] = val;
    SSD1306_Command(2, cmd);
}

void SSD1306_OFF(){
    i2cStart();
    i2cWrite(SSD1306_WRITE);
    cmd[0] = SSD1306_DISPLAY_OFF;
    SSD1306_Command(1, cmd); //Turn on Screen
    i2cStop();
}

void SSD1306_ON(){
    i2cStart();
    i2cWrite(SSD1306_WRITE);
    cmd[0] = SSD1306_DISPLAY_ON;
    SSD1306_Command(1, cmd); //Turn on Screen
    i2cStop();
}



void SSD1306_Test(){
            SSD1306_Init();
            SSD1306_Command(1, 0xB0);
            SSD1306_Command(1, 0x00);
            SSD1306_Command(1, 0x13);
            
            char cmd[] = {0x8D, 0x14};
            SSD1306_Command(1, 0xAE);
            SSD1306_Command(2, cmd);
            //SSD1306_Command(1, 0x14);
            SSD1306_Command(1, 0xAF);
           /* 
            i2cWrite(0x40);
            int zi = 0;
            for(zi = 0; zi<4; zi ++){
                i2cWrite(0xFF);
            }*/
            i2cStop();
            //i2cStart();
            //i2cWrite(SSD1306_DATA);
}

void SSD1306_Data(char data){
    i2cWrite(SSD1306_DATA);
    i2cWrite(data);
}

void SSD1306_Command(int numCmd, char cmd[]){
    i2cWrite(SSD1306_CMD);
    int a = 0;
    for(; a < numCmd; a ++){
        i2cWrite(cmd[a]);
    }
}


void oledGotoYX(unsigned char Row, unsigned char Column)
{
    i2cStart();
    i2cWrite(SSD1306_WRITE);
    //i2cWrite(0x02);
    cmd[0] = 0xB0 + Row;
    SSD1306_Command(cmd, 1);
    cmd[0] = 0x00 + (8*Column & 0x0F);
    SSD1306_Command(cmd, 1);
    cmd[0] = 0x10 + ((8*Column>>4)&0x0F);
    SSD1306_Command(cmd, 1);
    i2cStop();
}


void oledClear()
{
    for (int row = 0; row < 8; row++ ) {
        for (int col = 0; col < 16; col++ ) {
            oledGotoYX( row, col );
            oledPutChar( 'A' );
        }
    }
}


void oledPutChar( char ch ){
    if ( ( ch < 32 ) || ( ch > 127 ) ){
        ch = ' ';
    }
    
    i2cStart();
    i2cWrite(SSD1306_WRITE);
    //i2cWrite(0x02);
    i2cWrite(SSD1306_CONT_DATA);
    for(int z = 0; z < 8; z++){
        OledFont[ch - 32][z];
    }
    i2cStop();
    
}
